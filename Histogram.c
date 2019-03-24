#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <time.h>


static uint32_t img_size = 0x1200000;  //Assuming file size is exactly (12 x 4096 x 3072)/8 bytes
static int16_t bins = 32;


int main() {

    clock_t start = clock();

    int fd = open("img.raw12", O_RDONLY, S_IRUSR | S_IWUSR);

    int32_t *file_in_memory = mmap(NULL, img_size, PROT_READ, MAP_PRIVATE, fd, 0);


    int i,j;


    long long histogram[4][33]= {0};

    for (i=0;i<3072;i+=2) {
        for (j=0;j<512;j++){  //Odd Rows

            //8 pixels from 12bytes
            register int32_t byte1 = file_in_memory[i*1536 + j*3];
            register int32_t byte2 = file_in_memory[i*1536 + j*3 + 1];
            register int32_t byte3 = file_in_memory[i*1536 + j*3 + 2];


            //RED
            histogram[0][((((byte1 << 4) & 0xFF0) | ((byte1>>12)& 0xF)) /128)]++;
            histogram[0][((((byte1 >>20) & 0xFF0) | ((byte2>>4) & 0xF)) /128)]++;
            histogram[0][((((byte2 >> 12)& 0xFF0) | ((byte2>>28)& 0xF)) /128)]++;
            histogram[0][((((byte3 >> 4) & 0xFF0) | ((byte3>>20)& 0xF)) /128)]++;

            //GREEN
            histogram[1][((((byte1 & 0xF00)) | (((byte1) >> 16) & 0xFF)) /128)]++;
            histogram[1][((((byte2 << 8) & 0xF00) | ((byte2>>8) & 0xFF)) /128)]++;
            histogram[1][((((byte2 >> 16)& 0xF00) | (byte3 & 0xFF)) /128)]++;
            histogram[1][((((byte3 >> 8) & 0xF00) | ((byte3>>24)& 0xFF)) /128)]++;

        }

        for (j=0;j<512;j++){  //Even Rows
            register int32_t byte1 = file_in_memory[(i+1)*1536 + j*3];
            register int32_t byte2 = file_in_memory[(i+1)*1536 + j*3 + 1];
            register int32_t byte3 = file_in_memory[(i+1)*1536 + j*3 + 2];


            //GREEN 2
            histogram[2][((((byte1 << 4) & 0xFF0) | ((byte1>>12)  & 0xF)) /128)]++;
            histogram[2][((((byte1 >>20) & 0xFF0) | ((byte2>>4) & 0xF)) /128)]++;
            histogram[2][((((byte2 >> 12)& 0xFF0) | ((byte2>>28)& 0xF)) /128)]++;
            histogram[2][((((byte3 >> 4) & 0xFF0) | ((byte3>>20)& 0xF)) /128)]++;

            //BLUE
            histogram[3][((((byte1 & 0xF00)) | (((byte1) >> 16) & 0xFF)) /128)]++;
            histogram[3][((((byte2 << 8) & 0xF00) | ((byte2>>8) & 0xFF)) /128)]++;
            histogram[3][((((byte2 >> 16)& 0xF00) | (byte3 & 0xFF)) /128)]++;
            histogram[3][((((byte3 >> 8) & 0xF00) | ((byte3>>24)& 0xFF)) /128)]++;
        }

    }

    clock_t end = clock();

    for (i =0;i<bins;i++){
        printf("%d %lld %lld %lld %lld\n",i+1,histogram[0][i], histogram[1][i],histogram[2][i],histogram[3][i]);
    }

    printf("\nTime taken: %f",(double)(end-start)/CLOCKS_PER_SEC);

    return 0;
}
