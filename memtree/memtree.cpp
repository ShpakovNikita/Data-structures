#include "memtree.h"
#include <fstream>
#include <algorithm>
#include <sstream>
#include <string>

mem_tree::mem_tree(int t) : t_param(t) {
    root_node = new b_node(t, true);
    m_size = 0;
}

mem_tree::mem_tree(b_node* head, int t) : t_param(t), root_node(head) {
    m_size = 0;
}

// Helper function
void delete_node(b_node* node) {
    for (auto child : node->childs) {
        delete_node(child);
    }
    delete (node);
}

mem_tree::~mem_tree() {
    delete_node(root_node);
    std::cerr << "Tree deleted!" << std::endl;
}

void mem_tree::print() {
    print(root_node, 0);
}

uint32_t mem_tree::insert(int key) {
    // If there is filled root - create new one and split current root.
    // We need this not in the insert_inner because if there is filled root
    // it won't create a new root and it is impossible to check if the
    // current node is root node without references on the parent node
    // (more memory use)
    if (root_node->keys.size() == 2 * t_param - 1) {
        b_node* new_root = new b_node(t_param);
        new_root->childs.insert(new_root->childs.begin(), root_node);
        root_node = new_root;
        split_node(root_node, 0);
        return insert_inner(root_node, key);
    } else {
        return insert_inner(root_node, key);
    }
}

uint32_t mem_tree::remove(int key) {
    // If root is null, we don't have to delete any key
    if (!root_node)
        return 1;

    // Run recurrent function to delete key
    remove_inner(root_node, key);
    --m_size;

    // If root has no keys, we remove it. Also if it is not leaf, we make first
    // child as root
    if (!root_node->keys.size()) {
        b_node* temp_node = root_node;
        if (root_node->is_leaf)
            root_node = new b_node(t_param, true);
        else
            root_node = root_node->childs[0];

        delete (temp_node);
    }
    return 0;
}

// This is a shell of private find for user.
uint32_t mem_tree::find(int key) {
    return std::get<2>(find(root_node, key));
}

// Private inner realization of algorithms

// Find key or return error. O(logt(n) * log2(t))
std::tuple<b_node*, int, int> mem_tree::find(b_node* current_node, int key) {
    // Find our key or the key right after it by the value
    auto it = std::lower_bound(current_node->keys.begin(),
                               current_node->keys.end(), key);
    int index = it - current_node->keys.begin();

    // If the key is not found and the node is not leaf, then try to
    // find key in the child node by the index of greater key (it
    // keeps the keys less by value than itself)
    if (*it != key && !current_node->is_leaf) {
        return find(current_node->childs[index], key);
    } else if (*it == key) {
        return {current_node, index, key};
    } else {
        // This is error value. Note, that it is not good to add key with
        // the error code
        return {current_node, index, -1};
    }
}

// Split child node. O(t)
void mem_tree::split_node(b_node* parent_node, int index) {
    // First, we got our node from index, that has to be splitted
    b_node* splitting_node = parent_node->childs[index];

    // Get our center element from the splitting_node and insert it
    // into the parent node
    parent_node->keys.insert(parent_node->keys.begin() + index,
                             splitting_node->keys[t_param - 1]);

    // Right after the reference on the left node (our splitting_node)
    // we need a new reference on the new node. It will be initialized
    // later
    parent_node->childs.insert(parent_node->childs.begin() + index + 1,
                               nullptr);

    b_node* new_node = new b_node(t_param, true);

    // Copy from splitting node all the keys starts with center + 1 position
    // and erase them from the splitting_node
    auto nk_it_first = splitting_node->keys.begin() + t_param;
    auto nk_it_last = splitting_node->keys.end();

    new_node->keys = std::vector<int>(nk_it_first, nk_it_last);
    splitting_node->keys.erase(nk_it_first - 1, nk_it_last);

    // If splitting node is not the leaf node, then do the same things with
    // the childs
    if (!splitting_node->is_leaf) {
        auto nc_it_first = splitting_node->childs.begin() + t_param;
        auto nc_it_last = splitting_node->childs.end();

        new_node->childs = std::vector<b_node*>(nc_it_first, nc_it_last);

        // By default in this method new_node is leaf. But to be careful,
        // we have to mark is as not leaf in this block of code
        new_node->is_leaf = false;
        splitting_node->childs.erase(nc_it_first, nc_it_last);
    }

    // Our nullptr node on parent is now new node
    parent_node->childs[index + 1] = new_node;
}

// Private recursive realization of insert method. This method has to be
// called after the root check and creating a new root node in the case
// if it is full
uint32_t mem_tree::insert_inner(b_node* current_node, int key) {
    // We are trying to add key to the leaf
    if (current_node->is_leaf) {
        auto pos = std::lower_bound(current_node->keys.begin(),
                                    current_node->keys.end(), key);

        // If the key is already in the tree then return error value
        if (*pos == key)
            return 1;

        current_node->keys.insert(pos, key);
        ++m_size;
        return 0;
    } else {
        // Else we are checking if the child node by selected index has to be
        // splitted, and if yes we have to check should we go to the right or
        // to the left (because of lower_bound value changed)
        auto pos = std::lower_bound(current_node->keys.begin(),
                                    current_node->keys.end(), key);
        int index = pos - current_node->keys.begin();

        if (current_node->childs.at(index)->keys.size() == 2 * t_param - 1) {
            split_node(current_node, index);
            if (key > current_node->keys[index]) {
                ++index;
            }
        }

        // Run the recursion again
        return insert_inner(current_node->childs[index], key);
    }
}

uint32_t mem_tree::height() {
    // Just run recurrent loop in depth to find it's height
    b_node* current_node = root_node;
    unsigned height = 1;
    while (!current_node->is_leaf) {
        current_node = current_node->childs[0];
        ++height;
    }

    return height;
}

