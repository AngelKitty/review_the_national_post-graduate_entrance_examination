using System;

namespace tankaiha.sample1042_1
{
	class class1
	{
		public static void Main()
		{
			Console.WriteLine("Please input password");
			string s=Console.ReadLine();
			if(CheckValid(s)==true)
			{
				Console.WriteLine("password OK");
			}
			else
			{
				Console.WriteLine("invalid password");
			}
		}

		private static bool CheckValid(string pass)
		{
			return pass=="sample"? true:false;
		}
	};
}