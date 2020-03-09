namespace Platform::Collections::Methods::Tests
{
    template <std::size_t N, typename ...> class SizeBalancedTree;
    template <std::size_t N, typename TElement> class SizeBalancedTree<N, TElement> : public Platform::Collections::Methods::Trees::SizeBalancedTreeMethods<TElement>
    {
        struct TreeElement
        {
            public: TElement Size = 0;
            public: TElement Left = 0;
            public: TElement Right = 0;
        };

        private: TreeElement _elements[N] = { {0} };
        private: TElement _allocated = 0;

        public: TElement Root = 0;

        public: TElement GetCount() { return this->GetSizeOrZero(Root); }

        public: SizeBalancedTree() { _allocated = 1; }

        public: TElement Allocate()
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

        public: void Free(TElement node)
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

        public: bool IsEmpty(TElement node) { return iszero(this->GetElement(node), sizeof(TreeElement)); }

        protected: bool FirstIsToTheLeftOfSecond(TElement first, TElement second) override { return first < second; }

        protected: bool FirstIsToTheRightOfSecond(TElement first, TElement second) override { return first > second; }

        protected: TElement* GetLeftReference(TElement node) override { return &GetElement(node)->Left; }

        protected: TElement GetLeft(TElement node) override { return this->GetElement(node)->Left; }

        protected: TElement* GetRightReference(TElement node) override { return &GetElement(node)->Right; }

        protected: TElement GetRight(TElement node) override { return this->GetElement(node)->Right; }

        protected: TElement GetSize(TElement node) override { return this->GetElement(node)->Size; }

        protected: void SetLeft(TElement node, TElement left) override { this->GetElement(node)->Left = left; }

        protected: void SetRight(TElement node, TElement right) override { this->GetElement(node)->Right = right; }

        protected: void SetSize(TElement node, TElement size) override { this->GetElement(node)->Size = size; }

        private: TreeElement* GetElement(TElement node) { return &_elements[node]; }
    };
}
