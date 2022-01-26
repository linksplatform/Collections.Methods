namespace Platform::Collections::Methods
{
    using namespace Platform::Interfaces;

    template <class impl_t, typename ...> class GenericCollectionMethodsBase;
    template <class impl_t, typename TElement> class GenericCollectionMethodsBase<impl_t, TElement> : public Polymorph<impl_t>
    {
    };
}
