#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"

// Set chunk size to 1KB (matches xv6 block size)
#define CHUNK_SIZE 1024
// Set total size to 250KB (Safely under the vanilla 268KB limit)
#define TOTAL_SIZE (250 * 1024)

int main(void) {
    int fd;
    int i;
    uint start_time, end_time;
    char buf[CHUNK_SIZE];

    // Fill the buffer with dummy 'X' characters
    memset(buf, 'X', CHUNK_SIZE);

    printf("Starting Vanilla xv6 Big File Test...\n");
    printf("Target size: 250 KB (Writing in 1 KB chunks)\n\n");

    // Start the stopwatch
    start_time = uptime();

    // Create the file
    fd = open("bigtest.txt", O_CREATE | O_WRONLY);
    if(fd < 0){
        printf("Error: Could not create file.\n");
        exit(1);
    }

    // Write the data chunk by chunk
    for(i = 0; i < (TOTAL_SIZE / CHUNK_SIZE); i++){
        if(write(fd, buf, CHUNK_SIZE) != CHUNK_SIZE){
            printf("Error: Write failed at chunk %d\n", i);
            close(fd);
            exit(1);
        }
        
        // Print progress so you can watch the system crawl
        if ((i + 1) % 50 == 0) {
            printf("Written %d KB...\n", (i + 1));
        }
    }
    close(fd);

    // Stop the stopwatch
    end_time = uptime();

    // Print final results
    printf("\n--- RESULTS ---\n");
    printf("Total size written: %d KB\n", TOTAL_SIZE / 1024);
    printf("Time taken: %d clock ticks\n", end_time - start_time);

    exit(0);
}