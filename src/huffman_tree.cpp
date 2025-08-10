#include "huffman_tree.h"
#include <fstream>
#include <memory>

namespace huffman {

huffman_tree_node::huffman_tree_node(char symbol, int frequency) {
    symbol_ = symbol;
    frequency_ = frequency;
    left_child_ = nullptr;
    right_child_ = nullptr;
}

huffman_tree_node::huffman_tree_node(huffman_tree_node* left_child, huffman_tree_node* right_child) {
    frequency_ = left_child->frequency_ + right_child->frequency_;
    left_child_ = left_child;
    right_child_ = right_child;
}

int huffman_tree_node::get_frequency() const { return frequency_; }

int huffman_tree::get_alphabet_power() const { return alphabet_power_; }

std::map<char, int> huffman_tree::get_chars_frequency() const { return chars_frequency_; }

int huffman_tree::get_number_of_chars() const { return number_of_chars_; }

// builds char's frequency table
void huffman_tree::build_frequency_table(const std::string& filename) {
    char buf;
    number_of_chars_ = 0;
    std::ifstream in(filename);

    while (in.get(buf)) {
        if (chars_frequency_.find(buf) == chars_frequency_.end()) {
            chars_frequency_.insert({buf, 0});
        }
        chars_frequency_[buf] += 1;
        number_of_chars_ += 1;
    }

    alphabet_power_ = chars_frequency_.size();
}

void huffman_tree::build() {
    std::map<char, int>::iterator iter = chars_frequency_.begin();
    for (; iter != chars_frequency_.end(); ++iter) {
        huffman_tree_node* node = new huffman_tree_node(iter->first, iter->second);
        list_of_nodes_.push_back(node);
    }

    // connects two nodes into one parent
    while (list_of_nodes_.size() != 1) {
        list_of_nodes_.sort(node_comparing());
        huffman_tree_node* left_child = list_of_nodes_.front();
        list_of_nodes_.pop_front();
        huffman_tree_node* right_child = list_of_nodes_.front();
        list_of_nodes_.pop_front();

        huffman_tree_node* parent = new huffman_tree_node(left_child, right_child);
        list_of_nodes_.push_back(parent);
    }
    root_ = list_of_nodes_.front();
}

huffman_tree_node* huffman_tree_node::get_left_child() const { return left_child_; }

huffman_tree_node* huffman_tree_node::get_right_child() const { return right_child_; }

char huffman_tree_node::get_symbol() const { return symbol_; }

huffman_tree_node* huffman_tree::get_root() const { return root_; }

std::map<char, std::string> huffman_tree::get_table() const { return table_; }

void huffman_tree::set_alphabet_power(const int value) { alphabet_power_ = value; }

void huffman_tree::add_symbol(const char symbol, const int frequency) { chars_frequency_.insert({symbol, frequency}); }

// builds table with binary codes
void huffman_tree::build_table() { build_code(root_, ""); }

void huffman_tree::build_code(huffman_tree_node* node, std::string code) {
    if (node == nullptr) {
        return;
    }

    if (node == root_ && node->get_right_child() == nullptr && node->get_left_child() == nullptr) {
        table_[node->get_symbol()] = "1";
        return;
    }

    if (node->get_right_child() == nullptr && node->get_left_child() == nullptr) {
        table_[node->get_symbol()] = code;
    }

    build_code(node->get_left_child(), code + "0");
    build_code(node->get_right_child(), code + "1");
}

void huffman_tree::destroy(const huffman_tree_node* start_node) {
    if (start_node == nullptr) {
        return;
    }

    destroy(start_node->get_left_child());
    destroy(start_node->get_right_child());

    delete start_node;
    start_node = nullptr;
}

}  // namespace huffman
