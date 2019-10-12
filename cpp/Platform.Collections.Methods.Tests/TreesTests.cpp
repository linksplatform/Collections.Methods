namespace Platform::Collections::Methods::Tests
{
    class TreesTests
    {
    public:
        static const int _n = 100;
        static void SizeBalancedTreeMultipleAttachAndDetachTest()
        {
            SizeBalancedTree<std::uint32_t, 10000> sizeBalancedTree;
            TestExtensions::TestMultipleCreationsAndDeletions<std::uint32_t>(sizeBalancedTree, [&]()-> auto { return sizeBalancedTree.Allocate(); }, [&](std::uint32_t link)-> auto { sizeBalancedTree.Free(link); }, &sizeBalancedTree.Root, [&]()-> auto { return sizeBalancedTree.GetCount(); }, _n);
        }
        static void SizeBalancedTree2MultipleAttachAndDetachTest()
        {
            SizeBalancedTree2<std::uint32_t, 10000> sizeBalancedTree2;
            TestExtensions::TestMultipleCreationsAndDeletions<std::uint32_t>(sizeBalancedTree2, [&]()-> auto { return sizeBalancedTree2.Allocate(); }, [&](std::uint32_t link)-> auto { sizeBalancedTree2.Free(link); }, &sizeBalancedTree2.Root, [&]()-> auto { return sizeBalancedTree2.GetCount(); }, _n);
        }
        static void SizedAndThreadedAVLBalancedTreeMultipleAttachAndDetachTest()
        {
            SizedAndThreadedAVLBalancedTree<std::uint32_t, 10000> avlTree;
            TestExtensions::TestMultipleCreationsAndDeletions<std::uint32_t>(avlTree, [&]()-> auto { return avlTree.Allocate(); }, [&](std::uint32_t link)-> auto { avlTree.Free(link); }, &avlTree.Root, [&]()-> auto { return avlTree.GetCount(); }, _n);
        }
        static void SizeBalancedTreeMultipleRandomAttachAndDetachTest()
        {
            SizeBalancedTree<std::uint32_t, 10000> sizeBalancedTree;
            TestExtensions::TestMultipleRandomCreationsAndDeletions<std::uint32_t>(sizeBalancedTree, &sizeBalancedTree.Root, [&]()-> auto { return sizeBalancedTree.GetCount(); }, _n);
        }
        static void SizeBalancedTree2MultipleRandomAttachAndDetachTest()
        {
            SizeBalancedTree2<std::uint32_t, 10000> sizeBalancedTree2;
            TestExtensions::TestMultipleRandomCreationsAndDeletions<std::uint32_t>(sizeBalancedTree2, &sizeBalancedTree2.Root, [&]()-> auto { return sizeBalancedTree2.GetCount(); }, _n);
        }
        static void SizedAndThreadedAVLBalancedTreeMultipleRandomAttachAndDetachTest()
        {
            SizedAndThreadedAVLBalancedTree<std::uint32_t, 10000> avlTree;
            TestExtensions::TestMultipleRandomCreationsAndDeletions<std::uint32_t>(avlTree, &avlTree.Root, [&]()-> auto { return avlTree.GetCount(); }, _n);
        }
    };
}
