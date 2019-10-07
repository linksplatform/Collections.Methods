using System;
using Platform.RegularExpressions.Transformer;

namespace CSharpToCppTranslator
{
    public class Program
    {
        static int Main(string[] args)
        {
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
