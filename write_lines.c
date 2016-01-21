#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define KB 1024
#define MB (1024 * 1024)
#define OUTPUT_FILE_NAME "records.dat"
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
  if (argc != 2)
  {
    printf("Usage: write_lines <input filename>\n");
    return -1;
  }

  //Setting up arguments as variables
  char *file_name = argv[1];

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

  //Reads each line until EOF
  char read_line[LINE_MAX_LENGTH];
  char *split_line;
  Record *buffer = calloc(1, sizeof(Record));

  begin = clock();

  while(fgets(read_line, LINE_MAX_LENGTH, in_file) != NULL)
  {
    //Adds values to next location in buffer
    split_line = strtok(read_line, ",");
    buffer->uid1 = atoi(split_line);
    split_line = strtok(NULL, ",");
    buffer->uid2 = atoi(split_line);

    total_records++;

    fwrite(buffer, sizeof(Record), 1, out_file);
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
