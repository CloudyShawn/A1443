CC = gcc
CFLAGS = -Wall -Werror

all: write_blocks write_lines max_ave_seq_ram max_ave_seq_disk

write_blocks: write_blocks.c
	$(CC) -o $@ $^ $(CFLAGS)

write_lines: write_lines.c
	$(CC) -o $@ $^ $(CFLAGS)

max_ave_seq_ram: max_ave_seq_ram.c
	$(CC) -o $@ $^ $(CFLAGS)

max_ave_seq_disk: max_ave_seq_disk.c
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm write_lines write_blocks max_ave_seq_ram max_ave_seq_disk records.dat
