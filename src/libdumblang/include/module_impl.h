#pragma once

#include <llvm/IR/IRBuilder.h>

#include <filesystem>

namespace backend
{
    class ModuleImpl
    {
      public:
        llvm::Module &module() { return m_module; }

        void visit();
        void print_ir();
        void compile(const std::filesystem::path &object_file_path);

      private:
        llvm::LLVMContext m_context;
        llvm::IRBuilder<> m_ir_builder{m_context};
        llvm::Module m_module{"program", m_context};
    };
} // namespace backend