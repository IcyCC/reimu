# Reium -- 高性能网络库

## 目标

* reactor + 多线程
* 高性能日志库
* 基于切片的字符串处理
* 定时任务
* Tcp链接处理


## Q&A

* Q1: 并发模式？
A1:

<img src="./assets/01.png" width = 100% height = 100% />

把可能比较费时间的用户逻辑放在单独的线程里处理， 
这样又能保证一个套接字（内核程度）只有一个线程在操作， 不用加锁， 性能也不错, 线程之间通过SafeQueue做同步

* Q2: 事件处理？

只处理了 POLLIN 和 POLLOUT 具体的错误根据读写操作时的erron来判断

* Q3： 字符串处理？

用Slice 作为字符串的视图， 记录起始地址， 当用户真正需要的时候再自行拷贝。 Slice 会对 Buffer 做引用计数， 当有Slice对Buffer产生引用的时候
Buffer不会缩减空间

* Q4: 错误处理？

erron 的 EAGAIN EMFILE EINTR ECONNABORTED 不视作错误  直接重试或等下一次

* Q5: 日志处理？

基于SafeQueue做日志异步消费

* Q6: 超时处理？

可对每个链接设置超时时间， 调用超时的回调， 完成回调后直接关闭， 
服务器会记录超时的链接， 把超时的链接踢出去。


