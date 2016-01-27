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

  //Block setup
  int block_size = atoi(argv[2]);
  int num_records_per_block = block_size / sizeof(Record);
  if(num_records_per_block * sizeof(Record) != block_size)
  {
    printf("Block size must be a multiple of %lu\n", sizeof(Record));
    return -1;
  }

  //Calculating size of file
  fseek(in_file, 0, SEEK_END);
  unsigned long file_size = ftell(in_file);
  fseek(in_file, 0, SEEK_SET);

  //Setting up randomizer and used blocks array to keep track of checked blocks
  int num_blocks = file_size / block_size;
  int checked_blocks[num_blocks];
  int total_checked_blocks = 0;
  int random_block = rand() % num_blocks;

  //Allocating record buffer for block size
  Record *buffer = (Record *)(calloc(num_records_per_block, sizeof(Record)));

  //Setting up variables for counting followers
  fseek(in_file, 0, file_size - sizeof(Record));
  Record last_user;
  fread(&last_user, sizeof(Record), 1, in_file);
  fseek(in_file, 0, SEEK_SET);
  int total_users = last_user.uid1;
  int user_following[total_users];

  //Start clock
  begin = clock();

  while(total_checked_blocks < num_blocks)
  {
    if(checked_blocks[random_block] == 1)
    {
      random_block = rand() % num_blocks;
    }
    else
    {
      fseek(in_file, 0, random_block * sizeof(Record) * num_records_per_block);
      int read_records = fread(buffer, sizeof(*buffer), num_records_per_block, in_file);
      total_records += read_records;

      int i;
      for(i = 0; i < read_records; i++)
      {
        user_following[buffer[i].uid1]++;
      }

      checked_blocks[random_block]++;
    }
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
  //printf("Average Follows: %li\nMax Follows: user %d, with %d follows\n", total_records / total_users, max_follows_user, max_follows);

  return 0;
}
