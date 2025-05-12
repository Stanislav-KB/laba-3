#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mongoose/mongoose.h"
#include "constants.h"

static const char *s_http_port = "8080";

// Обработка HTTP-запросов
static void handle_request(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
    if (ev == MG_EV_HTTP_MSG) {
        struct mg_http_message *hm = (struct mg_http_message *) ev_data;
        if (mg_http_match_uri(hm, "/")) {
            // Отправляем страницу с формой
            mg_http_reply(c, 200, "Content-Type: text/html\r\n", "%s", 
                "<!DOCTYPE html><html lang=\"ru\"><head><meta charset=\"UTF-8\" /><title>Создать поздравление</title></head><body>"
                "<h1>Создать поздравление</h1>"
                "<form method=\"POST\" action=\"/submit\">"
                "<label for=\"name\">Имя:</label>"
                "<input type=\"text\" id=\"name\" name=\"name\" required><br><br>"
                "<label for=\"occasion\">Повод:</label>"
                "<select id=\"occasion\" name=\"occasion\">"
                "<option value=\"День рождения\">День рождения</option>"
                "<option value=\"Новый год\">Новый год</option>"
                "<option value=\"Выпускной\">Выпускной</option>"
                "</select><br><br>"
                "<input type=\"submit\" value=\"Поздравить\" />"
                "</form>"
                "</body></html>");
        } else if (mg_http_match_uri(hm, "/submit")) {
            // Обработка POST-запроса
            if (mg_http_get_request_body(hm, c->user_data, 1024) == 0) {
                // В случае ошибок
                mg_http_reply(c, 500, "", "Ошибка обработки запроса");
                return;
            }

            // Получаем параметры формы
            char name[50] = {0};
            char occasion[50] = {0};

            // парсим параметры из сообщения
            struct mg_str *body = &hm->body;
            char *body_str = strndup(body->ptr, body->len);
            if (body_str) {
                // парсим name
                char *p = strstr(body_str, "name=");
                if (p) {
                    p += 5;
                    char *amp = strchr(p, '&');
                    if (amp) *amp = '\0';
                    // декодируем
                    mg_url_decode(p, strlen(p), name, sizeof(name), 0);
                }
                // парсим occasion
                p = strstr(body_str, "occasion=");
                if (p) {
                    p += 9;
                    char *amp = strchr(p, '&');
                    if (amp) *amp = '\0';
                    mg_url_decode(p, strlen(p), occasion, sizeof(occasion), 0);
                }
                free(body_str);
            }

            // Формируем поздравление
            char response[1024];
            snprintf(response, sizeof(response),
                "<!DOCTYPE html><html lang=\"ru\"><head><meta charset=\"UTF-8\" /><title>Поздравление</title></head><body>"
                "<h1>Поздравление для %s</h1>"
                "<p>Поздравляю тебя с %s!</p>"
                "<p>Желаю тебе всего самого лучшего!</p>"
                "<a href=\"/\">Создать новое поздравление</a>"
                "</body></html>", name, occasion);

            mg_http_reply(c, 200, "Content-Type: text/html\r\n", "%s", response);
        } else {
            // Страница не найдена
            mg_http_reply(c, 404, "", "Страница не найдена");
        }
    }
}

int main(void) {
    struct mg_mgr mgr;
    mg_mgr_init(&mgr);

    struct mg_connection *c = mg_http_listen(&mgr, s_http_port, handle_request, NULL);
    if (c == NULL) {
        printf("Ошибка запуска сервера\n");
        return 1;
    }
    printf("Сервер запущен на порту %s\n", s_http_port);

    for (;;) {
        mg_mgr_poll(&mgr, 1000);
    }

    mg_mgr_free(&mgr);
    return 0;
}