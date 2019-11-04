namespace Platform::Collections::Methods::Lists
{
    template <typename TElement> class DoublyLinkedListMethodsBase : public GenericCollectionMethodsBase<TElement>
    {
    public:
        virtual TElement GetFirst() = 0;

        virtual TElement GetLast() = 0;

        virtual TElement GetPrevious(TElement element) = 0;

        virtual TElement GetNext(TElement element) = 0;

        virtual TElement GetSize() = 0;

        virtual void SetFirst(TElement element) = 0;

        virtual void SetLast(TElement element) = 0;

        virtual void SetPrevious(TElement element, TElement previous) = 0;

        virtual void SetNext(TElement element, TElement next) = 0;

        virtual void SetSize(TElement size) = 0;

        void IncrementSize() { this->SetSize(this->GetSize() + 1); }

        void DecrementSize() { this->SetSize(this->GetSize() - 1); }
    };
}
