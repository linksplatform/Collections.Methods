#include <iostream>
#include <unordered_set>
#include <chrono>

bool iszero(void* ptr, int bytes)
{
    char* bptr = (char*)ptr;
    while (bytes--)
        if (*bptr++)
            return false;
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
    struct avl2Wrappers {
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

    auto t1 = std::chrono::high_resolution_clock::now();
    Platform::Collections::Methods::Tests::TestExtensions::TestMultipleCreationsAndDeletions<std::uint32_t>(sizeBalancedTree, &sbtWrappers::Allocate, &sbtWrappers::Free, &sizeBalancedTree.Root, &sbtWrappers::GetCount, 2000);
    auto t2 = std::chrono::high_resolution_clock::now();
    auto duration1 = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
    std::cout << duration1 << "ms for SizeBalancedTreeMethods\n";

    auto t3 = std::chrono::high_resolution_clock::now();
    Platform::Collections::Methods::Tests::TestExtensions::TestMultipleCreationsAndDeletions<std::uint32_t>(sizeBalancedTree2, &sbt2Wrappers::Allocate, &sbt2Wrappers::Free, &sizeBalancedTree2.Root, &sbt2Wrappers::GetCount, 2000);
    auto t4 = std::chrono::high_resolution_clock::now();
    auto duration2 = std::chrono::duration_cast<std::chrono::milliseconds>(t4 - t3).count();
    std::cout << duration2 << "ms for SizeBalancedTreeMethods2\n";

    auto t5 = std::chrono::high_resolution_clock::now();
    Platform::Collections::Methods::Tests::TestExtensions::TestMultipleCreationsAndDeletions<std::uint32_t>(avlTree, &avl2Wrappers::Allocate, &avl2Wrappers::Free, &avlTree.Root, &avl2Wrappers::GetCount, 2000);
    auto t6 = std::chrono::high_resolution_clock::now();
    auto duration3 = std::chrono::duration_cast<std::chrono::milliseconds>(t6 - t5).count();
    std::cout << duration3 << "ms for SizedAndThreadedAVLBalancedTreeMethods\n";
}