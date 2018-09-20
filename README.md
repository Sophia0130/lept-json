# lept-json

关于 lept-json 的代码解读

https://github.com/miloyip/json-tutorial

<br />

## tutorial_01

对于 null 、false、true 的解析

可能出现的错误状态：

- 全部都是空格
- 无效的字符（即不是上面三种值）
- 数值后不只是空格还有别的字符

PS：JSON 文本应该有 3 部分：`JSON-text = 空格 + value + 空格`


