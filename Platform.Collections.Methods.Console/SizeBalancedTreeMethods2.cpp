

namespace Platform::Collections::Methods::Trees
{
    template <typename TElement> class SizeBalancedTreeMethods2 : public SizedBinaryTreeMethodsBase<TElement>
    { public:

        virtual TElement* GetLeftReference(TElement node) override = 0;

        virtual TElement* GetRightReference(TElement node) override = 0;

        virtual TElement GetLeft(TElement node) override = 0;

        virtual TElement GetRight(TElement node) override = 0;

        virtual TElement GetSize(TElement node) override = 0;

        virtual void SetLeft(TElement node, TElement left) override = 0;

        virtual void SetRight(TElement node, TElement right) override = 0;

        virtual void SetSize(TElement node, TElement size) override = 0;

        virtual bool FirstIsToTheLeftOfSecond(TElement first, TElement second) override = 0;

        virtual bool FirstIsToTheRightOfSecond(TElement first, TElement second) override = 0;

        void AttachCore(TElement* root, TElement node) override
        {
            if (*root == 0)
            {
                *root = node;
                SizedBinaryTreeMethodsBase<TElement>::IncrementSize(*root);
            }
            else
            {
                SizedBinaryTreeMethodsBase<TElement>::IncrementSize(*root);
                if (FirstIsToTheLeftOfSecond(node, *root))
                {
                    AttachCore(GetLeftReference(*root), node);
                    LeftMaintain(root);
                }
                else
                {
                    AttachCore(GetRightReference(*root), node);
                    RightMaintain(root);
                }
            }
        }

        void DetachCore(TElement* root, TElement nodeToDetach) override
        {
            TElement* currentNode = root;
            TElement* parent = root;
            TElement replacementNode = 0;
            while (currentNode != nodeToDetach)
            {
                SetSize(currentNode, GetSize(currentNode) - 1);
                if (FirstIsToTheLeftOfSecond(nodeToDetach, currentNode))
                {
                    parent = currentNode;
                    currentNode = GetLeftReference(currentNode);
                }
                else if (FirstIsToTheRightOfSecond(nodeToDetach, currentNode))
                {
                    parent = currentNode;
                    currentNode = GetRightReference(currentNode);
                }
                else
                {
                    throw std::exception("Duplicate link found in the tree.");
                }
            }
            TElement nodeToDetachLeft = GetLeft(nodeToDetach);
            TElement node = GetRight(nodeToDetach);
            if (nodeToDetachLeft != 0 && node != 0)
            {
                TElement minNode = node;
                TElement minNodeLeft = GetLeft(minNode);
                while (minNodeLeft != 0)
                {
                    minNode = minNodeLeft;
                    minNodeLeft = GetLeft(minNode);
                }
                DetachCore(GetRightReference(nodeToDetach), minNode);
                SetLeft(minNode, nodeToDetachLeft);
                node = GetRight(nodeToDetach);
                if (node != 0)
                {
                    SetRight(minNode, node);
                    SetSize(minNode, GetSize(nodeToDetachLeft) + GetSize(node) + 1);
                }
                else
                {
                    SetSize(minNode, GetSize(nodeToDetachLeft) + 1);
                }
                replacementNode = minNode;
            }
            else if (nodeToDetachLeft != 0)
            {
                replacementNode = nodeToDetachLeft;
            }
            else if (node != 0)
            {
                replacementNode = node;
            }
            if (*root == nodeToDetach)
            {
                *root = replacementNode;
            }
            else if (GetLeft(parent) == nodeToDetach)
            {
                SetLeft(parent, replacementNode);
            }
            else if (GetRight(parent) == nodeToDetach)
            {
                SetRight(parent, replacementNode);
            }
            SizedBinaryTreeMethodsBase<TElement>::ClearNode(nodeToDetach);
        }

        void LeftMaintain(TElement* root)
        {
            if (*root != 0)
            {
                TElement rootLeftNode = GetLeft(*root);
                if (rootLeftNode != 0)
                {
                    TElement rootRightNode = GetRight(*root);
                    TElement rootLeftNodeLeftNode = GetLeft(rootLeftNode);
                    if (rootLeftNodeLeftNode != 0 &&
                        (rootRightNode == 0 || GetSize(rootLeftNodeLeftNode) > GetSize(rootRightNode)))
                    {
                        SizedBinaryTreeMethodsBase<TElement>::RightRotate(root);
                    }
                    else
                    {
                        TElement rootLeftNodeRightNode = GetRight(rootLeftNode);
                        if (rootLeftNodeRightNode != 0 &&
                            (rootRightNode == 0 || GetSize(rootLeftNodeRightNode) > GetSize(rootRightNode)))
                        {
                            SizedBinaryTreeMethodsBase<TElement>::LeftRotate(GetLeftReference(*root));
                            SizedBinaryTreeMethodsBase<TElement>::RightRotate(root);
                        }
                        else
                        {
                            return;
                        }
                    }
                    LeftMaintain(GetLeftReference(*root));
                    RightMaintain(GetRightReference(*root));
                    LeftMaintain(root);
                    RightMaintain(root);
                }
            }
        }

        void RightMaintain(TElement* root)
        {
            if (*root != 0)
            {
                TElement rootRightNode = GetRight(*root);
                if (rootRightNode != 0)
                {
                    TElement rootLeftNode = GetLeft(*root);
                    TElement rootRightNodeRightNode = GetRight(rootRightNode);
                    if (rootRightNodeRightNode != 0 &&
                        (rootLeftNode == 0 || GetSize(rootRightNodeRightNode) > GetSize(rootLeftNode)))
                    {
                        SizedBinaryTreeMethodsBase<TElement>::LeftRotate(root);
                    }
                    else
                    {
                        TElement rootRightNodeLeftNode = GetLeft(rootRightNode);
                        if (rootRightNodeLeftNode != 0 &&
                            (rootLeftNode == 0 || GetSize(rootRightNodeLeftNode) > GetSize(rootLeftNode)))
                        {
                            SizedBinaryTreeMethodsBase<TElement>::RightRotate(GetRightReference(*root));
                            SizedBinaryTreeMethodsBase<TElement>::LeftRotate(root);
                        }
                        else
                        {
                            return;
                        }
                    }
                    LeftMaintain(GetLeftReference(*root));
                    RightMaintain(GetRightReference(*root));
                    LeftMaintain(root);
                    RightMaintain(root);
                }
            }
        }
    };
}