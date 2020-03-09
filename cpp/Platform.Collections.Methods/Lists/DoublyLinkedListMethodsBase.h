namespace Platform::Collections::Methods::Lists
{
    template <typename ...> class DoublyLinkedListMethodsBase;
    template <typename TElement> class DoublyLinkedListMethodsBase<TElement> : public GenericCollectionMethodsBase<TElement>
    {
        protected: virtual TElement GetFirst() = 0;

        protected: virtual TElement GetLast() = 0;

        protected: virtual TElement GetPrevious(TElement element) = 0;

        protected: virtual TElement GetNext(TElement element) = 0;

        protected: virtual TElement GetSize() = 0;

        protected: virtual void SetFirst(TElement element) = 0;

        protected: virtual void SetLast(TElement element) = 0;

        protected: virtual void SetPrevious(TElement element, TElement previous) = 0;

        protected: virtual void SetNext(TElement element, TElement next) = 0;

        protected: virtual void SetSize(TElement size) = 0;

        protected: void IncrementSize() { this->SetSize(this->GetSize() + 1); }

        protected: void DecrementSize() { this->SetSize(this->GetSize() - 1); }
    };
}
