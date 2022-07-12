/**
 * @file translate.c
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
#include <unistd.h>
#include <assert.h>
#include <errno.h>
#include <stdint.h>
#include <string.h>

#define PAGEMAP_ENTRY 8
#define GET_BIT(X,Y) (X & ((uint64_t)1<<Y)) >> Y
#define GET_PFN(X) X & 0x7FFFFFFFFFFFFF

const int __endian_bit = 1;
#define is_bigendian() ( (*(char*)&__endian_bit) == 0 )

int i, c, pid, status;
unsigned long virt_addr; 
uint64_t read_val, file_offset, page_size;
char path_buf [0x100] = {};
FILE * f;
char *end;

int read_pagemap(char * path_buf, unsigned long virt_addr);

int main(int argc, char ** argv){
    if(argc!=3){
        printf("Argument number is not correct!\n pagemap PID VIRTUAL_ADDRESS\n");
        return -1;
    }
    if(!memcmp(argv[1],"self",sizeof("self"))){
        sprintf(path_buf, "/proc/self/pagemap");
        pid = -1;
    } else {
        pid = strtol(argv[1],&end, 10);
        if (end == argv[1] || *end != '\0' || pid<=0){ 
            printf("PID must be a positive number or 'self'\n");
            return -1;
        }
    }
    virt_addr = strtoll(argv[2], NULL, 16);
    if(pid!=-1)
        sprintf(path_buf, "/proc/%u/pagemap", pid);

    page_size = getpagesize();
    read_pagemap(path_buf, virt_addr);
    return 0;
}

int read_pagemap(char * path_buf, unsigned long virt_addr){
    printf("Big endian? %s\n", is_bigendian()?"true":"false");
    f = fopen(path_buf, "rb");
    if(!f){
        printf("Error! Cannot open %s\n", path_buf);
        return -1;
    }

    //Shifting by virt-addr-offset number of bytes
    //and multiplying by the size of an address (the size of an entry in pagemap file)
    file_offset = virt_addr / page_size * PAGEMAP_ENTRY;
    printf("Vaddr: 0x%lx, Page_size: %ld, Entry_size: %d\n", virt_addr, page_size, PAGEMAP_ENTRY);
    printf("Reading %s at 0x%llx\n", path_buf, (unsigned long long) file_offset);
    status = fseek(f, file_offset, SEEK_SET);
    if(status){
        perror("Failed to do fseek!");
        return -1;
    }
    errno = 0;
    read_val = 0;
    unsigned char c_buf[PAGEMAP_ENTRY];
    for(i=0; i < PAGEMAP_ENTRY; i++){
        c = getc(f);
        if(c==EOF){
            printf("\nReached end of the file\n");
            return 0;
        }
        if(is_bigendian())
            c_buf[i] = c;
        else
            c_buf[PAGEMAP_ENTRY - i - 1] = c;
        printf("[%d]0x%x ", i, c);
    }
    for(i=0; i < PAGEMAP_ENTRY; i++){
        //printf("%d ",c_buf[i]);
        read_val = (read_val << 8) + c_buf[i];
    }
    printf("\n");
    printf("Result: 0x%llx\n", (unsigned long long) read_val);
    if(GET_BIT(read_val, 63)) {
        uint64_t pfn = GET_PFN(read_val);
        printf("PFN: 0x%lx (0x%lx)\n", pfn, pfn * page_size + virt_addr % page_size);
    } else
        printf("Page not present\n");
    if(GET_BIT(read_val, 62))
        printf("Page swapped\n");
    fclose(f);
    return 0;
}
