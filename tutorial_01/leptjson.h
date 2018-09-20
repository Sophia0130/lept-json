#ifndef LEPTJSON_H__
#define LEPTJSON_H__

// json ��ֵ����
typedef enum { LEPT_NULL, LEPT_FALSE, LEPT_TRUE, LEPT_NUMBER, LEPT_STRING, LEPT_ARRAY, LEPT_OBJECT } lept_type;  //ö�ٳ���Ϊint
// null��null
// boolean��true��false
// number
// string��"..."
// array��[...]
// object��{...}

// json ��ֵ����
typedef struct { lept_type type; }lept_value;

// �������ж�json�Ƿ�Ϸ�
// json�ı�����������ɣ��ո�+��ֵ+�ո�
enum {
	LEPT_PARSE_OK = 0,             // �޴���
	LEPT_PARSE_EXPECT_VALUE,       // ������1��ֻ���пո�
	LEPT_PARSE_INVALID_VALUE,      // ������2���ڶ�������ֵ������Ч��ֵ
	LEPT_PARSE_ROOT_NOT_SINGULAR   // ������3���������ֵĿո���������ַ���������'\0'
};

int lept_parse(lept_value* v, const char* json);  // ����json

lept_type lept_get_type(const lept_value* v);     // ��ȡ����

#endif /* LEPTJSON_H__ */

//JSON �ı��� 3 ������ɣ��հ�+ֵ+�հ�
