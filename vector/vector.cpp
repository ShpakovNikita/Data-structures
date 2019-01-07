#include "vector.h"

#include <algorithm>
#include <iostream>
#include <assert.h>

template <class T>
std::ostream& operator<<(std::ostream& out, const vector<T>& vec) {
    for (const auto& it : vec) {
        out << it << " ";
    }
    return out;
}

size_t size_max(size_t a, size_t b) {
    if (a > b) {
        return a;
    } else {
        return b;
    }
}

template <class T>
bool vector<T>::empty() const {
    return !size_;
}

template <class T>
size_t vector<T>::size() const {
    return size_;
}

template <class T>
size_t vector<T>::capacity() const {
    return max_size_;
}

template <class T>
void vector<T>::shrink_to_fit() {
    max_size_ = size_max(size_, max_size_init);
    T* temp = new T[max_size_];
    std::copy(data_ptr_, data_ptr_ + size_, temp);
    delete data_ptr_;
    data_ptr_ = temp;
}

template <class T>
void vector<T>::reserve(size_t count) {
    // yes, technically this is impossible
    if (count > max_size()) {
        throw std::length_error("too big value");
    }

    if (count > max_size_) {
        max_size_ = count;
        T* temp = new T[max_size_];
        std::copy(data_ptr_, data_ptr_ + size_, temp);
        delete data_ptr_;
        data_ptr_ = temp;
    }
}

template <class T>
size_t vector<T>::max_size() const {
    return static_cast<size_t>(-1);
}

template <class T>
vector<T>::vector(size_t count, const T& init_val) : size_(count) {
    max_size_ = size_max(size_, max_size_init);
    data_ptr_ = new T[max_size_];
    std::fill(data_ptr_, data_ptr_ + size_, init_val);
}

template <class T>
vector<T>::vector(const vector<T>& other)
    : size_(other.size()), max_size_(other.capacity()) {
    data_ptr_ = new T[max_size_];
    std::copy(other.data(), other.data() + size_, data_ptr_);
}

template <class T>
vector<T>::vector(vector<T>&& other) noexcept
    : size_(other.size()), max_size_(other.capacity()) {
    data_ptr_ = std::move(other.data());
    other.size_ = 0;
    other.data_ptr_ = nullptr;
}

template <class T>
vector<T>::vector(T* first, T* last) {
    size_t stride = 0;
    for (T *it = first; it != last; ++it, ++stride) {
    }
    size_ = stride;
    max_size_ = size_max(size_, max_size_init);
    data_ptr_ = new T[max_size_];

    int i = 0;
    for (T *it = first; it != last; ++it, ++i) {
        data_ptr_[i] = *it;
    }
}

template <class T>
T& vector<T>::operator[](size_t index) {
    if (index >= size_) {
        throw std::out_of_range("index out of range");
    }
    return data_ptr_[index];
}

template <class T>
const T& vector<T>::operator[](size_t index) const {
    if (index >= size_) {
        throw std::out_of_range("index out of range");
    }
    return data_ptr_[index];
}

template <class T>
T& vector<T>::at(size_t index) {
    return (*this)[index];
}

template <class T>
const T& vector<T>::at(size_t index) const {
    return (*this)[index];
}

template <class T>
T& vector<T>::front() {
    return (*this)[0];
}

template <class T>
const T& vector<T>::front() const {
    return (*this)[0];
}

template <class T>
T& vector<T>::back() {
    return (*this)[size_ - 1];
}

template <class T>
const T& vector<T>::back() const {
    return (*this)[size_ - 1];
}

template <class T>
bool vector<T>::operator==(const vector<T>& other) const {
    if (other.size() != size_) {
        return false;
    }

    for (size_t i = 0; i < size_; ++i) {
        if (data_ptr_[i] != other[i]) {
            return false;
        }
    }

    return true;
}

template <class T>
bool vector<T>::operator!=(const vector<T>& other) const {
    return !(*this == other);
}

template <class T>
bool vector<T>::operator<(const vector<T>& other) const {
    return std::lexicographical_compare(data_ptr_, data_ptr_ + size_,
                                        other.data(),
                                        other.data() + other.size());
}

template <class T>
bool vector<T>::operator>=(const vector<T>& other) const {
    return !(*this < other);
}

template <class T>
bool vector<T>::operator>(const vector<T>& other) const {
    return std::lexicographical_compare(other.data(),
                                        other.data() + other.size(), data_ptr_,
                                        data_ptr_ + size_);
}

template <class T>
bool vector<T>::operator<=(const vector<T>& other) const {
    return !(*this > other);
}

template <class T>
vector<T>& vector<T>::operator=(const vector<T>& other) const {
    *this = vector<T>(other);
    return *this;
}

template <class T>
T* vector<T>::data() {
    return data_ptr_;
}

template <class T>
const T* vector<T>::data() const {
    return data_ptr_;
}

template <class T>
void vector<T>::push_back(const T& value) {
    if (size_ >= max_size_) {
        reserve(max_size_ * 2);
    }

    data_ptr_[size_] = value;
    size_ += 1;
}

template <class T>
void vector<T>::pop_back() {
    if (size_ <= 0) {
        throw std::out_of_range("trying to pop empty");
    }
    size_ -= 1;
}

template <class T>
void vector<T>::resize(size_t count, const T& init_val) {
    if (size_ < count) {
        if (count > max_size_) {
            reserve(count);
        }
        std::fill(data_ptr_ + size_, data_ptr_ + count, init_val);
    }
    size_ = count;
}

template <class T>
void vector<T>::clear() {
    size_ = 0;
}

template <class T>
void vector<T>::insert(size_t index, size_t count, const T& init_val) {
    if (size_ + count > max_size_) {
        reserve(size_max(max_size_ * 2, size_ + count));
    }
    if (index > size_) {
        throw std::out_of_range("out of range");
    }

    for (size_t i = size_ + count - 1; i > index; --i) {
        data_ptr_[i] = data_ptr_[i - count];
    }
    std::fill(data_ptr_ + index, data_ptr_ + index + count, init_val);
    size_ += count;
}

template <class T>
void vector<T>::insert(size_t index, const T& value) {
    insert(index, 1, value);
}

template <class T>
void vector<T>::erase(vector<T>::iterator first, vector<T>::iterator second) {
    size_t stride = 0;
    for (vector<T>::iterator it = first; it != second; ++it, ++stride) {
    }

    if (first != end()) {
        for (vector<T>::iterator it = first; it + stride != end(); ++it) {
            *it = *(it + stride);
        }
    }

    size_ -= stride;
}

template <class T>
void vector<T>::erase(vector<T>::iterator pos) {
    erase(pos, pos + 1);
}

template <class T>
typename vector<T>::const_iterator vector<T>::cbegin() const {
    return const_iterator(data_ptr_);
}

template <class T>
typename vector<T>::iterator vector<T>::begin() const {
    return iterator(data_ptr_);
}

template <class T>
typename vector<T>::const_iterator vector<T>::cend() const {
    return const_iterator(data_ptr_ + size_);
}

template <class T>
typename vector<T>::iterator vector<T>::end() const {
    return iterator(data_ptr_ + size_);
}

template <class T>
vector<T>::~vector() {
    delete[] data_ptr_;
}
