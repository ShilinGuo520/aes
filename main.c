#include "stdio.h"

unsigned char s_box[16][16] = {
    {0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76},
    {0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0},
    {0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15},
    {0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75},
    {0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84},
    {0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf},
    {0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8},
    {0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2},
    {0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73},
    {0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb},
    {0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79},
    {0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08},
    {0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a},
    {0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e},
    {0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf},
    {0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16}
};

void sub_bytes(unsigned char *data, int size)
{
    int i = 0;
    while(i < size) {
        data[i] = s_box[0xf & (data[i] >> 4)][0xf & (data[i])];
        i++;
    }
}

unsigned int sub_word(unsigned int data)
{
    unsigned int ret = 0;
    unsigned char c_data[4] = {0};
    int i;
    for(i = 0; i < 4; i++) {
        c_data[i] |= (unsigned char)(0xff & (data >> (i * 8)));
    }
    sub_bytes(c_data, 4);
    ret = c_data[0] | (c_data[1] << 8) | (c_data[2] << 16) | (c_data[3] << 24);
    printf("sub_word:0x%8x \n\r", ret);
    
    return ret;
}


void shift(unsigned char *data, int shift)
{
    int i = 0;
    unsigned char temp;
    unsigned char *raw = data;

    while(shift--) {
        temp = *raw ;
        i = 0;
        while(i < 4) {
           *raw = *(raw + 1);
           raw++;
           i++;
        }
        *(--raw) = temp;
        raw = data;
    }
}

unsigned int rot_word(unsigned int data)
{
    unsigned char temp;
    unsigned int ret;
    temp = (unsigned char)(0xff & (data >> 24));
    data = data << 8;
    data |= temp;
    ret = data;
    printf("rto_word: ret=0x%8x\n\r", ret);
    return ret;
}

void shift_rows(unsigned char *data)
{
    int i = 0;
    unsigned char *in = data;
    while(i < 4) {
        shift((in + (i * 4)), i);
        i++;
    }
}

unsigned char gf_2_8(unsigned char a, unsigned char b)
{
    unsigned char p = 0, i = 0, hbs = 0;

    for (i = 0; i < 8; i++) {
        if (b & 1) {
            p ^= a;
        }

        hbs = a & 0x80;

        a <<= 1;

        if (hbs) a ^= 0x1b; // 0000 0001 0001 1011
        b >>= 1;
    }

    return p;
}


void column(unsigned char *data)
{
    unsigned char s0,s1,s2,s3;
    unsigned char *in_data = data;

    printf("\n\r");
    printf("0x%2x 0x%2x 0x%2x 0x%2x \n\r", (*in_data), (*(in_data + 4)), (*(in_data + 8)), (*(in_data + 12)));
    s0 = gf_2_8(2, (*in_data)) ^ gf_2_8(3, (*(in_data + 4))) ^ gf_2_8(1, (*(in_data + 8))) ^ gf_2_8(1, (*(in_data + 12)));
    s1 = gf_2_8(1, (*in_data)) ^ gf_2_8(2, (*(in_data + 4))) ^ gf_2_8(3, (*(in_data + 8))) ^ gf_2_8(1, (*(in_data + 12)));
    s2 = gf_2_8(1, (*in_data)) ^ gf_2_8(1, (*(in_data + 4))) ^ gf_2_8(2, (*(in_data + 8))) ^ gf_2_8(3, (*(in_data + 12)));
    s3 = gf_2_8(3, (*in_data)) ^ gf_2_8(1, (*(in_data + 4))) ^ gf_2_8(1, (*(in_data + 8))) ^ gf_2_8(2, (*(in_data + 12)));

    *in_data = s0;
    *(in_data + 4) = s1;
    *(in_data + 8) = s2;
    *(in_data + 12) = s3;

    printf("0x%2x 0x%2x 0x%2x 0x%2x \n\r", (*in_data), (*(in_data + 4)), (*(in_data + 8)), (*(in_data + 12)));
}

void mix_columns(unsigned char *data)
{
    int i = 0;
    while(i < 4) {
        column((data + i));
        i++;
    }
}


void key_expansion(unsigned char *key, unsigned int *w, int n)
{
    unsigned char rcon[] = {0x00, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36};
    int i = 0;
    unsigned int temp;
    while (i < n) {
        w[i] = (key[4 * i] << 24) | (key[4 * i + 1] << 16) | (key[4 * i + 2] << 8) | ((key[4 * i + 3]) << 0);
        i++;
    }

    i = n;
    while (i < (11 * n)) {
        temp = w[i - 1];

        if ((i%4) == 0) {
            temp = sub_word(rot_word(temp)) ^ (((unsigned int)rcon[i/n]) << 24 );
        }

        w[i] = w[i - n] ^ temp;
        i++;
    }
}


void key_test()
{
    unsigned char key[16] = {0x2b, 0x7e, 0x15, 0x16, \
                             0x28, 0xae, 0xd2, 0xa6, \
                             0xab, 0xf7, 0x15, 0x88, \
                             0x09, 0xcf, 0x4f, 0x3c};

    unsigned int ex_key[44] = {0};
    int i;

    key_expansion(key, ex_key, 4);
    for (i = 0; i < 44; i = i + 4) {
        printf("0x%8x 0x%8x 0x%8x 0x%8x \r\n", ex_key[i], ex_key[i + 1], ex_key[i + 2], ex_key[i + 3]);
    }

}

void mix_test()
{
    unsigned char buf[] = {0xc9,0x01,0x02,0x03,0x6e,0x05,0x06,0x07,0x46,0x09,0x10,0x11,0xa6,0x13,0x14,0x15};
    unsigned char i = 0,j = 0;
    mix_columns(buf);
    while(i < 4) {
        j = 0;
        printf("\n\r");
        while(j < 4) {
            printf("0x%2x ",buf[i*4 + j]);
            j++;
        }
        i++;
    }
    printf("\n\r");
}


void shift_test(void)
{
    unsigned char buf[] = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x10,0x11,0x12,0x13,0x14,0x15};
    unsigned char i = 0,j = 0;
    shift_rows(buf);
    while(i < 4) {
       j = 0;
       printf("\n\r");
       while(j < 4) {
           printf("0x%2x ",buf[i*4 + j]);
           j++;
       }
       i++;
    }
    printf("\n\r");
}

int main()
{
    unsigned char i = 0, j =0;
    unsigned char input[16] = {0x19, 0xa0, 0x9a, 0xe9, \
                               0x3d, 0xf4, 0xc6, 0xf8, \
                               0xe3, 0xe2, 0x8d, 0x48, \
                               0xbe, 0x2b, 0x2a, 0x08};
    key_test();
    return 0;
    sub_bytes(input, 16);
    printf("\n\rsub_bytes\n\r");
    while(i < 4) {
        j = 0;
        printf("\n\r");
        while(j < 4) {
            printf("0x%2x ",input[i*4 + j]);
            j++;
        }
        i++;
    }

    shift_rows(input);
    i = 0;
    j = 0;
    printf("\n\rshift_rows\n\r");
    while(i < 4) {  
            j = 0;
            printf("\n\r");
            while(j < 4) {
                    printf("0x%2x ",input[i*4 + j]);
                    j++;
            }
            i++;
    }

    mix_columns(input);
    i = 0;
    j = 0;
    printf("\n\rmix_columns\n\r");
    while(i < 4) {
            j = 0;
            printf("\n\r");
            while(j < 4) {
                    printf("0x%2x ",input[i*4 + j]);
                    j++;
            }
            i++;
    }

    return 0;
}


