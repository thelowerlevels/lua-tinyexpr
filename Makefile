CC = gcc

TARGET = te.so
SRC = src/lua_tinyexpr.c src/tinyexpr.c

CFLAGS = -O3 -fPIC

all:
	$(CC) $(SRC) -shared -o $(TARGET) $(CFLAGS)

clean:
	rm -f $(TARGET)

.PHONY: all clean
