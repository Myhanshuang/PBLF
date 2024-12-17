#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DEBUG_ENCODE 0
#define DEBUG_DECODE 0

//base64查询表
char *base64_encodetable = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

//base64反查询表
char base64_decodetable[128] = {
        -1, -1, -1, -1, -1, -1, -1, -1, //0-7
        -1, -1, -1, -1, -1, -1, -1, -1, //8-15
        -1, -1, -1, -1, -1, -1, -1, -1, //16-23
        -1, -1, -1, -1, -1, -1, -1, -1, //24-31
        -1, -1, -1, -1, -1, -1, -1, -1, //32-39
        -1, -1, -1, 62, -1, -1, -1, 63, //40-47, + /
        52, 53, 54, 55, 56, 57, 58, 59, //48-55, 0-7
        60, 61, -1, -1, -1,  0, -1, -1, //56-63, 8-9
        -1,  0,  1,  2,  3,  4,  5,  6, //64-71, A-G
        7,  8,  9, 10, 11, 12, 13, 14, //72-79, H-O
        15, 16, 17, 18, 19, 20, 21, 22, //80-87, P-W
        23, 24, 25, -1, -1, -1, -1, -1, //88-95, X-Z
        -1, 26, 27, 28, 29, 30, 31, 32, //96-103, a-g
        33, 34, 35, 36, 37, 38, 39, 40, //104-111, h-o
        41, 42, 43, 44, 45, 46, 47, 48, //112-119, p-w
        49, 50, 51, -1, -1, -1, -1, -1, //120-127, x-z
};

//数字字符串表
static char *num_table = "0123456789";

//char类型数转换为2进制字符串格式, 如2->"10", 6->"110"
static char *char2binstr(char value)
{
    int i = 0;
    char binstr[9] = {};

    //取到每一位之后，查表得到对应的字符拼接成一个字符串
    for (i = 0; i < 8; i++)
        binstr[7 - i] = num_table[(value & (0x1 << i)) >> i];

    return strdup(binstr);
}

//2进制字符串格式数据转换为char类型数，如"10"->2, "110"->6
static char binstr2char(char *binstr)
{
    int i = 0;
    char value = 0;
    int length = 0;

    if (!binstr)
        return 0;

    length = strlen(binstr);

    //取得2进制字符串的每一个字节，将其转化对应的数字，然后还原数据。
    for (i = 0; i < length; i++)
        value += (binstr[length - 1 - i] - 0x30) << i;

    return value;
}

/*
 * Base64编码说明
 *  Base64编码要求把3个8位字节（3*8=24）转化为4个6位的字节（4*6=24），之后在6位的前面补两个0，形成8位一个字节的形式。
 *  如果剩下的字符不足3个字节，则用0填充，输出字符使用'='，因此编码后输出的文本末尾可能会出现1或2个'='。
 *  Base64制定了一个编码表，以便进行统一转换。编码表的大小为2^6=64，这也是Base64名称的由来。
 */
static char *_base64_section_encode(char *src, int length)
{
    int i = 0;
    char dest[5] = "====";
    char binstr[24] = "000000000000000000000000";
    char tmp[7] = {};
    char *tmp1 = NULL;

    //每字节8位，每次转换3字节，不足3字节时，高位补0
    //先拼接一个完整的24字节2进制字符串
    for (i = 0; i < length; i++) {
        tmp1 =  char2binstr(src[i]);
        strncpy(binstr + i * 8, tmp1, 8);
#if DEBUG_ENCODE
        printf("run here, %s, %d, src[%d]:%#hhx, tmp1:%s, binstr:%s\n", __func__, __LINE__, i, src[i], tmp1, binstr);
#endif
        free(tmp1);
    }

    //24字节，分4组，每组6字节，将每组转换为数字格式,每个数字查表得到对应码。
    //3种情况：
    // 1. 如果最后剩下1个数据，编码结果后加2个=，即查表2次，查表次数正好是数据字节数 + 1
    // 2. 如果最后剩下2个数据，编码结果后加1个=，即查表3次
    // 3. 如果没有剩下任何数据(剩下3个数据)，就什么都不要加,即查表4次
    for (i = 0; i < length + 1; i++) {
        strncpy(tmp, binstr + 6 * i, 6);
        dest[i] = base64_encodetable[binstr2char(tmp)];
#if DEBUG_ENCODE
        printf("run here, %s, %d, tmp:%s, dest:%s\n", __func__, __LINE__, tmp, dest);
#endif
    }

    return strdup(dest);
}

