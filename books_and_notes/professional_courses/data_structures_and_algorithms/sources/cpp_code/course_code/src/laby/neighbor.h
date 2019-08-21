/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

#pragma once

inline Cell* neighbor ( Cell* cell ) { //查询当前位置的相邻格点
   switch ( cell->outgoing ) {
      case EAST  : return cell + LABY_MAX; //向东
      case SOUTH : return cell + 1;        //向南
      case WEST  : return cell - LABY_MAX; //向西
      case NORTH : return cell - 1;        //向北
      default    : exit ( -1 );
   }
}