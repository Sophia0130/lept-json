// 功能：解析数字

#include "leptjson.h"
#include <assert.h>  /* assert() */
#include <errno.h>   /* errno, ERANGE */
#include <math.h>    /* HUGE_VAL */
#include <stdlib.h>  /* NULL, strtod() */

#define EXPECT(c, ch)       do { assert(*c->json == (ch)); c->json++; } while(0)
#define ISDIGIT(ch)         ((ch) >= '0' && (ch) <= '9')
#define ISDIGIT1TO9(ch)     ((ch) >= '1' && (ch) <= '9')

typedef struct {
    const char* json;
}lept_context;

// 跳过空格
static void lept_parse_whitespace(lept_context* c) {
    const char *p = c->json;
    while (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r')
        p++;
    c->json = p;
}

// 解析 null、false、true
// 需要判断是否为 literal 这种数据类型
static int lept_parse_literal(lept_context* c, lept_value* v, const char* literal, lept_type type) {
    size_t i;
    EXPECT(c, literal[0]);
    for (i = 0; literal[i + 1]; i++)
        if (c->json[i] != literal[i + 1])
            return LEPT_PARSE_INVALID_VALUE;
    c->json += i;            // 注意：指针指向变了
    v->type = type;
    return LEPT_PARSE_OK;
}

// 解析数字
// number = [ "-" ] int [ frac ] [ exp ]
static int lept_parse_number(lept_context* c, lept_value* v) {
    const char* p = c->json;
    if (*p == '-') p++;                                            // 判断负号
    if (*p == '0') p++;                                            // 判断是否为单个0，注意0开头，只能是单个0
    else {
        if (!ISDIGIT1TO9(*p)) return LEPT_PARSE_INVALID_VALUE;    // 判断是否为1-9
        for (p++; ISDIGIT(*p); p++);   // 遍历整数部分
    }
    if (*p == '.') {                                              // 判断小数
        p++;
        if (!ISDIGIT(*p)) return LEPT_PARSE_INVALID_VALUE;
        for (p++; ISDIGIT(*p); p++);   // 遍历小数部分
    }
    if (*p == 'e' || *p == 'E') {                                 // 判断指数
        p++;
        if (*p == '+' || *p == '-') p++;
        if (!ISDIGIT(*p)) return LEPT_PARSE_INVALID_VALUE;
        for (p++; ISDIGIT(*p); p++);                             // 遍历指数部分
    }

	// 判断数字是否溢出
    errno = 0;
    v->n = strtod(c->json, NULL);     // 字符串转为浮点数 // 扫描字符串会自动跳过空格                            
    if (errno == ERANGE && (v->n == HUGE_VAL || v->n == -HUGE_VAL))    // 浮点数是否溢出 // 这里其实没有看懂 HUGE_VAL 这个宏定义?
        return LEPT_PARSE_NUMBER_TOO_BIG;
    v->type = LEPT_NUMBER;
    c->json = p;                // 注意：指针指向变了
    return LEPT_PARSE_OK;
}

// 判断是数字还是null、true、false
static int lept_parse_value(lept_context* c, lept_value* v) {
    switch (*c->json) {
        case 't':  return lept_parse_literal(c, v, "true", LEPT_TRUE);
        case 'f':  return lept_parse_literal(c, v, "false", LEPT_FALSE);
        case 'n':  return lept_parse_literal(c, v, "null", LEPT_NULL);
        default:   return lept_parse_number(c, v);
        case '\0': return LEPT_PARSE_EXPECT_VALUE;    // 全部都是空格
    }
}

int lept_parse(lept_value* v, const char* json) {
    lept_context c;
    int ret;   // json 合法性
    assert(v != NULL);
    c.json = json;
    v->type = LEPT_NULL;            // 类型初始化为 LEPT_NULL ，与TEST_ERROR部分相对应
    lept_parse_whitespace(&c);      // 跳过空格部分                         
    if ((ret = lept_parse_value(&c, v)) == LEPT_PARSE_OK) {  // 前半段具有合法性
        lept_parse_whitespace(&c);
        if (*c.json != '\0') {
            v->type = LEPT_NULL;    // 与TEST_ERROR部分相对应 //当数据不合法时，数据类型规定为 LEPT_NULL
            ret = LEPT_PARSE_ROOT_NOT_SINGULAR;
        }
    }
    return ret;
}

// json数据类型
lept_type lept_get_type(const lept_value* v) {
    assert(v != NULL);
    return v->type;
}

// 若是数字则返回double类型
double lept_get_number(const lept_value* v) {
    assert(v != NULL && v->type == LEPT_NUMBER);
    return v->n;
}
