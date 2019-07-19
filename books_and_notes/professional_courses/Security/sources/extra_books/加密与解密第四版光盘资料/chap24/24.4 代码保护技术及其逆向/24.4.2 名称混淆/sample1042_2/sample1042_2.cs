using System;
using tankaiha.sample1042_2_lib;

namespace tankaiha.sample1042_2
{
	class class1
	{
		public static void Main()
		{
			Console.WriteLine("Please input password");
			string s=Console.ReadLine();
			if(class2.CheckValid(s)==true)
			{
				Console.WriteLine("password OK");
			}
			else
			{
				Console.WriteLine("invalid password");
			}
		}
	};
}