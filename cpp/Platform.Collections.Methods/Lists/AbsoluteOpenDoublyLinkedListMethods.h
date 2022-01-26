namespace Platform::Collections::Methods::Lists
{
    template <class impl_t, typename ...> class AbsoluteOpenDoublyLinkedListMethods;
    template <class impl_t, typename TElement> class AbsoluteOpenDoublyLinkedListMethods<impl_t, TElement> : public AbsoluteDoublyLinkedListMethodsBase<impl_t, TElement>
    {
        public: using Polymorph<impl_t>::object;

        public: void AttachBefore(TElement baseElement, TElement newElement)
        {
            auto baseElementPrevious = object().GetPrevious(baseElement);
            object().SetPrevious(newElement, baseElementPrevious);
            object().SetNext(newElement, baseElement);
            if (baseElementPrevious == 0)
            {
                object().SetFirst(newElement);
            }
            else
            {
                object().SetNext(baseElementPrevious, newElement);
            }
            object().SetPrevious(baseElement, newElement);
            object().IncrementSize();
        }

        public: void AttachAfter(TElement baseElement, TElement newElement)
        {
            auto baseElementNext = object().GetNext(baseElement);
            object().SetPrevious(newElement, baseElement);
            object().SetNext(newElement, baseElementNext);
            if (baseElementNext == 0)
            {
                object().SetLast(newElement);
            }
            else
            {
                object().SetPrevious(baseElementNext, newElement);
            }
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
                object().SetPrevious(element, 0);
                object().SetNext(element, 0);
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
            if (elementPrevious == 0)
            {
                object().SetFirst(elementNext);
            }
            else
            {
                object().SetNext(elementPrevious, elementNext);
            }
            if (elementNext == 0)
            {
                object().SetLast(elementPrevious);
            }
            else
            {
                object().SetPrevious(elementNext, elementPrevious);
            }
            object().SetPrevious(element, 0);
            object().SetNext(element, 0);
            object().DecrementSize();
        }
    };
}
