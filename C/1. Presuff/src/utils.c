#include "../include/presuff.h"

void print_help_msg()
{
  printf("Usage: presuff [options]\n");
  printf("Options:\n");
  printf("  -b               Prepend pattern to basenames\n");
  printf("  -d <directory>   Specify directory\n");
  printf("  -e               Append pattern to basenames\n");
  printf("  -h               Display this help message\n");
  printf("  -p <pattern>     Specify pattern to prepend/append\n");
  printf(
      "  -x <extension>   Specify extension of files to perform renaming on\n");
  printf("\n");
  printf("Example usage:\n");
  printf("  presuff -b -d /path/to/directory -p pat_ -x txt\n");
  printf("  presuff -e -p _pat\n");
  printf("\n");
}