#ifndef HUFFMAN_TREE_H
#define HUFFMAN_TREE_H

#include <list>
#include <map>
#include <string>

namespace huffman {

class huffman_tree_node {
public:
    huffman_tree_node(char symbol, int frequency);
    huffman_tree_node(huffman_tree_node* left_child, huffman_tree_node* right_child);

    [[nodiscard]] int get_frequency() const;
    [[nodiscard]] char get_symbol() const;
    [[nodiscard]] huffman_tree_node* get_left_child() const;
    [[nodiscard]] huffman_tree_node* get_right_child() const;

private:
    char symbol_;
    int frequency_;
    huffman_tree_node* left_child_;
    huffman_tree_node* right_child_;
};

class huffman_tree {
public:
    void build();
    void build_table();
    void build_code(huffman_tree_node* node, std::string code);
    void build_frequency_table(const std::string& filename);

    [[nodiscard]] huffman_tree_node* get_root() const;
    [[nodiscard]] int get_alphabet_power() const;
    [[nodiscard]] std::map<char, int> get_chars_frequency() const;
    [[nodiscard]] std::map<char, std::string> get_table() const;
    [[nodiscard]] int get_number_of_chars() const;

    void set_alphabet_power(const int value);
    void add_symbol(const char symbol, const int frequency);

    void destroy(const huffman_tree_node* start_node);

private:
    huffman_tree_node* root_;

    std::map<char, int> chars_frequency_;
    std::map<char, std::string> table_;
    std::list<huffman_tree_node*> list_of_nodes_;
    int alphabet_power_;
    int number_of_chars_;
};

struct node_comparing {
    bool operator()(const huffman_tree_node* node1, const huffman_tree_node* node2) const {
        return node1->get_frequency() < node2->get_frequency();
    }
};
}  // namespace huffman

#endif
