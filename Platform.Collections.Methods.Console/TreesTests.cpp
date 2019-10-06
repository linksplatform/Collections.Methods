namespace Platform::Collections::Methods::Tests
{
    class TreesTests
    {
    public:
        static const int _n = 100;
        static void SizeBalancedTreeMultipleAttachAndDetachTest()
        {
            SizeBalancedTree<std::uint32_t, 10000> sizeBalancedTree;
            TestExtensions::TestMultipleCreationsAndDeletions(sizeBalancedTree, []()-> auto { return sizeBalancedTree.Allocate(); }, [](std::uint32_t link)-> auto { sizeBalancedTree.Free(link); }, &sizeBalancedTree.Root, []()-> auto { return sizeBalancedTree.Count; }, _n);
        }
        static void SizeBalancedTree2MultipleAttachAndDetachTest()
        {
            SizeBalancedTree2<std::uint32_t, 10000> sizeBalancedTree2;
            TestExtensions::TestMultipleCreationsAndDeletions(sizeBalancedTree2, []()-> auto { return sizeBalancedTree2.Allocate(); }, [](std::uint32_t link)-> auto { sizeBalancedTree2.Free(link); }, &sizeBalancedTree2.Root, []()-> auto { return sizeBalancedTree2.Count; }, _n);
        }
        static void SizedAndThreadedAVLBalancedTreeMultipleAttachAndDetachTest()
        {
            SizedAndThreadedAVLBalancedTree<std::uint32_t, 10000> avlTree;
            TestExtensions::TestMultipleCreationsAndDeletions(avlTree, []()-> auto { return avlTree.Allocate(); }, [](std::uint32_t link)-> auto { avlTree.Free(link); }, &avlTree.Root, []()-> auto { return avlTree.Count; }, _n);
        }
        static void SizeBalancedTreeMultipleRandomAttachAndDetachTest()
        {
            SizeBalancedTree<std::uint32_t, 10000> sizeBalancedTree;
            TestExtensions::TestMultipleRandomCreationsAndDeletions(sizeBalancedTree, &sizeBalancedTree.Root, []()-> auto { return sizeBalancedTree.Count; }, _n);
        }
        static void SizeBalancedTree2MultipleRandomAttachAndDetachTest()
        {
            SizeBalancedTree2<std::uint32_t, 10000> sizeBalancedTree2;
            TestExtensions::TestMultipleRandomCreationsAndDeletions(sizeBalancedTree2, &sizeBalancedTree2.Root, []()-> auto { return sizeBalancedTree2.Count; }, _n);
        }
        static void SizedAndThreadedAVLBalancedTreeMultipleRandomAttachAndDetachTest()
        {
            SizedAndThreadedAVLBalancedTree<std::uint32_t, 10000> avlTree;
            TestExtensions::TestMultipleRandomCreationsAndDeletions(avlTree, &avlTree.Root, []()-> auto { return avlTree.Count; }, _n);
        }
    };
}
