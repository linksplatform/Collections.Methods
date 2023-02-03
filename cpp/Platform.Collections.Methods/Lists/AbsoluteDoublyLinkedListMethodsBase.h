namespace Platform::Collections::Methods::Lists
{
    template <class TSelf, typename ...> class AbsoluteDoublyLinkedListMethodsBase;
    template <class TSelf, typename TElement> class AbsoluteDoublyLinkedListMethodsBase<TSelf, TElement> : public DoublyLinkedListMethodsBase<TSelf, TElement>
    {
        protected: void IncrementSize() { this->object().SetSize(this->object().GetSize() + 1); }

        protected: void DecrementSize() { this->object().SetSize(this->object().GetSize() - 1); }
    };
}
