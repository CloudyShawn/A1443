#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define KB 1024
#define MB (1024 * 1024)
#define OUTPUT_FILE_NAME "edges.dat"
#define LINE_MAX_LENGTH 255

typedef struct record
{
  int uid1;
  int uid2;
} Record;

int main(int argc, char *argv[])
{
  clock_t begin, end;
  double time_spent;
  long total_records = 0;

  //Checking for correct arguments
  if (argc != 3)
  {
    printf("Usage: write_blocks <input filename> <block size>\n");
    return -1;
  }

  //Setting up arguments as variables
  char *file_name = argv[1];
  int block_size = atoi(argv[2]);
  int num_records_per_block = block_size / sizeof(Record);
  if(num_records_per_block * sizeof(Record) != block_size)
  {
    printf("Block size must be a multiple of %lu\n", sizeof(Record));
    return -1;
  }

  //Opening file pointers for reading text and writing binary files
  FILE *in_file;
  FILE *out_file;
  if(!(in_file = fopen(file_name, "r")))
  {
    printf("Could not open file %s for reading\n", file_name);
    return -1;
  }
  if(!(out_file = fopen(OUTPUT_FILE_NAME, "wb")))
  {
    printf("Could not open file %s for writing\n", OUTPUT_FILE_NAME);
    return -1;
  }

  //Allocating record buffer for size of block
  Record *buffer = (Record *)(calloc(num_records_per_block, sizeof(Record)));
  int records_added = 0;

  //Reads each line until EOF
  char read_line[LINE_MAX_LENGTH];
  char *split_line;

  begin = clock();

  while(fgets(read_line, LINE_MAX_LENGTH, in_file) != NULL)
  {
    //Adds values to next location in buffer
    split_line = strtok(read_line, ",");
    buffer[records_added].uid1 = atoi(split_line);
    split_line = strtok(NULL, ",");
    buffer[records_added].uid2 = atoi(split_line);

    //Increment total Records in buffer
    records_added++;
    total_records++;

    //Writes to binary file if buffer is full
    if(records_added == num_records_per_block)
    {
      fwrite(buffer, sizeof(*buffer), records_added, out_file);
      records_added = 0;
    }
  }

  //Finishes writing left over buffer to file
  if(records_added > 0)
  {
    fwrite(buffer, sizeof(Record), records_added, out_file);
  }

  //Forces flush out of output stream buffer to disk
  fflush(out_file);

  end = clock();

  //Closes all files opened
  fclose(in_file);
  fclose(out_file);

  //Frees malloc'd buffer space
  free(buffer);

  time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
  /* result in MB per second */
	printf ("Data rate: %.3f MBPS\n", ((total_records*sizeof(Record))/time_spent)/1000000);
  printf("Total Records: %li\n", total_records);

  return 0;
}
