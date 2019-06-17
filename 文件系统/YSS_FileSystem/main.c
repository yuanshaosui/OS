#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "file.h"
#include "IO.h"

/*������*/
int main()
{
    printf("***********************************************************************************\n");
    printf("* ��ӭ����yss���ļ�ϵͳ������                                                     *\n");
    printf("* ���ܣ����ļ�ϵͳ��ģ��Linuxϵͳ���������������㽫���鵽�����е���Ȥ��           *\n");
    printf("*           ��ʽ���ļ�ϵͳ��     format                                           *\n");
    printf("*           �˳��ļ�ϵͳ��       quit                                             *\n");
    printf("*           ��ʾĿ¼���ݣ�       directory                                        *\n");
    printf("*           �����ļ���           create                                           *\n");
    printf("*           ɾ���ļ���           destroy                                          *\n");
    printf("*           ���ļ���           open                                             *\n");
    printf("*           �ر��ļ���           close                                            *\n");
    printf("*           �ļ�ָ��ƫ�ƣ�       lseek                                            *\n");
    printf("*           ���ļ���             read                                             *\n");
    printf("*           д�ļ���             write                                            *\n");
    printf("*           ���������           help                                             *\n");
    printf("*           ���̿�ʹ�������     sbm                                              *\n");
    printf("*           �ļ�������ʹ������� sim                                              *\n");
    printf("*           �Ѵ��ļ��б�     sOF                                              *\n");
    printf("* ���ˣ������뿪ʼ��ı��ݣ�����                                                  *\n");
    printf("***********************************************************************************\n");
    char comm[30],filename[20];
    int i,quit=0,choice,index,number;
    Disk = fopen(DISK,"r+");//�򿪴����ļ�
    if(Disk == NULL)
    {
        printf("�򿪴����ļ�ʧ�ܣ���鿴�Ƿ��Ѵ���disk.txt��\n");
    }
    init_fs();//��ʼ���ļ�ϵͳ
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
        /*��ʽ���ļ�ϵͳ*/
        case 0:
            format_fs();
            printf("\n");
            break;
        /*�˳��ļ�ϵͳ*/
        case 1:
            quit=1;
            break;
        /*��ʾĿ¼����*/
        case 2:
            directory();
            printf("\n");
            break;
        /*�����ļ�*/
        case 3:
            printf("\t�������ļ�����");
            scanf("%s",filename);
            create(filename);
            printf("\n");
            break;
        /*ɾ���ļ�*/
        case 4:
            printf("\t�������ļ�����");
            scanf("%s",filename);
            destroy(filename);
            printf("\n");
            break;
        /*���ļ�*/
        case 5:
            printf("\t�������ļ�����");
            scanf("%s",filename);
            open_file(filename);
            printf("\n");
            break;
        /*�ر��ļ�*/
        case 6:
            printf("\t�������ļ������ţ�");
            scanf("%d",&index);
            close_file(index);
            printf("\n");
            break;
        /*�ر��ļ�*/
        case 7:
            printf("\t�������ļ������ţ�");
            scanf("%d",&index);
            printf("\t������ָ��ƫ������");
            scanf("%d",&number);
            lseek_file(index,number);
            printf("\n");
            break;
        /*���ļ�*/
        case 8:
            printf("\t�������ļ������ţ�");
            scanf("%d",&index);
            printf("\t�������ȡ�ļ��ֽ���Ŀ��");
            scanf("%d",&number);
            read_file(index,number);//�����ݴ��ļ�д��BUFF
            printf("\n");
            break;
        /*д�ļ�*/
        case 9:
            printf("\t�������ļ������ţ�");
            scanf("%d",&index);
            printf("\t������д���ļ��ֽ���Ŀ��");
            scanf("%d",&number);
            write_file(index,number);//�����ݴ�BUFFд���ļ�
            printf("\n");
            break;
        /*�������*/
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
    close_fs();//�ر��ļ�ϵͳ
    fclose(Disk);//�رմ����ļ�
    return 0;
}
