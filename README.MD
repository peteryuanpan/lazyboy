# lazyboy

# CHANGE LOG

### version0.2

完成时间：2017-06-30

主要内容：
- 本版本论文总结：[基于深度搜索算法在中国象棋AI上的应用.pdf](https://github.com/peteryuanpan/lazyboy/blob/master/document/%E5%9F%BA%E4%BA%8E%E6%B7%B1%E5%BA%A6%E6%90%9C%E7%B4%A2%E7%AE%97%E6%B3%95%E5%9C%A8%E4%B8%AD%E5%9B%BD%E8%B1%A1%E6%A3%8BAI%E4%B8%8A%E7%9A%84%E5%BA%94%E7%94%A8.pdf)
- 代码部分包含 UCCI接口、棋盘数据结构、棋子着法、哈希表、多样搜索算法、评估函数
- 搜索及剪枝算法，包括 静态搜索、零窗口搜索、空着裁剪、Alpha-Beta 搜索、迭代加深搜索
- 代码能力比version0.1强，但强度一般，可以在此版本基础上继续拓展
- 编译方法：`g++ *.cpp -o lazyboy0.2.exe`

### version0.1

完成时间：2016-06-30

主要内容：
- 提供了一个简陋版本的象棋AI，拥有解决象棋巫师软件上133个象棋残局的能力（一共240个残局）
- 代码部分包含 UCCI接口、棋盘数据结构、棋子着法、哈希表、搜索算法
- 代码能力比较弱，属于第一代版本，不再维护
- 编译方法：`g++ *.cpp -o lazyboy0.1.exe`
