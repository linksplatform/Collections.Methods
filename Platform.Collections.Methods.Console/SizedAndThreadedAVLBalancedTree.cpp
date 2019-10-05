





namespace Platform::Collections::Methods::Tests
{
    template <typename TElement, std::size_t N> class SizedAndThreadedAVLBalancedTree : public Platform::Collections::Methods::Trees::SizedAndThreadedAVLBalancedTreeMethods<TElement>
    { public:
        struct TreeElement
        {
            TElement Size;
            TElement Left;
            TElement Right;
            std::int8_t Balance;
            bool LeftIsChild;
            bool RightIsChild;
        };

        TreeElement _elements[N] = {{0}};
        TElement _allocated;

        TElement Root = 0;

        TElement GetCount() { return Platform::Collections::Methods::Trees::SizedAndThreadedAVLBalancedTreeMethods<TElement>::GetSizeOrZero(Root); }

        SizedAndThreadedAVLBalancedTree() { _allocated = 1; }

        TElement Allocate()
        {
            auto newNode = _allocated;
            if (IsEmpty(newNode))
            {
                _allocated = _allocated + 1;
                return newNode;
            }
            else
            {
                throw std::exception("Allocated tree element is not empty.");
            }
        }

        void Free(TElement node)
        {
            while (_allocated != 1 && IsEmpty(node))
            {
                auto lastNode = _allocated - 1;
                if (lastNode == node)
                {
                    _allocated = lastNode;
                    node = node - 1;
                }
                else
                {
                    return;
                }
            }
        }

        bool IsEmpty(TElement node) { return iszero(GetElement(node), sizeof(TreeElement)); }

        bool FirstIsToTheLeftOfSecond(TElement first, TElement second) override { return first < second; }

        bool FirstIsToTheRightOfSecond(TElement first, TElement second) override { return first > second; }

        std::int8_t GetBalance(TElement node) override { return GetElement(node)->Balance; }

        bool GetLeftIsChild(TElement node) override { return GetElement(node)->LeftIsChild; }

        TElement* GetLeftReference(TElement node) override { return &GetElement(node)->Left; }

        TElement GetLeft(TElement node) override { return GetElement(node)->Left; }

        bool GetRightIsChild(TElement node) override { return GetElement(node)->RightIsChild; }

        TElement* GetRightReference(TElement node) override { return &GetElement(node)->Right; }

        TElement GetRight(TElement node) override { return GetElement(node)->Right; }

        TElement GetSize(TElement node) override { return GetElement(node)->Size; }

        void SetBalance(TElement node, std::int8_t value) override { GetElement(node)->Balance = value; }

        void SetLeft(TElement node, TElement left) override { GetElement(node)->Left = left; }

        void SetLeftIsChild(TElement node, bool value) override { GetElement(node)->LeftIsChild = value; }

        void SetRight(TElement node, TElement right) override { GetElement(node)->Right = right; }

        void SetRightIsChild(TElement node, bool value) override { GetElement(node)->RightIsChild = value; }

        void SetSize(TElement node, TElement size) override { GetElement(node)->Size = size; }

        TreeElement* GetElement(TElement node) { return &_elements[node]; }
    };
}
