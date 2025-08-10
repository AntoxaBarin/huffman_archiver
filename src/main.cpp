#include "encoding.h"

#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>

bool is_file_empty(const std::string& filename) {
    std::ifstream in(filename);
    return in.peek() == std::ifstream::traits_type::eof();
}

void compress(std::string input_file, std::string output_file) {
    huffman::huffman_compressor compressor;
    compressor.compress_file(input_file, output_file);
}

void decompress(std::string input_file, std::string output_file) {
    huffman::huffman_decompressor decompressor;
    decompressor.decompress_file(input_file, output_file);
}

int main(int argc, char** argv) {
    try {
        std::string mode, input_file, output_file;

        if (argc == 6) {
            for (int i = 1; i < 6; i++) {
                if (!strcmp(argv[i], "-c")) {
                    mode = argv[i];
                } else if (!strcmp(argv[i], "-d")) {
                    mode = argv[i];
                } else if (!strcmp(argv[i], "-f") || !strcmp(argv[i], "--file")) {
                    input_file = argv[i + 1];
                    i++;

                    if (!std::filesystem::exists(input_file)) {
                        throw std::runtime_error("Input file does not exist!");
                    }
                } else if (!strcmp(argv[i], "-o") || !strcmp(argv[i], "--output")) {
                    output_file = argv[i + 1];
                    i++;
                } else {
                    throw std::runtime_error("Incorrect argument!");
                }
            }
        } else {
            throw std::runtime_error("Incorrect arguments!");
        }

        if (mode == "-c") {
            if (!std::filesystem::exists(input_file)) {
                throw std::runtime_error("Input file doesn't exist!");
            }
            if (is_file_empty(input_file)) {
                std::cout << 0 << '\n' << 0 << '\n' << 0 << std::endl;
                std::ofstream out(output_file, std::ios_base::binary);
                return 0;
            }
            compress(input_file, output_file);
        } else if (mode == "-d") {
            if (!std::filesystem::exists(input_file)) {
                throw std::runtime_error("Input file doesn't exist!");
            }
            if (is_file_empty(input_file)) {
                std::cout << 0 << '\n' << 0 << '\n' << 0 << std::endl;
                std::ofstream out(output_file);
                return 0;
            }
            decompress(input_file, output_file);
        } else {
            throw std::runtime_error("Unknown mode!");
        }
    } catch (std::runtime_error const&) {
        std::cout << "Incorrect arguments!\nUsage:\nTo compress file: " << argv[0]
                  << " -c -f <decompressed_file> -o <compressed_file>"
                  << "\nTo decompress file: " << argv[0] << " -d -f <compressed_file> -o <decompressed_file>"
                  << std::endl;
    }

    return 0;
}
