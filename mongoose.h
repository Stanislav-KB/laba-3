#ifndef MONGOOSE_H
#define MONGOOSE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "input.h"

// Структуры и функции для работы с Mongoose
struct mg_connection;
struct mg_request_info;

// Функция для отправки статического файла
void mg_send_file(struct mg_connection *c, const char *filename);

// Функция для получения значения переменной из POST-запроса
void mg_get_var(const char *data, const char *var_name, char *buf, size_t buf_len);

// Функция для обработки входящего запроса
void mg_handle_request(struct mg_connection *c, struct mg_request_info *ri);

#endif // MONGOOSE_H