/*****************************************************************************
      To be the apostrophe which changed "Impossible" into "I'm possible"!
		
POC code of chapter 16.3 in book "Vulnerability Exploit and Analysis Technique"
 
file name	: fuzzer.c
author		: failwest  
date		: 2007.10.07
description	: block define sample code
Noticed		: have to complied with SPIKE library
version		: 1.0
E-mail		: failwest@gmail.com
		
	Only for educational purposes    enjoy the fun from exploiting :)
******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "spike.h"

main()
{
	int i,k;
	struct spike * p_spike = new_spike();
	setspike(p_spike);
	spike_clear();
		
	//s_init_fuzzing();//使用SPIKE自带畸形数据库
	s_add_fuzzstring("fuzz1111");//添加自定义的畸形数据
	s_add_fuzzstring("fuzz2222");//添加自定义的畸形数据
	//s_add_fuzzstring("failwest3");//添加自定义的畸形数据
	//s_add_fuzzstring("failwest4");//添加自定义的畸形数据
	s_resetfuzzvariable();	

	for (k = 0; k < 2; k++) {
		for (i = 0; i < s_get_max_fuzzstring(); i++) {
			spike_clear();
			s_incrementfuzzstring();
			s_string("aaaaaaaa");
			s_string_variable("");
			s_string("bbbbbbbb");
			s_string_variable("");
			s_string("cccccccc");
			s_print_buffer();			
		}
		s_incrementfuzzvariable();
	}
	
	spike_free(p_spike);
}



/* result

[test@localhost SPIKE_DEMOPKG]$ ./fuzzer
Datasize=48
Start buffer:
63 6d 64 5f 6d 61 69 6e
30 00 00 00 63 6d 64 5f
73 75 62 31 14 00 00 00
90 90 90 90 90 90 90 90
63 6d 64 5f 73 75 62 32
10 00 00 00 44 44 44 44

End buffer:


*/