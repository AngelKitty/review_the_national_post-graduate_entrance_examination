typedef  BiTree  SOSTree;  // 次优查找树采用二叉链表的存储结构

Status CreateSOSTree(SOSTree &T, SSTable ST) {  // 算法9.4
  // 由有序表ST构造一棵次优查找树T。ST的数据元素含有权域weight
  float sw[20];
  if (ST.length == 0) T = NULL;
  else {
    FindSW(sw, ST); // 按照有序表ST中各元素的weight域求累计权值表sw
    SecondOptimal(T, ST.elem, sw, 1, ST.length);
  }
  return OK;
} // CreateSOSTree
