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
    return 0;
}

/*
展示根目录
*/
int show_rootdir(char* rootdir)
{
    
}

#endif