using System;
using System.IO;
using System.Text.RegularExpressions;

namespace Translator
{
    class Program
    {
        static int Main(string[] args)
        {
            var sourceFilename = GetArgOrDefault(args, 0);
            if (!File.Exists(sourceFilename))
            {
                Console.WriteLine($"{sourceFilename} file does not exists.");
                return 0;
            }
            var targetFilename = GetArgOrDefault(args, 1);
            if (string.IsNullOrWhiteSpace(targetFilename))
            {
                targetFilename = Path.ChangeExtension(sourceFilename, ".cpp");
            }
            File.WriteAllText(targetFilename, Translate(sourceFilename, File.ReadAllText(sourceFilename)));
            Console.WriteLine($"{targetFilename} file writen.");
            return 0;
        }

        private static string GetArgOrDefault(string[] args, int index) => args.Length > index ? args[index] : null;

        //private static readonly string _indent = "    ";
        private static readonly RegexOptions _defaultOptions = RegexOptions.Compiled | RegexOptions.Multiline;

        private static readonly ValueTuple<Regex, string, Regex>[] _substitutions =
        {
            // #pragma warning disable CS1591 // Missing XML comment for publicly visible type or member
            // 
            (new Regex(@"^\s*?\#pragma[\sa-zA-Z0-9\/]+$", _defaultOptions), "", null),
            // [MethodImpl(MethodImplOptions.AggressiveInlining)]
            // 
            (new Regex(@"$\s+\[MethodImpl\(MethodImplOptions\.AggressiveInlining\)\]", _defaultOptions), "", null),
            (new Regex(@"{\s+[\r\n]+", _defaultOptions), "{" + Environment.NewLine, null),
            // Platform.Collections.Methods.Lists
            // Platform::Collections::Methods::Lists
            (new Regex(@"(namespace[^\r\n]+?)\.([^\r\n]+?)", _defaultOptions), "$1::$2", null),
            // => Integer<TElement>.Zero;
            // { return Integer<TElement>.Zero; }
            (new Regex(@"\)\s+=>\s+([^;]+)?;", _defaultOptions), ") { return $1; }", null),
            // public abstract class
            // class
            (new Regex(@"public abstract class", _defaultOptions), "class", null),
            // class GenericCollectionMethodsBase<TElement>
            // template <typename TElement> class GenericCollectionMethodsBase
            (new Regex(@"class ([a-zA-Z0-9]+)<([a-zA-Z0-9]+)>", _defaultOptions), "template <typename $2> class $1", null),
            // protected virtual
            // virtual
            (new Regex(@"protected virtual", _defaultOptions), "virtual", null),
            // protected abstract TElement GetFirst();
            // virtual TElement GetFirst() = 0;
            (new Regex(@"protected abstract ([^;]+);", _defaultOptions), "virtual $1 = 0;", null),
            // public virtual
            // virtual
            (new Regex(@"public virtual", _defaultOptions), "virtual", null),
            // protected readonly
            // 
            (new Regex(@"protected readonly ", _defaultOptions), "", null),
            // protected readonly TElement Zero;
            // TElement Zero;
            (new Regex(@"protected readonly ([a-zA-Z<>0-9]+) ([a-zA-Z0-9]+);", _defaultOptions), "$1 $2;", null),
            // private
            // 
            (new Regex(@"(\W)private ", _defaultOptions), "$1", null),
            // protected 
            // 
            (new Regex(@"(\W)protected ", _defaultOptions), "$1", null),
            // public 
            // 
            (new Regex(@"(\W)public ", _defaultOptions), "$1", null),
            // = ref GetLeftReference
            // = GetLeftReference
            (new Regex(@" = ref ([a-zA-Z0-9]+)", _defaultOptions), " = $1", null),
            // (ref left)
            // (left)
            (new Regex(@"\(ref ([a-zA-Z0-9]+)(\)|\(|,)", _defaultOptions), "($1$2", null),
            // ref TElement
            // TElement*
            (new Regex(@"ref ([a-zA-Z0-9]+)", _defaultOptions), "$1*", null),
            // TElement*, TElement*)
            // TElement, TElement)
            //(new Regex(@"([a-zA-Z0-9]+)*([,\)])", _defaultOptions), "$1$2"),
            // !AreEqual(parent, default)
            // parent != 0
            (new Regex(@"!AreEqual\(([a-zA-Z]+), default\)", _defaultOptions), "$1 != 0", null),
            // AreEqual(node, default)
            // node == 0
            (new Regex(@"AreEqual\(([a-zA-Z]+), default\)", _defaultOptions), "$1 == 0", null),
            // !AreEqual(size, expectedSize)
            // size != expectedSize
            (new Regex(@"!AreEqual\(([a-zA-Z]+), ([a-zA-Z]+)\)", _defaultOptions), "$1 != $2", null),
            // AreEqual(baseElement, GetFirst())
            // baseElement == GetFirst()
            (new Regex(@"AreEqual\(([a-zA-Z]+), ([a-zA-Z]+)\(([a-zA-Z]*)\)\)", _defaultOptions), "$1 == $2($3)", null),
            // AreEqual(elementNext, element)
            // elementNext == element
            (new Regex(@"AreEqual\(([a-zA-Z]+), ([a-zA-Z]+)\)", _defaultOptions), "$1 == $2", null),
            // AreEqual(GetLeft(parent), nodeToDetach)
            // GetLeft(parent) = nodeToDetach
            (new Regex(@"AreEqual\(([a-zA-Z]+)\(([a-zA-Z]*)\), ([a-zA-Z]+)\)", _defaultOptions), "$1($2) == $3", null),
            // !EqualToZero(root)
            // root != 0
            (new Regex(@"!EqualToZero\(([a-zA-Z]+)\)", _defaultOptions), "$1 != 0", null),
            // EqualToZero(node)
            // node == 0
            (new Regex(@"EqualToZero\(([a-zA-Z]+)\)", _defaultOptions), "$1 == 0", null),
            // default
            // 0
            (new Regex(@"(\W)default(\W)", _defaultOptions), "${1}0$2", null),
            // Arithmetic.Add(leftSize, rightSize)
            // leftSize + rightSize
            (new Regex(@"Arithmetic\.Add\(([a-zA-Z0-9]+), ([a-zA-Z0-9]+)\)", _defaultOptions), "$1 + $2", null),
            // Add(GetLeftSize(node), GetRightSize(node))
            // GetLeftSize(node) + GetRightSize(node)
            (new Regex(@"Add\(([a-zA-Z0-9]+)\(([a-zA-Z0-9 \+]+)\), ([a-zA-Z0-9]+)\(([a-zA-Z0-9 \+]+)\)\)", _defaultOptions), "$1($2) + $3($4)", null),
            // Add(rightSize, 2)
            // rightSize + 2
            (new Regex(@"Add\(([a-zA-Z0-9]+), ([a-zA-Z0-9]+)\)", _defaultOptions), "$1 + $2", null),
            // Arithmetic.Increment(leftSize + rightSize)
            // leftSize + rightSize + 1
            (new Regex(@"Arithmetic\.Increment\(([a-zA-Z0-9 \+]+)\)", _defaultOptions), "$1 + 1", null),
            // Increment(GetSize(node))
            // GetSize(node) + 1
            (new Regex(@"Increment\(([a-zA-Z0-9]+)\(([a-zA-Z0-9 \+]*)\)\)", _defaultOptions), "$1($2) + 1", null),
            // Increment(GetLeftSize(node) + GetRightSize(node))
            // GetLeftSize(node) + GetRightSize(node) + 1
            (new Regex(@"Increment\(([a-zA-Z0-9]+)\(([a-zA-Z0-9 \+]*)\) \+ ([a-zA-Z0-9]+)\(([a-zA-Z0-9 \+]*)\)\)", _defaultOptions), "$1($2) + $3($4) + 1", null),
            // Increment(rightLeftSize)
            // rightLeftSize + 1
            (new Regex(@"Increment\(([a-zA-Z0-9]+)\)", _defaultOptions), "$1 + 1", null),
            // Decrement(GetSize(node))
            // GetSize(node) - 1
            (new Regex(@"Decrement\(([a-zA-Z0-9]+)\(([a-zA-Z0-9 \+]*)\)\)", _defaultOptions), "$1($2) - 1", null),
            // Decrement(rightSize)
            // rightSize - 1
            (new Regex(@"Decrement\(([a-zA-Z0-9]+)\)", _defaultOptions), "$1 - 1", null),
            // GreaterThan(rightLeftSize + 1, leftSize)
            // (rightLeftSize + 1) > leftSize
            (new Regex(@"GreaterThan\(([a-zA-Z0-9]+) \+ ([a-zA-Z0-9]+), ([a-zA-Z0-9]+)\)", _defaultOptions), "($1 + $2) > $3", null),
            // GreaterThan(GetSizeOrZero(GetLeft(left)), decrementedRightSize)
            // GetSizeOrZero(GetLeft(left)) > decrementedRightSize
            (new Regex(@"GreaterThan\(([a-zA-Z0-9]+)\(([a-zA-Z0-9]+)\(([a-zA-Z0-9]*)\)\), ([a-zA-Z0-9]+)\)", _defaultOptions), "$1($2($3)) > $4", null),
            // GreaterThan(GetSize(rootLeftNodeLeftNode), GetSize(rootRightNode))
            // GetSize(rootLeftNodeLeftNode) > GetSize(rootRightNode)
            (new Regex(@"GreaterThan\(([a-zA-Z0-9]+)\(([a-zA-Z0-9]+)\), ([a-zA-Z0-9]+)\(([a-zA-Z0-9]*)\)\)", _defaultOptions), "$1($2) > $3($4)", null),
            // GreaterThanZero(leftSize)
            // leftSize > 0
            (new Regex(@"GreaterThanZero\(([a-zA-Z0-9]+)\)", _defaultOptions), "$1 > 0", null),
            // var
            // auto
            (new Regex(@"(\W)var(\W)", _defaultOptions), "$1auto$2", null),
            // unchecked
            // 
            (new Regex(@"[\r\n]{2}\s*?unchecked\s*?$", _defaultOptions), "", null),
            // $"
            // "
            (new Regex(@"\$""", _defaultOptions), "\"", null),
            // throw new InvalidOperationException
            // throw std::exception
            (new Regex(@"throw new (InvalidOperationException|Exception)", _defaultOptions), "throw std::exception", null),
            // override void PrintNode(TElement node, StringBuilder sb, int level)
            // void PrintNode(TElement node, StringBuilder sb, int level) override
            (new Regex(@"override ([a-zA-Z0-9 \+]+)(\([^\)]+?\))", _defaultOptions), "$1$2 override", null),
            // Zero
            // 0
            (new Regex(@"([^\.a-zA-Z0-9])Zero(\W)", _defaultOptions), "${1}0$2", null),
            // One
            // 1
            (new Regex(@"(\W)One(\W)", _defaultOptions), "${1}1$2", null),
            // Two
            // 2
            (new Regex(@"(\W)Two(\W)", _defaultOptions), "${1}2$2", null),
            // private const int MaxPath = 92;
            // static const int MaxPath = 92;
            (new Regex(@"private const ([a-zA-Z0-9]+) ([a-zA-Z0-9]+) = ([a-zA-Z0-9]+);", _defaultOptions), "static const $1 $2 = $3;", null),
            // Just delete it
            (new Regex(@"virtual TElement GetZero\(\)(.|\s)+V3068\s+}", _defaultOptions), "", null),
            // sbyte
            // std::int8_t
            (new Regex(@"(\W)sbyte(\W)", _defaultOptions), "$1std::int8_t$2", null),
            // using Platform.Numbers;
            // 
            (new Regex(@"([\r\n]{2}|^)\s*?using [\.a-zA-Z0-9]+;\s*?$", _defaultOptions), "", null),
            // \t}\r\n}
            // \t};\r\n}
            (new Regex(@"([\r\n])(\t|[ ]{4})}([\r\n]+)}", _defaultOptions), "$1$2};$3}", null),
            // Just delete it in SizedBinaryTreeMethodsBase.cs
            (new Regex(@"void FixSizes(.|\s)+};", _defaultOptions), "};", new Regex(@"SizedBinaryTreeMethodsBase\.cs", _defaultOptions)),
            // Just delete it in SizedAndThreadedAVLBalancedTreeMethods.cs
            (new Regex(@"void PrintNode(.|\s)+?}", _defaultOptions), "", new Regex(@"SizedAndThreadedAVLBalancedTreeMethods\.cs", _defaultOptions)),
            // GetFirst
            // DoublyLinkedListMethodsBase::GetFirst
            (new Regex(@"([^:])(GetFirst|GetLast|GetPrevious|GetNext|GetSize|SetFirst|SetLast|SetPrevious|SetNext|SetSize|IncrementSize|DecrementSize)(\()", _defaultOptions), "$1DoublyLinkedListMethodsBase::$2$3", new Regex(@"[a-zA-Z]+DoublyLinkedListMethods\.cs", _defaultOptions)),
            // GetSize
            // SizedBinaryTreeMethodsBase::GetSize
            (new Regex(@"([^:])(GetLeftReference|GetRightReference|GetLeft|GetRight|SetLeft|SetRight|FirstIsToTheLeftOfSecond|FirstIsToTheRightOfSecond|GetLeftOrDefault|GetRightOrDefault|GetLeftSize|GetRightSize|GetSizeOrZero|FixSize|LeftRotate|RightRotate|ClearNode|GetSize|SetSize|IncrementSize|DecrementSize)(\()", _defaultOptions), "$1SizedBinaryTreeMethodsBase::$2$3", new Regex(@"Size[a-zA-Z]+(s|2)\.cs", _defaultOptions)),
            // SizedBinaryTreeMethodsBase::ClearNode
            // ClearNode
            (new Regex(@"SizedBinaryTreeMethodsBase::(ClearNode|GetLeftOrDefault|GetRightOrDefault)", _defaultOptions), "$1", new Regex(@"SizedAndThreadedAVLBalancedTreeMethods\.cs", _defaultOptions)),
        };

        private static string Translate(string sourceFilename, string source)
        {
            var current = source;
            for (int i = 0; i < _substitutions.Length; i++)
            {
                var pattern = _substitutions[i].Item1;
                var filenamePattern = _substitutions[i].Item3;
                if (filenamePattern == null || filenamePattern.IsMatch(sourceFilename))
                {
                    var count = 0;
                    while (pattern.IsMatch(current))
                    {
                        current = pattern.Replace(current, _substitutions[i].Item2);
                        if (count++ > 10)
                        {
                            break;
                        }
                    }
                }
            }
            return current;
        }
    }
}
