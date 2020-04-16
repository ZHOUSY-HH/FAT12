#ifndef READ_FILE_H
#define READ_FILE_H
#include <stdio.h>
#include <stdlib.h>
#include "define.h"
#include "Struct.h"

//可以考虑判断有没有.来判断是否为根目录

/*
两个char类型转化为1个short
*/
short CHAR_SHORT(unsigned char *temp, int num)
{
    short data = temp[num + 1];
    data = data << 8;
    data += temp[num];
    return data;
}

/*
四个char类型转为为1个int
*/
int CHAR_INT(unsigned char *temp, int num)
{
    int data = 0;
    for (int i = 3; i >= 0; i--)
    {
        data = data << 8;
        data += temp[num + i];
    }
    return data;
}

/*
将char数组中的某一段复制到另一个char数组中
*/
int CHAR_CHAR(char *block, char *data, int num, int begin)
{
    for (int i = 0; i < num; i++)
        data[i] = block[i + begin];
    return 0;
}

/*
判断字符串是否相等
*/
int TEST_CHARE(char *const cmp1, char *const cmp2, int num)
{
    for (int i = 0; i < num; i++)
        if (cmp1[i] != cmp2[i])
            return 1;
    return 0;
}

FILE *Openfile()
{
    /*char filename[30];
    gets(filename);
    FILE* fp = fopen(filename, "rb");*/
    FILE *fp = fopen("dossys.img", "rb");
    if (fp == NULL)
    {
        printf("Open error!\n");
        exit(0);
    }
    return fp;
}

/*
读文件某一块
*/
unsigned char *BLOCK_READ(unsigned long long int number, FILE *fp)
{
    unsigned char *block = (unsigned char *)malloc(sizeof(char) * SIZE_BLOCK);
    number *= SIZE_BLOCK;
    fseek(fp, number, SEEK_SET);
    fread(block, sizeof(char), SIZE_BLOCK, fp);
    return block;
}

/*
写文件某一块
*/
int BLOCK_WRITE(unsigned long long int number, unsigned char *block, FILE *fp)
{
    number *= SIZE_BLOCK;
    fseek(fp, number, SEEK_SET);
    int count = fwrite(block, sizeof(char), SIZE_BLOCK, fp);
    return count != SIZE_BLOCK;
}

int INFO_MBR(MBR *temp, char *block)
{
    CHAR_CHAR(block, (*temp).BS_OEMName, 8, 3);                      //厂商名
    (*temp).BPB_BytsPerSec = CHAR_SHORT((unsigned char *)block, 11); //每扇区字节数
    (*temp).BPB_SecPerClus = block[13];                              //每簇扇区数
    (*temp).BPB_RsvdSecCnt = CHAR_SHORT((unsigned char *)block, 14); //Boot记录占用多少扇区
    (*temp).BPB_NumFATs = block[16];                                 //共有多少FAT表
    (*temp).BPB_RootEntCnt = CHAR_SHORT((unsigned char *)block, 17); //根目录文件数最大值
    (*temp).BPB_TotSec16 = CHAR_SHORT((unsigned char *)block, 19);   //扇区总数
    (*temp).BPB_Media = block[21];                                   //介质描述符
    (*temp).BPB_FATSz16 = CHAR_SHORT((unsigned char *)block, 22);    //每FAT扇区数
    (*temp).BPB_SecPerTrk = CHAR_SHORT((unsigned char *)block, 24);  //每磁道扇区数
    (*temp).BPB_NumHeads = CHAR_SHORT((unsigned char *)block, 26);   //磁头数,偏移26
    (*temp).BPB_HiddSec = CHAR_INT((unsigned char *)block, 28);      //隐藏扇区数
    (*temp).BPB_TotSec32 = CHAR_INT((unsigned char *)block, 32);     //值记录扇区数量
    (*temp).BS_DrvNum = block[36];                                   //中断13的驱动器号,偏移36
    (*temp).BS_Reserved1 = block[37];                                //未使用,偏移37
    (*temp).BS_BootSig = block[38];                                  //扩展引导标记,偏移38
    (*temp).BS_VolD = CHAR_INT((unsigned char *)block, 39);          //卷序列号,偏移39
    CHAR_CHAR(block, (*temp).BS_VolLab, 11, 43);                     //卷标
    CHAR_CHAR(block, (*temp).BS_FileSysType, 8, 54);                 //文件系统类型
    return 0;
}

//FAT表操作

/*
FAT表缓存
*/
char *GET_FAT(FILE *fp)
{
    unsigned int temp = SIZE_BLOCK * BLOCKNUM_FAT;
    char *fat = (char *)malloc(sizeof(char) * temp);
    fseek(fp, SIZE_BLOCK * BEGIN_FAT1, SEEK_SET);
    for (unsigned int i = 0; i < temp; i += SIZE_BLOCK)
        fread(&(fat[i]), sizeof(char), SIZE_BLOCK, fp);
    return fat;
}

/*
FAT簇号获得(0、1块不能改变,FF0表示坏块,FFF表示文件结束)
*/
short GET_FATCLUS(const unsigned char *const block, unsigned short num)
{
    unsigned short temp = num % 2;
    num = num / 2 * 3;
    if (temp)
    {
        temp = block[num + 2] << 4;
        temp += block[num + 1] >> 4;
    }
    else
    {
        temp = block[num + 1] & 0xf;
        temp = (temp << 8) + block[num];
    }
    return temp;
}

//根目录操作

/*
缓存根目录盘块
*/
char *GET_ROOTDIR(FILE *fp)
{
    unsigned int temp = SIZE_BLOCK * BLOCKNUM_DIR;
    char *dir = (char *)malloc(sizeof(char) * temp);
    fseek(fp, SIZE_BLOCK * BEGIN_DIR, SEEK_SET);
    for (unsigned int i = 0; i < temp; i += SIZE_BLOCK)
        fread(&(dir[i]), sizeof(char), SIZE_BLOCK, fp);
    return dir;
}

/*
天数转年月日
*/
int get_time1(mytime *data, unsigned short date, unsigned short time)
{
    (*data).year = (date & 0b1111111000000000) >> 9;
    (*data).year += 1980;
    (*data).month = (date & 0b0000000111100000) >> 5;
    (*data).day = date & 0b0000000000011111;
    (*data).hour = (time & 0b1111100000000000) >> 11;
    (*data).min = (time & 0b0000011111100000) >> 5;
    (*data).sec = time & 0b0000000000001111;
    return 0;
}

/*
根据首簇号打开用户文件
*/
FILE_BLOCK *GET_FILE(FILE *fp, const unsigned char *const fat, unsigned short first)
{
    unsigned short end = GET_FATCLUS(fat, 1);
    unsigned short now = GET_FATCLUS(fat, first);
    FILE_BLOCK *temp = NULL;
    FILE_BLOCK *head = NULL;
    temp = head = (FILE_BLOCK *)malloc(sizeof(FILE_BLOCK));
    head->data = BLOCK_READ(BEGIN_DATA + first - 2, fp);
    head->clus = first;
    head->next = NULL;
    while (now != end)
    {
        temp->next = (FILE_BLOCK *)malloc(sizeof(FILE_BLOCK));
        temp = temp->next;
        head->data = BLOCK_READ(BEGIN_DATA + now, fp);
        head->clus = now;
        head->next = NULL;
        now = GET_FATCLUS(fat, now);
    }
    return head;
}

#endif