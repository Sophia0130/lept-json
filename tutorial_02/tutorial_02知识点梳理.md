## tutorial_02知识点梳理

<br />

### 1. errno

errno 是记录系统的最后一次错误代码，一个int型的值，在\<erron.h\> 头文件下

当Linux C API函数发生异常时，一般会将errno变量赋一个整数值，同的值表示不同的含义，可以通过查看该值推测出错的原因。

PS：一开始不明白上面的解释为什么强调Linux平台，后来想了一下，应该这样解释

语言和平台没关  但是不同平台的函数库语言不同，调用的机制不同，对操作系统的接口不同，编译结果不同，所以这里会强调平台的作用

<br />

## 2. strtod() 函数

double strtod (const char* str, char** endptr);   //**将字符串转换成双精度浮点数(double)**

扫描参数str字符串，跳过空白字符，直到遇上数字或正负符号才开始做转换，到出现非数字或字符串结束时('\0')才结束转换，并将结果返回。参数 str 字符串可包含正负号、小数点或E(e)来表示指数部分

<br />

### 3. 宏中的 #、##、@、\

- \#： 字符串化操作符

- \#\# ：符号连接操作符

  **作用是先分隔，然后进行强制连接**

  在普通的宏定义中，预处理器一般把空格解释成分段标志，对于每一段和前面比较，**相同的就被替换**

- @\#：字符化操作符

- \： 行继续操作符

```
 #define A1(name, type)  type name_##type##_type 或
 #define A2(name, type)  type name##_##type##_type

 A1(a1, int);  /* 等价于: int name_int_type; */
 A2(a1, int);  /* 等价于: int a1_int_type;   */

   解释：

   1) 在第一个宏定义中，预处理器会把name_##type##_type解释成3段：

   “name_”、“type”、以及“_type”，只有“type”是在宏前面出现过，所以它可以被宏替换


   2) 在第二个宏定义中，预处理器会把name##_##type##_type解释成4段：
   “name”、“_”、“type”、以及“_type”，  “name”和“type”可以被宏替换

```

<br />

### 4. C 语言结构体中点运算符( . )和箭头运算符( -> )的区别

解释来自：https://blog.csdn.net/shenyuanluo/article/details/51146140

- 结构体变量通过点运算符( . )访问
- 指向结构体的指针通过箭头运算符( -> )访问

```c
typedef struct          // 定义一个结构体类型：DATA
{
    char key[10];      
    char name[20];   
    int age;          
}DATA;
    
DATA data;              // 声明一个结构体变量
DATA *pdata;            // 声明一个指向结构体的指针
    
// 访问数据
data.age = 24;          // 结构体变量通过点运算符( . )访问
pdata->age = 24;        // 指向结构体的指针通过箭头运算符( -> )访问

```

<br />

### 代码理解

`int lept_parse(lept_value* v, const char* json)` 中出现两次 `v->type = LEPT_NULL`

刚开始不理解，看到测试程序 `TEST_ERROR` 才反应过来

其实就是将所有没有合法性数据的类型设为 `LEPT_NULL `

<br />

```c
int lept_parse(lept_value* v, const char* json) {
    lept_context c;
    int ret;   
    assert(v != NULL);
    c.json = json;
    v->type = LEPT_NULL;            // 1-数据类型初始化为 LEPT_NULL 
    lept_parse_whitespace(&c);                           
    if ((ret = lept_parse_value(&c, v)) == LEPT_PARSE_OK) {  
        lept_parse_whitespace(&c);
        if (*c.json != '\0') {
            v->type = LEPT_NULL;    // 2-当数据不合法时，数据类型规定为 LEPT_NULL
            ret = LEPT_PARSE_ROOT_NOT_SINGULAR;
        }
    }
    return ret;
}
```

<br />

```c
#define TEST_ERROR(error, json)\
    do {\
        lept_value v;\
        v.type = LEPT_FALSE;\
        EXPECT_EQ_INT(error, lept_parse(&v, json));\
        EXPECT_EQ_INT(LEPT_NULL, lept_get_type(&v));\
    } while(0)
```