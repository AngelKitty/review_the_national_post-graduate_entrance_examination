void MarkList(GList GL) {  // 算法8.3
  // 遍历非空广义表GL(GL!=NULL且GL->mark==0)，
  // 对表中所有未加标志的结点加标志
  GList q = NULL, p = GL, t = NULL;  // t指示p的母表
  int finished = FALSE;   
  while (!finished) {
    while (p->mark==0) {
      p->mark = 1;
      // MarkHead(p)的细化：
      q = p->ptr.hp;   // q指向*p的表头
      if (q && q->mark==0) {
        if (q->tag==ATOM) q->mark = 1;   // ATOM，表头为原子结点
        else  // 继续遍历子表
          { p->ptr.hp = t;  p->tag = ATOM;  t = p;  p = q; } 
      }
    }  // 完成对表头的标志
    q = p->ptr.tp;    // q指向*p的表尾
    if (q && q->mark==0) {   // 继续遍历表尾
      p->ptr.tp = t;  t = p;  p = q; 
    } else {  // BackTrack(finished)的细化：
      while (t && t->tag==LIST) {  // LIST，表结点，从表尾回溯
        q = t;   t = q->ptr.tp;   q->ptr.tp = p;   p = q;
      }
      if (!t) finished = TRUE;   // 结束
      else {  // 从表头回溯
        q = t;   t = q->ptr.hp;   q->ptr.hp = p;   
        p = q;   p->tag = LIST;
      }  // 继续遍历表尾
    }
  }
} // MarkList
