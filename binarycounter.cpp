#include <iostream>
#include <vector>


int incrementCounter(std::vector<int>& counter) {
    int i = 0;
    int flipCount = 0;

    
    while (i < counter.size() && counter[i] == 1) {
        counter[i] = 0;
        ++i;
        ++flipCount; 
    }
    if (i < counter.size()) {
        counter[i] = 1;
        ++flipCount; 
    }

    return flipCount;
}


void displayCounter(const std::vector<int>& counter) {
    for (int i = counter.size() - 1; i >= 0; --i) {
        std::cout << counter[i];
    }
    std::cout << std::endl;
}

int main() {
    int k; 
    std::cout << "Enter the number of bits (k) for the counter: ";
    std::cin >> k;

    
    std::vector<int> counter(k, 0);

    int numIncrements;
    std::cout << "Enter the number of increments: ";
    std::cin >> numIncrements;

    int totalFlips = 0;

    std::cout << "Initial Counter: ";
    displayCounter(counter);

    for (int i = 0; i < numIncrements; ++i) {
        int flips = incrementCounter(counter);
        totalFlips += flips;
        std::cout << "After Incrementing: ";
        displayCounter(counter);
        std::cout << "Flips in this increment: " << flips << std::endl;
    }

    
    double amortizedCost = static_cast<double>(totalFlips) / numIncrements;
    std::cout << "Total number of bit flips: " << totalFlips << std::endl;
    std::cout << "Amortized cost per increment: " << amortizedCost << std::endl;

    return 0;
}
