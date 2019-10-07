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

class Program
{
public:
    static void Main(char* args[])
    {
        printf("Hello, world!\n");
    }
};

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

#include "GenericCollectionMethodsBase.cpp"

#include "DoublyLinkedListMethodsBase.cpp"
#include "CircularDoublyLinkedListMethods.cpp"
#include "OpenDoublyLinkedListMethods.cpp"

#include "SizedBinaryTreeMethodsBase.cpp"
#include "SizeBalancedTreeMethods.cpp"
#include "SizeBalancedTreeMethods2.cpp"
#include "SizedAndThreadedAVLBalancedTreeMethods.cpp"

#include "SizeBalancedTree.cpp"
#include "SizeBalancedTree2.cpp"
#include "SizedAndThreadedAVLBalancedTree.cpp"
#include "TestExtensions.cpp"
//#include "TreesTests.cpp"

int main()
{
    static Platform::Collections::Methods::Tests::SizeBalancedTree<std::uint32_t, 10000> sizeBalancedTree;
    struct sbtWrappers {
        static std::uint32_t Allocate()
        {
            return sizeBalancedTree.Allocate();
        }
        static void Free(std::uint32_t link)
        {
            return sizeBalancedTree.Free(link);
        }
        static std::uint32_t GetCount()
        {
            return sizeBalancedTree.GetCount();
        }
    };
    static Platform::Collections::Methods::Tests::SizeBalancedTree2<std::uint32_t, 10000> sizeBalancedTree2;
    struct sbt2Wrappers {
        static std::uint32_t Allocate()
        {
            return sizeBalancedTree2.Allocate();
        }
        static void Free(std::uint32_t link)
        {
            return sizeBalancedTree2.Free(link);
        }
        static std::uint32_t GetCount()
        {
            return sizeBalancedTree2.GetCount();
        }
    };
    static Platform::Collections::Methods::Tests::SizedAndThreadedAVLBalancedTree<std::uint32_t, 10000> avlTree;
    struct avlWrappers {
        static std::uint32_t Allocate()
        {
            return avlTree.Allocate();
        }
        static void Free(std::uint32_t link)
        {
            return avlTree.Free(link);
        }
        static std::uint32_t GetCount()
        {
            return avlTree.GetCount();
        }
    };

    auto n = 2000;

    auto t1 = std::chrono::high_resolution_clock::now();
    Platform::Collections::Methods::Tests::TestExtensions::TestMultipleCreationsAndDeletions<std::uint32_t>(sizeBalancedTree, &sbtWrappers::Allocate, &sbtWrappers::Free, &sizeBalancedTree.Root, &sbtWrappers::GetCount, n);
    auto t2 = std::chrono::high_resolution_clock::now();
    auto duration1 = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
    std::cout << duration1 << "ms (in order) for SizeBalancedTreeMethods\n";

    auto t3 = std::chrono::high_resolution_clock::now();
    Platform::Collections::Methods::Tests::TestExtensions::TestMultipleCreationsAndDeletions<std::uint32_t>(sizeBalancedTree2, &sbt2Wrappers::Allocate, &sbt2Wrappers::Free, &sizeBalancedTree2.Root, &sbt2Wrappers::GetCount, n);
    auto t4 = std::chrono::high_resolution_clock::now();
    auto duration2 = std::chrono::duration_cast<std::chrono::milliseconds>(t4 - t3).count();
    std::cout << duration2 << "ms (in order) for SizeBalancedTreeMethods2\n";

    auto t5 = std::chrono::high_resolution_clock::now();
    Platform::Collections::Methods::Tests::TestExtensions::TestMultipleCreationsAndDeletions<std::uint32_t>(avlTree, &avlWrappers::Allocate, &avlWrappers::Free, &avlTree.Root, &avlWrappers::GetCount, n);
    auto t6 = std::chrono::high_resolution_clock::now();
    auto duration3 = std::chrono::duration_cast<std::chrono::milliseconds>(t6 - t5).count();
    std::cout << duration3 << "ms (in order) for SizedAndThreadedAVLBalancedTreeMethods\n";

    auto t7 = std::chrono::high_resolution_clock::now();
    Platform::Collections::Methods::Tests::TestExtensions::TestMultipleRandomCreationsAndDeletions<std::uint32_t>(sizeBalancedTree, &sizeBalancedTree.Root, &sbtWrappers::GetCount, n);
    auto t8 = std::chrono::high_resolution_clock::now();
    auto duration4 = std::chrono::duration_cast<std::chrono::milliseconds>(t8 - t7).count();
    std::cout << duration4 << "ms (random) for SizeBalancedTreeMethods\n";

    auto t9 = std::chrono::high_resolution_clock::now();
    Platform::Collections::Methods::Tests::TestExtensions::TestMultipleRandomCreationsAndDeletions<std::uint32_t>(sizeBalancedTree2, &sizeBalancedTree2.Root, &sbt2Wrappers::GetCount, n);
    auto t10 = std::chrono::high_resolution_clock::now();
    auto duration5 = std::chrono::duration_cast<std::chrono::milliseconds>(t10 - t9).count();
    std::cout << duration5 << "ms (random) for SizeBalancedTreeMethods2\n";

    auto t11 = std::chrono::high_resolution_clock::now();
    Platform::Collections::Methods::Tests::TestExtensions::TestMultipleRandomCreationsAndDeletions<std::uint32_t>(avlTree, &avlTree.Root, &avlWrappers::GetCount, n);
    auto t12 = std::chrono::high_resolution_clock::now();
    auto duration6 = std::chrono::duration_cast<std::chrono::milliseconds>(t12 - t11).count();
    std::cout << duration6 << "ms (random) for SizedAndThreadedAVLBalancedTreeMethods\n";
}