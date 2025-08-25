CC = gcc
TARGET = auri

SRC = $(shell find . -type f \( -name '*.c' -o -name '*.h' \))
OUT_DIR = out
BIN = $(OUT_DIR)/$(TARGET)

all: $(OUT_DIR) $(BIN)

$(BIN): $(SRC)
	echo $(SRC)
	$(CC) $(SRC) -o $(BIN)

$(OUT_DIR):
	mkdir -p $(OUT_DIR)

clean:
	rm -rf $(OUT_DIR)

.PHONY: all clean
