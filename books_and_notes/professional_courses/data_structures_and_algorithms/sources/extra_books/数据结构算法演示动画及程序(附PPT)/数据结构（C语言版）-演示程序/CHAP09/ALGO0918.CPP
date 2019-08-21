Status InsertHash(HashTable &H, HElemType e) {  // 算法9.18
   // 查找不成功时插入数据元素e到开放定址哈希表H中，并返回OK；
   // 若冲突次数过大，则重建哈希表
   int c = 0;
   int p = 0;
   if (SearchHash(H, e.key, p, c) == SUCCESS )
      return DUPLICATE;        // 表中已有与e有相同关键字的元素
   else if (c < H.cursize) {   // 冲突次数c未达到上限，（阀值c可调）
      H.elem[p] = e;  ++H.count;  return SUCCESS;  // 插入e
   } else {
      RecreateHashTable(H);  // 重建哈希表
      return UNSUCCESS;
   } 
} // InsertHash