#pragma once

#include <exception>
#include <iostream>

namespace bmstu {
template <typename T>
class basic_string;

typedef basic_string<char> string;
typedef basic_string<wchar_t> wstring;
typedef basic_string<char16_t> u16string;
typedef basic_string<char32_t> u32string;

template <typename T>
class basic_string {
public:
    basic_string() : ptr_(new T[1]{0}), size_(0) {}
    
    basic_string(size_t size) : ptr_(new T[size + 1]), size_(size) {}
    
    basic_string(std::initializer_list<T> il) 
        : ptr_(new T[il.size() + 1]), size_(il.size()) {}
    
    basic_string(const T* c_str) {}
    basic_string(const basic_string& other) {}
    basic_string(basic_string&& dying) {}
    ~basic_string() {}
    
    const T* c_str() const { return ptr_; }
    size_t size() const { return 0; }
    
    basic_string& operator=(basic_string&& other) { return *this; }
    basic_string& operator=(const T* c_str) { return *this; }
    basic_string& operator=(const basic_string& other) { return *this; }
    
    friend basic_string<T> operator+(const basic_string<T>& left, const basic_string<T>& right) {
        return {};
    }
    
    template <typename S>
    friend S& operator<<(S& os, const basic_string& obj) {
        return os;
    }
    
    template <typename S>
    friend S& operator>>(S& is, basic_string& obj) {
        return is;
    }
    
    basic_string& operator+=(const basic_string& other) { return *this; }
    basic_string& operator+=(T symbol) { return *this; }
    
    T& operator[](size_t index) noexcept { return *(ptr_ + index); }
    T& at(size_t index) { throw std::out_of_range("Wrong index"); }
    T* data() { return ptr_; }

private:
    static size_t strlen_(const T* str) { return 0; }
    void clean_() {}
    
    T* ptr_ = nullptr;
    size_t size_;
};
}