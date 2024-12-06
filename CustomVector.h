#pragma once
#include <iostream>
#include <stdexcept>
using namespace std;

template <typename T>
class CustomVector {
private:
    T* data;
    size_t capacity;
    size_t size;

    void expand() {
        capacity = (capacity == 0) ? 1 : capacity * 2;
        T* newData = new T[capacity];
        for (size_t i = 0; i < size; ++i) {
            newData[i] = std::move(data[i]); // Use move semantics to avoid copying
        }
        delete[] data;
        data = newData;
    }

public:
    CustomVector() : data(nullptr), capacity(0), size(0) {}

    ~CustomVector() {
        delete[] data;
    }

    CustomVector(const CustomVector& other)
        : data(new T[other.capacity]), capacity(other.capacity), size(other.size) {
        for (size_t i = 0; i < size; ++i) {
            data[i] = other.data[i];
        }
    }

    void push_back(const T& value) {
        if (size == capacity) {
            expand();
        }
        data[size++] = value;
    }

    size_t getSize() const {
        return size;
    }

    size_t getCapacity() const {
        return capacity;
    }

    T& operator[](size_t index) {
        if (index >= size) {
            throw std::out_of_range("Index out of range");
        }
        return data[index];
    }

    const T& operator[](size_t index) const {
        if (index >= size) {
            throw std::out_of_range("Index out of range");
        }
        return data[index];
    }

    void clear() {
        size = 0;
    }

    void resize(size_t newSize, const T& defaultValue = T()) {
        if (newSize > capacity) {
            while (capacity < newSize) {
                expand();
            }
        }
        for (size_t i = size; i < newSize; ++i) {
            data[i] = defaultValue;
        }
        size = newSize;
    }

    T* begin() {
        return data;
    }

    T* end() {
        return data + size;
    }

    const T* begin() const {
        return data;
    }

    const T* end() const {
        return data + size;
    }
};