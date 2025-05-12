#ifndef CONSTANTS_H
#define CONSTANTS_H

#define TITLE "Конструктор поздравлений"
#define FORM_HTML "<form method=\"POST\" action=\"/submit\">" \
                  "<label for=\"name\">Имя:</label>" \
                  "<input type=\"text\" id=\"name\" name=\"name\" required><br><br>" \
                  "<label for=\"occasion\">Повод:</label>" \
                  "<select id=\"occasion\" name=\"occasion\">" \
                  "<option value=\"День рождения\">День рождения</option>" \
                  "<option value=\"Новый год\">Новый год</option>" \
                  "<option value=\"Выпускной\">Выпускной</option>" \
                  "</select><br><br>" \
                  "<input type=\"submit\" value=\"Поздравить\">" \
                  "</form>"

#endif // CONSTANTS_H