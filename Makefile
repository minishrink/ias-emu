
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

build_test_enc:
	$(CC) $(CFLAGS) $(INCLUDES) $(MAC)/_*.c $(TST)/test_encoding.c -o $(BIN)/$(EXEC)

test_enc: build_test_enc
	$(BIN)/$(EXEC)

test: test_instr test_enc

$(OBJ)/%.o: $(SRC)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

