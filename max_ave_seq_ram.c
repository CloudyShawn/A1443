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
    printf("Usage: write_blocks <input filename>\n");
    return -1;
  }

  //Setting up arguments as variables
  char *file_name = argv[1];

  //Opening file pointers for reading text and writing binary files
  FILE *in_file;
  if(!(in_file = fopen(file_name, "rb")))
  {
    printf("Could not open file %s for reading\n", file_name);
    return -1;
  }

  //Calculating size of file
  fseek(in_file, 0, SEEK_END);
  unsigned long file_size = ftell(in_file);
  fseek(in_file, 0, SEEK_SET);

  //Allocating record buffer for size of file
  total_records = file_size / sizeof(Record);
  Record *buffer = (Record *)(calloc(total_records, sizeof(Record)));

  //Variables for stats
  int curr_user = -1;
  int total_users = 0;
  int curr_follows = 0;
  int max_follows = -1;
  int max_follows_user = -1;

  //Read entire file into buffer
  fread(buffer, sizeof(Record), total_records, in_file);

  //Start clock
  begin = clock();

  int i;
  for(i = 0; i < total_records; i++)
  {
    if(curr_user != buffer[i].uid1)
    {
      if(curr_follows > max_follows)
      {
        max_follows = curr_follows;
        max_follows_user = curr_user;
        curr_follows = 0;
      }

      curr_user = buffer[i].uid1;
      total_users++;
    }

    curr_follows++;
  }

  //Final user # follows
  if(curr_follows > max_follows)
  {
    max_follows = curr_follows;
  }

  //Stop clock
  end = clock();

  //Closes all files opened
  fclose(in_file);

  //Frees malloc'd buffer space
  free(buffer);

  time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
  /* result in MB per second */
	printf ("Data rate: %.3f MBPS\n", ((total_records*sizeof(Record))/time_spent)/1000000);
  printf("Total Records: %li\n", total_records);
  printf("Average Follows: %li\nMax Follows: %d, with %d follows\n", total_records / total_users, max_follows_user, max_follows);

  return 0;
}
