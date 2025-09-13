#include <iostream>
#include <cassert>

// Include our CRTP implementation
#include "../cpp/Platform.Collections.Methods/Polymorph.h"

using namespace Platform::Interfaces;

// Test class that uses CRTP like the collections classes do
template <typename TSelf>
class TestBase : public Polymorph<TSelf>
{
public:
    void DoSomething()
    {
        std::cout << "Base calling derived method: ";
        this->object().Implementation();
    }
    
    int GetValue()
    {
        return this->object().GetValueImpl();
    }
};

// Derived class that implements the interface
class TestDerived : public TestBase<TestDerived>
{
public:
    void Implementation()
    {
        std::cout << "TestDerived::Implementation() called!" << std::endl;
    }
    
    int GetValueImpl()
    {
        return 42;
    }
};

// Another derived class to test polymorphism
class AnotherDerived : public TestBase<AnotherDerived>
{
public:
    void Implementation()
    {
        std::cout << "AnotherDerived::Implementation() called!" << std::endl;
    }
    
    int GetValueImpl()
    {
        return 100;
    }
};

int main()
{
    std::cout << "Testing CRTP (Curiously Recurring Template Pattern) implementation..." << std::endl;
    
    // Test TestDerived
    TestDerived derived1;
    derived1.DoSomething();
    assert(derived1.GetValue() == 42);
    std::cout << "TestDerived value: " << derived1.GetValue() << std::endl;
    
    // Test AnotherDerived
    AnotherDerived derived2;
    derived2.DoSomething();
    assert(derived2.GetValue() == 100);
    std::cout << "AnotherDerived value: " << derived2.GetValue() << std::endl;
    
    std::cout << "All tests passed! CRTP implementation is working correctly." << std::endl;
    
    return 0;
}