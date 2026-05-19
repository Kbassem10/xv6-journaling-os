#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"

// 256 KB: largest size that fits under xv6's MAXFILE ceiling
// (NDIRECT + NINDIRECT = 268 blocks). Do not raise past ~268 KB.
#define FILE_SIZE  (256 * 1024)
#define CHUNK_SIZE 1024
#define ITERATIONS (FILE_SIZE / CHUNK_SIZE)

int
main(int argc, char *argv[])
{
  char buf[CHUNK_SIZE];
  int fd, i;
  int start, end;

  for (i = 0; i < CHUNK_SIZE; i++)
    buf[i] = 'A' + (i % 26);

  printf("speedtest: writing %d bytes in %d-byte chunks\n",
         FILE_SIZE, CHUNK_SIZE);

  unlink("speedtest.dat");

  fd = open("speedtest.dat", O_CREATE | O_WRONLY);
  if (fd < 0) {
    printf("speedtest: cannot open file\n");
    exit(1);
  }

  start = uptime();

  for (i = 0; i < ITERATIONS; i++) {
    if (write(fd, buf, CHUNK_SIZE) != CHUNK_SIZE) {
      printf("speedtest: write failed at iteration %d\n", i);
      close(fd);
      exit(1);
    }
  }

  end = uptime();
  close(fd);

  int ticks = end - start;
  if (ticks == 0)
    ticks = 1;

  printf("speedtest: wrote %d KB in %d ticks\n", FILE_SIZE / 1024, ticks);
  printf("speedtest: throughput = %d KB/tick\n", (FILE_SIZE / 1024) / ticks);

  exit(0);
}
