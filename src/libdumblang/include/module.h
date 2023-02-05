#pragma once

#include <filesystem>
#include <memory>

namespace backend
{
    class ModuleImpl;
    class Module
    {
      public:
        Module();
        ~Module();
        void visit();
        void print_ir();
        void compile(const std::filesystem::path &object_file_path);

      private:
        std::unique_ptr<ModuleImpl> m_module_impl;
    };
} // namespace backend