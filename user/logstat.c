#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(void)
{
  struct logstats stats;
  if(logstat(&stats) < 0){
    printf("logstat failed\n");
    exit(1);
    }
   printf("Total commits: %d\n", stats.total_commits);
   printf("Data blocks bypassed: %d\n", stats.data_blocks_bypassed);
   printf("Torn commits prevented: %d\n", stats.torn_commits_prevented);
   
   exit(0);
 }
