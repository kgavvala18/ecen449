#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
    unsigned int result, i_read, j_read;
    int fd;   // file descriptor
    int i, j; // loop variables
    char input = 0;
    unsigned int *inputs = (unsigned int *)malloc(2 * sizeof(int));
    char *outbuf = (char *)malloc(3 * sizeof(int));
    int *charToint; // used to convert from char* to int* buffer
    // open device file for reading and writing
    // user open to open 'dev/multiplier'/
    fd = open("/dev/multiplier", O_RDWR);

    // handle error opening file
    if (fd == -1)
    {
        printf("Failed to open device file!\n");
        return -1;
    }

    while (input != 'q')
    { // continue until user enters 'q'
        for (i = 0; i <= 16; i++)
        {
            for (j = 0; j <= 16; j++)
            {
                inputs[0] = i;
                inputs[1] = j;
                char *buffer = (char *)inputs; // convert to char* buffer
                // write the values to the device file

                write(fd, buffer, 2 * sizeof(int));
                read(fd, outbuf, 3 * sizeof(int));
                // read the result from the device file
                // convert the char* buffer to int* buffer
                charToint = (int *)outbuf;
                i_read = charToint[0];
                j_read = charToint[1];
                result = charToint[2];

                printf("%u * %u = %u\n", i_read, j_read, result);
                if (result == (i * j))
                {
                    printf("Result Correct!\n");
                }
                else
                {
                    printf("Result Incorrect!\n");
                }
            }
        }
        input = getchar();
    }
    close(fd);
    free(inputs);
    free(outbuf);
    return 0;
}