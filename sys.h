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
                if (temp[i] != '.')
                {
                    filename[k] = temp[i];
                    k++;
                }
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
            while(temp[i]!=0&&temp[i]!=' ')
            {
                filename[k] = temp[i];
                k++;
                i++;
                if(k>=50)
                    return ORDER_ERROR;
            }
            filename[k] = ' ';
            k++;
            if(k>=50)
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

int exc_dir(FILE_BLOCK *now_file, char *rootdir)
{
    if (now_file == NULL)
        show_alldir((DIR *)rootdir, SIZE_ROOTDIRNUM);
    else
    {
        FILE_BLOCK *temp = now_file;
        while (temp != NULL)
        {
            show_alldir((DIR *)temp->data, SIZE_DIRNUM);
            temp = temp->next;
        }
    }
    return 0;
}

int error_cd()
{
    printf("文件名输入错误\n");
    fflush(stdout);
    return 0;
}
FILE_BLOCK *exc_cd(FILE *fp, FILE_BLOCK *now_file, char *rootdir, char name[50], const unsigned char *const fat)
{
    char name1[12];
    int count_zoro = 0;
    for (int i = 0; i < 12; i++)
    {
        name1[i] = name[i];
        if (name[i] == 0)
        {
            count_zoro += 1;
            break;
        }
    }
    if (count_zoro != 1)
    {
        error_cd();
        return 0;
    }
    if (now_file == NULL) //根目录
    {
        DIR *temp = find_dir((DIR *)rootdir, SIZE_ROOTDIRNUM, name1);
        if (temp == NULL || temp->DIR_Attr != 0x10)
        {
            error_cd();
            return now_file;
        }
        now_file = GET_FILE(fp, fat, temp->DIR_FstClus);
    }
    else //非根目录
    {
        FILE_BLOCK *temp = now_file;
        DIR *temp1 = NULL;
        while (temp != NULL)
        {
            temp1 = find_dir((DIR *)temp->data, SIZE_DIRNUM, name1);
            if (temp1 == NULL || temp1->DIR_Attr != 0x10)
            {
                temp = temp->next;
                continue;
            }
            break;
        }
        if (temp1 == NULL)
        {
            error_cd();
            return now_file;
        }
        temp = now_file;
        now_file = GET_FILE(fp, fat, temp1->DIR_FstClus);
        free_fileblock(temp);
    }
    return now_file;
}

#endif