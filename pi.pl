using System;
using System.Text;

namespace pi
{
	class Pi
	{
		static string ComputePi(int numdigits)
		{
			StringBuilder pi = new StringBuilder();
			pi.Length = numdigits + 1;

			int[] a = new int[10*numdigits / 3];
			for(int i = 0; i < a.Length; ++i)
				a[i] = 2;

			int piLength = 0;
			int nines = 0;
			int predigit = 0;
			for (int j = 0; j < numdigits; ++j)
			{
				int q = 0;
				int p = 2 * a.Length - 1;
				for (int i = a.Length; --i >= 0; )
				{
					int x = 10*a[i] + q*(i+1);
					a[i] = x % p;
					q = x / p;
					p -= 2;
				}
    
				a[0] = q % 10;
				q /= 10;
				if (q == 9)
					++nines;
				else if (q == 10)
				{
					pi[piLength] = (char) (predigit + 1 + '0');
					for (int k = 1; k <= nines; ++k)
						pi[piLength+k] = '0';
					piLength += nines + 1;
					predigit = 0;
					nines = 0;
				}
				else
				{
					pi[piLength] = (char)(predigit + '0');
					predigit = q;
					for (int k = 1; k <= nines; ++k)
						pi[piLength + k] = '9';
					piLength += nines + 1;
					nines = 0;
				}
			}
			pi[piLength] = (char)(predigit + '0');
			return pi.ToString();
		}
		
		[STAThread]
		static void Main(string[] args)
		{
			if (args.Length == 0)
			{
				System.Console.WriteLine("usage: pi #DIGITS [FILE]");
				return;
			}
			int numdigits = Int32.Parse(args[0]);

			if (args.Length > 1)
			{
				System.IO.StreamWriter file = new System.IO.StreamWriter(args[1]);
				file.WriteLine(ComputePi(numdigits));
			}
			else
				System.Console.WriteLine(ComputePi(numdigits));
		}
	}
}
