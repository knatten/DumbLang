#include "module.h"
#include "module_impl.h"

namespace backend
{
    Module::Module() : m_module_impl(new ModuleImpl{}) {}
    Module::~Module() = default;
    void Module::visit() { m_module_impl->visit(); }
    void Module::print_ir() { m_module_impl->print_ir(); }
    void Module::compile(const std::filesystem::path &object_file_path)
    {
        m_module_impl->compile(object_file_path);
    }
} // namespace backend