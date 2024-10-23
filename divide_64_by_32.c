#include <stdio.h>


void divide_64_by_32(unsigned int high, unsigned int low, unsigned int divisor, 
			unsigned int *quotient_l, unsigned int *quotient_h, unsigned int *remainder) {
    int i = 0;
	unsigned int temp_quotient = 0;
    unsigned int temp_remainder = 0;
	
	if (divisor == 0) {
        if(quotient_l) *quotient_l = 0;
        if(quotient_h) *quotient_h = 0;
        if(remainder) *remainder = 0xffffffffUL;
        return;
    }

    for (i = 0; i < 64; i++) {
        // Shift temp_remainder left by 1 bit
        temp_remainder = (temp_remainder << 1) | ((high >> 31) & 1);
        high = (high << 1) | ((low >> 31) & 1);
        low <<= 1;

        // If temp_remainder is greater than or equal to divisor, subtract divisor from temp_remainder
        if (temp_remainder >= divisor) {
            temp_remainder -= divisor;
            temp_quotient = (temp_quotient << 1) | 1;
        } else {
            temp_quotient <<= 1;
        }
        if(i == 31) {
            if(quotient_h) *quotient_h = temp_quotient;
        }
    }

    if(quotient_l) *quotient_l = temp_quotient;
    if(remainder) *remainder = temp_remainder;
}

int main() {
    unsigned int high = 0x12345678;
    unsigned int low = 0x87654321;
    unsigned int divisor = 0x1234;
    unsigned int quotient_l = 0;
    unsigned int quotient_h = 0;
    unsigned int remainder = 0;
    divide_64_by_32(high, low, divisor, &quotient_l, &quotient_h, &remainder);
    printf("quotient: 0x%x%08x\n",quotient_h,quotient_l);
    printf("remainder: 0x%x\n",remainder);

    unsigned long long int all = ((unsigned long long int)high<<32)|low;
    unsigned long long int quotient = 0;
    quotient = all / divisor;
    remainder = all % divisor;
    printf("quotient2: 0x%llx\n", quotient);
    printf("remainder2: 0x%x\n", remainder);

    return 0;
}