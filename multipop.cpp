#include <iostream>
#include <stack>

template <typename T>
class MultipopStack {
private:
    std::stack<T> mainStack;
    std::stack<T> auxStack;
    int lazySize; // Number of elements to move from mainStack to auxStack
    
public:
    MultipopStack() : lazySize(0) {}
    
    void push(T value) {
        mainStack.push(value);
    }
    
    void pop() {
        if (mainStack.empty()) {
            std::cout << "Stack is empty!" << std::endl;
            return;
        }
        
        if (auxStack.empty()) {
            lazySize = mainStack.size() / 2; // Move half of the elements
        }
        
        if (auxStack.size() < lazySize) {
            auxStack.push(mainStack.top());
            mainStack.pop();
        } else {
            auxStack.pop();
        }
    }
    
    T top() {
        if (mainStack.empty()) {
            std::cout << "Stack is empty!" << std::endl;
            return T();
        }
        
        if (auxStack.empty()) {
            lazySize = mainStack.size() / 2; // Move half of the elements
        }
        
        if (auxStack.size() < lazySize) {
            auxStack.push(mainStack.top());
            mainStack.pop();
        }
        
        return auxStack.top();
    }
};

int main() {
    MultipopStack<int> stack;
    
    stack.push(1);
    stack.push(2);
    stack.push(3);
    
    std::cout << stack.top() << std::endl; // Output: 3
    
    stack.pop();
    
    std::cout << stack.top() << std::endl; // Output: 2
    
    stack.pop();
    stack.pop();
    stack.pop(); // Output: Stack is empty!
    
    return 0;
}