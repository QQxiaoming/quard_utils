/**
 * @file mdio.c
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
#include <unistd.h>
#include <linux/mii.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <linux/sockios.h>
#include <linux/types.h>
#include <netinet/in.h>

#define reteck(ret)                                                   \
    if (ret < 0) {                                                    \
        printf("%d %m! \"%s\" : line: %d\n",ret, __func__, __LINE__); \
        goto fail;                                                     \
    }

#define help()                                        \
    printf("mdio:\n");                                \
    printf("read operation: mdio reg_addr\n");        \
    printf("write operation: mdio reg_addr value\n"); \
    printf("For example:\n");                         \
    printf("\tmdio eth0 1\n");                        \
    printf("\tmdio eth0 0 0x12\n\n");                 \


int main(int argc, char *argv[])
{
    int sockfd;
    struct mii_ioctl_data *mii = NULL;
    struct ifreq ifr;
    int ret = -1;

    if (argc == 2 && !strcmp(argv[1], "-h")) {
        help();
        return 0;
    }

    if (!(argc == 3 || argc == 4)) {
        help();
        return -1;
    }

    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_name, argv[1], IFNAMSIZ - 1);

    sockfd = socket(PF_LOCAL, SOCK_DGRAM, 0);
    reteck(sockfd);

    //get phy address in smi bus
    ret = ioctl(sockfd, SIOCGMIIPHY, &ifr);
    reteck(ret);

    mii = (struct mii_ioctl_data *)&ifr.ifr_data;

    if (argc == 3) {
        mii->reg_num = (uint16_t)strtoul(argv[2], NULL, 0);
        ret = ioctl(sockfd, SIOCGMIIREG, &ifr);
        reteck(ret);
        printf("read phy addr: 0x%x  reg: 0x%x   value : 0x%x\n\n", mii->phy_id, mii->reg_num, mii->val_out);
    }
    else if (argc == 4) {
        mii->reg_num = (uint16_t)strtoul(argv[2], NULL, 0);
        mii->val_in = (uint16_t)strtoul(argv[3], NULL, 0);
        ret = ioctl(sockfd, SIOCSMIIREG, &ifr);
        reteck(ret);
        printf("write phy addr: 0x%x  reg: 0x%x  value : 0x%x\n\n", mii->phy_id, mii->reg_num, mii->val_in);
    }

    ret = 0;

fail:
    close(sockfd);
    return ret;
}