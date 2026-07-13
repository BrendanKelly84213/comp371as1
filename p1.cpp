#include <iostream>

template <typename T>
class DynamicArray {
    public:
        DynamicArray(size_t len) : m_len(len)
        {
           m_data = new T[len]; 
        }

        ~DynamicArray()
        {
            delete m_data;
        }

        T* ptr() { return m_data; }
    private:
        T* m_data;
        size_t m_len;
};


int* createArray(size_t len) {
    auto arr = new int[len]; 
    return arr;
}

int initializeArray(int* arr, size_t len) {
    if (arr == nullptr) {
        return -1;
    }

    for (size_t i = 0; i < len; ++i) {
        arr[i] = i;
    }
    return 0;
}

int printArray(int* arr, size_t len) {
    if (arr == nullptr) {
        return -1;
    }
    for (size_t i = 0; i < len; ++i) {
        std::cout << arr[i] << '\n'; 
    }
    return 0;
}

void deleteArray(int* arr) {
   delete[] arr; 
}

int main()
{
    size_t len;
    std::cout << "arr length: ";
    std::cin >> len;
    std::cout << '\n';
    
    int* arr = createArray(len);
    if (arr == nullptr) {
        std::cout << "arr is null failed to initialize\n";
        return 1;
    }
    int res = initializeArray(arr, len);
    if (res == -1) {
        std::cout << "arr is null could not initialize\n";
        return 1;
    }

    res = printArray(arr, len);  
    if (res == -1) {
        std::cout << "arr is null could not initialize\n";
        return 1;
    }

    deleteArray(arr);
    return 0;
}
