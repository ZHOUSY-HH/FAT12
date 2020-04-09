#ifndef SHOW_H
#define SHOW_H
#include "Struct.h"
#include "define.h"
#include <stdio.h>

int show_char(char* const data, int num)
{
    for(int i=0; i<num; i++)
        printf("%c", data[i]);
}

/*
展示MBR
*/

int show_mbr(MBR* const data)
{
    show_char((*data).BS_OEMName,8);
    printf("\n");
    printf("%hd\n",(*data).BPB_BytsPerSec);
    printf("%hd\n",(*data).BPB_SecPerClus);
    printf("%hd\n",(*data).BPB_RsvdSecCnt);
    printf("%hd\n",(*data).BPB_NumFATs);
    printf("%hd\n",(*data).BPB_RootEntCnt);
    printf("%hd\n",(*data).BPB_TotSec16);
    return 0;
}

/*
展示文件名字
*/
int show_dirname(DIR* const dir)
{
    int i=0;
    while(i<8&&(*dir).DIR_NAME[i])
    {
        printf("%c",(*dir).DIR_NAME[i]);
        i++;
    }
    printf(".");
    while(i<13&&(*dir).DIR_NAME[i])
    {
        printf("%c",(*dir).DIR_NAME[i]);
        i++;
    }
    return 0;
}

/*
展示目录项时间
*/
int show_dirtime(DIR* const dir)
{
    
    return 0;
}

/*
展示目录项
*/
int show_rootdir(DIR* const dir)
{
    show_dirname(dir);
    printf("\n");
}

#endif