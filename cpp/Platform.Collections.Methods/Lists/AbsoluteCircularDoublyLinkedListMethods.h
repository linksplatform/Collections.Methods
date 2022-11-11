namespace Platform::Collections::Methods::Lists
{
    template <class impl_t, typename ...> class AbsoluteCircularDoublyLinkedListMethods;
    template <class impl_t, typename TElement> class AbsoluteCircularDoublyLinkedListMethods<impl_t, TElement> : public AbsoluteDoublyLinkedListMethodsBase<impl_t, TElement>
    {
        public: using Polymorph<impl_t>::object;

        public: void AttachBefore(TElement baseElement, TElement newElement)
        {
            auto baseElementPrevious = object().GetPrevious(baseElement);
            object().SetPrevious(newElement, baseElementPrevious);
            object().SetNext(newElement, baseElement);
            if (baseElement == object().GetFirst())
            {
                object().SetFirst(newElement);
            }
            object().SetNext(baseElementPrevious, newElement);
            object().SetPrevious(baseElement, newElement);
            object().IncrementSize();
        }

        public: void AttachAfter(TElement baseElement, TElement newElement)
        {
            auto baseElementNext = object().GetNext(baseElement);
            object().SetPrevious(newElement, baseElement);
            object().SetNext(newElement, baseElementNext);
            if (baseElement == object().GetLast())
            {
                object().SetLast(newElement);
            }
            object().SetPrevious(baseElementNext, newElement);
            object().SetNext(baseElement, newElement);
            object().IncrementSize();
        }

        public: void AttachAsFirst(TElement element)
        {
            auto first = object().GetFirst();
            if (first == 0)
            {
                object().SetFirst(element);
                object().SetLast(element);
                object().SetPrevious(element, element);
                object().SetNext(element, element);
                object().IncrementSize();
            }
            else
            {
                object().AttachBefore(first, element);
            }
        }

        public: void AttachAsLast(TElement element)
        {
            auto last = object().GetLast();
            if (last == 0)
            {
                object().AttachAsFirst(element);
            }
            else
            {
                object().AttachAfter(last, element);
            }
        }

        public: void Detach(TElement element)
        {
            auto elementPrevious = object().GetPrevious(element);
            auto elementNext = object().GetNext(element);
            if (elementNext == element)
            {
                object().SetFirst(0);
                object().SetLast(0);
            }
            else
            {
                object().SetNext(elementPrevious, elementNext);
                object().SetPrevious(elementNext, elementPrevious);
                if (element == object().GetFirst())
                {
                    object().SetFirst(elementNext);
                }
                if (element == object().GetLast())
                {
                    object().SetLast(elementPrevious);
                }
            }
            object().SetPrevious(element, 0);
            object().SetNext(element, 0);
            object().DecrementSize();
        }
    };
}
