#include <stdio.h>
#include <stdlib.h>
#include "show.h"
#include "Read_file.h"
#include "sys.h"

//隐藏文件标志为27
//文件名不允许为全为空格等
//目录项可以使用强制类型转化，MBR项目不行

int main()
{
    FILE *fp = Openfile(); //打开文件

    PATH *head = (PATH *)malloc(sizeof(PATH)); //路径头文件
    head->next = NULL;
    head->name[0] = 0; //初始化路径

    FILE_BLOCK *now_file = NULL;
    char *fat = GET_FAT(fp);
    FILE_BLOCK *rootdir = GET_ROOTDIR(fp);
    now_file = rootdir;

    char order[10];    //命令
    char par[20];      //参数
    char filename[50]; //文件名字
    while (order[0] != 'q' && order[0] != 'Q' || 1)
    {
        show_base(head);
        int temp1 = getorder(order, par, filename);
        if (temp1 == ORDER_ERROR)
        {
            printf("error\n");
            fflush(stdout);
            continue;
        }
        int temp = test(order);


        if (temp == ORDER_DIR)
            exc_dir(now_file);
        
        if (temp == ORDER_ERROR)
        {
            if (order[0] == 'q' || order[0] == 'Q')
                return 0;
            printf("error");
        }

        if (temp == ORDERC_CD)
        {
            now_file = exc_cd(fp,now_file,rootdir,filename,fat,&head);
        }

        if(temp == ORDER_INFO)
            exc_info(fp);
        
        if(temp == ORDER_TREE)
            exc_tree(fp,now_file,fat,1);
        if(temp == ORDER_R)
            exc_read(fp,fat,now_file,filename);
        
        if(temp == ORDER_R);
        printf("\n");
        fflush(stdout);
    }
    if(now_file != NULL)
        free_fileblock(now_file);
    if(rootdir != NULL)
        free_fileblock(rootdir);
    if(head != NULL)
        path_free(head);
    return 0;
}