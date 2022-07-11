#!/usr/bin/env python
# -*- coding:utf-8 -*-

import os
import sys
import getopt

main_include = """
#include <stdio.h>

"""

main_str = """

void check_type(checktype1_t * test)
{
    printf(\"check %p\\n\",test);
}

int main()
{
    checktype1_t *test1;
    checktype2_t *test2;
    check_type(test1);
    check_type(test2);
    return 0;
}
"""
def main(argv):
    typesrc = ''
    try:
        opts, args = getopt.getopt(argv,"hs:",["typesrc="])
    except getopt.GetoptError:
        print ('test.py -s <typesrc>')
        sys.exit(2)
    for opt, arg in opts:
        if opt == '-h':
            print ('test.py -s <typesrc>')
            sys.exit()
        elif opt in ("-s", "--typesrc"):
            typesrc = arg
    try:
        os.remove("/tmp/main.c")
    except Exception as e:
        pass
    os.mknod("/tmp/main.c")
    fp = open("/tmp/main.c","w")
    fp.write(main_include)
    fp.write(typesrc)
    fp.write(main_str)
    fp.close()
    ret = os.system("gcc /tmp/main.c -Werror=incompatible-pointer-types -o /tmp/a.out")
    if(ret == 0):
        print('\x1b[32mpass\x1b[0m')

if __name__ == '__main__':
    main(sys.argv[1:])
    