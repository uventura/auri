CC = gcc
TARGET = auri
FLAGS = -W
FLAGS += -Wall
FLAGS += -Werror
FLAGS += -pedantic
FLAGS += -g
FLAGS += -Wno-unused-parameter
FLAGS += -Wno-implicit-fallthrough
FLAGS += -std=c11
FLAGS += -DENABLE_DEBUG
FLAGS += -fsanitize=address
FLAGS += -g

OUT_DIR = out
BIN_DIR = $(OUT_DIR)/bin
BIN = $(BIN_DIR)/$(TARGET)

SRC_DIR = auri
SRC = $(shell find $(SRC_DIR) -name '*.c')

OBJ_DIR = $(OUT_DIR)/obj
OBJS=$(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o, $(SRC))

all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) -o $(BIN_DIR)/$@ $^ $(FLAGS) -I .

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(FLAGS) -o $@ -c $< -I .

clean:
	@echo "Clean up environment..."
	@echo "-----------------------"
	rm -rf $(OUT_DIR)
	@echo "-----------------------"
	@echo "Environment cleaned!"

format:
	@echo "Applying Format..."
	@find . -type f \( -name "*.c" \) -exec clang-format -i {} \;
	@find . -type f \( -name "*.h" -o -name "*.h" \) -exec clang-format -i {} \;
	@echo "Done! "

.PHONY: all clean
