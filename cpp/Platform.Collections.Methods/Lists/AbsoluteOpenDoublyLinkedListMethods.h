namespace Platform::Collections::Methods::Lists
{
    template <class impl_t, typename ...> class AbsoluteOpenDoublyLinkedListMethods;
    template <class impl_t, typename TElement> class AbsoluteOpenDoublyLinkedListMethods<impl_t, TElement> : public AbsoluteDoublyLinkedListMethodsBase<impl_t, TElement>
    {
        public: using base_t = AbsoluteDoublyLinkedListMethodsBase<impl_t, TElement>;
        friend base_t;

        public: void AttachBefore(TElement baseElement, TElement newElement)
        {
            auto baseElementPrevious = static_cast<impl_t*>(this)->GetPrevious(baseElement);
            static_cast<impl_t*>(this)->SetPrevious(newElement, baseElementPrevious);
            static_cast<impl_t*>(this)->SetNext(newElement, baseElement);
            if (baseElementPrevious == 0)
            {
                static_cast<impl_t*>(this)->SetFirst(newElement);
            }
            else
            {
                static_cast<impl_t*>(this)->SetNext(baseElementPrevious, newElement);
            }
            static_cast<impl_t*>(this)->SetPrevious(baseElement, newElement);
            static_cast<impl_t*>(this)->IncrementSize();
        }

        public: void AttachAfter(TElement baseElement, TElement newElement)
        {
            auto baseElementNext = static_cast<impl_t*>(this)->GetNext(baseElement);
            static_cast<impl_t*>(this)->SetPrevious(newElement, baseElement);
            static_cast<impl_t*>(this)->SetNext(newElement, baseElementNext);
            if (baseElementNext == 0)
            {
                static_cast<impl_t*>(this)->SetLast(newElement);
            }
            else
            {
                static_cast<impl_t*>(this)->SetPrevious(baseElementNext, newElement);
            }
            static_cast<impl_t*>(this)->SetNext(baseElement, newElement);
            static_cast<impl_t*>(this)->IncrementSize();
        }

        public: void AttachAsFirst(TElement element)
        {
            auto first = static_cast<impl_t*>(this)->GetFirst();
            if (first == 0)
            {
                static_cast<impl_t*>(this)->SetFirst(element);
                static_cast<impl_t*>(this)->SetLast(element);
                static_cast<impl_t*>(this)->SetPrevious(element, 0);
                static_cast<impl_t*>(this)->SetNext(element, 0);
                static_cast<impl_t*>(this)->IncrementSize();
            }
            else
            {
                static_cast<impl_t*>(this)->AttachBefore(first, element);
            }
        }

        public: void AttachAsLast(TElement element)
        {
            auto last = static_cast<impl_t*>(this)->GetLast();
            if (last == 0)
            {
                static_cast<impl_t*>(this)->AttachAsFirst(element);
            }
            else
            {
                static_cast<impl_t*>(this)->AttachAfter(last, element);
            }
        }

        public: void Detach(TElement element)
        {
            auto elementPrevious = static_cast<impl_t*>(this)->GetPrevious(element);
            auto elementNext = static_cast<impl_t*>(this)->GetNext(element);
            if (elementPrevious == 0)
            {
                static_cast<impl_t*>(this)->SetFirst(elementNext);
            }
            else
            {
                static_cast<impl_t*>(this)->SetNext(elementPrevious, elementNext);
            }
            if (elementNext == 0)
            {
                static_cast<impl_t*>(this)->SetLast(elementPrevious);
            }
            else
            {
                static_cast<impl_t*>(this)->SetPrevious(elementNext, elementPrevious);
            }
            static_cast<impl_t*>(this)->SetPrevious(element, 0);
            static_cast<impl_t*>(this)->SetNext(element, 0);
            static_cast<impl_t*>(this)->DecrementSize();
        }
    };
}
