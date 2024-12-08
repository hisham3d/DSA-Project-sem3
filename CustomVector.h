#pragma once
#include <iostream>
#include <stdexcept>
using namespace std;

template <typename T>
class CustomVector {
private:
    T* data;
    int capacity;
    int size;

    void expand() {
        capacity = (capacity == 0) ? 1 : capacity * 2;
        T* newData = new T[capacity];
        for (int i = 0; i < size; ++i) {
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
        for (int i = 0; i < size; ++i) {
            data[i] = other.data[i];
        }
    }

    void push_back(const T& value) {
        if (size == capacity) {
            expand();
        }
        data[size++] = value;
    }

    int getSize() const {
        return size;
    }

    int getCapacity() const {
        return capacity;
    }

    T& operator[](int index) {
        if (index >= size) {
            throw std::out_of_range("Index out of range");
        }
        return data[index];
    }

    const T& operator[](int index) const {
        if (index >= size) {
            throw std::out_of_range("Index out of range");
        }
        return data[index];
    }

    void clear() {
        size = 0;
    }

    void resize(int newSize, const T& defaultValue = T()) {
        if (newSize > capacity) {
            while (capacity < newSize) {
                expand();
            }
        }
        for (int i = size; i < newSize; ++i) {
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

    template <typename... Args>
    void emplace_back(Args&&... args) {
        if (size == capacity) {
            expand();
        }
        data[size++] = T(std::forward<Args>(args)...);
    }

    bool empty() const {
        return size == 0;
    }

    T& back() {
        if (empty()) {
            throw std::out_of_range("Vector is empty");
        }
        return data[size - 1];
    }

    const T& back() const {
        if (empty()) {
            throw std::out_of_range("Vector is empty");
        }
        return data[size - 1];
    }
    void erase(int index) {
        if (index >= size) {
            throw std::out_of_range("Index out of bounds");
        }

        // Shift elements to the left
        for (int i = index; i < size - 1; ++i) {
            data[i] = data[i + 1];
        }

        // Decrease size
        --size;
    }

};