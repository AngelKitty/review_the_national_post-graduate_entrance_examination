// 程序中用到的主要变量
EventList  ev;                // 事件表
Event      en;                // 事件
LinkQueue  q[5];              // 4个客户队列,q[0]未用
QElemType  customer;          // 客户记录
int TotalTime, CustomerNum;   // 累计客户逗留时间, 客户数
int CloseTime;

//---------------- 算法 3.7 ------------------

int cmp(Event a,  Event b) {
  // 依事件a的发生时刻< 或= 或> 事件b的发生时刻分别返回-1或0或1
  if (a.OccurTime < b.OccurTime) return -1;
  if (a.OccurTime > b.OccurTime) return +1;
  return 0;
}

void Random(int &durtime, int &intertime) { // 生成随机数
   durtime = random(2, 10);
   intertime = random(10);
}

int Minimum(LinkQueue q[]) {  // 求长度最短队列
   int minlen = QueueLength(q[1]);
   int i = 1;
   for (int j=2; j<=4; j++)
      if (QueueLength(q[j]) < minlen) {
         minlen = QueueLength(q[j]);
         i = j;
      }
   return i;
}

void OpenForDay() {
   // 初始化操作
   TotalTime = 0;  CustomerNum = 0;  // 初始化累计时间和客户数为0
   InitList(ev);                     // 初始化事件链表为空表
   en.OccurTime = 0;  en.NType = 0;  // 设定第一个客户到达事件
   OrderInsert(ev, en, cmp);    // 按事件发生时刻的次序插入事件表
   for (int i=1; i<=4; ++i) InitQueue(q[i]);   // 置空队列
} // OpenForDay

void CustomerArrived() {
  // 处理客户到达事件，en.NType=0
  int durtime, intertime, i, t;
  ++CustomerNum;
  printf("Customer %d arrived at %d and ", CustomerNum, en.OccurTime);
  Random(durtime, intertime);            // 生成随机数
  t = en.OccurTime + intertime;          // 下一客户到达时刻
  if (t<CloseTime)                       // 银行尚未关门，插入事件表
    OrderInsert(ev, MakeElem(t, 0), cmp);
  i = Minimum(q);                        // 求长度最短队列
  printf("enter the Queue %d\n", i);
  EnQueue(q[i], MakeQElem(en.OccurTime, durtime));
  if (QueueLength(q[i]) == 1) //设定第i队列的一个离开事件并插入事件表
    OrderInsert(ev, MakeElem(en.OccurTime+durtime, i), cmp);
} // CustomerArrived

void CustomerDeparture() {
  // 处理客户离开事件，en.NType>0
  printf("Customer departure at %d\n", en.OccurTime);
  int i = en.NType;  DeQueue(q[i], customer); //删除第i队列的排头客户
  TotalTime += en.OccurTime-customer.ArrivalTime; // 累计客户逗留时间
  if (!QueueEmpty(q[i])) {   // 设定第i队列的一个离开事件并插入事件表
    GetHead (q[i], customer);
    OrderInsert(ev, MakeElem(en.OccurTime+customer.Duration, i), cmp);
  }
} // CustomerDeparture

void Bank_Simulation(int closetime) {
  int i = 0;
  BLink p;
  CloseTime = closetime;
  printf("Bank_Simulation( %d ) ----- 银行业务模拟\n", closetime);
  OpenForDay();                     // 初始化
  while (!ListEmpty(ev)) {
    printList(ev);
    if (DelFirst(GetHead(ev), p)) {
      en = GetCurElem(p);
      if (en.NType == 0)
        CustomerArrived();       // 处理客户到达事件
      else CustomerDeparture();   // 处理客户离开事件
    }
    if (++i % 9 == 0) {
      printf("\n----- 按任意键，继续 -----");
      getch();
      printf("\n\n");
    }
  }
  // 计算并输出平均逗留时间
  printf("\nThe Average Time is %f\n", (float)TotalTime/CustomerNum);
} // Bank_Simulation
