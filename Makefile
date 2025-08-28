CC = g++
TARGET = auri
FLAGS = -W
FLAGS += -Wall
FLAGS += -ansi
FLAGS += -pedantic
FLAGS += -g
FLAGS += -Wno-unused-parameter
FLAGS += -Wno-implicit-fallthrough

SRC = $(shell find . -type f \( -name '*.cpp' -o -name '*.hpp' \))
OUT_DIR = out
BIN = $(OUT_DIR)/$(TARGET)

all: $(OUT_DIR) $(BIN)

$(BIN): $(SRC)
	echo $(SRC)
	$(CC) $(SRC) -o $(BIN) $(FLAGS)

$(OUT_DIR):
	mkdir -p $(OUT_DIR)

clean:
	rm -rf $(OUT_DIR)

format:
	@echo "Applying Format"
	@find . -type f \( -name "*.cpp" \) -exec clang-format -i {} \;
	@find . -type f \( -name "*.h" -o -name "*.hpp" \) -exec clang-format -i {} \;

.PHONY: all clean
