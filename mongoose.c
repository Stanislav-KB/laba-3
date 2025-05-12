#include "mongoose.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Функция для отправки статического файла
void mg_send_file(struct mg_connection *c, const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        mg_printf(c, "HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\n\r\n"
                     "<html><body><h1>404 Not Found</h1></body></html>");
        return;
    }

    // Получаем размер файла
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Отправляем заголовок
    mg_printf(c, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: %ld\r\n\r\n", file_size);

    // Отправляем содержимое файла
    char *buffer = (char *)malloc(file_size);
    fread(buffer, 1, file_size, file);
    fwrite(buffer, 1, file_size, c->send);
    free(buffer);
    fclose(file);
}

// Функция для получения значения переменной из POST-запроса
void mg_get_var(const char *data, const char *var_name, char *buf, size_t buf_len) {
    const char *start = strstr(data, var_name);
    if (start) {
        start += strlen(var_name) + 1; // Пропускаем '='
        const char *end = strchr(start, '&');
        if (end == NULL) {
            end = start + strlen(start);
        }
        size_t len = end - start;
        if (len < buf_len) {
            strncpy(buf, start, len);
            buf[len] = '\0'; // Завершаем строку
        }
    } else {
        buf[0] = '\0'; // Если переменная не найдена
    }
}

// Функция для обработки входящего запроса
void mg_handle_request(struct mg_connection *c, struct mg_request_info *ri) {
    if (strcmp(ri->uri, "/submit") == 0 && strcmp(ri->request_method, "POST") == 0) {
        handle_form_submission(c, ri);
    } else {
        // Отправляем HTML-форму
        mg_printf(c, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n"
                     "<html><body>"
                     "<form action=\"/submit\" method=\"POST\">"
                     "Имя: <input type=\"text\" name=\"name\"><br>"
                     "Повод: <select name=\"occasion\">"
                     "<option value=\"birthday\">День рождения</option>"
                     "<option value=\"new_year\">Новый год</option>"
                     "<option value=\"graduation\">Выпускной</option>"
                     "</select><br>"
                     "<input type=\"submit\" value=\"Отправить\">"
                     "</form></body></html>");
    }
}