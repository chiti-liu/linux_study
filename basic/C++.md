构造函数在创建对象的时候调用，先调用父类，再子类，析构相反

（1）当父类的指针new一个子类的对象时，

父类析构不是虚析构，则delete的时候不调用子类的，只是调用父类的析构函数，如果是virtual的析构函数，则先子类之后父类

（2）当子类的指针new一个子类的对象时，构造析构都会调用父类

 

注意子类对父类的访问和继承  不同类型的继承多看多搜

 

Vector <int> iterator arr = string.begin();

*arr++;

Vector 以链式存储在缓冲区，不连续存储，但是访问和添加方便

 

Int至少和一个short一样长  long至少和一个int一样长  

即 long >= int >=short

 

指针指向一块内存，它的内容是所指内存的地址；而引用则是某块内存的别名，引用不改变指向。

 

Constexpr int *p 类似 int *const p，都把p置成了顶层const,但是：

使用GNU gcc编译器时，constexpr指针所指变量必须是全局变量或者static变量(既存储在静态数据区的变量)。

 

Char const == const char

However  char const *p ： p point to char const or const char 

​      Char *const p ： const p point to char

 

封装：意味着把对象的属性和方法结合成一个独立的系统单位，并尽可能隐藏对象的内部细节。

 

抽象：是对具体问题进行概括的过程  是对一类公共问题进行统一概述的过程，为了使某些必要信息得到顺利的交流，就如同一个协议一般，使所有参与的个体都能得到有效的信息交流支持。

 

继承：对象用欧与基类相同的全部属性和方法。

 

多态：基类定义的属性和行为被子类继承后，可以具有不同的数据类型或者表现行为等特性

 

重载：不同语境  不同作用  单目  双目  [] ++ -- Iostream ->(最难)

 

虚函数在声明时加入vitual，可以父类中进行实现，如果为了后续更改，可以不实现且等于0（virtual double getVolume() = 0;），就是纯虚函数。

 

Ch=Getchar()//从输入流获得字符，这个时候缓冲区的数据是给了ch里，如果需要操作需要返回给输入流。如下：

ungetc(ch,stdin)//将变量ch中存放的字符退回给stdin输入流



explicit关键字可以防止构造函数进行隐式自动转换,这是种有隐患的操作

implicit为默认的隐式自动转换。