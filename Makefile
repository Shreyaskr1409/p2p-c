CC = gcc
CFLAGS = -g -Iinclude
# LDFLAGS = -Llib -Linclude -Linclude/libeom
LDFLAGS = -Llib
LIBS = -leom
TARGET = main.bin

default:
	$(CC) main.c $(CFLAGS) $(LDFLAGS) $(LIBS) -o $(TARGET)
