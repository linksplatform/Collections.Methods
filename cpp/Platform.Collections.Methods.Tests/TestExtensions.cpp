﻿namespace Platform::Collections::Methods::Tests
{
    class Assert
    {
    public:
        template<typename T1, typename T2>
        static void AreEqual(T1 a, T2 b)
        {
            if (a != b)
            {
                throw std::runtime_error("Values are not equal");
            }
        }
    };
    
    class TestExtensions
    {
        public: template <typename impl_t, typename TElement> static void TestMultipleCreationsAndDeletions(impl_t& tree, auto allocate, auto free, TElement* root, auto treeCount, std::int32_t maximumOperationsPerCycle)
        {
            for (auto N = 1; N < maximumOperationsPerCycle; N++)
            {
                auto currentCount = 0;
                for (auto i = 0; i < N; i++)
                {
                    auto node = allocate();
                    tree.Attach(root, node);
                    currentCount++;
                    Assert::AreEqual(currentCount, (std::int32_t)treeCount());
                }
                for (auto i = 1; i <= N; i++)
                {
                    TElement node = i;
                    if (tree.Contains(node, *root))
                    {
                        tree.Detach(root, node);
                        free(node);
                        currentCount--;
                        Assert::AreEqual(currentCount, (std::int32_t)treeCount());
                    }
                }
            }
        }

        public: template <typename impl_t, typename TElement> static void TestMultipleRandomCreationsAndDeletions(impl_t& tree, TElement* root, auto treeCount, std::int32_t maximumOperationsPerCycle)
        {
            std::srand(0);
            std::unordered_set<TElement> added;
            auto currentCount = 0;
            for (auto N = 1; N < maximumOperationsPerCycle; N++)
            {
                for (auto i = 0; i < N; i++)
                {
                    auto node = (std::rand() % N) + 1;
                    if (!added.contains(node))
                    {
                        added.insert(node);
                        tree.Attach(root, node);
                        currentCount++;
                        Assert::AreEqual(currentCount, (std::int32_t)treeCount());
                    }
                }
                for (auto i = 1; i <= N; i++)
                {
                    TElement node = (std::rand() % N) + 1;
                    if (tree.Contains(node, *root))
                    {
                        tree.Detach(root, node);
                        currentCount--;
                        Assert::AreEqual(currentCount, (std::int32_t)treeCount());
                        added.erase(node);
                    }
                }
            }
        }
    };
}
