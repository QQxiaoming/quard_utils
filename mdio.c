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