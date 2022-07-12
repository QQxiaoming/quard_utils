/**
 * @file macgen.c
 * @author Quard (2014500726@smail.xtu.edu.cn)
 * @version 1.0
 * @date 2022-07-12
 * 
 * @copyright Copyright (c) 2022, Quard
 * 
 * All rights reserved.
 * 
 * SPDX-License-Identifier: BSD 3-Clause
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 */
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