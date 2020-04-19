#ifndef SYS_H
#define SYS_H
#include "Struct.h"
#include "define.h"
#include "show.h"
#include "Read_file.h"
#include <string.h>
#include <stdio.h>

int dir(char *data, int maxsize)
{
    return show_alldir((DIR *)data, maxsize);
}

/*
测试命令类型
*/
int test(char order[10])
{
    if ((order[0] == 'd' || order[0] == 'D') && (order[1] == 'i' || order[1] == 'I') && (order[2] == 'r' || order[2] == 'R'))
        return ORDER_DIR;
    if ((order[0] == 'c' || order[0] == 'C') && (order[1] == 'd' || order[1] == 'D') && order[2] == 0)
        return ORDERC_CD;
    if ((order[0] == 'i' || order[0] == 'I') && (order[1] == 'n' || order[1] == 'N') && (order[2] == 'f' || order[2] == 'F') && (order[3] == 'o' || order[3] == 'O') && order[4] == 0)
        return ORDER_INFO;
    if ((order[0] == 't' || order[0] == 'T') && (order[1] == 'r' || order[1] == 'R') && (order[2] == 'e' || order[2] == 'E') && (order[3] == 'e' || order[3] == 'E') && order[4] == 0)
        return ORDER_TREE;
    if (order[0] == 'r' && order[1] == 'R')
        return ORDER_R;
    return ORDER_ERROR;
}

int getorder(char order[10], char par[20], char filename[50])
{
    char temp[100];
    gets(temp); //读取一串指令
    int i = 0;
    if (temp[100] != 0)
        temp[100] = 0;
    order[0] = par[0] = filename[0] = 0;

    while (temp[i] != ' ' && temp[i] != 0 && i < 9) //获得指令
    {
        order[i] = temp[i];
        i++;
    }
    order[i] = 0;
    if (temp[i] == 0)
        return 0;
    i++;

    int mark = 0; //有参数时标志为1；
    int j = 0;
    int k = 0;
    while (temp[i] != 0)
    {
        if (temp[i] == '-')
        {
            i++;
            mark = 1;
        }
        if (mark)
        {
            while (temp[i] != 0 && temp[i] != ' ')
            {
                par[j] = temp[i];
                j++;
                i++;
                if (j >= 20)
                    return ORDER_ERROR;
            }
            if (temp[i] == 0)
                return 0;
            j++;
            mark = 0;
        }
        else if (temp[i] != '.')
        {
            while (temp[i] != 0 && temp[i] != ' ')
            {
                filename[k] = temp[i];
                k++;
                i++;
                if (k >= 50)
                    return ORDER_ERROR;
            }
            filename[k] = ' ';
            k++;
            if (k >= 50)
                return ORDER_ERROR;
        }
        else
        {
            while (temp[i] != 0 && temp[i] != ' ')
            {
                filename[k] = temp[i];
                k++;
                i++;
                if (k >= 50)
                    return ORDER_ERROR;
            }
            filename[k] = ' ';
            k++;
            if (k >= 50)
                return ORDER_ERROR;
        }
        if (temp[i] == ' ')
            i++;
    }
    if (j > 0)
        par[j - 1] = 0;
    if (k > 0)
        filename[k - 1] = 0;
    return 0;
}

/*
执行dir命令
*/
int exc_dir(FILE_BLOCK *now_file)
{
    FILE_BLOCK *temp = now_file;
    while (temp != NULL)
    {
        show_alldir((DIR *)temp->data, SIZE_DIRNUM);
        temp = temp->next;
    }
    return 0;
}

/*
执行cd指令
*/
int error_cd()
{
    printf("文件名输入错误\n");
    fflush(stdout);
    return 0;
}
FILE_BLOCK *exc_cd(FILE *fp, FILE_BLOCK *now_file, FILE_BLOCK *rootdir, char name[50], const unsigned char *const fat, PATH **head)
{
    int temp;
    return FIND_PATH(fp, fat, rootdir, now_file, name, 1, head, 0x10, &temp);
}

/*
执行info指令
*/
int exc_info(FILE *fp)
{
    MBR temp;
    char *block = BLOCK_READ(BEGIN_MBR, fp);
    INFO_MBR(&temp, block);
    free(block);
    show_mbr(&temp);
    return 0;
}

/*
执行tree指令
*/
int exc_tree(FILE *fp, FILE_BLOCK *now_file, char *fat, int count)
{
    int conti = 0;
    int k = 0;
    while (now_file != NULL)
    {
        //printf("%d\n",k);
        //fflush(stdout);
        for (int i = 0; i < SIZE_DIRNUM; i++)
        {
            DIR *temp = &((DIR *)now_file->data)[i];
            if (temp->DIR_Attr == 0)
            {
                if (now_file != NULL && count != 1)
                    free_fileblock(now_file);
                return 0;
            }
            if (temp->DIR_Attr == 1)
                continue;
            for (int j = 0; j < count; j++)
                printf("\t");
            show_dir(temp);
            char tempchar;
            if ((temp->DIR_NAME[0] == '.' && temp->DIR_NAME[1] == ' ' && temp->DIR_NAME[8] == ' ') || (temp->DIR_NAME[0] == '.' && temp->DIR_NAME[1] == '.' && temp->DIR_NAME[2] == ' ' && temp->DIR_NAME[8] == ' '))
                continue;
            if (temp->DIR_Attr == 0x10)
            {
                FILE_BLOCK *temp_file = GET_FILE(fp, fat, temp->DIR_FstClus);
                exc_tree(fp, temp_file, fat, count + 1);
            }
        }
        now_file = now_file->next;
        k++;
    }
    return 0;
}

/*
执行r指令
*/
int exc_read(FILE *fp, char *fat, FILE_BLOCK *now_file, char *filename[50])
{
    FILE_BLOCK *rootdir = NULL;
    PATH *path = NULL;
    int size;
    FILE_BLOCK *file = FIND_PATH(fp, fat, rootdir, now_file, filename, 0, path, 0x20, &size);
    if (file == now_file || file == NULL)
    {
        printf("errorfile\n");
        return 1;
    }
    show_txt(filename,size);
    return 0;
}

#endif