Status SearchHash(HashTable H, HKeyType K, int &p, int &c) {  
   // 算法9.17
   // 在开放定址哈希表H中查找关键码为K的元素，
   // 若查找成功，以p指示待查数据元素在表中位置，并返回SUCCESS；
   // 否则，以p指示插入位置，并返回UNSUCCESS,
   // c用以计冲突次数，其初值置零，供建表插入时参考
   p = Hash(K);                          // 求得哈希地址
   while ((H.elem[p].key != NULLKEY) &&  // 该位置中填有记录
          !equal(K, (H.elem[p].key)))   // 并且关键字不相等
      collision(p, ++c);                 // 求得下一探查地址p
   if (equal(K, (H.elem[p].key)))
      return SUCCESS;        // 查找成功，p返回待查数据元素位置
   else return UNSUCCESS;    // 查找不成功(H.elem[p].key == NULLKEY)，
                             // p返回的是插入位置
} // SearchHash
