#pragma once
#include <cstddef>

const size_t max_size_init = 3;

template <class T>
class vector {
   public:
    typedef T value_type;
    typedef size_t size_type;

    class iterator {
       public:
        typedef T value_type;
        iterator(value_type* ptr) : ptr_(ptr) {}
        iterator operator++() {
            iterator it = *this;
            ptr_++;
            return it;
        }
        iterator operator++(int) {
            ptr_++;
            return *this;
        }
        iterator operator+(int stride) { return iterator(ptr_ + stride); }
        value_type& operator*() { return *ptr_; }
        value_type* operator->() { return ptr_; }
        value_type* operator&() { return ptr_; }
        bool operator==(const iterator& other) { return ptr_ == other.ptr_; }
        bool operator!=(const iterator& other) { return ptr_ != other.ptr_; }

       private:
        value_type* ptr_;
    };

    class const_iterator {
       public:
        typedef T value_type;
        const_iterator(value_type* ptr) : ptr_(ptr) {}
        const_iterator operator++() {
            const_iterator it = *this;
            ptr_++;
            return it;
        }
        const_iterator operator++(int) {
            ptr_++;
            return *this;
        }
        const_iterator operator+(int stride) {
            return const_iterator(ptr_ + stride);
        }
        const value_type& operator*() { return *ptr_; }
        const value_type* operator->() { return ptr_; }
        const value_type* operator&() { return ptr_; }
        bool operator==(const const_iterator& other) {
            return ptr_ == other.ptr_;
        }
        bool operator!=(const const_iterator& other) {
            return ptr_ != other.ptr_;
        }

       private:
        value_type* ptr_;
    };

    vector(
        size_type = 0,
        const value_type& = value_type());    // vector with 0 and passed size
    vector(value_type*, value_type*);         // constructor from pointers
    vector(const vector<value_type>&);        // copy constructor
    vector(vector<value_type>&&) noexcept;    // move constructor

    /*operators overload*/
    bool operator==(const vector<value_type>&) const;
    bool operator!=(const vector<value_type>&) const;
    bool operator<(const vector<value_type>&) const;
    bool operator>(const vector<value_type>&) const;
    bool operator<=(const vector<value_type>&) const;
    bool operator>=(const vector<value_type>&) const;
    vector<value_type>& operator=(const vector<value_type>&) const;

    /*size manipulations*/
    bool empty() const;            // true if no data in vector
    size_type size() const;        // returns vector size
    size_type capacity() const;    // returns current max capacity
    void reserve(size_type);       // reserve new memory
    size_type max_size() const;    // returns max valid size
    void shrink_to_fit();          // frees unused memory;

    /*data manipulations*/
    void clear();    // clear vector from data
    void push_back(
        const value_type&);    // append value to the end of the vector
    void pop_back();           // deleting the last element
    void resize(size_type,
                const value_type& = value_type());    // resize vector and init
                                                      // with default values
    void erase(vector<T>::iterator);    // erase value on given pointer
    void erase(vector<T>::iterator,
               vector<T>::iterator);    // erase values between pointers
    void insert(size_type,
                const value_type&);    // insert value at given position
    void insert(size_type,
                size_type,
                const value_type&);    // insert values at given position

    /*elements access*/
    value_type& operator[](size_type);
    const value_type& operator[](size_type) const;
    value_type& at(size_type);
    const value_type& at(size_type) const;
    value_type& front();
    const value_type& front() const;
    value_type& back();
    const value_type& back() const;
    value_type* data();                // get pointer on data for change
    const value_type* data() const;    // get pointer on data for read

    const_iterator cbegin() const;
    iterator begin() const;
    const_iterator cend() const;
    iterator end() const;

    ~vector();

   private:
    value_type* data_ptr_;
    size_type size_;
    size_type max_size_;
};
