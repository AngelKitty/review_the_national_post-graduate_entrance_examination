Status Pass(MazeType MyMaze, PosType CurPos);
void FootPrint(MazeType &MyMaze, PosType CurPos);
void MarkPrint(MazeType &MyMaze, PosType CurPos);
PosType NextPos(PosType CurPos, int Dir);

Status MazePath(MazeType &maze, PosType start, PosType end) {  
  // 算法3.3
  // 若迷宫maze中从入口 start到出口 end的通道，则求得一条存放在栈中
  // （从栈底到栈顶），并返回TRUE；否则返回FALSE
  Stack S;
  PosType curpos;
  int curstep;
  SElemType e;

  InitStack(S);
  curpos = start;  // 设定"当前位置"为"入口位置"
  curstep = 1;     // 探索第一步
  do {
    if (Pass(maze,curpos)) {  // 当前位置可通过，即是未曾走到过的通道块
      FootPrint(maze,curpos); // 留下足迹
      e.di =1;
      e.ord = curstep;
      e.seat= curpos;
      Push(S,e);              // 加入路径
      if (curpos.r == end.r && curpos.c==end.c)  
        return (TRUE);        // 到达终点（出口）
      curpos = NextPos(curpos, 1);        // 下一位置是当前位置的东邻
      curstep++;                          // 探索下一步
    } else {  // 当前位置不能通过
      if (!StackEmpty(S)) {
        Pop(S,e);
        while (e.di==4 && !StackEmpty(S)) {
          MarkPrint(maze,e.seat);  
          Pop(S,e);    // 留下不能通过的标记，并退回一步
        } // while
        if (e.di<4) {
          e.di++;
          Push(S, e);  // 换下一个方向探索
          curpos = NextPos(e.seat, e.di); // 当前位置设为新方向的相邻块
        } // if
      } // if
    } // else
  } while (!StackEmpty(S) );
  return FALSE;
} // MazePath

Status Pass( MazeType MyMaze,PosType CurPos) {
  if (MyMaze.arr[CurPos.r][CurPos.c]==' ')
    return 1;     // 如果当前位置是可以通过，返回1
  else return 0;  // 其它情况返回0
}

void FootPrint(MazeType &MyMaze,PosType CurPos) {
  MyMaze.arr[CurPos.r][CurPos.c]='*';
}

void MarkPrint(MazeType &MyMaze,PosType CurPos) {
  MyMaze.arr[CurPos.r][CurPos.c]='!';
}

PosType NextPos(PosType CurPos, int Dir) {
  PosType ReturnPos; 
  switch (Dir) {
    case 1:
        ReturnPos.r=CurPos.r;
        ReturnPos.c=CurPos.c+1;
        break;
    case 2:
        ReturnPos.r=CurPos.r+1;
        ReturnPos.c=CurPos.c;
        break;
    case 3:
        ReturnPos.r=CurPos.r;
        ReturnPos.c=CurPos.c-1;
        break;
    case 4:
        ReturnPos.r=CurPos.r-1;
        ReturnPos.c=CurPos.c;
        break;
  }
  return ReturnPos;
}
