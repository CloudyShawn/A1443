CC = gcc
CFLAGS = -Wall -Werror

all: write_blocks write_lines

write_blocks: write_blocks.c
	$(CC) -o $@ $^ $(CFLAGS)

write_lines: write_lines.c
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm write_lines write_blocks
