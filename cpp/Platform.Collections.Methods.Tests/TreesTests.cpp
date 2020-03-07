namespace Platform::Collections::Methods::Tests
{
    TEST_CLASS(Trees)
    {
        private: inline static const std::int32_t _n = 500;

        public: TEST_METHOD(RecursionlessSizeBalancedTreeMultipleAttachAndDetachTest)
        {
            RecursionlessSizeBalancedTree<std::uint32_t, 10000> recursionlessSizeBalancedTree;
            TestExtensions::TestMultipleCreationsAndDeletions<std::uint32_t>(recursionlessSizeBalancedTree, [&]()-> auto { return recursionlessSizeBalancedTree.Allocate(); }, [&](std::uint32_t link)-> auto { recursionlessSizeBalancedTree.Free(link); }, &recursionlessSizeBalancedTree.Root, [&]()-> auto { return recursionlessSizeBalancedTree.GetCount(); }, _n);
        }

        public: TEST_METHOD(SizeBalancedTreeMultipleAttachAndDetachTest)
        {
            SizeBalancedTree<std::uint32_t, 10000> sizeBalancedTree;
            TestExtensions::TestMultipleCreationsAndDeletions<std::uint32_t>(sizeBalancedTree, [&]()-> auto { return sizeBalancedTree.Allocate(); }, [&](std::uint32_t link)-> auto { sizeBalancedTree.Free(link); }, &sizeBalancedTree.Root, [&]()-> auto { return sizeBalancedTree.GetCount(); }, _n);
        }

        public: TEST_METHOD(SizedAndThreadedAVLBalancedTreeMultipleAttachAndDetachTest)
        {
            SizedAndThreadedAVLBalancedTree<std::uint32_t, 10000> avlTree;
            TestExtensions::TestMultipleCreationsAndDeletions<std::uint32_t>(avlTree, [&]()-> auto { return avlTree.Allocate(); }, [&](std::uint32_t link)-> auto { avlTree.Free(link); }, &avlTree.Root, [&]()-> auto { return avlTree.GetCount(); }, _n);
        }

        public: TEST_METHOD(RecursionlessSizeBalancedTreeMultipleRandomAttachAndDetachTest)
        {
            RecursionlessSizeBalancedTree<std::uint32_t, 10000> recursionlessSizeBalancedTree;
            TestExtensions::TestMultipleRandomCreationsAndDeletions<std::uint32_t>(recursionlessSizeBalancedTree, &recursionlessSizeBalancedTree.Root, [&]()-> auto { return recursionlessSizeBalancedTree.GetCount(); }, _n);
        }

        public: TEST_METHOD(SizeBalancedTreeMultipleRandomAttachAndDetachTest)
        {
            SizeBalancedTree<std::uint32_t, 10000> sizeBalancedTree;
            TestExtensions::TestMultipleRandomCreationsAndDeletions<std::uint32_t>(sizeBalancedTree, &sizeBalancedTree.Root, [&]()-> auto { return sizeBalancedTree.GetCount(); }, _n);
        }

        public: TEST_METHOD(SizedAndThreadedAVLBalancedTreeMultipleRandomAttachAndDetachTest)
        {
            SizedAndThreadedAVLBalancedTree<std::uint32_t, 10000> avlTree;
            TestExtensions::TestMultipleRandomCreationsAndDeletions<std::uint32_t>(avlTree, &avlTree.Root, [&]()-> auto { return avlTree.GetCount(); }, _n);
        }
    };
}
