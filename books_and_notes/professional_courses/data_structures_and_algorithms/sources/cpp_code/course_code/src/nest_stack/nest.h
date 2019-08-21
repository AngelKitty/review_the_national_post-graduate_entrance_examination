/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

#pragma once

#include<iostream>
using namespace std;

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../UniPrint/print.h"

#include "../stack/stack.h"

bool paren ( const char exp[], int lo, int hi );
void displaySubstring ( const char exp[], int lo, int hi );
void displayProgress ( const char exp[], int i, Stack<char> S );
