# LWIP协议学习心得

- 一次要处理多少数据需要设置MEM_SIZE
- 以太网帧一次不能超出1500个字节，就要进行分片，UDP在IP层进行分包，TCP必须在传输层进行分包。
- 无OS：RAW接口，有OS：netconn或者socket接口，可以在宏中定义，lwipopt.h
- netif：网络接口抽象（wifi/以太网。。。），进行    ethernet: 网络层数据进行封包进入MAC层 ???sys_arch: OS抽象
- IP：分包和组包     ARP：查询并获取对方MAC    ICMP：ping