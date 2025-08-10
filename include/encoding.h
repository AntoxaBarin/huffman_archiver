#ifndef ENCODING_H
#define ENCODING_H

#include <fstream>
#include <string>
#include "huffman_tree.h"

namespace huffman {

class binary_io {
public:
    void write_frequency_table(std::ofstream& output, const huffman_tree& tree);
    void write_bits(std::ofstream& output, std::string source_file, const huffman_tree& tree);

    void read_frequency_table(std::ifstream& input, huffman_tree& tree);
    void read_bits(std::ifstream& input, huffman_tree& tree, const std::string& filename);

    void print_sizes(std::string mode) const;

    size_t get_not_compressed_file_size() const;
    size_t get_compressed_file_size() const;
    size_t get_frequency_table_size() const;

private:
    int cur_position_in_byte;

    size_t not_compressed_file_size_;
    size_t compressed_file_size_;
    size_t frequency_table_size_;
};

class huffman_compressor {
public:
    void compress_file(const std::string filename, const std::string output_file) const;
};

class huffman_decompressor {
public:
    void decompress_file(const std::string filename, const std::string output_file) const;
};

}  // namespace huffman

#endif
