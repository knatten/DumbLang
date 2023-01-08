#include "parser.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>

int main(int argc, char *argv[])
{
    // TODO Use CLI library. Clipp is unmaintained, find another one
    if (argc != 2)
    {
        std::cerr << "USAGE: " << argv[0] << "<input file" << std::endl;
        return 1;
    }

    std::filesystem::path infile{argv[1]};
    std::cout << "Input file: " << infile << std::endl;

    if (!std::filesystem::exists(infile))
    {
        std::cerr << "Input file " << infile << " does not exist";
        return 1;
    }

    std::ifstream ifs;
    ifs.open(infile, std::ifstream::in);
    if (!ifs.is_open())
    {
        std::cerr << "Failed to open " << infile << std::endl;
        return 1;
    }

    std::cout << "-----\n";
    lexer::print_tokens(lexer::lex(ifs));
    std::cout << "\n-----\n";
}
