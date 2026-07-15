#include <cassert>
#include <print>
#include <iostream>


/**
 *
 * @param len Lenth
 * @return
 */
int* createArray(const size_t len)
{
    return new int[len];
}

void initializeArray(int arr[], const size_t len)
{
    assert(arr != nullptr);

    for (size_t i = 0; i < len; ++i)
    {
        arr[i] = i;
    }
}

void printArray(const int arr[], const size_t len)
{
    assert(arr != nullptr);

    std::print("Array contents: ");
    for (size_t i = 0; i < len; ++i)
    {
        std::print("{} ", arr[i]);
    }
    std::println();
}

void deleteArray(const int arr[])
{
    delete[] arr;
}

int main()
{
    size_t len;
    std::print("arr length: ");
    std::cin >> len;
    std::println();

    int* arr = createArray(len);

    initializeArray(arr, len);

    printArray(arr, len);

    deleteArray(arr);
    return 0;
}
