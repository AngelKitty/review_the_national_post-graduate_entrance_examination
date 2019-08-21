/******************************************************************************************
 * Data Structures in C++
 * ISBN: 7-302-33064-6 & 7-302-33065-3 & 7-302-29652-2 & 7-302-26883-3
 * Junhui DENG, deng@tsinghua.edu.cn
 * Computer Science & Technology, Tsinghua University
 * Copyright (c) 2006-2013. All rights reserved.
 ******************************************************************************************/

#pragma once

typedef enum { AVAILABLE, ROUTE, BACKTRACKED, WALL } Status; //迷宫单元状态
//原始可用的、在当前路径上的、所有方向均尝试失败后回溯过的、不可使用的（墙）

typedef enum { UNKNOWN, EAST, SOUTH, WEST, NORTH, NO_WAY } ESWN; //单元的相对邻接方向
//未定、东、南、西、北、无路可通

inline ESWN nextESWN ( ESWN eswn ) { return ESWN ( eswn + 1 ); } //依次转至下一邻接方向

struct Cell { //迷宫格点
   int x, y; Status status; //x坐标、y坐标、类型
   ESWN incoming, outgoing; //进入、走出方向
};

#define LABY_MAX 24 //最大迷宫尺寸
Cell laby[LABY_MAX][LABY_MAX]; //迷宫