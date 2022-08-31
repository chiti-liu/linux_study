### Important Website

```
https://home.code-nav.cn/
https://codetop.cc/home
https://visualgo.net/zh
https://the-algorithms.com/
https://www.cxyxiaowu.com/
https://leetcode.cn/
```

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

