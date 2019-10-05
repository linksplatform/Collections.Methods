// CppConsoleExample.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

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

// template <typename TElement> class SizedBinaryTreeMethodsBase : GenericCollectionMethodsBase<TElement>
template<typename TElement> class A // : A<TElement>
{
public:
	virtual TElement* GetLeftReference(TElement node) = 0;
	virtual void AttachCore(TElement* root, TElement node) = 0;
};

// template <typename TElement> class SizeBalancedTreeMethods : SizedBinaryTreeMethodsBase<TElement>
template<typename TElement> class B : public A<TElement>
{
public:
	void AttachCore(TElement* root, TElement node) override
	{
		GetLeftReference(0);
	}
	virtual TElement* GetLeftReference(TElement node) override = 0;
};

// template <typename TElement, std::size_t N> class SizeBalancedTree : Platform::Collections::Methods::Trees::SizeBalancedTreeMethods<TElement>
template<typename TElement, std::size_t N> class C : public B<TElement>
{
public:
	TElement* GetLeftReference(TElement node) override { return nullptr; }
};


int main()
{
	static Platform::Collections::Methods::Tests::SizeBalancedTree<std::uint32_t, 10000> sizeBalancedTree;
	struct X {
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
	//X::Allocate();
	//auto* pointer = &X::Allocate;
	//C<std::uint32_t, 10000> c;
	//c.AttachCore(nullptr, 0);
	//Platform::Collections::Methods::Tests::SizeBalancedTree2<std::uint32_t, 10000> sizeBalancedTree2;
	//Platform::Collections::Methods::Tests::SizedAndThreadedAVLBalancedTree<std::uint32_t, 10000> avlTree;

	//Platform::Collections::Methods::Trees::SizeBalancedTreeMethods<std::uint32_t>* sbt = &sizeBalancedTree;
	//Platform::Collections::Methods::Trees::SizedBinaryTreeMethodsBase<std::uint32_t>* base = sbt;

	auto t1 = std::chrono::high_resolution_clock::now();
	Platform::Collections::Methods::Tests::TestExtensions::TestMultipleCreationsAndDeletions<std::uint32_t>(sizeBalancedTree, &X::Allocate, &X::Free, &sizeBalancedTree.Root, &X::GetCount, 2000);
	auto t2 = std::chrono::high_resolution_clock::now();

	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();

	std::cout << duration;

	Assert::Equal(1, 1);
	std::cout << "Hello World!\n";
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
