### 输出文件类型

```

```



### 主流编译器

```
https://blog.csdn.net/ZCShouCSDN/article/details/89553323
```

#### 编译过程

1. 预处理		//.i文件

   #扩展预处理宏定义、头文件

   gcc -E test.c -o test.i

2. 编译            //.s文件

   #-S编译选项告诉gcc在为C代码生成汇编文件后停止

   gcc -S test.i -o test.s

3. 汇编           //.o文件

   #-c选项告诉gcc编译器把代码编译成机器语言的目标代码

   gcc -c test.s -o test.o

4. 链接           //bin文件

   #生成可执行文件

   gxx test.o -o test

#### 参数

![image-20220829111717962](../typora-user-images/image-20220829111717962.png)

![image-20220829111523343](../typora-user-images/image-20220829111523343.png)

![image-20220829112645375](../typora-user-images/image-20220829112645375.png)

![image-20220829112906804](../typora-user-images/image-20220829112906804.png)

![image-20220829112951590](../typora-user-images/image-20220829112951590.png)

![image-20220829151709991](../typora-user-images/image-20220829151709991.png)

![image-20220829151730286](../typora-user-images/image-20220829151730286.png)
