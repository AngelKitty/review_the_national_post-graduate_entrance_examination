/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

#include "../queen_brute_force_1/queen.h"

void place3Queens() { //3皇后剪枝算法
   int solu[3]; //解法
   for ( solu[0] = 0; solu[0] < 3; solu[0]++ )
      if ( !collide ( solu, 0 ) )
         for ( solu[1] = 0; solu[1] < 3; solu[1]++ )
            if ( !collide ( solu, 1 ) )
               for ( solu[2] = 0; solu[2] < 3; solu[2]++ )
                  if ( !collide ( solu, 2 ) ) {
                     nSolu++; displaySolution ( solu, 3 );
                  }
}