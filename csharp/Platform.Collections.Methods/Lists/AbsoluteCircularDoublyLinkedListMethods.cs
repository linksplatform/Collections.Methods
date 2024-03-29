using System.Numerics;

#pragma warning disable CS1591 // Missing XML comment for publicly visible type or member

namespace Platform.Collections.Methods.Lists
{
    /// <summary>
    /// <para>
    /// Represents the absolute circular doubly linked list methods.
    /// </para>
    /// <para></para>
    /// </summary>
    /// <seealso cref="AbsoluteDoublyLinkedListMethodsBase{TElement}"/>
    public abstract class AbsoluteCircularDoublyLinkedListMethods<TElement> : AbsoluteDoublyLinkedListMethodsBase<TElement> where TElement: IUnsignedNumber<TElement>, IComparisonOperators<TElement, TElement, bool>
    {
        /// <summary>
        /// <para>
        /// Attaches the before using the specified base element.
        /// </para>
        /// <para></para>
        /// </summary>
        /// <param name="baseElement">
        /// <para>The base element.</para>
        /// <para></para>
        /// </param>
        /// <param name="newElement">
        /// <para>The new element.</para>
        /// <para></para>
        /// </param>
        public void AttachBefore(TElement baseElement, TElement newElement)
        {
            var baseElementPrevious = GetPrevious(baseElement);
            SetPrevious(newElement, baseElementPrevious);
            SetNext(newElement, baseElement);
            if ((baseElement == GetFirst()))
            {
                SetFirst(newElement);
            }
            SetNext(baseElementPrevious, newElement);
            SetPrevious(baseElement, newElement);
            IncrementSize();
        }

        /// <summary>
        /// <para>
        /// Attaches the after using the specified base element.
        /// </para>
        /// <para></para>
        /// </summary>
        /// <param name="baseElement">
        /// <para>The base element.</para>
        /// <para></para>
        /// </param>
        /// <param name="newElement">
        /// <para>The new element.</para>
        /// <para></para>
        /// </param>
        public void AttachAfter(TElement baseElement, TElement newElement)
        {
            var baseElementNext = GetNext(baseElement);
            SetPrevious(newElement, baseElement);
            SetNext(newElement, baseElementNext);
            if ((baseElement == GetLast()))
            {
                SetLast(newElement);
            }
            SetPrevious(baseElementNext, newElement);
            SetNext(baseElement, newElement);
            IncrementSize();
        }

        /// <summary>
        /// <para>
        /// Attaches the as first using the specified element.
        /// </para>
        /// <para></para>
        /// </summary>
        /// <param name="element">
        /// <para>The element.</para>
        /// <para></para>
        /// </param>
        public void AttachAsFirst(TElement element)
        {
            var first = GetFirst();
            if (first == TElement.Zero)
            {
                SetFirst(element);
                SetLast(element);
                SetPrevious(element, element);
                SetNext(element, element);
                IncrementSize();
            }
            else
            {
                AttachBefore(first, element);
            }
        }

        /// <summary>
        /// <para>
        /// Attaches the as last using the specified element.
        /// </para>
        /// <para></para>
        /// </summary>
        /// <param name="element">
        /// <para>The element.</para>
        /// <para></para>
        /// </param>
        public void AttachAsLast(TElement element)
        {
            var last = GetLast();
            if (last == TElement.Zero)
            {
                AttachAsFirst(element);
            }
            else
            {
                AttachAfter(last, element);
            }
        }

        /// <summary>
        /// <para>
        /// Detaches the element.
        /// </para>
        /// <para></para>
        /// </summary>
        /// <param name="element">
        /// <para>The element.</para>
        /// <para></para>
        /// </param>
        public void Detach(TElement element)
        {
            var elementPrevious = GetPrevious(element);
            var elementNext = GetNext(element);
            if (elementNext == element)
            {
                SetFirst(TElement.Zero);
                SetLast(TElement.Zero);
            }
            else
            {
                SetNext(elementPrevious, elementNext);
                SetPrevious(elementNext, elementPrevious);
                if ((element == GetFirst()))
                {
                    SetFirst(elementNext);
                }
                if ((element == GetLast()))
                {
                    SetLast(elementPrevious);
                }
            }
            SetPrevious(element, TElement.Zero);
            SetNext(element, TElement.Zero);
            DecrementSize();
        }
    }
}
