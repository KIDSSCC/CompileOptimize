# CompileOptimize
My Own Compile 

### 2023/08/14
仓库重新建立，开始梳理现有代码

### 2023/08/15
在词法分析和语法分析中添加了对于计时函数的解析
目前可以通过性能测试了

### 2023/08/17
浮点39的汇编错误：
1. 等于与不等于运算结束后没有保存计算结果进行进一步的计算:修改了instruction.cpp
2. 布尔运算两端比较是出现类型不一致，整型比较浮点。:修改了ast.cpp，增加了Binaryexpr构造中的判断
3. float函数也需要额外补充返回语句：修改了ast.cpp