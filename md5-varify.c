#include <stdio.h>

int shift[64] = { 7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22 , 5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20 , 4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23 , 6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21 };
unsigned char PADDING[64] = {
  0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

unsigned int rotateleft(unsigned int x, unsigned int n);
void md5(unsigned char * data,unsigned int data_len[2],unsigned char * result);
unsigned char * md5_padding_message(unsigned char * message,unsigned char * padding);
unsigned char * break_to_uchar(unsigned char * original_data,unsigned char * breaked_data,int character);
void uchar_to_uint(unsigned char * original_data,unsigned int * breaked_data,size_t len);
unsigned int * uint_to_uchar(unsigned int * original_data,unsigned char * breaked_data);
void message_append_len(unsigned char * message,unsigned char * data,size_t len);
void message_append(unsigned char * message,unsigned char * data);
void uchar_memset(unsigned char * buf,int input,size_t len);
unsigned int F(unsigned int X,unsigned int Y,unsigned int Z);
unsigned int G(unsigned int X,unsigned int Y,unsigned int Z);
unsigned int H(unsigned int X,unsigned int Y,unsigned int Z);
unsigned int I(unsigned int X,unsigned int Y,unsigned int Z);
int main()
{
    unsigned char data[100000] = "fdfdfd";
    unsigned int number[2] = {0,6};//number[0]擺後面?? which first?
    unsigned char buf[16];
    md5(&data[0], number, &buf[0]);
    int counter;
    for(counter = 0;counter < 16;counter++)
    {
        printf("%02x",buf[counter]);
    }
    printf("\n");

    return 1;
}



void md5(unsigned char * message,unsigned int message_len[2],unsigned char * result)
{
    //initializing

     unsigned int k[64]={ 0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee, 0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501, 0x698098d8, 0x8b44f7af, 0xffff5bb1,
     0x895cd7be, 0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821, 0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa, 0xd62f105d, 0x02441453, 0xd8a1e681,
     0xe7d3fbc8, 0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed, 0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a, 0xfffa3942, 0x8771f681, 0x6d9d6122,
     0xfde5380c, 0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70, 0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05, 0xd9d4d039, 0xe6db99e5, 0x1fa27cf8,
     0xc4ac5665, 0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039, 0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1, 0x6fa87e4f, 0xfe2ce6e0, 0xa3014314,
     0x4e0811a1, 0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391};

/*
    int counter;
    printf("k:");
    for(counter = 0;counter < 64;counter++)
    {
        k[counter] = (unsigned int)(floor((double)4294967296*abs(sin(counter+1))));
        printf("0x%08X ",k[counter]);
    }
    printf("\n");
*/
    unsigned int h0 = 0x67452301;
    unsigned int h1 = 0xefcdab89;
    unsigned int h2 = 0x98badcfe;
    unsigned int h3 = 0x10325476;
    //padding
    unsigned char buf[8];
    uchar_memset(&buf[0], 0, 8);
    message_len[1] = (message_len[1] << 3) | (message_len[0] >> 29);
    message_len[0] <<= 3;
    uint_to_uchar(&message_len[1], &buf[0]);
    uint_to_uchar(&message_len[0], &buf[4]);

//--problem-start
//    unsigned char * message_ptr = message;
//    while(*message_ptr != 0)
//        message_ptr++;
//    *message_ptr = 0x80;
    unsigned char * message_ptr;
    message_ptr = md5_padding_message(message,&PADDING[0]);
//--problem-end
    message_append_len(message_ptr, &buf[0],8);
/*
    int counter;
    printf("message:");
    for(counter = 0;counter < 64;counter++)
    {
        printf("%u ",*(message+counter));
    }
    printf("\n");
*/
    while(*message)
    {
        //breaking message to block
        unsigned char needed_data[64];
        unsigned int needed_data_int[16];
        message = break_to_uchar(message, &needed_data[0], 64);
        uchar_to_uint(&needed_data[0], &needed_data_int[0],64);
/*
        int j;
        for(j = 0;j < 16;j++)
        {
            printf("%u ",needed_data_int[j]);
        }
        printf("\n");
*/
        //transforming
        unsigned int a = h0;
        unsigned int b = h1;
        unsigned int c = h2;
        unsigned int d = h3;
        int i;
        for(i = 0;i < 64;i++)
        {
            unsigned int f;
            unsigned int g;
            if(i >= 0 && i <= 15)
            {
                f = F(b,c,d);
//                printf("F:%u\n",f);
                g = i;
            }
            else if(i >= 16 && i <= 31)
            {
                f = G(b,c,d);
//                printf("G:%u\n",f);
                g = ((i * 5) + 1) % 16;
            }
            else if(i >= 32 && i <= 47)
            {
                f = H(b,c,d);
//                printf("H:%u\n",f);
                g = ((i * 3) + 5) % 16;
            }
            else if(i >= 48 && i <= 63)
            {
                f = I(b,c,d);
//                printf("I:%u\n",f);
                g = (i * 7) % 16;
            }
            unsigned int Dtemp = d;
            d = c;
            c = b;
            b = b + rotateleft(a+f+k[i]+needed_data_int[g], shift[i]);
            a = Dtemp;
//            printf("a=%u b=%u c=%u d=%u\n",a,b,c,d);
        }
        h0 += a;
        h1 += b;
        h2 += c;
        h3 += d;
    }
    unsigned char digest[16];
    unsigned char h0_char[5],h1_char[5],h2_char[5],h3_char[5];
    uint_to_uchar(&h0, &h0_char[0]);
    uint_to_uchar(&h1, &h1_char[0]);
    uint_to_uchar(&h2, &h2_char[0]);
    uint_to_uchar(&h3, &h3_char[0]);
    h0_char[4] = '\0';
    h1_char[4] = '\0';
    h2_char[4] = '\0';
    h3_char[4] = '\0';
    uchar_memset(&digest[0], 0, 16);
    message_append(&digest[0], &h0_char[0]);
    message_append(&digest[0], &h1_char[0]);
    message_append(&digest[0], &h2_char[0]);
    message_append(&digest[0], &h3_char[0]);
    message_append(result, &digest[0]);
}
unsigned int rotateleft(unsigned int x, unsigned int n)
{
    return (x << n) | (x >> (32 - n));// & ~(-1 << n)
}
unsigned char * md5_padding_message(unsigned char * message,unsigned char * padding)
{
    unsigned char * message_ptr = message;
    unsigned char * padding_ptr = padding;
    unsigned long long int len = 1;
    while(*message_ptr != 0)
    {
        message_ptr++;
        len++;
    }
    *message_ptr = *padding_ptr;
    message_ptr++;
    padding_ptr++;
    while(len % 64 != 56)
    {
        *message_ptr = *padding_ptr;
        message_ptr++;
        padding_ptr++;
        len++;
    }
    *message_ptr = '\0';
    return (message_ptr);
}
unsigned char * break_to_uchar(unsigned char * original_data,unsigned char * breaked_data,int character)
{
    int counter;
    for(counter = 0;counter < character;counter++)
    {
        *(breaked_data + counter) = *(original_data + counter);
    }
    return (original_data + (character*8));
}
void uchar_to_uint(unsigned char * uchar,unsigned int * uint,size_t len)
{
    //4個8 bit uchar存成1個32 bit uint
    //使用bitwise operating
    int counter_i,counter_j;
    for(counter_i = 0,counter_j = 0;counter_j < len;counter_i++,counter_j+=4)
//        *(uint + counter_i) = *((unsigned int *)uchar+counter_i);
        *(uint + counter_i) = (unsigned int)*(uchar+counter_j) | (unsigned int)(*(uchar + counter_j + 1) << 8) | (unsigned int)(*(uchar + counter_j + 2) << 16) | ((unsigned int)*(uchar + counter_j + 3) << 24);
}
unsigned int * uint_to_uchar(unsigned int * uint,unsigned char * uchar)
{
    //32 bit uint 存成4個8 bit uchar
//    unsigned int* uchar_group = (unsigned int*)uchar;
//    *uchar_group = *uint;
    *uchar = (unsigned char)((*uint) & 0xff);
    *(uchar + 1) = (unsigned char)(((*uint) >> 8) & 0xff);
    *(uchar + 2) = (unsigned char)(((*uint) >> 16) & 0xff);
    *(uchar + 3) = (unsigned char)(((*uint) >> 24) & 0xff);
    return (uint + (sizeof(unsigned int)*8));
}
void uchar_memset(unsigned char * buf,int input,size_t len)
{
    int counter;
    for(counter = 0;counter < len;counter++)
    {
        *(buf + counter) = input;
    }
}
void message_append_len(unsigned char * message, unsigned char * data, size_t len)
{
    unsigned char * message_ptr = message;
    unsigned char * data_ptr = data;
    int counter;
    for(counter = 0;counter < len;counter++)
    {
        *message_ptr = *data_ptr;
        message_ptr++;
        data_ptr++;
    }
    *message_ptr = '\0';
}
void message_append(unsigned char * message,unsigned char * data)
{
    unsigned char * message_ptr = message;
    unsigned char * data_ptr = data;
    while(*message_ptr != 0)
    {
        message_ptr++;
    }
    while(*data_ptr != '\0')
    {
        *message_ptr = *data_ptr;
        message_ptr++;
        data_ptr++;
    }
    *message_ptr = '\0';
}

unsigned int F(unsigned int X,unsigned int Y,unsigned int Z)
{
    return (X & Y) | (~X & Z);
}
unsigned int G(unsigned int X,unsigned int Y,unsigned int Z)
{
    return (X & Z) | (Y & ~Z);
}
unsigned int H(unsigned int X,unsigned int Y,unsigned int Z)
{
    return X ^ Y ^ Z;
}
unsigned int I(unsigned int X,unsigned int Y,unsigned int Z)
{
    return Y ^ (X | ~Z);
}
