# skiplist
lightweight KV storage

采用skiplist作为底层数据结构，支持插入，删除，查询等常见操作。
使用C++模板编程，使用类似STL，支持自定义类型与自定义比较函数（可以传入lambda与仿函数），迭代器遍历。

## 跳表
跳表是对有序链表的改进，是一种查找数据结构，支持对数据的快速查找，插入和删除。
跳表的期望空间复杂度为 O(n)，跳表的查询，插入和删除操作的期望时间复杂度都为 O(log n)。

跳表在有序链表的基础上，引入了`分层`的概念。首先，跳表的每一层都是一个有序链表，特别地，最底层是初始的有序链表。每个位于第 i 层的节点有 p 的概率出现在第 i+1 层，p 为常数。

在跳表中查找，就是从第 L(n) 层开始，水平地逐个比较直至当前节点的下一个节点大于等于目标节点，然后移动至下一层。重复这个过程直至到达第一层且无法继续进行操作。
此时，若下一个节点是目标节点，则成功查找；反之，则元素不存在。这样一来，查找的过程中会跳过一些没有必要的比较，所以相比于有序链表的查询，跳表的查询更快。
可以证明，跳表查询的平均复杂度为 O(log n)。
