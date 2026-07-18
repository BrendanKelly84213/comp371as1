#include <cassert>
#include <print>
#include <iostream>


/**
 * Creates an empty uninitialized int array
 * @param len Length of the array to create
 * @return The newly created array
 */
int* createArray(const size_t len)
{
    return new int[len];
}

/**
 * Initializes an int array with an incrementing sequence of numbers
 * @param arr The array to initialize. Must be non-null
 * @param len The length of #arr
 */
void initializeArray(int arr[], const size_t len)
{
    assert(arr != nullptr);

    for (auto i = 0; i < len; ++i)
    {
        arr[i] = i;
    }
}

/**
 * Prints out the contents of an int array, delimited by spaces
 * @param arr The array to print out. Must be non-null
 * @param len The length #arr
 */
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

/**
 * Deletes an array created using createArray
 * @param arr The array to delete
 */
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
