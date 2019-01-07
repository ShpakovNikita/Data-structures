#include "memtree.h"
#include "memtree_test.h"

#include <iostream>
#include <assert.h>

void memtree_test::insert_test() {
    mem_tree tree1(3);
    for (int i = 0; i < 100000; ++i) {
        tree1.insert(rand() % 10000);
    }

    mem_tree tree2(100);
    for (int i = 0; i < 100000; ++i) {
        tree2.insert(i);
    }

    assert(tree2.insert(1) != 0);
    assert(tree2.insert(100001) == 0);

    assert(tree2.size() == 100000);
}

void memtree_test::remove_test() {
    mem_tree tree(3);

    assert(tree.size() == 0);

    tree.insert(1);
    tree.insert(2);
    tree.insert(5);

    assert(tree.size() == 3);

    assert(tree.remove(3) != 0);
    assert(tree.remove(5) == 0);
    assert(tree.remove(5) != 0);

    assert(tree.size() == 2);

    assert(tree.remove(1) == 0);
    assert(tree.remove(2) == 0);

    assert(tree.size() == 0);

    for (int i = 0; i < 100; ++i) {
        tree.insert(i);
    }

    assert(tree.size() == 100);
    assert(tree.remove(2) != 0);
    assert(tree.remove(2) == 0);
}

void memtree_test::find_test() {
    mem_tree tree(3);

    for (int i = 0; i < 100; ++i) {
        tree.insert(i);
    }

    tree.find(2);
    tree.find(3);

    assert(tree.find(2) == 2);
    assert(tree.find(10) == 10);
    assert(tree.find(1000) != 1000);
}

void memtree_test::run() {
    insert_test();
    remove_test();
    find_test();

    std::cerr << "all memtree tests are passed" << std::endl;
}
