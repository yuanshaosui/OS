#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "file.h"
#include "IO.h"

/*主函数*/
int main()
{
    printf("***********************************************************************************\n");
    printf("* 欢迎来到yss的文件系统！！！                                                     *\n");
    printf("* 介绍：此文件系统是模拟Linux系统所制作，在这里你将体验到命令行的乐趣！           *\n");
    printf("*           格式化文件系统：     format                                           *\n");
    printf("*           退出文件系统：       quit                                             *\n");
    printf("*           显示目录内容：       directory                                        *\n");
    printf("*           创建文件：           create                                           *\n");
    printf("*           删除文件：           destroy                                          *\n");
    printf("*           打开文件：           open                                             *\n");
    printf("*           关闭文件：           close                                            *\n");
    printf("*           文件指针偏移：       lseek                                            *\n");
    printf("*           读文件：             read                                             *\n");
    printf("*           写文件：             write                                            *\n");
    printf("*           请求帮助：           help                                             *\n");
    printf("*           磁盘块使用情况：     sbm                                              *\n");
    printf("*           文件描述符使用情况： sim                                              *\n");
    printf("*           已打开文件列表：     sOF                                              *\n");
    printf("* 好了！现在请开始你的表演！！！                                                  *\n");
    printf("***********************************************************************************\n");
    char comm[30],filename[20];
    int i,quit=0,choice,index,number;
    Disk = fopen(DISK,"r+");//打开磁盘文件
    if(Disk == NULL)
    {
        printf("打开磁盘文件失败，请查看是否已创建disk.txt！\n");
    }
    init_fs();//初始化文件系统
    while(1)
    {
        printf("%s ",path);
        scanf("%s",comm);
        choice=-1;
        for(i=0; i<CommanNum; ++i)
        {
            if(strcmp(comm,command[i])==0)
            {
                choice=i;
                break;
            }
        }
        switch(choice)
        {
        /*格式化文件系统*/
        case 0:
            format_fs();
            printf("\n");
            break;
        /*退出文件系统*/
        case 1:
            quit=1;
            break;
        /*显示目录内容*/
        case 2:
            directory();
            printf("\n");
            break;
        /*创建文件*/
        case 3:
            printf("\t请输入文件名：");
            scanf("%s",filename);
            create(filename);
            printf("\n");
            break;
        /*删除文件*/
        case 4:
            printf("\t请输入文件名：");
            scanf("%s",filename);
            destroy(filename);
            printf("\n");
            break;
        /*打开文件*/
        case 5:
            printf("\t请输入文件名：");
            scanf("%s",filename);
            open_file(filename);
            printf("\n");
            break;
        /*关闭文件*/
        case 6:
            printf("\t请输入文件索引号：");
            scanf("%d",&index);
            close_file(index);
            printf("\n");
            break;
        /*关闭文件*/
        case 7:
            printf("\t请输入文件索引号：");
            scanf("%d",&index);
            printf("\t请输入指针偏移量：");
            scanf("%d",&number);
            lseek_file(index,number);
            printf("\n");
            break;
        /*读文件*/
        case 8:
            printf("\t请输入文件索引号：");
            scanf("%d",&index);
            printf("\t请输入读取文件字节数目：");
            scanf("%d",&number);
            read_file(index,number);//将数据从文件写入BUFF
            printf("\n");
            break;
        /*写文件*/
        case 9:
            printf("\t请输入文件索引号：");
            scanf("%d",&index);
            printf("\t请输入写入文件字节数目：");
            scanf("%d",&number);
            write_file(index,number);//将数据从BUFF写入文件
            printf("\n");
            break;
        /*请求帮助*/
        case 10:
            help();
            printf("\n");
            break;
        case 11:
            show_blk_map();
            printf("\n");
            break;
        case 12:
            show_inode_map();
            printf("\n");
            break;
        case 13:
            show_OpenFile();
            printf("\n");
            break;
        default:
            printf("%s command not found\n",comm);
        }
        if(quit) break;
    }
    close_fs();//关闭文件系统
    fclose(Disk);//关闭磁盘文件
    return 0;
}
