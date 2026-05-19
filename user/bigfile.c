#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"

// 256 KB is the practical ceiling for a single xv6 file
// (MAXFILE = NDIRECT + NINDIRECT = 268 blocks).
#define BIG_SIZE       (256 * 1024) // 256 KB, the largest file size that fits under xv6's MAXFILE ceiling
#define BUF_SIZE       (64 * 1024) // matches filewrite()'s 64 KB chunk
#define VANILLA_CHUNK  (3 * 1024) // vanilla xv6: (MAXOPBLOCKS-4)/2 * BSIZE

int
main(int argc, char *argv[])
{
  static char buf[BUF_SIZE];   // buffer for writing to the file
  int fd, i;
  int iterations = BIG_SIZE / BUF_SIZE;
  int start, end;

  for (i = 0; i < BUF_SIZE; i++)
    buf[i] = 'X';

  printf("bigfile: creating a %d-byte file using %d-byte writes\n",
         BIG_SIZE, BUF_SIZE);

  unlink("big.dat");
  fd = open("big.dat", O_CREATE | O_WRONLY);
  if (fd < 0) {
    printf("bigfile: open failed\n");
    exit(1);
  }

  start = uptime();

  for (i = 0; i < iterations; i++) {
    if (write(fd, buf, BUF_SIZE) != BUF_SIZE) {
      printf("bigfile: write failed at iteration %d\n", i);
      close(fd);
      exit(1);
    }
  }

  end = uptime();
  close(fd);

  int vanilla = (BIG_SIZE + VANILLA_CHUNK - 1) / VANILLA_CHUNK;

  printf("bigfile: %d KB written in %d ticks using %d KB chunks\n",
         BIG_SIZE / 1024, end - start, BUF_SIZE / 1024);
  printf("bigfile: only %d transactions needed (vanilla xv6 would need ~%d)\n",
         iterations, vanilla);

  printf("bigfile: throughput = %d KB/tick\n", (BIG_SIZE / 1024) / (end - start));

  exit(0);
}
