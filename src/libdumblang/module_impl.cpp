#include "module_impl.h"

#include <fmt/format.h>
#include <llvm/ADT/APInt.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/MC/TargetRegistry.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/Host.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Target/TargetOptions.h>

namespace backend
{
    void ModuleImpl::visit()
    {
        const auto main_func_type =
            llvm::FunctionType::get(m_ir_builder.getInt1Ty(), false);
        const auto main_func = llvm::Function::Create(
            main_func_type, llvm::Function::ExternalLinkage, "main", m_module);
        const auto entry_block =
            llvm::BasicBlock::Create(m_context, "entrypoint", main_func);
        m_ir_builder.SetInsertPoint(entry_block);
        m_ir_builder.CreateRet(
            llvm::ConstantInt::get(m_context, llvm::APInt(32, 42)));
    }

    void ModuleImpl::print_ir() { m_module.print(llvm::outs(), nullptr); }

    void ModuleImpl::compile(const std::filesystem::path &object_file_path)
    {
        auto target_triple = llvm::sys::getDefaultTargetTriple();
        llvm::InitializeNativeTarget();
        llvm::InitializeNativeTargetAsmParser();
        llvm::InitializeNativeTargetAsmPrinter();

        std::string error;
        auto target = llvm::TargetRegistry::lookupTarget(target_triple, error);
        if (!target)
        {
            throw std::runtime_error(error);
        }

        auto target_machine =
            target->createTargetMachine(target_triple, "generic", "", {}, {});
        m_module.setDataLayout(target_machine->createDataLayout());
        m_module.setTargetTriple(target_triple);

        std::error_code EC;
        llvm::raw_fd_ostream dest(object_file_path.string(), EC,
                                  llvm::sys::fs::OF_None);
        if (EC)
        {
            throw std::runtime_error(
                fmt::format("Could not open file '{}': '{}'",
                            object_file_path.string(), EC.message()));
        }

        llvm::legacy::PassManager pass;
        if (target_machine->addPassesToEmitFile(pass, dest, nullptr,
                                                llvm::CGFT_ObjectFile))
        {
            throw std::runtime_error("Can't emit a file of this type");
        }

        pass.run(m_module);
        dest.flush();
    }
} // namespace backend
