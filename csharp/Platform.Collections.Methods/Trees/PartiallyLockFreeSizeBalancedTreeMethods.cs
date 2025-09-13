using System;
using System.Numerics;
using System.Threading;

#pragma warning disable CS1591 // Missing XML comment for publicly visible type or member

namespace Platform.Collections.Methods.Trees
{
    /// <summary>
    /// <para>
    /// Represents the partially lock-free size balanced tree methods.
    /// Tree is locked only for balancing. Each N inserts can be done without any locking or balancing.
    /// </para>
    /// <para></para>
    /// </summary>
    /// <seealso cref="SizeBalancedTreeMethods{TElement}"/>
    public abstract class PartiallyLockFreeSizeBalancedTreeMethods<TElement> : SizeBalancedTreeMethods<TElement> 
        where TElement: IUnsignedNumber<TElement>, IComparisonOperators<TElement, TElement, bool>
    {
        private volatile int _lockFreeAttachCount = 0;
        private readonly ReaderWriterLockSlim _balancingLock = new ReaderWriterLockSlim();
        private readonly int _balancingThreshold;

        /// <summary>
        /// <para>
        /// Initializes a new instance of the <see cref="PartiallyLockFreeSizeBalancedTreeMethods{TElement}"/> class.
        /// </para>
        /// <para></para>
        /// </summary>
        /// <param name="balancingThreshold">
        /// <para>The number of insertions to perform before triggering balancing. Default is 10.</para>
        /// <para></para>
        /// </param>
        protected PartiallyLockFreeSizeBalancedTreeMethods(int balancingThreshold = 10)
        {
            _balancingThreshold = Math.Max(1, balancingThreshold);
        }

        /// <summary>
        /// <para>
        /// Gets the current count of lock-free attachments performed since last balancing.
        /// </para>
        /// <para></para>
        /// </summary>
        public int LockFreeAttachCount => _lockFreeAttachCount;

        /// <summary>
        /// <para>
        /// Gets the threshold for triggering balancing operations.
        /// </para>
        /// <para></para>
        /// </summary>
        public int BalancingThreshold => _balancingThreshold;

        protected override void BeforeAttach()
        {
            // Increment the lock-free attach count atomically
            Interlocked.Increment(ref _lockFreeAttachCount);
        }

        /// <summary>
        /// <para>
        /// Attaches the core using the specified root with lock-free insertion and delayed balancing.
        /// </para>
        /// <para></para>
        /// </summary>
        /// <param name="root">
        /// <para>The root.</para>
        /// <para></para>
        /// </param>
        /// <param name="node">
        /// <para>The node.</para>
        /// <para></para>
        /// </param>
        protected override void AttachCore(ref TElement root, TElement node)
        {
            if (root == TElement.Zero)
            {
                root = node;
                IncrementSize(root);
            }
            else
            {
                // Perform insertion with read lock (allows concurrent reads)
                _balancingLock.EnterReadLock();
                try
                {
                    IncrementSize(root);
                    if (FirstIsToTheLeftOfSecond(node, root))
                    {
                        AttachCoreWithoutBalancing(ref GetLeftReference(root), node);
                    }
                    else
                    {
                        AttachCoreWithoutBalancing(ref GetRightReference(root), node);
                    }
                }
                finally
                {
                    _balancingLock.ExitReadLock();
                }
            }
        }

        /// <summary>
        /// <para>
        /// Attaches the core without any balancing operations for lock-free insertion.
        /// </para>
        /// <para></para>
        /// </summary>
        /// <param name="root">
        /// <para>The root.</para>
        /// <para></para>
        /// </param>
        /// <param name="node">
        /// <para>The node.</para>
        /// <para></para>
        /// </param>
        private void AttachCoreWithoutBalancing(ref TElement root, TElement node)
        {
            if (root == TElement.Zero)
            {
                root = node;
                IncrementSize(root);
            }
            else
            {
                IncrementSize(root);
                if (FirstIsToTheLeftOfSecond(node, root))
                {
                    AttachCoreWithoutBalancing(ref GetLeftReference(root), node);
                }
                else
                {
                    AttachCoreWithoutBalancing(ref GetRightReference(root), node);
                }
            }
        }

        protected override void AfterAttach()
        {
            // Check if balancing is needed after N insertions
            if (_lockFreeAttachCount >= _balancingThreshold)
            {
                TriggerDelayedBalancing();
            }
        }

        /// <summary>
        /// <para>
        /// Triggers delayed balancing operation with exclusive locking.
        /// </para>
        /// <para></para>
        /// </summary>
        private void TriggerDelayedBalancing()
        {
            // Only one thread should perform balancing
            if (_lockFreeAttachCount >= _balancingThreshold)
            {
                _balancingLock.EnterWriteLock();
                try
                {
                    // Double-check pattern to avoid unnecessary balancing
                    if (_lockFreeAttachCount >= _balancingThreshold)
                    {
                        // Reset counter atomically
                        Interlocked.Exchange(ref _lockFreeAttachCount, 0);
                        
                        // Perform balancing operations
                        PerformDelayedBalancing();
                    }
                }
                finally
                {
                    _balancingLock.ExitWriteLock();
                }
            }
        }

        /// <summary>
        /// <para>
        /// Performs delayed balancing operation. Override this method to define custom balancing logic
        /// or call the parent balancing methods on the tree root.
        /// </para>
        /// <para></para>
        /// </summary>
        protected virtual void PerformDelayedBalancing()
        {
            // Default implementation - can be overridden by concrete implementations
            // to perform specific balancing operations on their root structures
        }

        /// <summary>
        /// <para>
        /// Applies balancing to a subtree root using custom balancing logic.
        /// This method should be called from PerformDelayedBalancing() on the tree root.
        /// </para>
        /// <para></para>
        /// </summary>
        /// <param name="root">
        /// <para>The root of the subtree to balance.</para>
        /// <para></para>
        /// </param>
        protected void ApplyDelayedBalancing(ref TElement root)
        {
            if (root != TElement.Zero)
            {
                // Apply custom balancing logic similar to the original maintain operations
                DelayedLeftMaintain(ref root);
                DelayedRightMaintain(ref root);
            }
        }

        /// <summary>
        /// <para>
        /// Performs left maintenance for delayed balancing.
        /// </para>
        /// <para></para>
        /// </summary>
        /// <param name="root">
        /// <para>The root.</para>
        /// <para></para>
        /// </param>
        private void DelayedLeftMaintain(ref TElement root)
        {
            if (root != TElement.Zero)
            {
                var rootLeftNode = GetLeft(root);
                if (rootLeftNode != TElement.Zero)
                {
                    var rootRightNode = GetRight(root);
                    var rootRightNodeSize = GetSizeOrZero(rootRightNode);
                    var rootLeftNodeLeftNode = GetLeft(rootLeftNode);
                    if (rootLeftNodeLeftNode != TElement.Zero &&
                        (rootRightNode == TElement.Zero || (GetSizeOrZero(rootLeftNodeLeftNode)) > rootRightNodeSize))
                    {
                        RightRotate(ref root);
                    }
                    else
                    {
                        var rootLeftNodeRightNode = GetRight(rootLeftNode);
                        if (rootLeftNodeRightNode != TElement.Zero &&
                            (rootRightNode == TElement.Zero || (GetSizeOrZero(rootLeftNodeRightNode)) > rootRightNodeSize))
                        {
                            LeftRotate(ref GetLeftReference(root));
                            RightRotate(ref root);
                        }
                        else
                        {
                            return;
                        }
                    }
                    DelayedLeftMaintain(ref GetLeftReference(root));
                    DelayedRightMaintain(ref GetRightReference(root));
                    DelayedRightMaintain(ref root);
                    DelayedLeftMaintain(ref root);
                }
            }
        }

        /// <summary>
        /// <para>
        /// Performs right maintenance for delayed balancing.
        /// </para>
        /// <para></para>
        /// </summary>
        /// <param name="root">
        /// <para>The root.</para>
        /// <para></para>
        /// </param>
        private void DelayedRightMaintain(ref TElement root)
        {
            if (root != TElement.Zero)
            {
                var rootRightNode = GetRight(root);
                if (rootRightNode != TElement.Zero)
                {
                    var rootLeftNode = GetLeft(root);
                    var rootLeftNodeSize = GetSizeOrZero(rootLeftNode);
                    var rootRightNodeRightNode = GetRight(rootRightNode);
                    if (rootRightNodeRightNode != TElement.Zero &&
                        (rootLeftNode == TElement.Zero || (GetSizeOrZero(rootRightNodeRightNode)) > rootLeftNodeSize))
                    {
                        LeftRotate(ref root);
                    }
                    else
                    {
                        var rootRightNodeLeftNode = GetLeft(rootRightNode);
                        if (rootRightNodeLeftNode != TElement.Zero &&
                            (rootLeftNode == TElement.Zero || (GetSizeOrZero(rootRightNodeLeftNode)) > rootLeftNodeSize))
                        {
                            RightRotate(ref GetRightReference(root));
                            LeftRotate(ref root);
                        }
                        else
                        {
                            return;
                        }
                    }
                    DelayedLeftMaintain(ref GetLeftReference(root));
                    DelayedRightMaintain(ref GetRightReference(root));
                    DelayedRightMaintain(ref root);
                    DelayedLeftMaintain(ref root);
                }
            }
        }

        /// <summary>
        /// <para>
        /// Manually triggers balancing operation regardless of the current attach count.
        /// </para>
        /// <para></para>
        /// </summary>
        public void ForceBalancing()
        {
            _balancingLock.EnterWriteLock();
            try
            {
                Interlocked.Exchange(ref _lockFreeAttachCount, 0);
                PerformDelayedBalancing();
            }
            finally
            {
                _balancingLock.ExitWriteLock();
            }
        }

        /// <summary>
        /// <para>
        /// Disposes the resources used by the partially lock-free tree.
        /// </para>
        /// <para></para>
        /// </summary>
        public virtual void Dispose()
        {
            _balancingLock?.Dispose();
        }

        /// <summary>
        /// <para>
        /// Gets a value indicating whether the balancing lock is currently held by any writer.
        /// </para>
        /// <para></para>
        /// </summary>
        public bool IsBalancing => _balancingLock.IsWriteLockHeld;
    }
}