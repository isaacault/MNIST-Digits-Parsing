#include <stdlib.h>
#include <stdio.h>

#ifdef DEBUG
#define DEBUG_PRINT(x) printf x
#else
#define DEBUG_PRINT(x) do {} while (0)
#endif

int main(int argc, char* argv[])
{
    if (argc < 2) {
        printf("Must pass filename as argument\n");
        exit(0);
    }

    FILE *fp;

    DEBUG_PRINT(("Opening file %s\n", argv[1]));
    fp = fopen(argv[1], "r+");
    if( fp == NULL ) {
        perror("fopen");
        return(-1);
    }
    unsigned int buffer;

    fread(&buffer, sizeof(buffer), 1, fp);
    unsigned int magic_num = ((buffer>>24)&0xff) | // move byte 3 to byte 0
                            ((buffer<<8)&0xff0000) | // move byte 1 to byte 2
                            ((buffer>>8)&0xff00) | // move byte 2 to byte 1
                            ((buffer<<24)&0xff000000); // byte 0 to byte 3
    DEBUG_PRINT(("Read %d after changing endianness", magic_num));

    /*****************************************************************************
     * The MNIST data begins with a "magic number" which has now been read and 
     * swapped endianness. For the files they provide there are two cases with
     * this number:
     * 
     * Case 1:  magic_num == 2049. In this case the file being read is a label 
     *          file and there is only one more item in the header, the number
     *          of items in the file.
     * Case 2:  magic_num == 2051. In this case the file being read is an image
     *          file and there are 3 more items in the header, the number of
     *          images, number of rows, and number of columns.
     * 
     * **************************************************************************/

    unsigned int swapped;
    for (unsigned int i = 2048; i < magic_num; i++) {
        fread(&buffer, sizeof(buffer), 1, fp);
        swapped = ((buffer>>24)&0xff) | // move byte 3 to byte 0
                    ((buffer<<8)&0xff0000) | // move byte 1 to byte 2
                    ((buffer>>8)&0xff00) | // move byte 2 to byte 1
                    ((buffer<<24)&0xff000000); // byte 0 to byte 3 
        fprintf(stdout, "%d\n", swapped);
    }

    fclose(fp);


    return 0;
}