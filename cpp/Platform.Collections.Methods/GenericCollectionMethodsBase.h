namespace Platform::Collections::Methods
{
    using namespace Platform::Interfaces;

    template <typename TSelf, typename ...> class GenericCollectionMethodsBase;
    template <typename TSelf, typename TElement> class GenericCollectionMethodsBase<TSelf, TElement> : public Polymorph<TSelf>
    {
    };
}
