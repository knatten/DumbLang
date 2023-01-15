#include "ast_dumper.h"
#include "parser.h"
#include "version.h"

#include <argparse/argparse.hpp>

#include <filesystem>
#include <fstream>
#include <iostream>

int main(int argc, char *argv[])
{
    argparse::ArgumentParser args("dumblang", version);
    args.add_argument("input_file").help("The .dumb file to compile");
    args.add_argument("-d", "--dump-ast")
        .help("Dump ast to stdout")
        .default_value(false)
        .implicit_value(true);
    try
    {
        args.parse_args(argc, argv);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        std::cerr << args;
        std::exit(1);
    }

    const std::string input_file_name = args.get("input_file");
    const std::filesystem::path input_file{input_file_name};
    std::cout << "Input file: " << input_file << std::endl;

    if (!std::filesystem::exists(input_file))
    {
        std::cerr << "Input file " << input_file << " does not exist";
        return 1;
    }

    std::ifstream ifs;
    ifs.open(input_file, std::ifstream::in);
    if (!ifs.is_open())
    {
        std::cerr << "Failed to open " << input_file << std::endl;
        return 1;
    }

    const auto program = parser::parse(ifs);
    std::cout << "Successfully parsed '" << input_file_name << "'\n";
    if (args["--dump-ast"] == true)
    {
        std::cout << "-----\n";
        AST::AstDumper{program, std::cout}.dump();
        std::cout << "-----\n";
    }
}
