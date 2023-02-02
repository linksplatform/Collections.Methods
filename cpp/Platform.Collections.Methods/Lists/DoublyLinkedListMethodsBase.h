namespace Platform::Collections::Methods::Lists
{
    template <class TSelf, typename ...> class DoublyLinkedListMethodsBase;
    template <class TSelf, typename TElement> class DoublyLinkedListMethodsBase<TSelf, TElement> : public GenericCollectionMethodsBase<TSelf, TElement> {};
}
