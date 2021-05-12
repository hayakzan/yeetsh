CC = gcc
CFLAGS = -Wall -Wextra

SHELLNAME = yeetsh
CFLAGS_REL = -O3 -g
CFLAGS_DEB = -Og -g3 -fno-omit-frame-pointer
CFLAGS_SAN = -fsanitize=undefined -fsanitize=address -fno-omit-frame-pointer -g

SRCS = yeetsh.c util.c
HEADERS = util.h
FILES = $(SRCS) $(HEADERS)

$(SHELLNAME): $(FILES)
	$(CC) $(CFLAGS) $(CFLAGS_REL) $(SRCS) -o $(SHELLNAME)

debug: $(FILES)
	$(CC) $(CFLAGS) $(CFLAGS_DEB) $(SRCS) -o $(SHELLNAME)

asan: $(FILES)
	$(CC) $(CFLAGS) $(CFLAGS_SAN) $(SRCS) -o $(SHELLNAME)
