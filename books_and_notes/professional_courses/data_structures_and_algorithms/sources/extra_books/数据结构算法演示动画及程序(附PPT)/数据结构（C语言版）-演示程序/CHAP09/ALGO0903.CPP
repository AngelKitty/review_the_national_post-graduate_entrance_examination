Status SecondOptimal(BiTree &T, ElemType R[], float sw[], 
                     int low, int high) {  // 算法9.3
   // 由有序表R[low..high]及其累计权值表sw
   // (其中sw[0]==0)递归构造次优查找树T。
   int i,j;
   float min,dw;
   i = low;  min = (float)fabs(sw[high]-sw[low]);  
   dw = sw[high]+sw[low-1];
   for (j=low+1;  j<=high;  ++j)           // 选择最小的ΔPi值
      if (fabs(dw-sw[j]-sw[j-1]) < min) {
         i = j;  min = (float)fabs(dw-sw[j]-sw[j-1]);
      } 
   if (!(T = (BiTree)malloc(sizeof(BiTNode)))) return ERROR;
   T->data = R[i];                                   // 生成结点
   if (i==low) T->lchild = NULL;                     // 左子树空
   else SecondOptimal(T->lchild, R, sw, low, i-1);   // 构造左子树
   if (i==high) T->rchild = NULL;                    // 右子树空
   else SecondOptimal(T->rchild, R, sw, i+1, high);  // 构造右子树
   return OK;
} // SecondOptimal