//输入原始内容和长度，得到base64编码
char *base64_encode(char *src, int length)
{
    int i = 0;
    char *base64code = NULL;
    int codelength = 0;
    char *tmp = NULL;

    //每3个字节一组转化为4字节（每6位一组得到4组，转换为新字符），不足3字节的填充到3字节进行转化。
    //因此计算共有多少组转换后的4字节乘以4就是编码后的长度
    codelength = ((length / 3) + (length % 3 > 0 ? 1 : 0)) * 4;
#if DEBUG_ENCODE
    printf("run here, %s, %d, length:%d, codelength:%d\n", __func__, __LINE__, length, codelength);
#endif
    base64code = malloc(codelength + 1);
    memset(base64code, 0, codelength + 1);

    for (i = 0; i < length / 3; i++) {
        tmp = _base64_section_encode(src + i * 3, 3);
        strcat(base64code, tmp);
#if DEBUG_ENCODE
        printf("run here, %s, %d, tmp:%s, base64code:%s\n", __func__, __LINE__, tmp, base64code);
#endif
        free(tmp);
    }

    if (length % 3) {
        tmp = _base64_section_encode(src + length - (length % 3), length % 3);
        strcat(base64code, tmp);
#if DEBUG_ENCODE
        printf("run here, %s, %d, tmp:%s, base64code:%s\n", __func__, __LINE__, tmp, base64code);
#endif
        free(tmp);
    }

#if DEBUG_ENCODE
    printf("run here, %s, %d, base64code:%s\n", __func__, __LINE__, base64code);
#endif
    return base64code;
}

//4字节反转化为3字节
static char *_base64_section_decode(char *dest)
{
    int i = 0;
    int j = 0;
    static char src[3] = {};
    char tmp[9] = {};
#if DEBUG_DECODE
    printf("run here, %s, %d, encode section:%s\n", __func__, __LINE__, dest);
#endif

    memset(src, 0, sizeof(src)/sizeof(src[0]));
    for (i = 0; i < 3; i++) {
        strncpy(tmp, dest + 8 * i, 8);
        src[i] = binstr2char(tmp);
#if DEBUG_DECODE
        printf("run here, %s, %d, tmp:%s, src:", __func__, __LINE__, tmp);
        for (j = 0; j < sizeof(src); j++) {
            printf("%#hhx ", src[j]);
        }
        printf(", tmp:%s\n", tmp);
#endif
    }

    return src;
}

int base64_decode_length(char *src)
{
    int length = 0;
    int padding_count = 0;

    if (src[strlen(src) - 1] == '=')
        padding_count++;

    if (src[strlen(src) - 2] == '=')
        padding_count++;

    length = strlen(src) / 4 * 3 - padding_count;
}

//输入base64编码后文本，解码出原始内容
char *base64_decode(char *src, int *destlen)
{
    int i = 0;
    int j = 0;
    char *binstr = NULL;
    int binstr_length = 0;
    char *tmp = NULL;
    char *tmp1 = NULL;
    char *dest = NULL;
    int cnt = 0;

#if DEBUG_DECODE
    printf("run here, %s, %d, src:%s\n", __func__, __LINE__, src);
#endif

    binstr_length = strlen(src) * 8 + 1;
    binstr = malloc(binstr_length);
    memset(binstr, 0, binstr_length);

    *destlen = base64_decode_length(src);
    dest = malloc(*destlen);
    memset(dest, 0, *destlen);
#if DEBUG_DECODE
    printf("run here, %s, %d, decode length:%d\n", __func__, __LINE__, *destlen);
#endif

    for(i = 0; i < strlen(src) / 4; i++) {
        for (j = 0; j < 4; j++) {
            tmp1 = char2binstr(base64_decodetable[src[i * 4 + j]]);
            strcat(binstr, tmp1 + 2);
#if DEBUG_DECODE
            printf("run here, %s, %d, tmp1:%s, binstr:%s\n", __func__, __LINE__, tmp1, binstr);
#endif
            free(tmp1);
        }
    }

    for (i = 0; i < strlen(src) / 4; i++) {
        //因为_base64_section_decode函数每次固定返回3字节数据，此处需要根据提前计算出来的解码总长度和已解码长度来判断最后1次需要拷贝的准确数据量。
        tmp = _base64_section_decode(binstr + i * 24);
        memmove(dest + i * 3, tmp, *destlen - cnt < 3 ? *destlen - cnt : 3);
        cnt += 3;
#if DEBUG_DECODE
        printf("run here, %s, %d, dest:", __func__, __LINE__);
        for (j = 0; j < *destlen; j++) {
            printf("%#hhx ", dest[j]);
        }
        printf("\n");
#endif
    }

    free(binstr);

    return dest;
}
