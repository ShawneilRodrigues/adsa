#include <iostream>
#include <vector>

class BloomFilter 
{
public:
    static int hash1(int num) 
    {
        return (num * 709) % 512;
    }

    static int hash2(int num) 
    {
        return (num * 739) % 512;
    }

    static int hash3(int num) 
    {
        return (num * 617) % 512;
    }
};

int main() 
{
    std::vector<bool> arr(512, false);
    int y = 0;
    while (y == 0) 
    {
        std::cout << "Enter 1 to add, 2 to check and 3 to exit: ";
        int choice;
        std::cin >> choice;
        
        switch (choice) 
        {
            case 1:
            {
                std::cout << "Enter the number to be added: ";
                int num;
                std::cin >> num;
                arr[BloomFilter::hash1(num)] = true;
                arr[BloomFilter::hash2(num)] = true;
                arr[BloomFilter::hash3(num)] = true;
                break;
            }
            case 2:
            {
                std::cout << "Enter the number which is to be searched: ";
                int num1;
                std::cin >> num1;
                if (!arr[BloomFilter::hash1(num1)] || !arr[BloomFilter::hash2(num1)] || !arr[BloomFilter::hash3(num1)]) 
                {
                    std::cout << "Number not found" << std::endl;
                } 
                else
                {
                    std::cout << "Number found" << std::endl;
                }
                break;
            }
            case 3:
                y = 1;
                break;
        }
    }

    return 0;
}
