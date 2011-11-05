CC			= gcc
CPP			= g++

CFLAGS		= -Iinclude -Iext/jazlib/include -Wall -Wno-unused-function
CPPFLAGS	=
LDFLAGS		=

# Config

BUILD_DIR	=	build

# Objects

OBJS		=	src/lispy.o \
				src/lexer.o \
				src/object.o \
				src/parser.o \
				src/intern.o

# Products

BIN_TARGET	=	$(BUILD_DIR)/lispy

#
#
#

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

%.o: %.cpp
	$(CPP) $(CPPFLAGS) -c -o $@ $<

obj: $(OBJS)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BIN_TARGET): $(BUILD_DIR) obj
	$(CC) $(LDFLAGS) -o $(BIN_TARGET) $(OBJS)

all: $(BIN_TARGET)

clean:
	rm -rf $(BUILD_DIR)
	find . -name '*.o' -delete
