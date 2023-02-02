namespace Platform::Collections::Methods::Lists
{
    template <class impl_t, typename ...> class DoublyLinkedListMethodsBase;
    template <class impl_t, typename TElement> class DoublyLinkedListMethodsBase<impl_t, TElement> : public GenericCollectionMethodsBase<impl_t, TElement> {};
}
