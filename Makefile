# ----- Config -----
CC      = gcc
CFLAGS  = -Wall -Wextra -O2 -std=c11
LDFLAGS = -mwindows
TARGET  = mouse_monitor.exe
SRC_DIR = src
OBJ_DIR = build

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

# ----- Regras -----
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir $(OBJ_DIR)

clean:
	rmdir /S /Q $(OBJ_DIR) 2>NUL
	del /Q $(TARGET) 2>NUL

run: $(TARGET)
	.\\$(TARGET)

.PHONY: all clean run