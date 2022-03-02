#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WIDTH 64
#define HEIGHT 64
#define BUF_SIZE 65536
#define META_SIZE 138

int main(int argc, char *argv[]) {
    FILE *fp;
    unsigned char buf[BUF_SIZE];
    fp = fopen(argv[1], "rb");
    printf("%p\n", fp);

    if (fp != NULL) {
        fread(buf, 1, META_SIZE, fp);
        for (int i = 0; i < META_SIZE; i++) {
            printf("%02x ", buf[i]);
        }

        switch (buf[1] << 8 | buf[0]) {
            case 0x4d42: {
                printf("\x1b[32m[%s]\x1b[0m\n", "BMP");

                int addr = 0;
                for (int size; (size = fread(buf, 1, BUF_SIZE, fp));) {
                    for (int i = 0; i < size; i += 4, addr++) {
                        int xpos = addr % WIDTH;
                        int ypos = addr / WIDTH;

                        printf("\x1b[%d;%dH", ypos / 2, xpos);
                        if (ypos % 2 == 0) 
                            printf("\x1b[48;2;%d;%d;%dm ",
                                buf[i],
                                buf[i + 1],
                                buf[i + 2]
                            );
                        else 
                            printf("\x1b[38;2;%d;%d;%dm▄",
                                buf[i + 2],
                                buf[i + 1],
                                buf[i]
                            );
                            
                        if ((xpos + 1) % WIDTH == 0)
                            printf("\x1b[0m\n");
                    }
                }
            } break;
        }

        fclose(fp);
    }
}