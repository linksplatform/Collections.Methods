namespace Platform::Collections::Methods::Lists
{
    template <typename ...> class RelativeDoublyLinkedListMethodsBase;
    template <typename TElement> class RelativeDoublyLinkedListMethodsBase<TElement> : public DoublyLinkedListMethodsBase<TElement>
    {
        protected: virtual TElement GetFirst(TElement headElement) = 0;

        protected: virtual TElement GetLast(TElement headElement) = 0;

        protected: virtual TElement GetSize(TElement headElement) = 0;

        protected: virtual void SetFirst(TElement headElement, TElement element) = 0;

        protected: virtual void SetLast(TElement headElement, TElement element) = 0;

        protected: virtual void SetSize(TElement headElement, TElement size) = 0;

        protected: void IncrementSize(TElement headElement) { this->SetSize(headElement, this->GetSize(headElement) + 1); }

        protected: void DecrementSize(TElement headElement) { this->SetSize(headElement, this->GetSize(headElement) - 1); }
    };
}
