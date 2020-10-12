namespace Platform::Collections::Methods::Lists
{
    template <typename ...> class AbsoluteDoublyLinkedListMethodsBase;
    template <typename TElement> class AbsoluteDoublyLinkedListMethodsBase<TElement> : public DoublyLinkedListMethodsBase<TElement>
    {
        protected: virtual TElement GetFirst() = 0;

        protected: virtual TElement GetLast() = 0;

        protected: virtual TElement GetSize() = 0;

        protected: virtual void SetFirst(TElement element) = 0;

        protected: virtual void SetLast(TElement element) = 0;

        protected: virtual void SetSize(TElement size) = 0;

        protected: void IncrementSize() { this->SetSize(this->GetSize() + 1); }

        protected: void DecrementSize() { this->SetSize(this->GetSize() - 1); }
    };
}
