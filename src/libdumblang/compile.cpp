#include "module.h"

#include <filesystem>

int main()
{
    backend::Module module;
    module.visit();
    module.print_ir();
    module.compile(std::filesystem::path{"output.o"});
}
