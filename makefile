# Makefile

# 编译器和源文件目录
CC = gcc
CFLAGS = -I./include
LDFLAGS = -lcurl -ljson-c

# 源文件和目标文件
SRCS = $(wildcard src/*.c)
OBJS = $(patsubst src/%.c, obj/%.o, $(SRCS))
DEPS = $(wildcard include/*.h)

# 目标
all: $(OBJS)
	$(CC) -o bin/main $(OBJS) $(LDFLAGS)

# 规则
obj/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@ $(LDFLAGS)

# 清理目标
clean:
	rm -f $(OBJS) bin/main
