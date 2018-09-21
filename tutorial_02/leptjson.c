// ���ܣ���������

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

// �����ո�
static void lept_parse_whitespace(lept_context* c) {
    const char *p = c->json;
    while (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r')
        p++;
    c->json = p;
}

// ���� null��false��true
// ��Ҫ�ж��Ƿ�Ϊ literal ������������
static int lept_parse_literal(lept_context* c, lept_value* v, const char* literal, lept_type type) {
    size_t i;
    EXPECT(c, literal[0]);
    for (i = 0; literal[i + 1]; i++)
        if (c->json[i] != literal[i + 1])
            return LEPT_PARSE_INVALID_VALUE;
    c->json += i;            // ע�⣺ָ��ָ�����
    v->type = type;
    return LEPT_PARSE_OK;
}

// ��������
// number = [ "-" ] int [ frac ] [ exp ]
static int lept_parse_number(lept_context* c, lept_value* v) {
    const char* p = c->json;
    if (*p == '-') p++;                                            // �жϸ���
    if (*p == '0') p++;                                            // �ж��Ƿ�Ϊ����0��ע��0��ͷ��ֻ���ǵ���0
    else {
        if (!ISDIGIT1TO9(*p)) return LEPT_PARSE_INVALID_VALUE;    // �ж��Ƿ�Ϊ1-9
        for (p++; ISDIGIT(*p); p++);   // ������������
    }
    if (*p == '.') {                                              // �ж�С��
        p++;
        if (!ISDIGIT(*p)) return LEPT_PARSE_INVALID_VALUE;
        for (p++; ISDIGIT(*p); p++);   // ����С������
    }
    if (*p == 'e' || *p == 'E') {                                 // �ж�ָ��
        p++;
        if (*p == '+' || *p == '-') p++;
        if (!ISDIGIT(*p)) return LEPT_PARSE_INVALID_VALUE;
        for (p++; ISDIGIT(*p); p++);                             // ����ָ������
    }

	// �ж������Ƿ����
    errno = 0;
    v->n = strtod(c->json, NULL);     // �ַ���תΪ������ // ɨ���ַ������Զ������ո�                            
    if (errno == ERANGE && (v->n == HUGE_VAL || v->n == -HUGE_VAL))    // �������Ƿ���� // ������ʵû�п��� HUGE_VAL ����궨��?
        return LEPT_PARSE_NUMBER_TOO_BIG;
    v->type = LEPT_NUMBER;
    c->json = p;                // ע�⣺ָ��ָ�����
    return LEPT_PARSE_OK;
}

// �ж������ֻ���null��true��false
static int lept_parse_value(lept_context* c, lept_value* v) {
    switch (*c->json) {
        case 't':  return lept_parse_literal(c, v, "true", LEPT_TRUE);
        case 'f':  return lept_parse_literal(c, v, "false", LEPT_FALSE);
        case 'n':  return lept_parse_literal(c, v, "null", LEPT_NULL);
        default:   return lept_parse_number(c, v);
        case '\0': return LEPT_PARSE_EXPECT_VALUE;    // ȫ�����ǿո�
    }
}

int lept_parse(lept_value* v, const char* json) {
    lept_context c;
    int ret;   // json �Ϸ���
    assert(v != NULL);
    c.json = json;
    v->type = LEPT_NULL;            // ���ͳ�ʼ��Ϊ LEPT_NULL ����TEST_ERROR�������Ӧ
    lept_parse_whitespace(&c);      // �����ո񲿷�                         
    if ((ret = lept_parse_value(&c, v)) == LEPT_PARSE_OK) {  // ǰ��ξ��кϷ���
        lept_parse_whitespace(&c);
        if (*c.json != '\0') {
            v->type = LEPT_NULL;    // ��TEST_ERROR�������Ӧ //�����ݲ��Ϸ�ʱ���������͹涨Ϊ LEPT_NULL
            ret = LEPT_PARSE_ROOT_NOT_SINGULAR;
        }
    }
    return ret;
}

// json��������
lept_type lept_get_type(const lept_value* v) {
    assert(v != NULL);
    return v->type;
}

// ���������򷵻�double����
double lept_get_number(const lept_value* v) {
    assert(v != NULL && v->type == LEPT_NUMBER);
    return v->n;
}
