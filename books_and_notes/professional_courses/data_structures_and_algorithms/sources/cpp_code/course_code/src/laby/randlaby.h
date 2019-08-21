/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

#pragma once

void randLaby() { //生成随机的迷宫
   labySize = LABY_MAX / 2 + rand() % ( LABY_MAX / 2 ); /*DSA*/printf ( "Using a laby of size %d ...\n", labySize ); getchar();
   for ( int i = 0; i < labySize; i++ )
      for ( int j = 0; j < labySize; j++ ) {
         laby[i][j].x = i;
         laby[i][j].y = j;
         laby[i][j].incoming   =
            laby[i][j].outgoing   =   UNKNOWN;
         laby[i][j].status = WALL; //边界格点必须是墙
      }
   for ( int i = 1; i < labySize - 1; i++ )
      for ( int j = 1; j < labySize - 1; j++ )
         if ( rand() % 4 ) laby[i][j].status = AVAILABLE; //75%的格点为空可用
   startCell = &laby[rand() % ( labySize-2 ) + 1][rand() % ( labySize-2 ) + 1];
   goalCell  = &laby[rand() % ( labySize-2 ) + 1][rand() % ( labySize-2 ) + 1];
   startCell->status = goalCell->status = AVAILABLE; //起始格点必须可用
}