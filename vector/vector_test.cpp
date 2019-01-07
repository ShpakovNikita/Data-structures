#include "vector.h"
#include "vector_test.h"

#include <iostream>
#include <assert.h>

void vector_test::constructors_test() {
    // pointers constructor
    int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    vector<int> vec(arr, arr + sizeof(arr) / sizeof(arr[0]));
    for (size_t i = 0; i < vec.size(); ++i) {
        assert(vec[i] == arr[i]);
    }

    // copy constructor is used here
    auto vec2 = vec;
    assert(vec2 == vec);

    vector<int> vec3(vec2);
    assert(vec3 == vec2);

    // move constructor
    auto vec4 = std::move(vec2);
    assert(vec2 != vec4);
    assert(vec3 == vec4);

    // size - default val constructor
    vector<int> vec5(10);
    for (auto it : vec5) {
        assert(it == int());
    }

    vector<int> vec6(12, 2);
    for (auto it : vec6) {
        assert(it == 2);
    }
    std::cerr << "cunstructors_test() is passed" << std::endl;
}

void vector_test::insert_erase_test() {
    int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    vector<int> vec(arr, arr + sizeof(arr) / sizeof(arr[0]));

    vec.insert(4, 55);
    assert(vec[4] == 55);
    assert(vec[5] == 5);

    vector<int> vec2(arr, arr + sizeof(arr) / sizeof(arr[0]));
    vec2.insert(4, 10, 22);
    for (int i = 4; i < 14; ++i) {
        assert(vec2[i] == 22);
    }
    assert(vec2[14] == 5);

    // borders check (and also checking case if the capacity equals to size)
    vector<int> vec3(arr, arr + sizeof(arr) / sizeof(arr[0]));
    vec3.insert(vec3.size(), 10);
    assert(vec3[9] == 10);
    assert(vec3[8] == 9);
    assert(vec3.size() == 10);

    vector<int> vec4(arr, arr + sizeof(arr) / sizeof(arr[0]));
    vec4.insert(0, 10);
    assert(vec4[9] == 9);
    assert(vec4[8] == 8);
    assert(vec4.size() == 10);
    assert(vec4[0] == 10);
    assert(vec4[1] == 1);

    // erase test
    vec4.erase(vec4.begin());
    assert(vec4[0] == 1);
    assert(vec4[1] == 2);

    vec4.erase(vec4.end());
    assert(vec4.size() == 8);
    assert(vec4[7] == 8);

    vec4.erase(vec4.begin() + 1, vec4.begin() + 7);
    int arr2[] = {1, 8};
    for (size_t i = 0; i < vec4.size(); ++i) {
        assert(vec4[i] == arr2[i]);
    }

    std::cerr << "insert_erase_test() is passed" << std::endl;
}

void vector_test::size_manipulations_test() {
    int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    vector<int> vec(arr, arr + sizeof(arr) / sizeof(arr[0]));

    assert(vec.size() == 9);
    assert(vec.capacity() >= vec.size());

    vec.reserve(100);
    assert(vec.size() == 9);
    assert(vec.capacity() == 100);

    vec.resize(50);
    assert(vec.size() == 50);
    assert(vec.capacity() == 100);

    vec.resize(500);
    assert(vec.size() == 500);
    assert(vec.capacity() == 500);

    vec.resize(10);
    assert(vec.size() == 10);
    assert(vec.capacity() == 500);

    vec.shrink_to_fit();
    assert(vec.size() == 10);
    assert(vec.capacity() == 10);

    std::cerr << "size_manipulations_test() is passed" << std::endl;
}

void vector_test::push_pop_test() {
    int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    vector<int> vec(arr, arr + sizeof(arr) / sizeof(arr[0]));

    vec.push_back(10);
    vec.push_back(11);
    assert(vec.size() == 11);
    assert(vec[10] == 11);
    assert(vec[8] == 9);

    vec.pop_back();
    assert(vec.size() == 10);
    assert(vec[9] == 10);

    vec.pop_back();
    vec.pop_back();
    assert(vec.size() == 8);

    std::cerr << "push_pop_test() is passed" << std::endl;
}

void vector_test::compare_test() {
    int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    vector<int> vec1(arr, arr + sizeof(arr) / sizeof(arr[0]));
    vector<int> vec2(arr, arr + sizeof(arr) / sizeof(arr[0]));

    assert(vec1 == vec2);
    vec2.push_back(1);
    assert(vec1 != vec2);

    char arr1[] = {'a', 'b', 'c'};
    vector<char> vec3(arr1, arr1 + sizeof(arr1) / sizeof(arr1[0]));
    vector<char> vec4(arr1, arr1 + sizeof(arr1) / sizeof(arr1[0]));

    assert(vec3 == vec4);

    vec3.push_back('d');
    vec4.push_back('c');
    assert(vec3 > vec4);
    assert(vec4 <= vec3);
    assert(!(vec3 < vec4));
    assert(!(vec4 >= vec3));

    std::cerr << "compare_test() is passed" << std::endl;
}

void vector_test::elements_access_test() {
    int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    vector<int> vec(arr, arr + sizeof(arr) / sizeof(arr[0]));

    assert(vec.at(0) == 1);
    assert(vec[0] == 1);
    assert(vec[8] == 9);

    bool exception_thrown = false;
    try {
        vec[9];
    } catch (const std::out_of_range& err) {
        exception_thrown = true;
    }
    assert(exception_thrown);

    assert(vec.front() == 1);
    assert(vec.back() == 9);

    std::cerr << "elements_access_test() is passed" << std::endl;
}

void vector_test::run() {
    constructors_test();
    insert_erase_test();
    size_manipulations_test();
    push_pop_test();
    compare_test();
    elements_access_test();

    std::cerr << "all vector tests are passed" << std::endl;
}
