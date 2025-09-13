#include <iostream>
#include <cassert>

// Include our CRTP implementation
#include "../cpp/Platform.Collections.Methods/Polymorph.h"

using namespace Platform::Interfaces;

// Simulate the GenericCollectionMethodsBase pattern
template <typename TSelf, typename TElement>
class GenericCollectionMethodsBase : public Polymorph<TSelf>
{
};

// Simulate a DoublyLinkedList methods class
template <typename TSelf, typename TElement>
class DoublyLinkedListMethodsBase : public GenericCollectionMethodsBase<TSelf, TElement>
{
protected:
    void IncrementSize()
    {
        this->object().SetSize(this->object().GetSize() + 1);
    }
    
    void DecrementSize()
    {
        this->object().SetSize(this->object().GetSize() - 1);
    }
};

// Concrete implementation that would be provided by user
class MyDoublyLinkedList : public DoublyLinkedListMethodsBase<MyDoublyLinkedList, int>
{
private:
    int size = 0;
    
public:
    int GetSize() const { return size; }
    void SetSize(int newSize) { size = newSize; }
    
    void TestIncrementDecrement()
    {
        std::cout << "Initial size: " << GetSize() << std::endl;
        
        IncrementSize();
        std::cout << "After increment: " << GetSize() << std::endl;
        
        IncrementSize();
        std::cout << "After second increment: " << GetSize() << std::endl;
        
        DecrementSize();
        std::cout << "After decrement: " << GetSize() << std::endl;
    }
};

int main()
{
    std::cout << "Testing Collections CRTP pattern..." << std::endl;
    
    MyDoublyLinkedList list;
    list.TestIncrementDecrement();
    
    assert(list.GetSize() == 1);
    std::cout << "Final size: " << list.GetSize() << " - Test passed!" << std::endl;
    
    return 0;
}