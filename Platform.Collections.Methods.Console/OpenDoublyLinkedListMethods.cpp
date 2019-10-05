
namespace Platform::Collections::Methods::Lists
{
    template <typename TElement> class OpenDoublyLinkedListMethods : public DoublyLinkedListMethodsBase<TElement>
    { public:
        void AttachBefore(TElement baseElement, TElement newElement)
        {
            auto baseElementPrevious = DoublyLinkedListMethodsBase<TElement>::GetPrevious(baseElement);
            DoublyLinkedListMethodsBase<TElement>::SetPrevious(newElement, baseElementPrevious);
            DoublyLinkedListMethodsBase<TElement>::SetNext(newElement, baseElement);
            if (baseElementPrevious == 0)
            {
                DoublyLinkedListMethodsBase<TElement>::SetFirst(newElement);
            }
            else
            {
                DoublyLinkedListMethodsBase<TElement>::SetNext(baseElementPrevious, newElement);
            }
            DoublyLinkedListMethodsBase<TElement>::SetPrevious(baseElement, newElement);
            DoublyLinkedListMethodsBase<TElement>::IncrementSize();
        }

        void AttachAfter(TElement baseElement, TElement newElement)
        {
            auto baseElementNext = DoublyLinkedListMethodsBase<TElement>::GetNext(baseElement);
            DoublyLinkedListMethodsBase<TElement>::SetPrevious(newElement, baseElement);
            DoublyLinkedListMethodsBase<TElement>::SetNext(newElement, baseElementNext);
            if (baseElementNext == 0)
            {
                DoublyLinkedListMethodsBase<TElement>::SetLast(newElement);
            }
            else
            {
                DoublyLinkedListMethodsBase<TElement>::SetPrevious(baseElementNext, newElement);
            }
            DoublyLinkedListMethodsBase<TElement>::SetNext(baseElement, newElement);
            DoublyLinkedListMethodsBase<TElement>::IncrementSize();
        }

        void AttachAsFirst(TElement element)
        {
            auto first = DoublyLinkedListMethodsBase<TElement>::GetFirst();
            if (first == 0)
            {
                DoublyLinkedListMethodsBase<TElement>::SetFirst(element);
                DoublyLinkedListMethodsBase<TElement>::SetLast(element);
                DoublyLinkedListMethodsBase<TElement>::SetPrevious(element, 0);
                DoublyLinkedListMethodsBase<TElement>::SetNext(element, 0);
                DoublyLinkedListMethodsBase<TElement>::IncrementSize();
            }
            else
            {
                AttachBefore(first, element);
            }
        }

        void AttachAsLast(TElement element)
        {
            auto last = DoublyLinkedListMethodsBase<TElement>::GetLast();
            if (last == 0)
            {
                AttachAsFirst(element);
            }
            else
            {
                AttachAfter(last, element);
            }
        }

        void Detach(TElement element)
        {
            auto elementPrevious = DoublyLinkedListMethodsBase<TElement>::GetPrevious(element);
            auto elementNext = DoublyLinkedListMethodsBase<TElement>::GetNext(element);
            if (elementPrevious == 0)
            {
                DoublyLinkedListMethodsBase<TElement>::SetFirst(elementNext);
            }
            else
            {
                DoublyLinkedListMethodsBase<TElement>::SetNext(elementPrevious, elementNext);
            }
            if (elementNext == 0)
            {
                DoublyLinkedListMethodsBase<TElement>::SetLast(elementPrevious);
            }
            else
            {
                DoublyLinkedListMethodsBase<TElement>::SetPrevious(elementNext, elementPrevious);
            }
            DoublyLinkedListMethodsBase<TElement>::SetPrevious(element, 0);
            DoublyLinkedListMethodsBase<TElement>::SetNext(element, 0);
            DoublyLinkedListMethodsBase<TElement>::DecrementSize();
        }
    };
}
