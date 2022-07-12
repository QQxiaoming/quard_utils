#! /bin/bash
# SPDX-License-Identifier: BSD 3-Clause

# will pass
echo "example1:"
python3 check_c_type.py -s "                        \
typedef int (*(*(*checktype1_t)(int *))[5])(int *); \
                                                    \
typedef int (*temp1_t)(int *);                      \
typedef temp1_t (*temp2_t)[5];                      \
typedef temp2_t (*checktype2_t)(int *);             \
"

# will no pass
echo "example2:"
python3 check_c_type.py -s "                        \
typedef int (*(*(*checktype1_t)(int *))[5])(int *); \
                                                    \
typedef int (*temp1_t)(int *);                      \
typedef temp1_t (*temp2_t)[5];                      \
typedef temp2_t (**checktype2_t)(int *);            \
"

# will pass
echo "example3:"
python3 check_c_type.py -s "                                         \
typedef int (*(***(**checktype1_t)(int (***)(int **)))[5])(int ***); \
                                                                     \
typedef int (*temp0_t)(int **);                                      \
typedef int (*temp1_t)(int ***);                                     \
typedef temp1_t (***temp2_t)[5];                                     \
typedef temp2_t (**checktype2_t)(temp0_t **);                        \
"
