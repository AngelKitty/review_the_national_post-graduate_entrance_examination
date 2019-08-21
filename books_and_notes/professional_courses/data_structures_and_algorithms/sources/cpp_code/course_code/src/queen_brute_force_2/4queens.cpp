/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

#include "../queen_brute_force_1/queen.h"

void place4Queens() { //4皇后剪枝算法
   int solu[4]; //解法
   for ( solu[0] = 0; solu[0] < 4; solu[0]++ )
      if ( !collide ( solu, 0 ) )
         for ( solu[1] = 0; solu[1] < 4; solu[1]++ )
            if ( !collide ( solu, 1 ) )
               for ( solu[2] = 0; solu[2] < 4; solu[2]++ )
                  if ( !collide ( solu, 2 ) )
                     for ( solu[3] = 0; solu[3] < 4; solu[3]++ )
                        if ( !collide ( solu, 3 ) ) {
                           nSolu++; displaySolution ( solu, 4 );
                        }
}