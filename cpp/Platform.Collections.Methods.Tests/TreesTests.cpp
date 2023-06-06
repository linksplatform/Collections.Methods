namespace Platform::Collections::Methods::Tests
{
    class TreesTests : public ::testing::Test
    {
    protected:
        inline static const std::int32_t _n = 500;
    };

    TEST_F(TreesTests, RecursionlessSizeBalancedTreeMultipleAttachAndDetachTest)
    {
        RecursionlessSizeBalancedTree<10000, std::uint32_t> recursionlessSizeBalancedTree;
        TestExtensions::TestMultipleCreationsAndDeletions<RecursionlessSizeBalancedTree<10000, std::uint32_t>, std::uint32_t>(
            recursionlessSizeBalancedTree,
            [&]() -> auto { return recursionlessSizeBalancedTree.Allocate(); },
            [&](std::uint32_t link) -> auto { recursionlessSizeBalancedTree.Free(link); },
            &recursionlessSizeBalancedTree.Root,
            [&]() -> auto { return recursionlessSizeBalancedTree.Count(); },
            _n);
    }

    TEST_F(TreesTests, SizeBalancedTreeMultipleAttachAndDetachTest)
    {
        SizeBalancedTree<10000, std::uint32_t> sizeBalancedTree;
        TestExtensions::TestMultipleCreationsAndDeletions<SizeBalancedTree<10000, std::uint32_t>, std::uint32_t>(
            sizeBalancedTree,
            [&]() -> auto { return sizeBalancedTree.Allocate(); },
            [&](std::uint32_t link) -> auto { sizeBalancedTree.Free(link); },
            &sizeBalancedTree.Root,
            [&]() -> auto { return sizeBalancedTree.Count(); },
            _n);
    }

    TEST_F(TreesTests, SizedAndThreadedAVLBalancedTreeMultipleAttachAndDetachTest)
    {
        SizedAndThreadedAVLBalancedTree<10000, std::uint32_t> avlTree;
        TestExtensions::TestMultipleCreationsAndDeletions<SizedAndThreadedAVLBalancedTree<10000, std::uint32_t>, std::uint32_t>(
            avlTree,
            [&]() -> auto { return avlTree.Allocate(); },
            [&](std::uint32_t link) -> auto { avlTree.Free(link); },
            &avlTree.Root,
            [&]() -> auto { return avlTree.Count(); },
            _n);
    }

    TEST_F(TreesTests, RecursionlessSizeBalancedTreeMultipleRandomAttachAndDetachTest)
    {
        RecursionlessSizeBalancedTree<10000, std::uint32_t> recursionlessSizeBalancedTree;
        TestExtensions::TestMultipleRandomCreationsAndDeletions<RecursionlessSizeBalancedTree<10000, std::uint32_t>, std::uint32_t>(
            recursionlessSizeBalancedTree,
            &recursionlessSizeBalancedTree.Root,
            [&]() -> auto { return recursionlessSizeBalancedTree.Count(); },
            _n);
    }

    TEST_F(TreesTests, SizeBalancedTreeMultipleRandomAttachAndDetachTest)
    {
        SizeBalancedTree<10000, std::uint32_t> sizeBalancedTree;
        TestExtensions::TestMultipleRandomCreationsAndDeletions<SizeBalancedTree<10000, std::uint32_t>, std::uint32_t>(
            sizeBalancedTree,
            &sizeBalancedTree.Root,
            [&]() -> auto { return sizeBalancedTree.Count(); },
            _n);
    }

    TEST_F(TreesTests, SizedAndThreadedAVLBalancedTreeMultipleRandomAttachAndDetachTest)
    {
        SizedAndThreadedAVLBalancedTree<10000, std::uint32_t> avlTree;
        TestExtensions::TestMultipleRandomCreationsAndDeletions<SizedAndThreadedAVLBalancedTree<10000, std::uint32_t>, std::uint32_t>(
            avlTree,
            &avlTree.Root,
            [&]() -> auto { return avlTree.Count(); },
            _n);
    }
}