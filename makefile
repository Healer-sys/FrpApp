# 编译器和源文件目录
CC = gcc
CFLAGS = -I./include -Wall -Wextra
LDFLAGS = -lcurl -ljson-c

# 源文件和目标文件
SRCS = $(wildcard src/*.c)
OBJS = $(patsubst src/%.c, obj/%.o, $(SRCS))
DEPS = $(wildcard include/*.h)

# 目标目录
OBJDIR = obj
BINDIR = bin

# 目标
all: $(BINDIR)/main

$(BINDIR)/main: $(OBJS) | $(BINDIR)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)

# 创建目标目录
$(OBJDIR):
	mkdir -p $(OBJDIR)

$(BINDIR):
	mkdir -p $(BINDIR)

# 规则
$(OBJDIR)/%.o: src/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# DEBUG模式
debug: CFLAGS += -DDEBUG
debug: $(BINDIR)/main_debug

$(BINDIR)/main_debug: $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)

# GDB模式
gdb: CFLAGS += -g
gdb: $(BINDIR)/main_gdb

$(BINDIR)/main_gdb: $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)

# 清理目标
clean:
	rm -f $(OBJDIR)/*.o $(BINDIR)/*

.PHONY: all debug gdb clean
