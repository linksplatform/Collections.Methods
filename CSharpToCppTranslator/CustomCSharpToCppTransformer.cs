using System;
using System.Collections.Generic;
using System.Linq;
using System.Text.RegularExpressions;
using Platform.RegularExpressions.Transformer;
using Platform.RegularExpressions.Transformer.CSharpToCpp;

namespace CSharpToCppTranslator
{
    public class CustomCSharpToCppTransformer : CSharpToCppTransformer
    {
        public static readonly IList<ISubstitutionRule> Rules = new List<SubstitutionRule>()
        {
            // left = node;
            // *left = node;
            (new Regex(@"([^*])(left|right|root) = ([a-zA-Z0-9]+);"), "$1*$2 = $3;", null, 0),
            // root = RightRotate(root);
            // *root = RightRotate(root);
            (new Regex(@"([^*])(left|right|root) = ([a-zA-Z0-9]+)\(([a-zA-Z0-9]*)\);"), "$1*$2 = $3($4);", null, 0),
            // (left)
            // (*left)
            (new Regex(@"(\(|, |= )(left|right|root|currentNode|parent)(\)|,|;)"), "$1*$2$3", null, 0),
            // Integer<TElement>.Zero
            // 0
            (new Regex(@"(\W)Integer<([a-zA-Z0-9]+)>\.Zero(\W)"), "${1}0$3", null, 0),
            // Integer<TElement>.One
            // 1
            (new Regex(@"(\W)Integer<([a-zA-Z0-9]+)>\.One(\W)"), "${1}1$3", null, 0),
            // Integer<TElement>.Two
            // 2
            (new Regex(@"(\W)Integer<([a-zA-Z0-9]+)>\.Two(\W)"), "${1}2$3", null, 0),
            // (Integer<TElement>)
            // 
            (new Regex(@"\(Integer<[a-zA-Z0-9]+>\)"), "", null, 0),
            // Assert.Equal
            // Assert::Equal
            (new Regex(@"Assert\.Equal"), "Assert::Equal", null, 0),
            // Comparer.Compare(first, second) < 0
            // first < second
            (new Regex(@"Comparer\.Compare\(([_a-zA-Z0-9\*]+), ([_a-zA-Z0-9\*]+)\) (\S{1,2}) 0"), "$1 $3 $2", null, 0),
            // EqualityComparer<TreeElement>.Default.Equals(GetElement(node), default)
            // iszero(GetElement(node), sizeof(TreeElement))
            (new Regex(@"EqualityComparer<([a-zA-Z0-9]+)>\.Default\.Equals\(([_a-zA-Z0-9\*]+)\(([_a-zA-Z0-9\*]*)\), default\)"), "iszero($2($3), sizeof($1))", null, 0),
            // !EqualityComparer.Equals(_allocated, 1)
            // _allocated == 1
            (new Regex(@"!EqualityComparer\.Equals\(([_a-zA-Z0-9\*]+), ([_a-zA-Z0-9\*]+)\)"), "$1 != $2", null, 0),
            // EqualityComparer.Equals(lastNode, node)
            // lastNode == node
            (new Regex(@"EqualityComparer\.Equals\(([_a-zA-Z0-9\*]+), ([_a-zA-Z0-9\*]+)\)"), "$1 == $2", null, 0),
            // EqualityComparer<TreeElement>.Default.Equals(GetElement(node), 0)
            // GetElement(node) == 0
            (new Regex(@"!EqualityComparer<[a-zA-Z0-9]+>\.Default\.Equals\(([_a-zA-Z0-9\*]+)\(([_a-zA-Z0-9\*]*)\), ([_a-zA-Z0-9\*]+)\)"), "$1($2) != $3", null, 0),
            // EqualityComparer<TreeElement>.Default.Equals(GetElement(node), 0)
            // GetElement(node) == 0
            (new Regex(@"EqualityComparer<[a-zA-Z0-9]+>\.Default\.Equals\(([_a-zA-Z0-9\*]+)\(([_a-zA-Z0-9\*]*)\), ([_a-zA-Z0-9\*]+)\)"), "$1($2) == $3", null, 0),
            // !AreEqual(parent, default)
            // parent != 0
            (new Regex(@"!AreEqual\(([a-zA-Z\*]+), default\)"), "$1 != 0", null, 0),
            // AreEqual(node, default)
            // node == 0
            (new Regex(@"AreEqual\(([a-zA-Z\*]+), default\)"), "$1 == 0", null, 0),
            // !AreEqual(size, expectedSize)
            // size != expectedSize
            (new Regex(@"!AreEqual\(([a-zA-Z\*]+), ([a-zA-Z\*]+)\)"), "$1 != $2", null, 0),
            // AreEqual(baseElement, GetFirst())
            // baseElement == GetFirst()
            (new Regex(@"AreEqual\(([a-zA-Z\*]+), ([a-zA-Z\*]+)\(([a-zA-Z\*]*)\)\)"), "$1 == $2($3)", null, 0),
            // AreEqual(elementNext, element)
            // elementNext == element
            (new Regex(@"AreEqual\(([a-zA-Z\*]+), ([a-zA-Z\*]+)\)"), "$1 == $2", null, 0),
            // AreEqual(GetLeft(parent), nodeToDetach)
            // GetLeft(parent) = nodeToDetach
            (new Regex(@"AreEqual\(([a-zA-Z\*]+)\(([a-zA-Z\*]*)\), ([a-zA-Z\*]+)\)"), "$1($2) == $3", null, 0),
            // !EqualToZero(root)
            // root != 0
            (new Regex(@"!EqualToZero\(([a-zA-Z\*]+)\)"), "$1 != 0", null, 0),
            // EqualToZero(node)
            // node == 0
            (new Regex(@"(\W)EqualToZero\(([a-zA-Z\*]+)\)"), "$1$2 == 0", null, 0),
            // Arithmetic.Add(leftSize, rightSize)
            // leftSize + rightSize
            (new Regex(@"Arithmetic\.Add\(([a-zA-Z0-9\*]+), ([a-zA-Z0-9\*]+)\)"), "$1 + $2", null, 0),
            // Add(GetLeftSize(node), GetRightSize(node))
            // GetLeftSize(node) + GetRightSize(node)
            (new Regex(@"Add\(([a-zA-Z0-9\*]+)\(([a-zA-Z0-9 \+\*]+)\), ([a-zA-Z0-9\*]+)\(([a-zA-Z0-9 \+\*]+)\)\)"), "$1($2) + $3($4)", null, 0),
            // Add(rightSize, 2)
            // rightSize + 2
            (new Regex(@"Add\(([a-zA-Z0-9\*]+), ([a-zA-Z0-9\*]+)\)"), "$1 + $2", null, 0),
            // Arithmetic.Increment(leftSize + rightSize)
            // leftSize + rightSize + 1
            (new Regex(@"Arithmetic\.Increment\(([_a-zA-Z0-9 \+\*]+)\)"), "$1 + 1", null, 0),
            // Increment(GetSize(node))
            // GetSize(node) + 1
            (new Regex(@"Increment\(([a-zA-Z0-9\*]+)\(([a-zA-Z0-9 \+\*]*)\)\)"), "$1($2) + 1", null, 0),
            // Increment(GetLeftSize(node) + GetRightSize(node))
            // GetLeftSize(node) + GetRightSize(node) + 1
            (new Regex(@"Increment\(([a-zA-Z0-9\*]+)\(([a-zA-Z0-9 \+\*]*)\) \+ ([a-zA-Z0-9\*]+)\(([a-zA-Z0-9 \+\*]*)\)\)"), "$1($2) + $3($4) + 1", null, 0),
            // Increment(rightLeftSize)
            // rightLeftSize + 1
            (new Regex(@"Increment\(([a-zA-Z0-9\*]+)\)"), "$1 + 1", null, 0),
            // Arithmetic.Decrement(leftSize + rightSize)
            // leftSize + rightSize - 1
            (new Regex(@"Arithmetic\.Decrement\(([_a-zA-Z0-9 \+\*]+)\)"), "$1 - 1", null, 0),
            // Decrement(GetSize(node))
            // GetSize(node) - 1
            (new Regex(@"Decrement\(([a-zA-Z0-9\*]+)\(([a-zA-Z0-9 \+\*]*)\)\)"), "$1($2) - 1", null, 0),
            // Decrement(rightSize)
            // rightSize - 1
            (new Regex(@"Decrement\(([a-zA-Z0-9\*]+)\)"), "$1 - 1", null, 0),
            // GreaterThan(rightLeftSize + 1, leftSize)
            // (rightLeftSize + 1) > leftSize
            (new Regex(@"GreaterThan\(([a-zA-Z0-9\*]+) \+ ([a-zA-Z0-9\*]+), ([a-zA-Z0-9\*]+)\)"), "($1 + $2) > $3", null, 0),
            // GreaterThan(GetSizeOrZero(GetLeft(left)), decrementedRightSize)
            // GetSizeOrZero(GetLeft(left)) > decrementedRightSize
            (new Regex(@"GreaterThan\(([a-zA-Z0-9\*]+)\(([a-zA-Z0-9\*]+)\(([a-zA-Z0-9\*]*)\)\), ([a-zA-Z0-9\*]+)\)"), "$1($2($3)) > $4", null, 0),
            // GreaterThan(GetSize(rootLeftNodeLeftNode), GetSize(rootRightNode))
            // GetSize(rootLeftNodeLeftNode) > GetSize(rootRightNode)
            (new Regex(@"GreaterThan\(([a-zA-Z0-9\*]+)\(([a-zA-Z0-9\*]+)\), ([a-zA-Z0-9\*]+)\(([a-zA-Z0-9\*]*)\)\)"), "$1($2) > $3($4)", null, 0),
            // GreaterThan(leftSize, rightSize)
            // leftSize > rightSize
            (new Regex(@"GreaterThan\(([a-zA-Z0-9\*]+), ([a-zA-Z0-9\*]+)\)"), "$1 > $2", null, 0),
            // GreaterThanZero(leftSize)
            // leftSize > 0
            (new Regex(@"GreaterThanZero\(([a-zA-Z0-9\*]+)\)"), "$1 > 0", null, 0),
            // Zero
            // 0
            (new Regex(@"(\W)Zero(\W)"), "${1}0$2", null, 0),
            // One
            // 1
            (new Regex(@"(\W)One(\W)"), "${1}1$2", null, 0),
            // Two
            // 2
            (new Regex(@"(\W)Two(\W)"), "${1}2$2", null, 0),
            // Just delete it
            (new Regex(@"virtual TElement GetZero\(\)(.|\s)+V3068\s+}"), "", new Regex(@"GenericCollectionMethodsBase\.cs"), 0),
            // Just delete it in SizedBinaryTreeMethodsBase.cs
            (new Regex(@"[\t ]+void FixSizes(.|\s)+};"), "    };", new Regex(@"SizedBinaryTreeMethodsBase\.cs"), 0),
            // Just delete it in SizedAndThreadedAVLBalancedTreeMethods.cs
            (new Regex(@"void PrintNode(.|\s)+?}"), "", new Regex(@"SizedAndThreadedAVLBalancedTreeMethods\.cs"), 0),
            // GetFirst
            // DoublyLinkedListMethodsBase<TElement>::GetFirst
            (new Regex(@"([^>])(GetFirst|GetLast|GetPrevious|GetNext|GetSize|SetFirst|SetLast|SetPrevious|SetNext|SetSize|IncrementSize|DecrementSize)(\()"), "$1this->$2$3", new Regex(@"[a-zA-Z]+DoublyLinkedListMethods\.cs"), 1),
            // GetSize
            // SizedBinaryTreeMethodsBase<TElement>::GetSize
            (new Regex(@"([^>])(GetLeftReference|GetRightReference|GetLeft|GetRight|SetLeft|SetRight|FirstIsToTheLeftOfSecond|FirstIsToTheRightOfSecond|GetLeftOrDefault|GetRightOrDefault|GetLeftSize|GetRightSize|GetSizeOrZero|FixSize|LeftRotate|RightRotate|ClearNode|GetSize|SetSize|IncrementSize|DecrementSize)(\()"), "$1this->$2$3", new Regex(@"Size[a-zA-Z]+Methods2?\.cs"), 1),
            // GetSizeOrZero
            // SizeBalancedTreeMethods<TElement>::GetSizeOrZero
            (new Regex(@"([^:])(GetSizeOrZero)(\()"), "$1this->$2$3", new Regex(@"SizeBalancedTree\.cs"), 0),
            // GetSizeOrZero
            // SizeBalancedTreeMethods2<TElement>::GetSizeOrZero
            (new Regex(@"([^:])(GetSizeOrZero)(\()"), "$1this->$2$3", new Regex(@"SizeBalancedTree2\.cs"), 0),
            // GetSizeOrZero
            // SizedAndThreadedAVLBalancedTreeMethods<TElement>::GetSizeOrZero
            (new Regex(@"([^:])(GetSizeOrZero)(\()"), "$1this->$2$3", new Regex(@"SizedAndThreadedAVLBalancedTree\.cs"), 0),
            // template <typename TElement> class SizeBalancedTree : SizeBalancedTreeMethods<TElement>
            // template <typename TElement, std::size_t N> class SizeBalancedTree : Platform::Collections::Methods::Trees::SizeBalancedTreeMethods<TElement>
            (new Regex(@"template <typename TElement> class ([a-zA-Z0-9]+) : public ([a-zA-Z0-9]+)<TElement>"), "template <typename TElement, std::size_t N> class $1 : public Platform::Collections::Methods::Trees::$2<TElement>", new Regex(@"Size[a-zA-Z]+Tree2?\.cs"), 0),
            // SizeBalancedTree(int capacity) { (_elements, _allocated) = (new TreeElement[capacity], 1); }
            // SizeBalancedTree() { _allocated = 1; }
            (new Regex(@"([a-zA-Z0-9]+)\(int capacity\) { \(_elements, _allocated\) = \(new TreeElement\[capacity\], 1\); }"), "$1() { _allocated = 1; }", new Regex(@"Size[a-zA-Z]+Tree2?\.cs"), 0),
            // *left
            // left
            (new Regex(@"(void SetLeft)([\S\s]+?)\*(left|right)([\S\s]+?)(void SetSize)"), "$1$2$3$4$5", new Regex(@"Size[a-zA-Z]+Tree2?\.cs"), 20),
            // TreeElement _elements[N];
            // TreeElement _elements[N] = { {0} };
            (new Regex(@"TreeElement _elements\[N\];"), "TreeElement _elements[N] = { {0} };", new Regex(@"Size[a-zA-Z]+Tree2?\.cs"), 0),
            // void PrintNodeValue(TElement node, StringBuilder sb) override { sb.Append(node); }
            //
            (new Regex(@"[\r\n]{1,2}\s+[\r\n]{1,2}\s+void PrintNodeValue\(TElement node, StringBuilder sb\) override { sb\.Append\(node\); }"), "", new Regex(@"Size[a-zA-Z]+Tree2?\.cs"), 0),
            // TElement Root;
            // TElement Root = 0;
            (new Regex(@"TElement Root;"), "TElement Root = 0;", new Regex(@"Size[a-zA-Z]+Tree2?\.cs"), 0),
            // SizedBinaryTreeMethodsBase<TElement>::ClearNode
            // ClearNode
            (new Regex(@"this->(ClearNode|GetLeftOrDefault|GetRightOrDefault)"), "$1", new Regex(@"SizedAndThreadedAVLBalancedTreeMethods\.cs"), 0),
            // *root
            // root
            (new Regex(@"(virtual bool Contains)([\S\s]+?)\*root([\S\s]+?)(virtual void ClearNode)"), "$1$2root$3$4", new Regex(@"SizedBinaryTreeMethodsBase\.cs"), 20),
            (new Regex(@"(TElement RightRotate)([\S\s]+?)\*(root|left)([\S\s]+?)(virtual bool Contains)"), "$1$2$3$4$5", new Regex(@"SizedBinaryTreeMethodsBase\.cs"), 20),
            (new Regex(@"(TElement LeftRotate)([\S\s]+?)\*(root|right)([\S\s]+?)(void RightRotate)"), "$1$2$3$4$5", new Regex(@"SizedBinaryTreeMethodsBase\.cs"), 20),
            (new Regex(@"(TElement Balance)([\S\s]+?)\*(left|right)([\S\s]+?)(TElement GetNext)"), "$1$2$3$4$5", new Regex(@"SizedAndThreadedAVLBalancedTreeMethods\.cs"), 30),
            (new Regex(@"(void DetachCore)([\S\s]+?)\*(left|right|currentNode|parent)([\S\s]+?)(void ClearNode)"), "$1$2$3$4$5", new Regex(@"SizedAndThreadedAVLBalancedTreeMethods\.cs"), 60),
            (new Regex(@"(void AttachCore)([\S\s]+?)\*(currentNode|parent)([\S\s]+?)(TElement Balance)"), "$1$2$3$4$5", new Regex(@"SizedAndThreadedAVLBalancedTreeMethods\.cs"), 45),
            (new Regex(@"(bool Contains)([\S\s]+?)\*(root|parent)([\S\s]+?)(void IncrementBalance)"), "$1$2$3$4$5", new Regex(@"SizedAndThreadedAVLBalancedTreeMethods\.cs"), 10),
            // auto path = new TElement[MaxPath];
            // TElement path[MaxPath] = { {0} };
            (new Regex(@"auto path = new TElement\[MaxPath\];"), "TElement path[MaxPath] = { {0} };", new Regex(@"SizedAndThreadedAVLBalancedTreeMethods\.cs"), 0),
            // auto sizeBalancedTree = new SizeBalancedTree<uint>(10000);
            // SizeBalancedTree<uint, 10000> sizeBalancedTree;
            (new Regex(@"auto ([a-zA-Z0-9]+) = new ([a-zA-Z0-9]+)<([_a-zA-Z0-9:]+)>\(([0-9]+)\);"), "$2<$3, $4> $1;", new Regex(@"TreesTests\.cs"), 0),
            // sizeBalancedTree2.Allocate
            // []()-> auto { return sizeBalancedTree2.Allocate(); }
            (new Regex(@"(\(|, )([a-zA-Z0-9]+)\.(Allocate)"), "$1[]()-> auto { return $2.$3(); }", new Regex(@"TreesTests\.cs"), 0),
            // sizeBalancedTree2.Free
            // [](std::uint32_t link)-> auto { sizeBalancedTree2.Free(link); }
            (new Regex(@"(\(|, )([a-zA-Z0-9]+)\.(Free)"), "$1[](std::uint32_t link)-> auto { $2.$3(link); }", new Regex(@"TreesTests\.cs"), 0),
            // sizeBalancedTree.TestMultipleCreationsAndDeletions(
            // TestExtensions::TestMultipleCreationsAndDeletions(sizeBalancedTree, 
            (new Regex(@"([a-zA-Z0-9]+)\.(TestMultipleCreationsAndDeletions|TestMultipleRandomCreationsAndDeletions)\("), "TestExtensions::$2($1, ", new Regex(@"TreesTests\.cs"), 0),
            // auto random = new System.Random(0);
            // 
            (new Regex(@"[\r\n]{1,4}\s+auto random = new System\.Random\(0\);"), "", new Regex(@"TestExtensions\.cs"), 0),
            // random.Next(1, N)
            // (std::rand() % N) + 1
            (new Regex(@"random\.Next\(1, N\)"), "(std::rand() % N) + 1", new Regex(@"TestExtensions\.cs"), 0),
            // auto added = new HashSet<TElement>();
            // std::unordered_set<TElement> added;
            (new Regex(@"auto ([a-zA-Z0-9]+) = new HashSet<([a-zA-Z0-9]+)>\(\);"), "std::unordered_set<$2> $1;", new Regex(@"TestExtensions\.cs"), 0),
            // added.Add(node)
            // added.insert(node)
            (new Regex(@"added\.Add\(([a-zA-Z0-9]+)\)"), "added.insert($1)", new Regex(@"TestExtensions\.cs"), 0),
            // added.Remove(node)
            // added.erase(node) 
            (new Regex(@"added\.Remove\(([a-zA-Z0-9]+)\)"), "added.erase($1)", new Regex(@"TestExtensions\.cs"), 0),
            // if (added.insert(node)) {
            // if (added.find(node) == added.end()) { added.insert(node);
            (new Regex(@"if \(added\.insert\(node\)\)([\t ]*[\r\n]+)([\t ]*){"), "if (added.find(node) == added.end())$1$2{" + Environment.NewLine + "$2    added.insert(node);", new Regex(@"TestExtensions\.cs"), 0),
            // SizedBinaryTreeMethodsBase
            // Platform::Collections::Methods::Trees::SizedBinaryTreeMethodsBase
            (new Regex(@"\(SizedBinaryTreeMethodsBase<TElement>"), "(Platform::Collections::Methods::Trees::SizedBinaryTreeMethodsBase<TElement>&", new Regex(@"TestExtensions\.cs"), 0),
        }.Cast<ISubstitutionRule>().ToList();

        public CustomCSharpToCppTransformer() : base(Rules) { }
    }
}
