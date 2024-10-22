#include <iostream>
#include <vector>
#include <functional>

class BloomFilter {
public:
    BloomFilter(size_t size, size_t numHashes)
        : bits(size), numHashes(numHashes) {}

    void add(const std::string& item) {
        for (size_t i = 0; i < numHashes; ++i) {
            bits[hash(item, i) % bits.size()] = true;
        }
    }

    bool possiblyContains(const std::string& item) const {
        for (size_t i = 0; i < numHashes; ++i) {
            if (!bits[hash(item, i) % bits.size()]) {
                return false;
            }
        }
        return true;
    }

private:
    std::vector<bool> bits;
    size_t numHashes;

    size_t hash(const std::string& item, size_t i) const {
        return std::hash<std::string>{}(item) + i * std::hash<size_t>{}(i);
    }
};

int main() {
    BloomFilter filter(1000, 7);

    filter.add("hello");
    filter.add("world");

    std::cout << "Possibly contains 'hello': " << filter.possiblyContains("hello") << std::endl;
    std::cout << "Possibly contains 'world': " << filter.possiblyContains("world") << std::endl;
    std::cout << "Possibly contains 'test': " << filter.possiblyContains("test") << std::endl;

    return 0;
}