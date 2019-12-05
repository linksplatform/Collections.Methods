namespace Platform::Collections::Methods::Tests
{
    template <typename TElement, std::size_t N> class SizeBalancedTree : public Platform::Collections::Methods::Trees::SizeBalancedTreeMethods<TElement>
    {
    public:
        struct TreeElement
        {
            TElement Size = 0;
            TElement Left = 0;
            TElement Right = 0;
        };

        TreeElement _elements[N] = { {0} };
        TElement _allocated = 0;

        TElement Root = 0;

        TElement GetCount() { return this->GetSizeOrZero(Root); }

        SizeBalancedTree() { _allocated = 1; }

        TElement Allocate()
        {
            auto newNode = _allocated;
            if (this->IsEmpty(newNode))
            {
                _allocated = _allocated + 1;
                return newNode;
            }
            else
            {
                throw std::runtime_error("Allocated tree element is not empty.");
            }
        }

        void Free(TElement node)
        {
            while (_allocated != 1 && this->IsEmpty(node))
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

        bool IsEmpty(TElement node) { return iszero(this->GetElement(node), sizeof(TreeElement)); }

        bool FirstIsToTheLeftOfSecond(TElement first, TElement second) override { return first < second; }

        bool FirstIsToTheRightOfSecond(TElement first, TElement second) override { return first > second; }

        TElement* GetLeftReference(TElement node) override { return &GetElement(node)->Left; }

        TElement GetLeft(TElement node) override { return this->GetElement(node)->Left; }

        TElement* GetRightReference(TElement node) override { return &GetElement(node)->Right; }

        TElement GetRight(TElement node) override { return this->GetElement(node)->Right; }

        TElement GetSize(TElement node) override { return this->GetElement(node)->Size; }

        void SetLeft(TElement node, TElement left) override { this->GetElement(node)->Left = left; }

        void SetRight(TElement node, TElement right) override { this->GetElement(node)->Right = right; }

        void SetSize(TElement node, TElement size) override { this->GetElement(node)->Size = size; }

        TreeElement* GetElement(TElement node) { return &_elements[node]; }
    };
}
