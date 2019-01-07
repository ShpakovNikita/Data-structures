set(MEM_TREE_SRC
memtree.cpp 
memtree.h
memtree_test.cpp
memtree_test.h)

add_library(memtree ${MEM_TREE_SRC})

message("memtree build")