/*****************************************************************************
      To be the apostrophe which changed "Impossible" into "I'm possible"!
		
POC code of book "Vulnerability Exploit and Analysis Technique"
 
file name	: stack_show.c
author		: failwest  
date		: 2006.9.23
description	: used as a demo to show how stack works
Noticed		: just for debugging and stack watching    
version		: 1.0
E-mail		: failwest@gmail.com
		
	Only for educational purposes    enjoy the fun from exploiting :)
******************************************************************************/

int	func_B(int arg_B1, int arg_B2)
{
	int var_B1, var_B2;
	var_B1=arg_B1+arg_B2;
	var_B2=arg_B1-arg_B2;
	return var_B1*var_B2;
}

int	func_A(int arg_A1, int arg_A2)
{
	int var_A;
	var_A = func_B(arg_A1,arg_A2) + arg_A1 ;
	return var_A;
}

int main(int argc, char **argv, char **envp)
{
	int var_main;
	var_main=func_A(4,3);
	return var_main;
}
