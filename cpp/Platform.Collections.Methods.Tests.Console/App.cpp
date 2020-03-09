// Platform.Collections.Methods.Tests.Console.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <chrono>

class Assert
{
public:
    template<typename T1, typename T2>
    static void AreEqual(T1 a, T2 b)
    {
        if (a != b)
        {
            throw std::exception("Values are not equal");
        }
    }
};

#include <Platform.Collections.Methods.h>
#include <Trees.h>

using namespace Platform::Collections::Methods::Tests;

int main()
{
    static const int _n = 2000;
    RecursionlessSizeBalancedTree<10000, std::uint32_t> recursionlessSizeBalancedTree;
    SizeBalancedTree<10000, std::uint32_t> sizeBalancedTree;
    SizedAndThreadedAVLBalancedTree<10000, std::uint32_t> avlTree;

    auto t1 = std::chrono::high_resolution_clock::now();
    TestExtensions::TestMultipleCreationsAndDeletions<std::uint32_t>(recursionlessSizeBalancedTree, [&]()-> auto { return recursionlessSizeBalancedTree.Allocate(); }, [&](std::uint32_t link)-> auto { recursionlessSizeBalancedTree.Free(link); }, & recursionlessSizeBalancedTree.Root, [&]()-> auto { return recursionlessSizeBalancedTree.GetCount(); }, _n);
    auto t2 = std::chrono::high_resolution_clock::now();
    auto duration1 = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
    std::cout << duration1 << "ms (in order) for RecursionlessSizeBalancedTreeMethods\n";

    auto t3 = std::chrono::high_resolution_clock::now();
    TestExtensions::TestMultipleCreationsAndDeletions<std::uint32_t>(sizeBalancedTree, [&]()-> auto { return sizeBalancedTree.Allocate(); }, [&](std::uint32_t link)-> auto { sizeBalancedTree.Free(link); }, & sizeBalancedTree.Root, [&]()-> auto { return sizeBalancedTree.GetCount(); }, _n);
    auto t4 = std::chrono::high_resolution_clock::now();
    auto duration2 = std::chrono::duration_cast<std::chrono::milliseconds>(t4 - t3).count();
    std::cout << duration2 << "ms (in order) for SizeBalancedTreeMethods\n";

    auto t5 = std::chrono::high_resolution_clock::now();
    TestExtensions::TestMultipleCreationsAndDeletions<std::uint32_t>(avlTree, [&]()-> auto { return avlTree.Allocate(); }, [&](std::uint32_t link)-> auto { avlTree.Free(link); }, & avlTree.Root, [&]()-> auto { return avlTree.GetCount(); }, _n);
    auto t6 = std::chrono::high_resolution_clock::now();
    auto duration3 = std::chrono::duration_cast<std::chrono::milliseconds>(t6 - t5).count();
    std::cout << duration3 << "ms (in order) for SizedAndThreadedAVLBalancedTreeMethods\n";

    auto t7 = std::chrono::high_resolution_clock::now();
    TestExtensions::TestMultipleRandomCreationsAndDeletions<std::uint32_t>(recursionlessSizeBalancedTree, &recursionlessSizeBalancedTree.Root, [&]()-> auto { return recursionlessSizeBalancedTree.GetCount(); }, _n);
    auto t8 = std::chrono::high_resolution_clock::now();
    auto duration4 = std::chrono::duration_cast<std::chrono::milliseconds>(t8 - t7).count();
    std::cout << duration4 << "ms (random) for RecursionlessSizeBalancedTreeMethods\n";

    auto t9 = std::chrono::high_resolution_clock::now();
    TestExtensions::TestMultipleRandomCreationsAndDeletions<std::uint32_t>(sizeBalancedTree, &sizeBalancedTree.Root, [&]()-> auto { return sizeBalancedTree.GetCount(); }, _n);
    auto t10 = std::chrono::high_resolution_clock::now();
    auto duration5 = std::chrono::duration_cast<std::chrono::milliseconds>(t10 - t9).count();
    std::cout << duration5 << "ms (random) for SizeBalancedTreeMethods\n";

    auto t11 = std::chrono::high_resolution_clock::now();
    TestExtensions::TestMultipleRandomCreationsAndDeletions<std::uint32_t>(avlTree, &avlTree.Root, [&]()-> auto { return avlTree.GetCount(); }, _n);
    auto t12 = std::chrono::high_resolution_clock::now();
    auto duration6 = std::chrono::duration_cast<std::chrono::milliseconds>(t12 - t11).count();
    std::cout << duration6 << "ms (random) for SizedAndThreadedAVLBalancedTreeMethods\n";
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
