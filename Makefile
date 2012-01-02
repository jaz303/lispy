CC			= gcc

CFLAGS		= -Iinclude -Iext/jazlib/include -Wall -Wno-unused-function
LDFLAGS		=

BUILD_DIR	=	build
BIN_TARGET	=	$(BUILD_DIR)/lispy

OBJS		=	ext/jazlib/src/common.o \
				src/env.o \
				src/gc.o \
				src/lispy.o \
				src/repl.o \
				src/lexer.o \
				src/intern.o \
				src/parser.o \
				src/binding.o \
				src/eval.o \
				src/native.o

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

default: $(BIN_TARGET)

obj: $(OBJS)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BIN_TARGET): $(BUILD_DIR) obj
	$(CC) $(LDFLAGS) -o $(BIN_TARGET) $(OBJS)

all: $(BIN_TARGET)

clean:
	rm -rf $(BUILD_DIR)
	find . -name '*.o' -delete
