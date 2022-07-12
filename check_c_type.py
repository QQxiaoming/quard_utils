#!/usr/bin/env python3
# -*- coding:utf-8 -*-

# @file check_c_type.py
# @author Quard (2014500726@smail.xtu.edu.cn)
# @version 1.0
# @date 2022-07-12
# 
# @copyright Copyright (c) 2022, Quard
# 
# All rights reserved.
# 
# SPDX-License-Identifier: BSD 3-Clause
# 
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
# 
# 1. Redistributions of source code must retain the above copyright notice, this
#    list of conditions and the following disclaimer.
# 
# 2. Redistributions in binary form must reproduce the above copyright notice,
#    this list of conditions and the following disclaimer in the documentation
#    and/or other materials provided with the distribution.
# 
# 3. Neither the name of the copyright holder nor the names of its
#    contributors may be used to endorse or promote products derived from
#    this software without specific prior written permission.
# 
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

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
    