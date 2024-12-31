# Compiler and Flags
CC = gcc
CFLAGS = -O3 -ftree-vectorize -ffast-math \
        -funsafe-math-optimizations -funroll-loops 
PROFILE_CFLAGS = -Wall -Wextra -Werror -g3 -O0 -pg -fno-omit-frame-pointer 

INCLUDES = -I/usr/local/include
LIBS = `sdl2-config --libs` -lpthread

SRC_DIR = src
BUILD_DIR = build

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))
TARGET = pathtracer

.PHONY: all clean release profile info

all: $(TARGET)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -MMD -c $< -o $@

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^ $(LIBS)

-include $(OBJS:.o=.d)

clean:
	rm -rf $(BUILD_DIR) $(TARGET)
