# lept-json

关于 lept-json 的代码解读

https://github.com/miloyip/json-tutorial

<br />

## tutorial_01

功能：对null、true、false的解析

『JSON  文本特点』

```
JSON-text = ws value ws

ws = *(%x20 / %x09 / %x0A / %x0D)  空格
value = null / false / true 
```

<br />


## tutorial_02

功能：对数字的解析，并且将字符串转为double类型存储

『JSON  数字特点』

```
number = [ "-" ] int [ frac ] [ exp ]

//数字由负号、整数、小数、指数四部分组成，其中只有整数是必需部分
-                                          //只有负号，正号不合法
int = "0" / digit1-9 *digit                //如果是 0 开始，只能是单个 0，即012这样不合法
frac = "." 1*digit                         //小数部分
exp = ("e" / "E") ["-" / "+"] 1*digit      //科学记数法
```

<br />

<br />

<br />

## TDD

测试驱动开发（test-driven development, TDD）：软件开发方法

先写测试，再实现功能，这样就不会写一些不需要的代码，或是没有被测试的代码

### 1. 步骤

- 加入一个测试
- 运行所有测试，新的测试应该会失败
- 编写实现代码
- 运行所有测试，若有测试失败回到上一步
- 重构代码



## 2.重构（refactoring） 

在不改变代码外在行为的情况下，对代码作出修改，以改进程序的内部结构
