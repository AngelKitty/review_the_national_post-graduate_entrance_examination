/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

#include "../queen_brute_force_1/queen.h"

void place5Queens() { //5皇后剪枝算法
   int solu[5]; //解法
   for ( solu[0] = 0; solu[0] < 5; solu[0]++ )
      if ( !collide ( solu, 0 ) )
         for ( solu[1] = 0; solu[1] < 5; solu[1]++ )
            if ( !collide ( solu, 1 ) )
               for ( solu[2] = 0; solu[2] < 5; solu[2]++ )
                  if ( !collide ( solu, 2 ) )
                     for ( solu[3] = 0; solu[3] < 5; solu[3]++ )
                        if ( !collide ( solu, 3 ) )
                           for ( solu[4] = 0; solu[4] < 5; solu[4]++ )
                              if ( !collide ( solu, 4 ) ) {
                                 nSolu++; displaySolution ( solu, 5 );
                              }
}