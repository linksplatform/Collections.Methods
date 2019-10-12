#include <functional>
#include <iostream>
#include <unordered_set>
#include <chrono>

bool iszero(void* ptr, int bytes)
{
    char* bptr = (char*)ptr;
    while (bytes--)
    {
        if (*bptr++)
        {
            return false;
        }
    }
    return true;
}

class Assert
{
public:
    template<typename T1, typename T2>
    static void Equal(T1 a, T2 b)
    {
        if (a != b)
        {
            throw std::exception("Values are not equal");
        }
    }
};

#include <Platform.Collections.Methods.h>

#include <SizeBalancedTree.cpp>
#include <SizeBalancedTree2.cpp>
#include <SizedAndThreadedAVLBalancedTree.cpp>
#include <TestExtensions.cpp>
#include <TreesTests.cpp>

int main()
{
    auto t1 = std::chrono::high_resolution_clock::now();
    Platform::Collections::Methods::Tests::TreesTests::SizeBalancedTreeMultipleAttachAndDetachTest();
    auto t2 = std::chrono::high_resolution_clock::now();
    auto duration1 = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
    std::cout << duration1 << "ms (in order) for SizeBalancedTreeMethods\n";

    auto t3 = std::chrono::high_resolution_clock::now();
    Platform::Collections::Methods::Tests::TreesTests::SizeBalancedTree2MultipleAttachAndDetachTest();
    auto t4 = std::chrono::high_resolution_clock::now();
    auto duration2 = std::chrono::duration_cast<std::chrono::milliseconds>(t4 - t3).count();
    std::cout << duration2 << "ms (in order) for SizeBalancedTreeMethods2\n";

    auto t5 = std::chrono::high_resolution_clock::now();
    Platform::Collections::Methods::Tests::TreesTests::SizedAndThreadedAVLBalancedTreeMultipleAttachAndDetachTest();
    auto t6 = std::chrono::high_resolution_clock::now();
    auto duration3 = std::chrono::duration_cast<std::chrono::milliseconds>(t6 - t5).count();
    std::cout << duration3 << "ms (in order) for SizedAndThreadedAVLBalancedTreeMethods\n";

    auto t7 = std::chrono::high_resolution_clock::now();
    Platform::Collections::Methods::Tests::TreesTests::SizeBalancedTreeMultipleRandomAttachAndDetachTest();
    auto t8 = std::chrono::high_resolution_clock::now();
    auto duration4 = std::chrono::duration_cast<std::chrono::milliseconds>(t8 - t7).count();
    std::cout << duration4 << "ms (random) for SizeBalancedTreeMethods\n";

    auto t9 = std::chrono::high_resolution_clock::now();
    Platform::Collections::Methods::Tests::TreesTests::SizeBalancedTree2MultipleRandomAttachAndDetachTest();
    auto t10 = std::chrono::high_resolution_clock::now();
    auto duration5 = std::chrono::duration_cast<std::chrono::milliseconds>(t10 - t9).count();
    std::cout << duration5 << "ms (random) for SizeBalancedTreeMethods2\n";

    auto t11 = std::chrono::high_resolution_clock::now();
    Platform::Collections::Methods::Tests::TreesTests::SizedAndThreadedAVLBalancedTreeMultipleRandomAttachAndDetachTest();
    auto t12 = std::chrono::high_resolution_clock::now();
    auto duration6 = std::chrono::duration_cast<std::chrono::milliseconds>(t12 - t11).count();
    std::cout << duration6 << "ms (random) for SizedAndThreadedAVLBalancedTreeMethods\n";
}