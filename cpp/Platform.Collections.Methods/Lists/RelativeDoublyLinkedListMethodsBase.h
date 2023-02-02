namespace Platform::Collections::Methods::Lists
{
    template <class TSelf, typename ...> class RelativeDoublyLinkedListMethodsBase;
    template <class TSelf, typename TElement> class RelativeDoublyLinkedListMethodsBase<TSelf, TElement> : public DoublyLinkedListMethodsBase<TSelf, TElement>
    {
        protected: void IncrementSize(TElement headElement) { this->object().SetSize(headElement, this->object().GetSize(headElement) + 1); }

        protected: void DecrementSize(TElement headElement) { this->object().SetSize(headElement, this->object().GetSize(headElement) - 1); }
    };
}
