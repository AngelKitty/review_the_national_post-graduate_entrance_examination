/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

#pragma once

/******************************************************************************************
 *   输出某一迷宫格的信息
 ******************************************************************************************/
void printLabyCell ( Cell* elem ) {
   printf ( "%d -> (%d, %d) -> %d\n",
            ( ( Cell* ) elem )->incoming,
            ( ( Cell* ) elem )->x,
            ( ( Cell* ) elem )->y,
            ( ( Cell* ) elem )->outgoing );
}

/******************************************************************************************
 * 显示迷宫
 ******************************************************************************************/
void displayLaby() { //┘└┐┌│─
   static char*   pattern[5][5] = {
      "┼", "┼", "┼", "┼", "┼",
      "┼", "  ", "┌", "─", "└",
      "┼", "┌", "  ", "┐", "│",
      "┼", "─", "┐", "  ", "┘",
      "┼", "└", "│", "┘", "  "
   };
   system ( "cls" );
   printf ( "  " );
   for ( int j = 0; j < labySize; j++ )
      ( j < 10 ) ? printf ( "%2X", j ) : printf ( " %c", 'A' - 10 + j );
   printf ( "\n" );
   for ( int j = 0; j < labySize; j++ ) {
      ( j < 10 ) ? printf ( "%2X", j ) : printf ( " %c", 'A' - 10 + j );
      for ( int i = 0; i < labySize; i++ )
         if ( goalCell == &laby[i][j] )
            printf ( "﹩" );
         else
            switch ( laby[i][j].status ) {
               case WALL:  printf ( "█" );   break;
               case BACKTRACKED: printf ( "○" );   break;
               case AVAILABLE: printf ( "  " );   break;
               default   : printf ( "%s", pattern[laby[i][j].outgoing][laby[i][j].incoming] );   break;
            }
      printf ( "\n" );
   }//for
}//displayLaby