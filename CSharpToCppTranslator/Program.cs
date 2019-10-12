using System;
using Platform.RegularExpressions.Transformer;

namespace CSharpToCppTranslator
{
    public class Program
    {
        static int Main(string[] args)
        {
            //for (int i = 0; i < args.Length; i++)
            //{
            //    Console.WriteLine(args[i]);
            //}
            //args = new string[] { @"C:\Code\Links\Collections.Methods\Platform.Collections.Methods\GenericCollectionMethodsBase.cs", @"C:\Code\Links\Collections.Methods\Platform.Collections.Methods.Console\" };
            var csharpToCpp = new CustomCSharpToCppTransformer();
            var cli = new TransformerCLI(csharpToCpp);
            if (cli.Run(args, out string message))
            {
                WriteMessageIfAvailable(message);
                return 0;
            }
            else
            {
                WriteMessageIfAvailable(message);
                return -1;
            }
        }

        private static void WriteMessageIfAvailable(string message)
        {
            if (!string.IsNullOrWhiteSpace(message))
            {
                Console.WriteLine(message);
            }
        }
    }
}
