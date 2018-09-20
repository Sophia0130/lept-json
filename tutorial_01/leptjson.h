#ifndef LEPTJSON_H__
#define LEPTJSON_H__

// json 数值类型
typedef enum { LEPT_NULL, LEPT_FALSE, LEPT_TRUE, LEPT_NUMBER, LEPT_STRING, LEPT_ARRAY, LEPT_OBJECT } lept_type;  //枚举常量为int
// null：null
// boolean：true、false
// number
// string："..."
// array：[...]
// object：{...}

// json 数值类型
typedef struct { lept_type type; }lept_value;

// 解析器判断json是否合法
// json文本由三部分组成：空格+数值+空格
enum {
	LEPT_PARSE_OK = 0,             // 无错误
	LEPT_PARSE_EXPECT_VALUE,       // 错误码1：只含有空格
	LEPT_PARSE_INVALID_VALUE,      // 错误码2：第二部分数值部分无效数值
	LEPT_PARSE_ROOT_NOT_SINGULAR   // 错误码3：第三部分的空格后还有其它字符，而不是'\0'
};

int lept_parse(lept_value* v, const char* json);  // 解析json

lept_type lept_get_type(const lept_value* v);     // 获取类型

#endif /* LEPTJSON_H__ */

//JSON 文本由 3 部分组成：空白+值+空白
