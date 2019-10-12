#include "pch.h"
#include "CppUnitTest.h"
#include <functional>
#include <unordered_set>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

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

#include "SizeBalancedTree.cpp"
#include "SizeBalancedTree2.cpp"
#include "SizedAndThreadedAVLBalancedTree.cpp"
#include "TestExtensions.cpp"
#include "TreesTests.cpp"
