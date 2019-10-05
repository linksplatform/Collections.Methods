using System;
using System.IO;
using System.Text.RegularExpressions;

namespace Translator
{
    class Program
    {
        private static readonly RegexOptions _options = RegexOptions.Compiled | RegexOptions.Multiline;

        private static readonly ValueTuple<Regex, string, Regex, int>[] _transformations =
        {
            // #pragma warning disable CS1591 // Missing XML comment for publicly visible type or member
            // 
            (new Regex(@"^\s*?\#pragma[\sa-zA-Z0-9\/]+$", _options), "", null, 0),
            // [MethodImpl(MethodImplOptions.AggressiveInlining)]
            // 
            (new Regex(@"$\s+\[MethodImpl\(MethodImplOptions\.AggressiveInlining\)\]", _options), "", null, 0),
            // [Fact]
            // 
            (new Regex(@"$\s+\[Fact\]", _options), "", null, 0),
            // {\n\n\n
            // {
            (new Regex(@"{\s+[\r\n]+", _options), "{" + Environment.NewLine, null, 0),
            // Platform.Collections.Methods.Lists
            // Platform::Collections::Methods::Lists
            (new Regex(@"(namespace[^\r\n]+?)\.([^\r\n]+?)", _options), "$1::$2", null, 20),
            // public abstract class
            // class
            (new Regex(@"(public abstract|static) class", _options), "class", null, 0),
            // class GenericCollectionMethodsBase {
            // class GenericCollectionMethodsBase { public:
            (new Regex(@"class ([a-zA-Z0-9]+)(\s+){", _options), "class $1$2{" + Environment.NewLine + "    public:", null, 0),
            // class GenericCollectionMethodsBase<TElement> {
            // template <typename TElement> class GenericCollectionMethodsBase { public:
            (new Regex(@"class ([a-zA-Z0-9]+)<([a-zA-Z0-9]+)>([^{]+){", _options), "template <typename $2> class $1$3{"  + Environment.NewLine +  "    public:", null, 0),
            // static void TestMultipleCreationsAndDeletions<TElement>(SizedBinaryTreeMethodsBase<TElement> tree, TElement* root)
            // template<typename T> static void TestMultipleCreationsAndDeletions<TElement>(SizedBinaryTreeMethodsBase<TElement> tree, TElement* root)
            (new Regex(@"static ([a-zA-Z0-9]+) ([a-zA-Z0-9]+)<([a-zA-Z0-9]+)>\(([^\)]+)\)", _options), "template <typename $3> static $1 $2($4)", null, 0),
            // (this 
            // (
            (new Regex(@"\(this ", _options), "(", null, 0),
            // Func<TElement> treeCount
            // TElement(*treeCount)()
            (new Regex(@"Func<([a-zA-Z0-9]+)> ([a-zA-Z0-9]+)", _options), "$1(*$2)()", null, 0),
            // Action<TElement> free
            // void (*free)(TElement)
            (new Regex(@"Action<([a-zA-Z0-9]+)> ([a-zA-Z0-9]+)", _options), "void (*$2)($1)", null, 0),
            // private const int MaxPath = 92;
            // static const int MaxPath = 92;
            (new Regex(@"private const ([a-zA-Z0-9]+) ([_a-zA-Z0-9]+) = ([a-zA-Z0-9]+);", _options), "static const $1 $2 = $3;", null, 0),
            // protected virtual
            // virtual
            (new Regex(@"protected virtual", _options), "virtual", null, 0),
            // protected abstract TElement GetFirst();
            // virtual TElement GetFirst() = 0;
            (new Regex(@"protected abstract ([^;]+);", _options), "virtual $1 = 0;", null, 0),
            // public virtual
            // virtual
            (new Regex(@"public virtual", _options), "virtual", null, 0),
            // protected readonly
            // 
            (new Regex(@"protected readonly ", _options), "", null, 0),
            // protected readonly TreeElement[] _elements;
            // TreeElement _elements[N];
            (new Regex(@"(protected|private) readonly ([a-zA-Z<>0-9]+)([\[\]]+) ([_a-zA-Z0-9]+);", _options), "$2 $4[N];", null, 0),
            // protected readonly TElement Zero;
            // TElement Zero;
            (new Regex(@"(protected|private) readonly ([a-zA-Z<>0-9]+) ([_a-zA-Z0-9]+);", _options), "$2 $3;", null, 0),
            // private
            // 
            (new Regex(@"(\W)(private|protected|public|internal) ", _options), "$1", null, 0),
            // SizeBalancedTree(int capacity) => a = b;
            // SizeBalancedTree(int capacity) { a = b; }
            (new Regex(@"(^\s+)(override )?(void )?([a-zA-Z0-9]+)\(([^\(]+)\)\s+=>\s+([^;]+);", _options), "$1$2$3$4($5) { $6; }", null, 0),
            // => Integer<TElement>.Zero,
            // { return Integer<TElement>.Zero; },
            (new Regex(@"\)\s+=>\s+([^\r\n,]+?),", _options), ") { return $1; },", new Regex(@"TreesTests\.cs", _options), 0),
            // => Integer<TElement>.Zero;
            // { return Integer<TElement>.Zero; }
            (new Regex(@"\)\s+=>\s+([^\r\n;]+?);", _options), ") { return $1; }", null, 0),
            // () { return avlTree.Count; }
            // []()-> auto { return avlTree.Count; }
            (new Regex(@", \(\) { return ([^;]+); }", _options), ", []()-> auto { return $1; }", null, 0),
            // Count => GetSizeOrZero(Root);
            // GetCount() { return GetSizeOrZero(Root); }
            (new Regex(@"([A-Z][a-z]+)\s+=>\s+([^;]+);", _options), "Get$1() { return $2; }", null, 0),
            // left = node;
            // *left = node;
            (new Regex(@"([^*])(left|right|root) = ([a-zA-Z0-9]+);", _options), "$1*$2 = $3;", null, 0),
            // root = RightRotate(root);
            // *root = RightRotate(root);
            (new Regex(@"([^*])(left|right|root) = ([a-zA-Z0-9]+)\(([a-zA-Z0-9]*)\);", _options), "$1*$2 = $3($4);", null, 0),
            // (left)
            // (*left)
            (new Regex(@"(\(|, |= )(left|right|root)(\)|,|;)", _options), "$1*$2$3", null, 0),
            // ref sizeBalancedTree2.Root
            // &sizeBalancedTree2.Root
            (new Regex(@"ref ([a-zA-Z0-9]+)\.([a-zA-Z0-9]+)", _options), "&$1.$2", null, 0),
            // ref GetElement(node).Right
            // &GetElement(node).Right
            (new Regex(@"ref ([a-zA-Z0-9]+)\(([a-zA-Z0-9]+)\)\.([a-zA-Z0-9]+)", _options), "&$1($2).$3", null, 0),
            // GetElement(node).Right
            // GetElement(node)->Right
            (new Regex(@"([a-zA-Z0-9]+)\(([a-zA-Z0-9]+)\)\.([a-zA-Z0-9]+)", _options), "$1($2)->$3", null, 0),
            // = ref GetLeftReference
            // = GetLeftReference
            (new Regex(@" = ref ([a-zA-Z0-9]+)", _options), " = $1", null, 0),
            // (ref left)
            // (left)
            (new Regex(@"\(ref ([a-zA-Z0-9]+)(\)|\(|,)", _options), "($1$2", null, 0),
            // ref TElement
            // TElement*
            (new Regex(@"ref ([a-zA-Z0-9\*]+)", _options), "$1*", null, 0),
            // return ref _elements[(Integer<TElement>)node];
            // return &_elements[node];
            (new Regex(@"return ref ([_a-zA-Z0-9\*]+)\[\(Integer<([a-zA-Z0-9\*]+)>\)([_a-zA-Z0-9\*]+)\];", _options), "return &$1[$3];", null, 0),
            // Integer<TElement>.Zero
            // 0
            (new Regex(@"(\W)Integer<([a-zA-Z0-9]+)>\.Zero(\W)", _options), "${1}0$3", null, 0),
            // Integer<TElement>.One
            // 1
            (new Regex(@"(\W)Integer<([a-zA-Z0-9]+)>\.One(\W)", _options), "${1}1$3", null, 0),
            // Integer<TElement>.Two
            // 2
            (new Regex(@"(\W)Integer<([a-zA-Z0-9]+)>\.Two(\W)", _options), "${1}2$3", null, 0),
            // (Integer<TElement>)
            // 
            (new Regex(@"\(Integer<[a-zA-Z0-9]+>\)", _options), "", null, 0),
            // Assert.Equal
            // Assert::Equal
            (new Regex(@"Assert\.Equal", _options), "Assert::Equal", null, 0),
            // Comparer.Compare(first, second) < 0
            // first < second
            (new Regex(@"Comparer\.Compare\(([_a-zA-Z0-9\*]+), ([_a-zA-Z0-9\*]+)\) (\S{1,2}) 0", _options), "$1 $3 $2", null, 0),
            // EqualityComparer<TreeElement>.Default.Equals(GetElement(node), default)
            // iszero(GetElement(node), sizeof(TreeElement))
            (new Regex(@"EqualityComparer<([a-zA-Z0-9]+)>\.Default\.Equals\(([_a-zA-Z0-9\*]+)\(([_a-zA-Z0-9\*]*)\), default\)", _options), "iszero($2($3), sizeof($1))", null, 0),
            // !EqualityComparer.Equals(_allocated, 1)
            // _allocated == 1
            (new Regex(@"!EqualityComparer\.Equals\(([_a-zA-Z0-9\*]+), ([_a-zA-Z0-9\*]+)\)", _options), "$1 != $2", null, 0),
            // EqualityComparer.Equals(lastNode, node)
            // lastNode == node
            (new Regex(@"EqualityComparer\.Equals\(([_a-zA-Z0-9\*]+), ([_a-zA-Z0-9\*]+)\)", _options), "$1 == $2", null, 0),
            // EqualityComparer<TreeElement>.Default.Equals(GetElement(node), 0)
            // GetElement(node) == 0
            (new Regex(@"!EqualityComparer<[a-zA-Z0-9]+>\.Default\.Equals\(([_a-zA-Z0-9\*]+)\(([_a-zA-Z0-9\*]*)\), ([_a-zA-Z0-9\*]+)\)", _options), "$1($2) != $3", null, 0),
            // EqualityComparer<TreeElement>.Default.Equals(GetElement(node), 0)
            // GetElement(node) == 0
            (new Regex(@"EqualityComparer<[a-zA-Z0-9]+>\.Default\.Equals\(([_a-zA-Z0-9\*]+)\(([_a-zA-Z0-9\*]*)\), ([_a-zA-Z0-9\*]+)\)", _options), "$1($2) == $3", null, 0),
            // !AreEqual(parent, default)
            // parent != 0
            (new Regex(@"!AreEqual\(([a-zA-Z\*]+), default\)", _options), "$1 != 0", null, 0),
            // AreEqual(node, default)
            // node == 0
            (new Regex(@"AreEqual\(([a-zA-Z\*]+), default\)", _options), "$1 == 0", null, 0),
            // !AreEqual(size, expectedSize)
            // size != expectedSize
            (new Regex(@"!AreEqual\(([a-zA-Z\*]+), ([a-zA-Z\*]+)\)", _options), "$1 != $2", null, 0),
            // AreEqual(baseElement, GetFirst())
            // baseElement == GetFirst()
            (new Regex(@"AreEqual\(([a-zA-Z\*]+), ([a-zA-Z\*]+)\(([a-zA-Z\*]*)\)\)", _options), "$1 == $2($3)", null, 0),
            // AreEqual(elementNext, element)
            // elementNext == element
            (new Regex(@"AreEqual\(([a-zA-Z\*]+), ([a-zA-Z\*]+)\)", _options), "$1 == $2", null, 0),
            // AreEqual(GetLeft(parent), nodeToDetach)
            // GetLeft(parent) = nodeToDetach
            (new Regex(@"AreEqual\(([a-zA-Z\*]+)\(([a-zA-Z\*]*)\), ([a-zA-Z\*]+)\)", _options), "$1($2) == $3", null, 0),
            // !EqualToZero(root)
            // root != 0
            (new Regex(@"!EqualToZero\(([a-zA-Z\*]+)\)", _options), "$1 != 0", null, 0),
            // EqualToZero(node)
            // node == 0
            (new Regex(@"(\W)EqualToZero\(([a-zA-Z\*]+)\)", _options), "$1$2 == 0", null, 0),
            // default
            // 0
            (new Regex(@"(\W)default(\W)", _options), "${1}0$2", null, 0),
            // Arithmetic.Add(leftSize, rightSize)
            // leftSize + rightSize
            (new Regex(@"Arithmetic\.Add\(([a-zA-Z0-9\*]+), ([a-zA-Z0-9\*]+)\)", _options), "$1 + $2", null, 0),
            // Add(GetLeftSize(node), GetRightSize(node))
            // GetLeftSize(node) + GetRightSize(node)
            (new Regex(@"Add\(([a-zA-Z0-9\*]+)\(([a-zA-Z0-9 \+\*]+)\), ([a-zA-Z0-9\*]+)\(([a-zA-Z0-9 \+\*]+)\)\)", _options), "$1($2) + $3($4)", null, 0),
            // Add(rightSize, 2)
            // rightSize + 2
            (new Regex(@"Add\(([a-zA-Z0-9\*]+), ([a-zA-Z0-9\*]+)\)", _options), "$1 + $2", null, 0),
            // Arithmetic.Increment(leftSize + rightSize)
            // leftSize + rightSize + 1
            (new Regex(@"Arithmetic\.Increment\(([_a-zA-Z0-9 \+\*]+)\)", _options), "$1 + 1", null, 0),
            // Increment(GetSize(node))
            // GetSize(node) + 1
            (new Regex(@"Increment\(([a-zA-Z0-9\*]+)\(([a-zA-Z0-9 \+\*]*)\)\)", _options), "$1($2) + 1", null, 0),
            // Increment(GetLeftSize(node) + GetRightSize(node))
            // GetLeftSize(node) + GetRightSize(node) + 1
            (new Regex(@"Increment\(([a-zA-Z0-9\*]+)\(([a-zA-Z0-9 \+\*]*)\) \+ ([a-zA-Z0-9\*]+)\(([a-zA-Z0-9 \+\*]*)\)\)", _options), "$1($2) + $3($4) + 1", null, 0),
            // Increment(rightLeftSize)
            // rightLeftSize + 1
            (new Regex(@"Increment\(([a-zA-Z0-9\*]+)\)", _options), "$1 + 1", null, 0),
            // Arithmetic.Decrement(leftSize + rightSize)
            // leftSize + rightSize - 1
            (new Regex(@"Arithmetic\.Decrement\(([_a-zA-Z0-9 \+\*]+)\)", _options), "$1 - 1", null, 0),
            // Decrement(GetSize(node))
            // GetSize(node) - 1
            (new Regex(@"Decrement\(([a-zA-Z0-9\*]+)\(([a-zA-Z0-9 \+\*]*)\)\)", _options), "$1($2) - 1", null, 0),
            // Decrement(rightSize)
            // rightSize - 1
            (new Regex(@"Decrement\(([a-zA-Z0-9\*]+)\)", _options), "$1 - 1", null, 0),
            // GreaterThan(rightLeftSize + 1, leftSize)
            // (rightLeftSize + 1) > leftSize
            (new Regex(@"GreaterThan\(([a-zA-Z0-9\*]+) \+ ([a-zA-Z0-9\*]+), ([a-zA-Z0-9\*]+)\)", _options), "($1 + $2) > $3", null, 0),
            // GreaterThan(GetSizeOrZero(GetLeft(left)), decrementedRightSize)
            // GetSizeOrZero(GetLeft(left)) > decrementedRightSize
            (new Regex(@"GreaterThan\(([a-zA-Z0-9\*]+)\(([a-zA-Z0-9\*]+)\(([a-zA-Z0-9\*]*)\)\), ([a-zA-Z0-9\*]+)\)", _options), "$1($2($3)) > $4", null, 0),
            // GreaterThan(GetSize(rootLeftNodeLeftNode), GetSize(rootRightNode))
            // GetSize(rootLeftNodeLeftNode) > GetSize(rootRightNode)
            (new Regex(@"GreaterThan\(([a-zA-Z0-9\*]+)\(([a-zA-Z0-9\*]+)\), ([a-zA-Z0-9\*]+)\(([a-zA-Z0-9\*]*)\)\)", _options), "$1($2) > $3($4)", null, 0),
            // GreaterThan(leftSize, rightSize)
            // leftSize > rightSize
            (new Regex(@"GreaterThan\(([a-zA-Z0-9\*]+), ([a-zA-Z0-9\*]+)\)", _options), "$1 > $2", null, 0),
            // GreaterThanZero(leftSize)
            // leftSize > 0
            (new Regex(@"GreaterThanZero\(([a-zA-Z0-9\*]+)\)", _options), "$1 > 0", null, 0),
            // var
            // auto
            (new Regex(@"(\W)var(\W)", _options), "$1auto$2", null, 0),
            // unchecked
            // 
            (new Regex(@"[\r\n]{2}\s*?unchecked\s*?$", _options), "", null, 0),
            // $"
            // "
            (new Regex(@"\$""", _options), "\"", null, 0),
            // throw new InvalidOperationException
            // throw std::exception
            (new Regex(@"throw new (InvalidOperationException|Exception)", _options), "throw std::exception", null, 0),
            // override void PrintNode(TElement node, StringBuilder sb, int level)
            // void PrintNode(TElement node, StringBuilder sb, int level) override
            (new Regex(@"override ([a-zA-Z0-9 \*\+]+)(\([^\)]+?\))", _options), "$1$2 override", null, 0),
            // Zero
            // 0
            (new Regex(@"(\W)Zero(\W)", _options), "${1}0$2", null, 0),
            // One
            // 1
            (new Regex(@"(\W)One(\W)", _options), "${1}1$2", null, 0),
            // Two
            // 2
            (new Regex(@"(\W)Two(\W)", _options), "${1}2$2", null, 0),
            // Just delete it
            (new Regex(@"virtual TElement GetZero\(\)(.|\s)+V3068\s+}", _options), "", null, 0),
            // sbyte
            // std::int8_t
            (new Regex(@"(\W)sbyte(\W)", _options), "$1std::int8_t$2", null, 0),
            // uint
            // std::uint32_t
            (new Regex(@"(\W)uint(\W)", _options), "$1std::uint32_t$2", null, 0),
            // using Platform.Numbers;
            // 
            (new Regex(@"([\r\n]{2}|^)\s*?using [\.a-zA-Z0-9]+;\s*?$", _options), "", null, 0),
            // struct TreeElement { }
            // struct TreeElement { };
            (new Regex(@"struct ([a-zA-Z0-9]+)(\s+){([\sa-zA-Z0-9;:_]+?)}([^;])", _options), "struct $1$2{$3};$4", null, 0),
            // \t}\n}
            // \t};\n}
            (new Regex(@"([\r\n])[\t ]{0,8}\}[ \t]*[\r\n]+\}", _options), "$1    };" + Environment.NewLine + "}", null, 0),
            // void PrintNodeValue(TElement node, StringBuilder sb) override { sb.Append(node); }
            //
            (new Regex(@"[\r\n]{1,2}\s+[\r\n]{1,2}\s+void PrintNodeValue\(TElement node, StringBuilder sb\) override { sb\.Append\(node\); }", _options), "", null, 0),
            // Just delete it in SizedBinaryTreeMethodsBase.cs
            (new Regex(@"[\t ]+void FixSizes(.|\s)+};", _options), "    };", new Regex(@"SizedBinaryTreeMethodsBase\.cs", _options), 0),
            // Just delete it in SizedAndThreadedAVLBalancedTreeMethods.cs
            (new Regex(@"void PrintNode(.|\s)+?}", _options), "", new Regex(@"SizedAndThreadedAVLBalancedTreeMethods\.cs", _options), 0),
            // GetFirst
            // DoublyLinkedListMethodsBase<TElement>::GetFirst
            (new Regex(@"([^>])(GetFirst|GetLast|GetPrevious|GetNext|GetSize|SetFirst|SetLast|SetPrevious|SetNext|SetSize|IncrementSize|DecrementSize)(\()", _options), "$1this->$2$3", new Regex(@"[a-zA-Z]+DoublyLinkedListMethods\.cs", _options), 1),
            // GetSize
            // SizedBinaryTreeMethodsBase<TElement>::GetSize
            (new Regex(@"([^>])(GetLeftReference|GetRightReference|GetLeft|GetRight|SetLeft|SetRight|FirstIsToTheLeftOfSecond|FirstIsToTheRightOfSecond|GetLeftOrDefault|GetRightOrDefault|GetLeftSize|GetRightSize|GetSizeOrZero|FixSize|LeftRotate|RightRotate|ClearNode|GetSize|SetSize|IncrementSize|DecrementSize)(\()", _options), "$1this->$2$3", new Regex(@"Size[a-zA-Z]+Methods2?\.cs", _options), 1),
            // auto*
            // TElement*
            (new Regex(@"auto\*", _options), "TElement*", new Regex(@"Size[a-zA-Z]+Methods2?\.cs", _options), 0),
            // auto
            // TElement
            (new Regex(@"auto", _options), "TElement", new Regex(@"Size[a-zA-Z]+Methods2?\.cs", _options), 0),
            // GetSizeOrZero
            // SizeBalancedTreeMethods<TElement>::GetSizeOrZero
            (new Regex(@"([^:])(GetSizeOrZero)(\()", _options), "$1this->$2$3", new Regex(@"SizeBalancedTree\.cs", _options), 0),
            // GetSizeOrZero
            // SizeBalancedTreeMethods2<TElement>::GetSizeOrZero
            (new Regex(@"([^:])(GetSizeOrZero)(\()", _options), "$1this->$2$3", new Regex(@"SizeBalancedTree2\.cs", _options), 0),
            // GetSizeOrZero
            // SizedAndThreadedAVLBalancedTreeMethods<TElement>::GetSizeOrZero
            (new Regex(@"([^:])(GetSizeOrZero)(\()", _options), "$1this->$2$3", new Regex(@"SizedAndThreadedAVLBalancedTree\.cs", _options), 0),
            // class SizedBinaryTreeMethodsBase : GenericCollectionMethodsBase
            // class SizedBinaryTreeMethodsBase : public GenericCollectionMethodsBase
            (new Regex(@"class ([a-zA-Z0-9]+) : ([a-zA-Z0-9]+)", _options), "class $1 : public $2", null, 0),
            // template <typename TElement> class SizeBalancedTree : SizeBalancedTreeMethods<TElement>
            // template <typename TElement, std::size_t N> class SizeBalancedTree : Platform::Collections::Methods::Trees::SizeBalancedTreeMethods<TElement>
            (new Regex(@"template <typename TElement> class ([a-zA-Z0-9]+) : public ([a-zA-Z0-9]+)<TElement>", _options), "template <typename TElement, std::size_t N> class $1 : public Platform::Collections::Methods::Trees::$2<TElement>", new Regex(@"Size[a-zA-Z]+Tree2?\.cs", _options), 0),
            // SizeBalancedTree(int capacity) { (_elements, _allocated) = (new TreeElement[capacity], 1); }
            // SizeBalancedTree() { _allocated = 1; }
            (new Regex(@"([a-zA-Z0-9]+)\(int capacity\) { \(_elements, _allocated\) = \(new TreeElement\[capacity\], 1\); }", _options), "$1() { _allocated = 1; }", new Regex(@"Size[a-zA-Z]+Tree2?\.cs", _options), 0),
            // *left
            // left
            (new Regex(@"(void SetLeft)([\S\s]+?)\*(left|right)([\S\s]+?)(void SetSize)", _options), "$1$2$3$4$5", new Regex(@"Size[a-zA-Z]+Tree2?\.cs", _options), 20),
            // TreeElement _elements[N];
            // TreeElement _elements[N] = { {0} };
            (new Regex(@"TreeElement _elements\[N\];", _options), "TreeElement _elements[N] = { {0} };", new Regex(@"Size[a-zA-Z]+Tree2?\.cs", _options), 0),
            // TElement Root;
            // TElement Root = 0;
            (new Regex(@"TElement Root;", _options), "TElement Root = 0;", new Regex(@"Size[a-zA-Z]+Tree2?\.cs", _options), 0),
            // SizedBinaryTreeMethodsBase<TElement>::ClearNode
            // ClearNode
            (new Regex(@"this->(ClearNode|GetLeftOrDefault|GetRightOrDefault)", _options), "$1", new Regex(@"SizedAndThreadedAVLBalancedTreeMethods\.cs", _options), 0),
            // *root
            // root
            (new Regex(@"(virtual bool Contains)([\S\s]+?)\*root([\S\s]+?)(virtual void ClearNode)", _options), "$1$2root$3$4", new Regex(@"SizedBinaryTreeMethodsBase\.cs", _options), 20),
            (new Regex(@"(TElement RightRotate)([\S\s]+?)\*(root|left)([\S\s]+?)(virtual bool Contains)", _options), "$1$2$3$4$5", new Regex(@"SizedBinaryTreeMethodsBase\.cs", _options), 20),
            (new Regex(@"(TElement LeftRotate)([\S\s]+?)\*(root|right)([\S\s]+?)(void RightRotate)", _options), "$1$2$3$4$5", new Regex(@"SizedBinaryTreeMethodsBase\.cs", _options), 20),
            (new Regex(@"(TElement Balance)([\S\s]+?)\*(left|right)([\S\s]+?)(TElement GetNext)", _options), "$1$2$3$4$5", new Regex(@"SizedAndThreadedAVLBalancedTreeMethods\.cs", _options), 30),
            (new Regex(@"(void DetachCore)([\S\s]+?)\*(left|right)([\S\s]+?)(void ClearNode)", _options), "$1$2$3$4$5", new Regex(@"SizedAndThreadedAVLBalancedTreeMethods\.cs", _options), 15),
            // auto sizeBalancedTree = new SizeBalancedTree<uint>(10000);
            // SizeBalancedTree<uint, 10000> sizeBalancedTree;
            (new Regex(@"auto ([a-zA-Z0-9]+) = new ([a-zA-Z0-9]+)<([_a-zA-Z0-9:]+)>\(([0-9]+)\);", _options), "$2<$3, $4> $1;", new Regex(@"TreesTests\.cs", _options), 0),
            // sizeBalancedTree2.Allocate
            // []()-> auto { return sizeBalancedTree2.Allocate(); }
            (new Regex(@"(\(|, )([a-zA-Z0-9]+)\.(Allocate)", _options), "$1[]()-> auto { return $2.$3(); }", new Regex(@"TreesTests\.cs", _options), 0),
            // sizeBalancedTree2.Free
            // [](std::uint32_t link)-> auto { sizeBalancedTree2.Free(link); }
            (new Regex(@"(\(|, )([a-zA-Z0-9]+)\.(Free)", _options), "$1[](std::uint32_t link)-> auto { $2.$3(link); }", new Regex(@"TreesTests\.cs", _options), 0),
            // sizeBalancedTree.TestMultipleCreationsAndDeletions(
            // TestExtensions::TestMultipleCreationsAndDeletions(sizeBalancedTree, 
            (new Regex(@"([a-zA-Z0-9]+)\.(TestMultipleCreationsAndDeletions|TestMultipleRandomCreationsAndDeletions)\(", _options), "TestExtensions::$2($1, ", new Regex(@"TreesTests\.cs", _options), 0),
            // auto random = new System.Random(0);
            // 
            (new Regex(@"[\r\n]{1,4}\s+auto random = new System\.Random\(0\);", _options), "", new Regex(@"TestExtensions\.cs", _options), 0),
            // random.Next(1, N)
            // (std::rand() % N) + 1
            (new Regex(@"random\.Next\(1, N\)", _options), "(std::rand() % N) + 1", new Regex(@"TestExtensions\.cs", _options), 0),
            // auto added = new HashSet<TElement>();
            // std::unordered_set<TElement> added;
            (new Regex(@"auto ([a-zA-Z0-9]+) = new HashSet<([a-zA-Z0-9]+)>\(\);", _options), "std::unordered_set<$2> $1;", new Regex(@"TestExtensions\.cs", _options), 0),
            // added.Add(node)
            // added.insert(node)
            (new Regex(@"added\.Add\(([a-zA-Z0-9]+)\)", _options), "added.insert($1)", new Regex(@"TestExtensions\.cs", _options), 0),
            // added.Remove(node)
            // added.erase(node) 
            (new Regex(@"added\.Remove\(([a-zA-Z0-9]+)\)", _options), "added.erase($1)", new Regex(@"TestExtensions\.cs", _options), 0),
            // if (added.insert(node)) {
            // if (added.find(node) == added.end()) { added.insert(node);
            (new Regex(@"if \(added\.insert\(node\)\)([\t ]*[\r\n]+)([\t ]*){", _options), "if (added.find(node) == added.end())$1$2{" + Environment.NewLine + "$2    added.insert(node);", new Regex(@"TestExtensions\.cs", _options), 0),
            // SizedBinaryTreeMethodsBase
            // Platform::Collections::Methods::Trees::SizedBinaryTreeMethodsBase
            (new Regex(@"\(SizedBinaryTreeMethodsBase<TElement>", _options), "(Platform::Collections::Methods::Trees::SizedBinaryTreeMethodsBase<TElement>&", new Regex(@"TestExtensions\.cs", _options), 0),
            // //#define ENABLE_TREE_AUTO_DEBUG_AND_VALIDATION
            //
            (new Regex(@"\/\/[ \t]*\#define[ \t]+[_a-zA-Z0-9]+[ \t]*", _options), "", null, 0),
            // #if USEARRAYPOOL\r\n#endif
            //
            (new Regex(@"#if [a-zA-Z0-9]+\s+#endif", _options), "", null, 0),
            // \n ... namespace
            // namespace
            (new Regex(@"(\S[\r\n]{2})?\s+namespace", _options), "$1namespace", null, 0),
        };

