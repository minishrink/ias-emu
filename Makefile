
.PHONY: build clean test

BIN=bin
MAC=machine
TST=test

CC=clang
CFLAGS=-Wall -Werror -std=c17 -Wno-unused-function -g
INCLUDES=-I$(MAC) -I$(TST) $(TST)/_*.c
EXEC=test.exe

clean:
	rm -rf bin/* obj/*.o || true

build_test_instr:
	$(CC) $(CFLAGS) $(INCLUDES) $(TST)/test_instructions.c -o $(BIN)/$(EXEC)

test_instr: build_test_instr
	$(BIN)/$(EXEC)

build_test_bits:
	$(CC) $(CFLAGS) $(INCLUDES) $(TST)/test_bitmasks.c -o $(BIN)/$(EXEC)

test_bits: build_test_bits
	$(BIN)/$(EXEC)

test: test_instr test_bits

$(OBJ)/%.o: $(SRC)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

