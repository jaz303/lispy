CC			= gcc

CFLAGS		= -Iinclude -Iext/jazlib/include -Wall -Wno-unused-function
LDFLAGS		=

BUILD_DIR	=	build
BIN_TARGET	=	$(BUILD_DIR)/lispy

OBJS		=	src/lispy.o \
				src/lexer.o \
				src/object.o \
				src/parser.o \
				src/intern.o \
				src/scope.o \
				src/eval.o

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

obj: $(OBJS)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BIN_TARGET): $(BUILD_DIR) obj
	$(CC) $(LDFLAGS) -o $(BIN_TARGET) $(OBJS)

all: $(BIN_TARGET)

clean:
	rm -rf $(BUILD_DIR)
	find . -name '*.o' -delete
