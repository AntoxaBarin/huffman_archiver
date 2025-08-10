#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "encoding.h"
#include "huffman_tree.h"

DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_BEGIN
#include <fstream>
#include <iterator>
#include <set>
DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_END

bool compareFiles(const std::string& filename1, const std::string& filename2) {
    std::ifstream f1(filename1, std::ifstream::binary | std::ifstream::ate);
    std::ifstream f2(filename2, std::ifstream::binary | std::ifstream::ate);

    if (f1.fail() || f2.fail()) {
        return false;
    }

    if (f1.tellg() != f2.tellg()) {
        return false;
    }

    // seek back to beginning and use std::equal to compare contents
    f1.seekg(0, std::ifstream::beg);
    f2.seekg(0, std::ifstream::beg);
    return std::equal(
        std::istreambuf_iterator<char>(f1.rdbuf()),
        std::istreambuf_iterator<char>(),
        std::istreambuf_iterator<char>(f2.rdbuf())
    );
}

TEST_SUITE("Huffman-tree test") {
    TEST_CASE("Frequency table test") {
        huffman::huffman_tree tree;
        tree.build_frequency_table("../samples/frequency_table_test.txt");
        std::map<char, int> table = tree.get_chars_frequency();

        CHECK(table['a'] == 4);
        CHECK(table['b'] == 4);
        CHECK(table['c'] == 4);
        CHECK(table['d'] == 4);

        CHECK(tree.get_alphabet_power() == 4);
        CHECK(tree.get_number_of_chars() == 16);

        tree.add_symbol('q', 100);
        CHECK(tree.get_chars_frequency()['q'] == 100);
    }

    TEST_CASE("Build code-tree test") {
        huffman::huffman_tree tree;
        tree.build_frequency_table("../samples/frequency_table_test.txt");
        tree.build();
        tree.build_table();

        auto code_table = tree.get_table();

        CHECK(code_table['a'].length() == 2);
        CHECK(code_table['b'].length() == 2);
        CHECK(code_table['c'].length() == 2);
        CHECK(code_table['d'].length() == 2);

        huffman::huffman_tree_node* root = tree.get_root();

        CHECK(root->get_left_child() != nullptr);
        CHECK(root->get_right_child() != nullptr);
        CHECK(root->get_frequency() == 16);

        huffman::huffman_tree_node* left_root_child = root->get_left_child();
        huffman::huffman_tree_node* right_root_child = root->get_right_child();

        CHECK(left_root_child->get_frequency() == 8);
        CHECK(right_root_child->get_frequency() == 8);

        std::set<std::string> codes;
        codes.insert(code_table['a']);
        codes.insert(code_table['b']);
        codes.insert(code_table['c']);
        codes.insert(code_table['d']);

        CHECK(codes.size() == 4);  // 4 unique codes

        CHECK(left_root_child->get_left_child()->get_symbol() != '\0');
        CHECK(left_root_child->get_left_child()->get_frequency() == 4);

        tree.destroy(tree.get_root());
    }

    TEST_CASE("One symbol text test") {
        huffman::huffman_tree tree;
        tree.build_frequency_table("../samples/one_symbol_test.txt");
        tree.build();
        tree.build_table();

        auto code_table = tree.get_table();
        huffman::huffman_tree_node* root = tree.get_root();

        CHECK(code_table[')'].length() == 1);
        CHECK(code_table[')'] == "1");
        CHECK(tree.get_alphabet_power() == 1);
        CHECK(tree.get_chars_frequency()[')'] == 100);

        CHECK(root->get_symbol() == ')');
        CHECK(root->get_frequency() == 100);
        CHECK(root->get_left_child() == nullptr);
        CHECK(root->get_right_child() == nullptr);

        tree.destroy(root);
    }
}

TEST_SUITE("Huffman-encoding test") {
    TEST_CASE("Read frequency table test") {
        huffman::huffman_tree tree;
        huffman::binary_io binary_in;

        std::ifstream input("../samples/read_frequency_table_test.bin", std::ios_base::binary);

        binary_in.read_frequency_table(input, tree);

        auto frequency_table = tree.get_chars_frequency();
        CHECK(frequency_table['a'] == 4);
        CHECK(frequency_table['b'] == 4);
        CHECK(frequency_table['c'] == 4);
        CHECK(frequency_table['d'] == 4);
    }

    TEST_CASE("Compress-decompress 1MB test") {
        huffman::huffman_tree tree;
        huffman::huffman_compressor compressor;
        huffman::huffman_decompressor decompressor;

        compressor.compress_file("../samples/big_text_to_compress.txt", "../samples/binary_buf.bin");
        decompressor.decompress_file("../samples/binary_buf.bin", "../samples/big_text_decompressed.txt");

        CHECK(compareFiles("../samples/big_text_to_compress.txt", "../samples/big_text_decompressed.txt") == true);
    }
}
