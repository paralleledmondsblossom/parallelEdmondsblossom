#ifndef STACK_H
#define STACK_H

#include <vector>

template <typename IT>
class Stack {
private:
    std::vector<IT> data;
    size_t top;
    size_t capacity;

public:
    // Default Constructor
    Stack();

    // Constructor
    Stack(size_t capacity);

    // Copy constructor
    Stack(const Stack& other);

    // Method to push an element onto the stack
    void push_back(const IT& val);

    // Method to pop an element from the stack
    IT back();
    
    // Method to pop an element from the stack
    size_t size();

    // Method to pop an element from the stack
    bool empty();

    void pop_back();
    // Method to clear the stack
    void clear();

    // Iterator support
    using iterator = typename std::vector<IT>::iterator;
    using const_iterator = typename std::vector<IT>::const_iterator;

    iterator begin() {
        return data.begin();
    }

    const_iterator begin() const {
        return data.begin();
    }

    iterator end() {
        return data.begin() + top;
    }

    const_iterator end() const {
        return data.begin() + top;
    }
};

// Constructor
template <typename IT>
Stack<IT>::Stack() {}

// Constructor
template <typename IT>
Stack<IT>::Stack(size_t capacity) : data(capacity), top(0), capacity(capacity) {}

// Copy constructor
template <typename IT>
Stack<IT>::Stack(const Stack& other) : data(other.data), top(other.top), capacity(other.capacity) {}

// Method to push an element onto the stack
template <typename IT>
size_t Stack<IT>::size() {
    return top;
}


// Method to push an element onto the stack
template <typename IT>
void Stack<IT>::push_back(const IT& val) {
    if (top < capacity) {
        data[top++] = val;
    } else {
        throw std::overflow_error("Stack overflow");
    }
}

// Method to pop an element from the stack
template <typename IT>
IT Stack<IT>::back() {
    if (top)
    return data[top-1];
    else 
    throw std::underflow_error("Stack empty");
}

// Method to pop an element from the stack
template <typename IT>
bool Stack<IT>::empty() {
    return !top;
}

// Method to pop an element from the stack
template <typename IT>
void Stack<IT>::pop_back() {
    if (top > 0) {
        --top;
    } else {
        throw std::underflow_error("Stack underflow");
    }
}

// Method to clear the stack
template <typename IT>
void Stack<IT>::clear() {
    top = 0;
}

#endif // STACK_H
