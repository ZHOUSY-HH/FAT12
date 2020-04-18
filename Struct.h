#ifndef STRUCT_H
#define STRUCT_H
#include "define.h"
#include <stdio.h>
/*
注释
*/

typedef struct
{
    char BS_OEMName[8];            //厂商名,偏移3
    unsigned short BPB_BytsPerSec; //每扇区字节数,偏移11
    unsigned char BPB_SecPerClus;  //每簇扇区数,偏移13
    unsigned short BPB_RsvdSecCnt; //Boot记录占用多少扇区,偏移14
    unsigned char BPB_NumFATs;     //共有多少FAT表,偏移16
    unsigned short BPB_RootEntCnt; //根目录文件数最大值,偏移17
    unsigned short BPB_TotSec16;   //扇区总数,偏移19
    unsigned char BPB_Media;       //介质描述符,偏移21
    unsigned short BPB_FATSz16;    //每FAT扇区数,偏移22
    unsigned short BPB_SecPerTrk;  //每磁道扇区数,偏移24
    unsigned short BPB_NumHeads;   //磁头数,偏移26
    unsigned int BPB_HiddSec;      //隐藏扇区数,偏移28
    unsigned int BPB_TotSec32;     //值记录扇区数,偏移32
    unsigned char BS_DrvNum;       //中断13的驱动器号,偏移36
    unsigned char BS_Reserved1;    //未使用,偏移37
    unsigned char BS_BootSig;      //扩展引导标记,偏移38
    unsigned int BS_VolD;          //卷序列号,偏移39
    char BS_VolLab[11];            //卷标,偏移43
    char BS_FileSysType[8];        //文件系统类型,偏移54
    //偏移62开始为引导代码
} MBR;

/*
目录项
*/
typedef struct
{
    char DIR_NAME[11];          //文件名8字节，扩展名3字节，偏移0
    char DIR_Attr;              //文件属性，偏移11
    char Reserve[10];           //保留位，偏移12
    unsigned short DIR_WrtTime; //最后一次写入时间,偏移22，秒数
    unsigned short DIR_WrtData; //最后一次写入日期，偏移24，天数
    unsigned short DIR_FstClus; //文件开始的簇号，偏移26
    unsigned int DIR_FileSize;  //文件大小，偏移28
} DIR;

//时间结构体
typedef struct
{
    unsigned short year;
    unsigned short month;
    unsigned short day;
    unsigned short hour;
    unsigned short min;
    unsigned short sec;
} mytime;

//文件块结构体
typedef struct FILE_BLOCK
{
    char *data;
    unsigned short clus;
    struct FILE_BLOCK *next;
} FILE_BLOCK;
//释放文件块
FILE_BLOCK *free_fileblock(FILE_BLOCK *head)
{
    while (head != NULL)
    {
        FILE_BLOCK *temp = head->next;
        free(head);
        head = head->next;
    }
    return head;
}

//路径结构体
typedef struct PATH
{
    char name[LEATH_NAME+1];
    struct PATH *next;
} PATH;

/*
理解成进入下一级目录
*/
PATH *path_add(char *temp, PATH *head)
{
    PATH *now = (PATH *)malloc(sizeof(PATH));
    now->next = head;
    for (int i = 0; i < LEATH_NAME; i++)
        now->name[i] = temp[i];
    now -> name[LEATH_NAME+1] = 0;
    return now;
}
/*
理解成返回上一级目录
*/
PATH *path_sub(PATH *head)
{
    if(head ==NULL)
        return head;
    PATH *temp = head->next;
    free(head);
    return temp;
}
/*
理解成返回根目录
*/
PATH* path_return(PATH* head)
{
    if(head == NULL)
        return head;
    while(head != NULL)
    {
        PATH* temp = head;
        head = head ->next;
        free(temp);
    }
    return head;
}

/*
文件名结构体
*/
typedef struct FILE_NAME
{
    char name [13];
    struct FILE_NAME* next;
}FILE_NAME;
/*
释放空间
*/
FILE_NAME* FILE_NAME_free(FILE_NAME* file)
{
    while(file != NULL)
    {
        FILE_NAME* temp = file;
        file = file -> next;
        free(temp);
    }
    return file;
}

#endif