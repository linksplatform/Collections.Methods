namespace Platform::Collections::Methods::Lists
{
    template <class impl_t, typename ...> class RelativeCircularDoublyLinkedListMethods;
    template <class impl_t, typename TElement> class RelativeCircularDoublyLinkedListMethods<impl_t, TElement> : public RelativeDoublyLinkedListMethodsBase<impl_t, TElement>
    {
        public: using Polymorph<impl_t>::object;

        public: void AttachBefore(TElement headElement, TElement baseElement, TElement newElement)
        {
            auto baseElementPrevious = object().GetPrevious(baseElement);
            object().SetPrevious(newElement, baseElementPrevious);
            object().SetNext(newElement, baseElement);
            if (baseElement == object().GetFirst(headElement))
            {
                object().SetFirst(headElement, newElement);
            }
            object().SetNext(baseElementPrevious, newElement);
            object().SetPrevious(baseElement, newElement);
            object().IncrementSize(headElement);
        }

        public: void AttachAfter(TElement headElement, TElement baseElement, TElement newElement)
        {
            auto baseElementNext = object().GetNext(baseElement);
            object().SetPrevious(newElement, baseElement);
            object().SetNext(newElement, baseElementNext);
            if (baseElement == object().GetLast(headElement))
            {
                object().SetLast(headElement, newElement);
            }
            object().SetPrevious(baseElementNext, newElement);
            object().SetNext(baseElement, newElement);
            object().IncrementSize(headElement);
        }

        public: void AttachAsFirst(TElement headElement, TElement element)
        {
            auto first = object().GetFirst(headElement);
            if (first == 0)
            {
                object().SetFirst(headElement, element);
                object().SetLast(headElement, element);
                object().SetPrevious(element, element);
                object().SetNext(element, element);
                object().IncrementSize(headElement);
            }
            else
            {
                object().AttachBefore(headElement, first, element);
            }
        }

        public: void AttachAsLast(TElement headElement, TElement element)
        {
            auto last = object().GetLast(headElement);
            if (last == 0)
            {
                object().AttachAsFirst(headElement, element);
            }
            else
            {
                object().AttachAfter(headElement, last, element);
            }
        }

        public: void Detach(TElement headElement, TElement element)
        {
            auto elementPrevious = object().GetPrevious(element);
            auto elementNext = object().GetNext(element);
            if (elementNext == element)
            {
                object().SetFirst(headElement, 0);
                object().SetLast(headElement, 0);
            }
            else
            {
                object().SetNext(elementPrevious, elementNext);
                object().SetPrevious(elementNext, elementPrevious);
                if (element == object().GetFirst(headElement))
                {
                    object().SetFirst(headElement, elementNext);
                }
                if (element == object().GetLast(headElement))
                {
                    object().SetLast(headElement, elementPrevious);
                }
            }
            object().SetPrevious(element, 0);
            object().SetNext(element, 0);
            object().DecrementSize(headElement);
        }
    };
}
