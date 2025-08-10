#include "encoding.h"

#include <iostream>
#include <map>

namespace huffman {

// writes power of alphabet and frequency table at the beginning of compressed file
void binary_io::write_frequency_table(std::ofstream& output, const huffman_tree& tree) {
    frequency_table_size_ = 0;

    int alphabet_size = tree.get_alphabet_power();
    int number_of_chars = tree.get_number_of_chars();
    output.write(reinterpret_cast<const char*>(&number_of_chars), sizeof(int));
    output.write(reinterpret_cast<const char*>(&alphabet_size), sizeof(int));

    frequency_table_size_ += 2 * sizeof(int);

    for (auto element : tree.get_chars_frequency()) {
        output.write(&element.first, sizeof(element.first));
        output.write(reinterpret_cast<const char*>(&element.second), sizeof(element.second));
        frequency_table_size_ += sizeof(element.first) + sizeof(element.second);
    }

    not_compressed_file_size_ = number_of_chars;
}

// pack huffman codes into bytes and write them into result file
void binary_io::write_bits(std::ofstream& output, std::string source_file, const huffman_tree& tree) {
    std::map<char, std::string> table = tree.get_table();
    std::ifstream source(source_file);

    compressed_file_size_ = 0;
    cur_position_in_byte = 0;
    char buf = 0;
    char symbol;

    while (source.get(symbol)) {
        std::string symbol_code = table[symbol];
        for (size_t i = 0; i < symbol_code.size(); ++i) {
            buf |= (symbol_code[i] - 48) << (7 - cur_position_in_byte);  // '0' ascii code = 48
            cur_position_in_byte += 1;

            if (cur_position_in_byte == 8) {
                cur_position_in_byte = 0;
                compressed_file_size_ += 1;
                output << buf;
                buf = 0;
            }
        }
    }

    if (cur_position_in_byte != 0) {
        compressed_file_size_ += 1;
        output << buf;
    }
}

void binary_io::read_frequency_table(std::ifstream& input, huffman_tree& tree) {
    frequency_table_size_ = 0;

    int alphabet_power, size_buf, number_buf;
    input.read(reinterpret_cast<char*>(&size_buf), sizeof(int));
    input.read(reinterpret_cast<char*>(&alphabet_power), sizeof(int));

    frequency_table_size_ += 2 * sizeof(int);
    not_compressed_file_size_ = size_buf;

    char symbol_buf;
    for (int i = 0; i < alphabet_power; ++i) {
        input.get(symbol_buf);
        input.read(reinterpret_cast<char*>(&number_buf), sizeof(int));
        tree.add_symbol(symbol_buf, number_buf);
        frequency_table_size_ += sizeof(char) + sizeof(int);
    }
}

void binary_io::read_bits(std::ifstream& input, huffman_tree& tree, const std::string& filename) {
    std::ofstream output(filename);
    char buf;
    unsigned char bit;
    size_t counter_chars = 0;
    const huffman_tree_node* node = tree.get_root();
    compressed_file_size_ = 0;

    while (input.get(buf)) {
        compressed_file_size_ += 1;

        for (int j = 7; j >= 0; --j) {
            bit = (buf >> j) & 1;
            if (bit == 0) {
                node = node->get_left_child();
            } else if (bit == 1 && tree.get_root()->get_left_child() == nullptr &&
                       tree.get_root()->get_right_child() == nullptr) {
                counter_chars += 1;
                output << node->get_symbol();
            } else if (bit == 1) {
                node = node->get_right_child();
            }

            if (counter_chars == not_compressed_file_size_) {
                break;
            }

            if (node->get_left_child() == nullptr && node->get_right_child() == nullptr && node != tree.get_root()) {
                counter_chars += 1;
                output << node->get_symbol();
                node = tree.get_root();
            }
        }
    }
}

void binary_io::print_sizes(std::string mode) const {
    if (mode == "compress") {
        std::cout << not_compressed_file_size_ << std::endl;
        std::cout << compressed_file_size_ << std::endl;
        std::cout << frequency_table_size_ << std::endl;
    } else if (mode == "decompress") {
        std::cout << compressed_file_size_ << std::endl;
        std::cout << not_compressed_file_size_ << std::endl;
        std::cout << frequency_table_size_ << std::endl;
    }
}

size_t binary_io::get_compressed_file_size() const { return compressed_file_size_; }

size_t binary_io::get_frequency_table_size() const { return frequency_table_size_; }

size_t binary_io::get_not_compressed_file_size() const { return not_compressed_file_size_; }

void huffman_compressor::compress_file(const std::string filename, const std::string output_file) const {
    std::ofstream output(output_file, std::ios_base::binary);
    binary_io bin_out;
    huffman_tree tree;

    tree.build_frequency_table(filename);
    tree.build_tree();
    tree.build_table();

    bin_out.write_frequency_table(output, tree);
    bin_out.write_bits(output, filename, tree);

    tree.destroy_tree(tree.get_root());

    bin_out.print_sizes("compress");
}

void huffman_decompressor::decompress_file(const std::string input_file, const std::string output_file) const {
    std::ifstream input(input_file, std::ios_base::binary);
    binary_io bin_in;
    huffman_tree tree;

    bin_in.read_frequency_table(input, tree);
    tree.build_tree();
    tree.build_table();
    bin_in.read_bits(input, tree, output_file);

    tree.destroy_tree(tree.get_root());

    bin_in.print_sizes("decompress");
}

}  // namespace huffman
