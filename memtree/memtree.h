#pragma once

#include <vector>
#include <iostream>
#include <tuple>
#include "stdint.h"

struct b_node {
    std::vector<int> keys;
    std::vector<b_node*> childs;

    b_node(int t, bool _is_leaf = false) {
        keys.reserve(t);
        if (_is_leaf) {
            is_leaf = true;
            return;
        }

        childs.reserve(t + 1);
    }

    b_node() {}

    ~b_node() {
        if (!childs.empty()) {
            childs.erase(childs.begin(), childs.end());
        }
        if (!keys.empty()) {
            keys.erase(keys.begin(), keys.end());
        }
        // std::cerr << "b_node cleared" << std::endl;
    }

    bool is_leaf = false;    // TODO: maybe true
};

class mem_tree {
   public:
    mem_tree(int);
    mem_tree(b_node*, int);
    virtual ~mem_tree();

    // Print recursively tree
    void print();

    // Insert key to the tree. Return error code if there is already that key
    // O(t*logt(n)*log2(t) + n)
    uint32_t insert(int key);

    // Remove key from the tree. Really complex method, and it's asymptotic is
    // about O(t*logt(n)*log2(t) + n)
    uint32_t remove(int key);

    // Find key in the tree or return NULL_KEY, O(logt(n))
    uint32_t find(int key);

    size_t size() { return m_size; }
    bool empty() { return !m_size; }

   private:
    // This is defined T value for tree, need for the algorithm
    int t_param;
    size_t m_size;
    b_node* root_node;

    // Tuple need for remove operation. The first element is the current node,
    // the second is the index of the key, the last one is the key himself
    std::tuple<b_node*, int, int> find(b_node*, int);

    // Tree height. O(logt(n))
    uint32_t height();

    // This function runs the recursion from insert method
    uint32_t insert_inner(b_node*, int);

    // This function runs the recursion from print(int) method
    void print(b_node*, int);

    // This function runs the recursion from remove method
    void remove_inner(b_node*, int key);

    // Further functions has to be runned inside remove_inner method. After all
    // changes and rebuilding the tree we choose to delete from leaf or not,
    // depends on the node where our key has been found
    void remove_from_leaf(b_node*, int key);
    void remove_from_node(b_node*, int key);

    // Fill the child trying to take key from the siblings, or if not, trying to
    // merge nodes decrementing the tree's height
    void fill_child(b_node*, int index);

    // Borrow key form the right or left sibling, if they have more than t - 1
    // keys
    void get_key_prev(b_node*, int index);
    void get_key_next(b_node*, int index);

    // Decrement tree's height and merge current node with the left node and
    // delete current
    void merge_node(b_node*, int index);

    // Get next or previous by value key
    int get_succ_key(b_node*, int index);
    int get_pred_key(b_node*, int index);

    // Split node on two nodes
    void split_node(b_node*, int);
};
