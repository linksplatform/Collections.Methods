namespace Platform::Collections::Methods::Tests
{
    class TestExtensions
    {
    public:
        template <typename TElement> static void TestMultipleCreationsAndDeletions(Platform::Collections::Methods::Trees::SizedBinaryTreeMethodsBase<TElement>& tree, std::function<TElement()> allocate, std::function<void(TElement)> free, TElement* root, std::function<TElement()> treeCount, int maximumOperationsPerCycle)
        {
            for (auto N = 1; N < maximumOperationsPerCycle; N++)
            {
                auto currentCount = 0;
                for (auto i = 0; i < N; i++)
                {
                    auto node = allocate();
                    tree.Attach(root, node);
                    currentCount++;
                    Assert::Equal(currentCount, (int)treeCount());
                }
                for (auto i = 1; i <= N; i++)
                {
                    TElement node = i;
                    if (tree.Contains(node, *root))
                    {
                        tree.Detach(root, node);
                        free(node);
                        currentCount--;
                        Assert::Equal(currentCount, (int)treeCount());
                    }
                }
            }
        }

        template <typename TElement> static void TestMultipleRandomCreationsAndDeletions(Platform::Collections::Methods::Trees::SizedBinaryTreeMethodsBase<TElement>& tree, TElement* root, std::function<TElement()> treeCount, int maximumOperationsPerCycle)
        {
            std::unordered_set<TElement> added;
            auto currentCount = 0;
            for (auto N = 1; N < maximumOperationsPerCycle; N++)
            {
                for (auto i = 0; i < N; i++)
                {
                    auto node = (std::rand() % N) + 1;
                    if (added.find(node) == added.end())
                    {
                        added.insert(node);
                        tree.Attach(root, node);
                        currentCount++;
                        Assert::Equal(currentCount, (int)treeCount());
                    }
                }
                for (auto i = 1; i <= N; i++)
                {
                    TElement node = (std::rand() % N) + 1;
                    if (tree.Contains(node, *root))
                    {
                        tree.Detach(root, node);
                        currentCount--;
                        Assert::Equal(currentCount, (int)treeCount());
                        added.erase(node);
                    }
                }
            }
        }
    };
}
