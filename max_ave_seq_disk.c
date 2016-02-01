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
  if (argc != 3)
  {
    printf("Usage: max_ave_seq_disk <input filename> <block size>\n");
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

  int block_size = atoi(argv[2]);
  int num_records_per_block = block_size / sizeof(Record);
  if(num_records_per_block * sizeof(Record) != block_size)
  {
    printf("Block size must be a multiple of %lu\n", sizeof(Record));
    return -1;
  }

  //Allocating record buffer for block size
  Record *buffer = (Record *)(calloc(num_records_per_block, sizeof(Record)));

  //Variables for stats
  int curr_user = -1;
  int total_users = 0;
  int curr_follows = 0;
  int max_follows = -1;
  int max_follows_user = -1;

  //Start clock
  begin = clock();

  int read_records = fread(buffer, sizeof(Record), num_records_per_block, in_file);
  while(read_records > 0)
  {
    total_records += read_records;

    int i;
    for(i = 0; i < read_records; i++)
    {
      if(curr_user != buffer[i].uid1)
      {
        if(curr_follows > max_follows)
        {
          max_follows = curr_follows;
          max_follows_user = curr_user;
        }

        curr_follows = 0;
        curr_user = buffer[i].uid1;
        total_users++;
      }

      curr_follows++;
    }
    read_records = fread(buffer, sizeof(*buffer), num_records_per_block, in_file);
  }

  //Final user # follows
  if(curr_follows > max_follows)
  {
    max_follows = curr_follows;
    max_follows_user = curr_user;
  }

  //Stop clock
  end = clock();

  //Closes all files opened
  fclose(in_file);

  //Frees malloc'd buffer space
  free(buffer);

  time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

  double avg_val = (double) total_records / total_users;
  /* result in MB per second */
	printf ("Data rate: %.3f MBPS\n", ((total_records*sizeof(Record))/time_spent)/1000000);
  printf("Total Records: %li\n", total_records);
  printf("Average Follows: %.3f\nMax Follows: user %d, with %d follows\n", avg_val, max_follows_user, max_follows);
  return 0;
}
