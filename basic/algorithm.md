### Important Website

```
https://home.code-nav.cn/
https://codetop.cc/home
https://visualgo.net/zh
https://the-algorithms.com/
https://www.cxyxiaowu.com/
https://leetcode.cn/
```

### 递归和归纳

递归法则

- 基准情形，需要有一个可以直接计算出的基准值，个人理解一般在程序中需要在X变量取值为1或0时，需要有个确定的值，且方程式取值的变量范围必须一样。例：f(x) = f(x/3 +1)+2x;此时1/x+1取值范围为1，要么在递归中设置为1时有确定值。
- 不断推进，需要对计算值进一步的计算中不断接近基准值
- 设计情形：所有的递归都能调用
- 合成效益：统一问题的同一实例，切忌在不同的递归调用中做重复性的工作

归纳法

反向证明，f(n)=***,用f(n+1),例：n平方的累加和

### 两数相加

**哈希**：`map`基于**STL**，基于**< 键值 对数>**的结构可以自由声明，在两数相加中查找`hashmap.count(target-nums[i])`个数，若当前数列没有对应键值，则把当前数列的下标作为对数，值作为键值，加入`map`，如果找到则返回`vector<int>`。

```
class Solution {
public:
    vector<int> twoSum(vector<int>& nums, int target) {
        map<int,int>hashmap;
        for(int i = 0; i < nums.size(); i++)
        {
            if(hashmap.count(target-nums[i]))
                return vector<int>{hashmap[target-nums[i]],i};
            else
                hashmap[nums[i]]=i;
        }
        return vector<int>{};    
    }
};
```

