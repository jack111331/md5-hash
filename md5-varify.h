/* MD5 header
    |****************************************************|
    |md5-varify.c                                        |
    |md5-varify.h                                        |
    |****************************************************|
*/
#ifndef MD5-HASH-LIB
#define MD5-HASH-LIB
#endif

//@param 1: the unsigned char type of pointer of data.
//the data which will be bound to hash into.
//@param 2: the unsigned int type of data length.
//the data length's [0] stores fore-32-bit length of data,and the data length's[1] stores back-32-bit length of data.
//@param 3: the unsigned char type of pointer of data.
//the result which is 16-bit string hash result.

void md5(unsigned char * data,unsigned int data_len[2],unsigned char * result);