        static int Main(string[] args)
        {
            var sourceFilename = GetArgOrDefault(args, 0);
            if (!File.Exists(sourceFilename))
            {
                Console.WriteLine($"{sourceFilename} file does not exist.");
                return 0;
            }
            var targetFilename = GetArgOrDefault(args, 1);
            if (string.IsNullOrWhiteSpace(targetFilename))
            {
                targetFilename = Path.ChangeExtension(sourceFilename, ".cpp");
            }
            File.WriteAllText(targetFilename, Translate(sourceFilename, File.ReadAllText(sourceFilename)));
            Console.WriteLine($"{targetFilename} file written.");
            return 0;
        }

        private static string GetArgOrDefault(string[] args, int index) => args.Length > index ? args[index] : null;

        private static string Translate(string sourceFilename, string source)
        {
            var current = source;
            for (int i = 0; i < _transformations.Length; i++)
            {
                var matchPattern = _transformations[i].Item1;
                var substitutionPattern = _transformations[i].Item2;
                var filenamePattern = _transformations[i].Item3;
                var maximumRepeatCount = _transformations[i].Item4;
                if (filenamePattern == null || filenamePattern.IsMatch(sourceFilename))
                {
                    var count = 0;
                    do
                    {
                        current = matchPattern.Replace(current, substitutionPattern);
                        if (++count > maximumRepeatCount)
                        {
                            break;
                        }
                    }
                    while (matchPattern.IsMatch(current));
                }
            }
            return current;
        }
    }
}
