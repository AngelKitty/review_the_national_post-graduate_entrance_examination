/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

#pragma once

void readLaby ( char* labyFile ) { //∂¡»Î√‘π¨
   FILE* fp;
   if ( ! ( fp = fopen ( labyFile, "r" ) ) )
      { cout << "can't open " << labyFile << endl; exit ( -1 ); }
   fscanf ( fp, "Laby Size = %d\n", &labySize );
   if ( LABY_MAX < labySize )
      { cout << "Laby size " << labySize << " > " << LABY_MAX << endl; exit ( -1 ); }
   int startX, startY; fscanf ( fp, "Start = (%d, %d)\n", &startX, &startY );
   startCell = &laby[startX][startY];
   int goalX, goalY; fscanf ( fp, "Goal = (%d, %d)\n", &goalX, &goalY );
   goalCell = &laby[goalX][goalY];
   for ( int j = 0; j < labySize; j ++ )
      for ( int i = 0; i < labySize; i ++ ) {
         laby[i][j].x = i;
         laby[i][j].y = j;
         int type; fscanf ( fp, "%d", &type );
         switch ( type ) {
            case 1:   laby[i][j].status = WALL;      break;
            case 0:   laby[i][j].status = AVAILABLE;   break;
            default:   exit ( -1 );
         }
         laby[i][j].incoming =
            laby[i][j].outgoing =   UNKNOWN;
      }
   fclose ( fp );
}