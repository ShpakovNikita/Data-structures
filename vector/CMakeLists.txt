set(VECTOR_SRC
vector.cpp 
vector.h
vector_test.cpp
vector_test.h)

add_library(vector ${VECTOR_SRC})

target_link_libraries(vector memtree)

message("vector build and memtree linked")