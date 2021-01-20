namespace Platform::Collections::Methods::Lists
{
    template <class impl_t, typename ...> class RelativeCircularDoublyLinkedListMethods;
    template <class impl_t, typename TElement> class RelativeCircularDoublyLinkedListMethods<impl_t, TElement> : public RelativeDoublyLinkedListMethodsBase<impl_t, TElement>
    {
        public: using base_t = RelativeDoublyLinkedListMethodsBase<impl_t, TElement>;
        friend base_t;

        public: void AttachBefore(TElement headElement, TElement baseElement, TElement newElement)
        {
            auto baseElementPrevious = static_cast<impl_t*>(this)->GetPrevious(baseElement);
            static_cast<impl_t*>(this)->SetPrevious(newElement, baseElementPrevious);
            static_cast<impl_t*>(this)->SetNext(newElement, baseElement);
            if (baseElement == static_cast<impl_t*>(this)->GetFirst(headElement))
            {
                static_cast<impl_t*>(this)->SetFirst(headElement, newElement);
            }
            static_cast<impl_t*>(this)->SetNext(baseElementPrevious, newElement);
            static_cast<impl_t*>(this)->SetPrevious(baseElement, newElement);
            static_cast<impl_t*>(this)->IncrementSize(headElement);
        }

        public: void AttachAfter(TElement headElement, TElement baseElement, TElement newElement)
        {
            auto baseElementNext = static_cast<impl_t*>(this)->GetNext(baseElement);
            static_cast<impl_t*>(this)->SetPrevious(newElement, baseElement);
            static_cast<impl_t*>(this)->SetNext(newElement, baseElementNext);
            if (baseElement == static_cast<impl_t*>(this)->GetLast(headElement))
            {
                static_cast<impl_t*>(this)->SetLast(headElement, newElement);
            }
            static_cast<impl_t*>(this)->SetPrevious(baseElementNext, newElement);
            static_cast<impl_t*>(this)->SetNext(baseElement, newElement);
            static_cast<impl_t*>(this)->IncrementSize(headElement);
        }

        public: void AttachAsFirst(TElement headElement, TElement element)
        {
            auto first = static_cast<impl_t*>(this)->GetFirst(headElement);
            if (first == 0)
            {
                static_cast<impl_t*>(this)->SetFirst(headElement, element);
                static_cast<impl_t*>(this)->SetLast(headElement, element);
                static_cast<impl_t*>(this)->SetPrevious(element, element);
                static_cast<impl_t*>(this)->SetNext(element, element);
                static_cast<impl_t*>(this)->IncrementSize(headElement);
            }
            else
            {
                static_cast<impl_t*>(this)->AttachBefore(headElement, first, element);
            }
        }

        public: void AttachAsLast(TElement headElement, TElement element)
        {
            auto last = static_cast<impl_t*>(this)->GetLast(headElement);
            if (last == 0)
            {
                static_cast<impl_t*>(this)->AttachAsFirst(headElement, element);
            }
            else
            {
                static_cast<impl_t*>(this)->AttachAfter(headElement, last, element);
            }
        }

        public: void Detach(TElement headElement, TElement element)
        {
            auto elementPrevious = static_cast<impl_t*>(this)->GetPrevious(element);
            auto elementNext = static_cast<impl_t*>(this)->GetNext(element);
            if (elementNext == element)
            {
                static_cast<impl_t*>(this)->SetFirst(headElement, 0);
                static_cast<impl_t*>(this)->SetLast(headElement, 0);
            }
            else
            {
                static_cast<impl_t*>(this)->SetNext(elementPrevious, elementNext);
                static_cast<impl_t*>(this)->SetPrevious(elementNext, elementPrevious);
                if (element == static_cast<impl_t*>(this)->GetFirst(headElement))
                {
                    static_cast<impl_t*>(this)->SetFirst(headElement, elementNext);
                }
                if (element == static_cast<impl_t*>(this)->GetLast(headElement))
                {
                    static_cast<impl_t*>(this)->SetLast(headElement, elementPrevious);
                }
            }
            static_cast<impl_t*>(this)->SetPrevious(element, 0);
            static_cast<impl_t*>(this)->SetNext(element, 0);
            static_cast<impl_t*>(this)->DecrementSize(headElement);
        }
    };
}
