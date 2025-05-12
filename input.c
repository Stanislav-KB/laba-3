#include "input.h"
#include <string.h>

// Простая функция для поиска и парсинга параметра
int parse_form_param(const char *body, const char *param_name, char *output, size_t maxlen) {
    const char *p = body;
    size_t pname_len = strlen(param_name);
    while (p && *p) {
        if (strncmp(p, param_name, pname_len) == 0 && p[pname_len] == '=') {
            p += pname_len + 1; // переходим к значению
            const char *end = strchr(p, '&');
            size_t len = end ? (size_t)(end - p) : strlen(p);
            if (len >= maxlen) len = maxlen - 1;
            strncpy(output, p, len);
            output[len] = '\0';
            // Декодируем URL
            return url_decode(output, output, maxlen);
        }
        p = strchr(p, '&');
        if (p) p++;
    }
    return 0; // не найдено
}

// Простая декодировка URL (замена %xx на символ)
int url_decode(const char *src, char *dest, size_t maxlen) {
    size_t si = 0, di = 0;
    while (src[si] && di < maxlen -1) {
        if (src[si] == '%') {
            if (isxdigit(src[si+1]) && isxdigit(src[si+2])) {
                char hex[3] = {src[si+1], src[si+2], 0};
                dest[di++] = (char)strtol(hex, NULL, 16);
                si += 3;
            } else {
                dest[di++] = '%';
                si++;
            }
        } else if (src[si] == '+') {
            dest[di++] = ' ';
            si++;
        } else {
            dest[di++] = src[si++];
        }
    }
    dest[di] = '\0';
    return 1;
}