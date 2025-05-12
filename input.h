#ifndef INPUT_H
#define INPUT_H

#include <stddef.h>

// Функция для парсинга параметра из строки формы
// input - строка вида "name=value&..."
// param_name - имя параметра, например "name" или "occasion"
// output - буфер для результата
// maxlen - размер буфера
// возвращает 1, если найдено и успешно скопировано, 0 иначе
int parse_form_param(const char *body, const char *param_name, char *output, size_t maxlen);

// Можно добавить функции для безопасного парсинга и декодирования
int url_decode(const char *src, char *dest, size_t maxlen);

#endif // INPUT_H