void mem_tree::print(b_node* current_node, int offset) {
    // Print white spaces to show nesting
    std::cout << std::endl << std::string(offset, ' ');
    std::string line = "";
    for (int i : current_node->keys) {
        line += (std::to_string(i) + " ");
    }

    std::cout << "|" << line << "|";

    // Print recurrent all remaining child nodes of the current node
    if (!current_node->is_leaf) {
        for (b_node* n : current_node->childs) {
            print(n, offset + line.size() + 2);
        }
    }

    // Winner's endl;
    std::cout << std::endl;
}

void mem_tree::remove_inner(b_node* current_node, int key) {
    // Find our key to delete
    auto it = std::lower_bound(current_node->keys.begin(),
                               current_node->keys.end(), key);

    // Index of lower_bound
    int index = it - current_node->keys.begin();

    // If we found our index is less then size (border check) then get it, and
    // if iterator's value is key, then removing it
    if (index < current_node->keys.size() && *it == key) {
        if (current_node->is_leaf) {
            remove_from_leaf(current_node, index);
        } else {
            remove_from_node(current_node, index);
        }
    } else {
        // We dont found it
        if (current_node->is_leaf) {
            // The key doesn't exist
            return;
        }

        // This flag is to remember, if current index is pointing on the last
        // child
        bool end_iter = ((index == current_node->keys.size()) ? true : false);

        // If it has less then t (t - 1), then we fill child
        if (current_node->childs[index]->keys.size() < t_param) {
            fill_child(current_node, index);
        }

        // If we have border index, remove one from right, else remove by
        // default
        if (end_iter && index > current_node->keys.size()) {
            remove_inner(current_node->childs[index - 1], key);
        } else {
            remove_inner(current_node->childs[index], key);
        }
    }
}

void mem_tree::fill_child(b_node* current_node, int index) {
    // This function fills the child. We get key from left or right
    if (index > 0 && current_node->childs[index - 1]->keys.size() >= t_param) {
        get_key_prev(current_node, index);
    } else if (index < current_node->keys.size() &&
               current_node->childs[index + 1]->keys.size() >= t_param) {
        get_key_next(current_node, index);
    } else {
        if (index != current_node->keys.size()) {
            merge_node(current_node, index);
        } else {
            merge_node(current_node, index - 1);
        }
    }
}

void mem_tree::get_key_prev(b_node* current_node, int index) {
    // get key from left node
    b_node* child_node = current_node->childs[index];
    b_node* left_node = current_node->childs[index - 1];

    child_node->keys.insert(child_node->keys.begin(),
                            current_node->keys[index - 1]);
    if (!child_node->is_leaf) {
        child_node->childs.insert(child_node->childs.begin(),
                                  left_node->childs[left_node->keys.size()]);
        left_node->childs.pop_back();
    }

    current_node->keys[index - 1] = *(left_node->keys.end() - 1);
    left_node->keys.pop_back();
}

void mem_tree::get_key_next(b_node* current_node, int index) {
    // get key from right node
    b_node* child_node = current_node->childs[index];
    b_node* right_node = current_node->childs[index + 1];

    child_node->keys.push_back(current_node->keys[index]);
    if (!child_node->is_leaf) {
        child_node->childs.push_back(*right_node->childs.begin());
        right_node->childs.erase(right_node->childs.begin());
    }

    current_node->keys[index] = right_node->keys[0];
    right_node->keys.erase(right_node->keys.begin());
}

void mem_tree::merge_node(b_node* current_node, int index) {
    // get right child, then merge them with the child by current index and move
    // our "parent" key to it
    b_node* child_node = current_node->childs[index];
    b_node* right_node = current_node->childs[index + 1];

    child_node->keys.insert(
        child_node->keys.begin() + t_param - 1,
        *current_node->keys.erase(current_node->keys.begin() + index));

    child_node->keys.insert(child_node->keys.end(), right_node->keys.begin(),
                            right_node->keys.end());

    if (!child_node->is_leaf) {
        child_node->childs.insert(child_node->childs.end(),
                                  right_node->childs.begin(),
                                  right_node->childs.end());
    }
    current_node->childs.erase(current_node->childs.begin() + index + 1);

    delete (right_node);
}

void mem_tree::remove_from_node(b_node* current_node, int index) {
    // We check left and right child to move key to them or merge siblings and
    // move parent key to it
    int key = current_node->keys[index];

    if (current_node->childs[index]->keys.size() >= t_param) {
        int pred_key = get_pred_key(current_node, index);
        current_node->keys[index] = pred_key;
        remove_inner(current_node->childs[index], pred_key);
    }

    else if (current_node->childs[index + 1]->keys.size() >= t_param) {
        int succ_key = get_succ_key(current_node, index);
        current_node->keys[index] = succ_key;
        remove_inner(current_node->childs[index + 1], succ_key);
    }

    else {
        merge_node(current_node, index);
        remove_inner(current_node->childs[index], key);
    }
}

void mem_tree::remove_from_leaf(b_node* current_node, int index) {
    // Just erase it
    current_node->keys.erase(current_node->keys.begin() + index);
}

int mem_tree::get_succ_key(b_node* current_node, int index) {
    // Just visit the most left key in right child
    current_node = current_node->childs[index + 1];
    while (!current_node->is_leaf) {
        current_node = current_node->childs[0];
    }

    return current_node->keys[0];
}

int mem_tree::get_pred_key(b_node* current_node, int index) {
    // Just visit the most right key in left child
    current_node = current_node->childs[index];
    while (!current_node->is_leaf) {
        current_node = current_node->childs[current_node->keys.size()];
    }

    return current_node->keys[current_node->keys.size() - 1];
}
