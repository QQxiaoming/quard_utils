#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>

int main(int argc, char *argv[]) {
    unsigned int num = 1;
    bool config_oui = false;
    unsigned int oui[3];

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-n") == 0) {
            num = atoi(argv[i + 1]);
            if (num < 1) {
                printf("-n <NUM>: must be NUM > 1\n");
                return -1;
            }
            i++;
        } else if (strcmp(argv[i], "-oui") == 0) {
            int in = sscanf(argv[i + 1], "%x:%x:%x", &oui[0], &oui[1], &oui[2]);
            if (3 == in && oui[0] < 256 && oui[1] < 256 && oui[2] < 256) {
                config_oui = true;
            } else {
                printf("[-oui <XX:XX:XX>]: format err\n");
                return -1;
            }
            i++;
        } else if (strcmp(argv[i], "-h") == 0) {
            printf("Usage: macgen [-h] [-n <NUM>] [-oui <XX:XX:XX>]\n");
            return 0;
        } else {
            printf("Usage: macgen [-h] [-n <NUM>] [-oui <XX:XX:XX>]\n");
            return -1;
        }
    }

    srand(time(NULL) + getpid());
    
    for (; num > 0; num--) {
        for (unsigned char i = 0; i < 6; i++) {
            unsigned char tp = rand() % 256;
            if (i == 0)
                tp &= ~0x3;
            if (config_oui && i < 3)
                tp = (unsigned char)oui[i];
            printf("%s%X%s", tp < 16 ? "0" : "", tp, i < 5 ? ":" : "\n");
        }
    }

    return EXIT_SUCCESS;
}