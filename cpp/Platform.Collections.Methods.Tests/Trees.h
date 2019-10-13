#ifndef TREES
#define TREES

#include <functional>
#include <unordered_set>

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

#include "RecursionlessSizeBalancedTree.cpp"
#include "SizeBalancedTree.cpp"
#include "SizedAndThreadedAVLBalancedTree.cpp"
#include "TestExtensions.cpp"

#endif // Trees