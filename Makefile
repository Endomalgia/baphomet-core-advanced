# Baphomet

# Version
VERSION = 0.2

# Project Name
NAME = baphomet

# Includes and Libs
INCS = -Iinclude -Iinclude/baphomet -I/usr/local/include -I/usr/include/libxml2 -I/usr/include/freetype2 -I./
LIBS = -L/usr/local/lib64 -lxml2 -lglfw -lGL -lm -lcglm -lpcre -lpng -lsndfile -ldl -logg -lvorbis -lportaudio -lpthread -lSDL3 -lfreetype

# Flags
CFLAGS = ${INCS} -std=gnu17
LDFLAGS = ${LIBS} -Wall # LD_CONFIG Manually for SDL3

# Compiler & Platform/Architecture
CC = gcc
PLATFORM = $(shell uname -s)
ARCHITECTURE = $(shell uname -m)

# Directiories containing various types of file
BUILD_DIR := ./build

# Files to manipulate
SRC := baphomet.c src/ring.c src/gfx.c src/glad.c src/util.c src/aud.c
OBJ := $(SRC:%=$(BUILD_DIR)/%.o)

all: options ${SO} build

options:
	@echo Baphomet build options:
	@echo "CFLAGS	= ${CFLAGS}"
	@echo "LDFLAGS	= ${LDFLAGS}"
	@echo "CC	= ${CC}"
	@echo "PLATFORM = ${PLATFORM}"
	@echo "ARCHITECTURE = ${ARCHITECTURE}"

# For every .c.so in ${SO} compile its .c

${BUILD_DIR}/%.c.o: %.c
	mkdir -p $(dir $@)
	${CC} ${CFLAGS} -c $< -o $@

run:
	./${NAME}

build: ${OBJ}
	${CC} -o ${NAME} ${OBJ} ${LDFLAGS}

clean:
	rm -r ${BUILD_DIR}
	rm ${NAME}

.PHONY: all run options clean
