void Arrange(SLinkList &SL) {  // 算法10.3
  // 根据静态链表SL中各结点的指针值调整记录位置，
  // 使得SL中记录按关键字非递减有序顺序排列
  SLNode temp;
  int i;
  int p,q;
  p = SL.r[0].next;  // p指示第一个记录的当前位置
  for (i=1; i<SL.length; ++i) { 
    // SL.r[1..i-1]中记录已按关键字有序排列，
    // 第i个记录在SL中的当前位置应不小于i
    while (p<i)  // 找到第i个记录，并用p指示其在SL中当前位置
      p = SL.r[p].next;
    q = SL.r[p].next; // q指示尚未调整的表尾
    if (p!= i) {
      temp=SL.r[p];
      SL.r[p]=SL.r[i];
      SL.r[i]=temp;
      SL.r[i].next=p; // 指向被移走的记录，使得以后可由while循环找回
    }
    p=q;              // p指示尚未调整的表尾，为找第i+1个记录作准备
  }
} // Arrange
