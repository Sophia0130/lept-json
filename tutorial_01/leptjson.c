// 功能：对null、false、true的解析

// lept_type     json的数值类型                    枚举
// lept_value    json的数值类型                    结构体-lept_type
// lept_context  json当前字符串的位置               结构体-字符指针
//               json合法性                        int

#include "leptjson.h"
#include <assert.h>  /* assert() */
#include <stdlib.h>  /* NULL */

// 跳过空格后，检测当前字符
#define EXPECT(c, ch)       do { assert(*c->json == (ch)); c->json++; } while(0)

typedef struct {
	const char* json;   // json只存储字符串当前位置
}lept_context;

// 跳过空格
static void lept_parse_whitespace(lept_context* c) {
    const char *p = c->json;
    while (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r')
        p++;
    c->json = p;        // json此时指向的字符串不是空格，可能是字符，或是字符的结束
}

// 判断是否为 true
static int lept_parse_true(lept_context* c, lept_value* v) {
    EXPECT(c, 't');
    if (c->json[0] != 'r' || c->json[1] != 'u' || c->json[2] != 'e')
        return LEPT_PARSE_INVALID_VALUE;
    c->json += 3;
    v->type = LEPT_TRUE;
    return LEPT_PARSE_OK;
}

// 判断是否为 false
static int lept_parse_false(lept_context* c, lept_value* v) {
    EXPECT(c, 'f');
    if (c->json[0] != 'a' || c->json[1] != 'l' || c->json[2] != 's' || c->json[3] != 'e')
        return LEPT_PARSE_INVALID_VALUE;
    c->json += 4;
    v->type = LEPT_FALSE;
    return LEPT_PARSE_OK;
}

// 判断是否为 null
static int lept_parse_null(lept_context* c, lept_value* v) {
    EXPECT(c, 'n');
    if (c->json[0] != 'u' || c->json[1] != 'l' || c->json[2] != 'l')
        return LEPT_PARSE_INVALID_VALUE;
    c->json += 3;
    v->type = LEPT_NULL;
    return LEPT_PARSE_OK;
}

// 判断是 null、true、false、全空格
static int lept_parse_value(lept_context* c, lept_value* v) {
    switch (*c->json) {
        case 't':  return lept_parse_true(c, v);
        case 'f':  return lept_parse_false(c, v);
        case 'n':  return lept_parse_null(c, v);
        case '\0': return LEPT_PARSE_EXPECT_VALUE;
        default:   return LEPT_PARSE_INVALID_VALUE;
    }
}


// 解析核心：v 获得json数值类型，返回json合法性
// json文本由三部分组成：空格+数值+空格
int lept_parse(lept_value* v, const char* json) {
    lept_context c;                    // 当前字符的指针
    int ret;
    assert(v != NULL);
    c.json = json;
    v->type = LEPT_NULL;              // 数值类型初始化为null
    lept_parse_whitespace(&c);        // 跳过空格
    if ((ret = lept_parse_value(&c, v)) == LEPT_PARSE_OK) {        // 若json合法
        lept_parse_whitespace(&c);
        if (*c.json != '\0')
            ret = LEPT_PARSE_ROOT_NOT_SINGULAR;                    // 第三部分空格后还有字符而不是字符结束'\0'
    }
    return ret;
}

// 返回json的数值类型
lept_type lept_get_type(const lept_value* v) {
    assert(v != NULL);
    return v->type;
}
