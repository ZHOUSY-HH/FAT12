#ifndef SHOW_H
#define SHOW_H
#include "Struct.h"
#include "define.h"
#include "Read_file.h"
#include <stdio.h>

int show_char(char *const data, int num)
{
    for (int i = 0; i < num; i++)
        printf("%c", data[i]);
}

/*
展示MBR
*/

int show_mbr(MBR *const data)
{
    show_char((*data).BS_OEMName, 8);
    printf("\n");
    char temp1[30] = "每扇区字节数： ";
    printf("%-s", temp1);
    printf("%hd\n", (*data).BPB_BytsPerSec);
    char temp2[30] = "每簇扇区数： ";
    printf("%s", temp2);
    printf("%hd\n", (*data).BPB_SecPerClus);
    char temp3[30] = "FAT表数目： ";
    printf("%s", temp3);
    printf("%hd\n", (*data).BPB_NumFATs);
    char temp4[30] = "根目录文件最大值： ";
    printf("%s", temp4);
    printf("%hd\n", (*data).BPB_RootEntCnt);
    char temp5[30] = "每个FAT表扇区数目： ";
    printf("%s", temp5);
    printf("%hd\n", (*data).BPB_TotSec16);
    return 0;
}

/*
展示文件名字
*/
int show_dirname(DIR *const dir)
{
    int i = 0;
    int count = 1;
    while (i < 8 && (*dir).DIR_NAME[i])
    {
        printf("%c", (*dir).DIR_NAME[i]);
        i++;
    }
    count += 8 - i;
    i = 8;
    if ((*dir).DIR_NAME[i] != ' ')
    {
        printf(".");
        while (i < 13 && (*dir).DIR_NAME[i])
        {
            printf("%c", (*dir).DIR_NAME[i]);
            i++;
        }
        count += 13 - i;
        while (count-- > 0)
            printf(" ");
        return 0;
    }
    for (int i = 0; i < 7; i++)
        printf(" ");
    return 1;
}

/*
展示目录项时间
*/
int show_dirtime(DIR *const dir)
{
    mytime temp;
    get_time1(&temp, (*dir).DIR_WrtData, (*dir).DIR_WrtTime);
    printf("%d/%d/%d %d:%d", temp.year, temp.month, temp.day, temp.hour, temp.min);
    return 0;
}

/*
展示目录项
*/
int show_dir(DIR *const dir)
{
    int temp = show_dirname(dir);
    show_dirtime(dir);
    if (!temp)
        printf(" %dB", (*dir).DIR_FileSize);
    printf("\n");
    return 0;
}

/*
记录目录项个数
*/
int count_dir(DIR *dir, int maxsize)
{
    int i, count = 0;
    while (i < maxsize)
    {
        if (dir[i].DIR_Attr == 0)
            break;
        if (dir[i].DIR_Attr != 1)
            count++;
        i++;
    }
    return count;
}

/*
遍历目录项
*/
int show_alldir(DIR *dir, int maxsize)
{
    int i = 0, count = 0;
    while (i < maxsize)
    {
        if (dir[i].DIR_Attr == 0)
            break;
        if (dir[i].DIR_Attr != 1)
        {
            if (dir[i].DIR_Attr != 0x27)
                show_dir(&dir[i]);
            count++;
        }
        i++;
    }
    return count;
}

/*
展示路径
*/
int show_nowpath(PATH *head,int count)
{
    if (head == NULL)
        return 1;
    if (head->next != NULL)
    {
        show_nowpath(head->next,count+1);
        printf("/");
        printf("%s", head->name);
    }
    if(count==1)
        printf("/");
    return 0;
}

/*
展示开头
*/
int show_base(PATH *path)
{
    printf("zsy ");
    show_nowpath(path,1);
    printf(":");
    fflush(stdout);
}

/*
展示文件内容
*/
int show_txt(FILE_BLOCK* file, int maxsize)
{
    int count = 1;
    while(file!=NULL)
    {
        for(int i=0; i<SIZE_BLOCK; i++)
        {
            if(count > maxsize)
                break;
            printf("%c",file->data[i]);
            count ++;
        }
        if(count > maxsize)
            break;
        file = file -> next;
    }
    return 0;
}

#endif