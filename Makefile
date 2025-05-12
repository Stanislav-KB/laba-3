CC = gcc
CFLAGS = -I. -I./mongoose
SRC = main.c mongoose.c
OBJ = $(SRC:.c=.o)
TARGET = app

# Правило по умолчанию
all: $(TARGET)

# Правило для сборки исполняемого файла
$(TARGET): $(OBJ)
	$(CC) -o $@ $^

# Правило для компиляции объектных файлов
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Правило для очистки
clean:
	rm -f $(OBJ) $(TARGET)

# Правило для запуска приложения
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run