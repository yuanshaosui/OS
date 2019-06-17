#ifndef IO_H_INCLUDED
#define IO_H_INCLUDED

#include <stdio.h>
/*自定义常量*/
#define C	                8//柱面总数
#define H	                8//磁头总数
#define B	                16//一个磁道的扇区数
#define L		            1024//扇区总数;L=C*H*B
#define DISK 		        "disk.txt"//磁盘文件
/*文件指针*/
FILE* Disk;
/*结构体*/
typedef struct
{
    char record[512];//每个扇区大小为512字节
} Sector; //扇区
/*模拟磁盘数组*/
Sector ldisk[C][H][B];//CHB 分别表示柱面号，磁头号和扇区号
/*IO函数*/
int FileToArray(void);//把文件内容恢复到数组
int ArrayToFile(void);//把数组ldisk 存储到文件
int read_block(int x, char *p, int strlen);//把逻辑块i的内容读入到指针p指向的内存位置，拷贝的字符个数为strlen
int write_block(int i, char *p, int strlen);//把指针p指向的内容写入逻辑块i，拷贝的字符个数为strlen
void intTOchar4(int x,char p[4]);//int类型转换为char[4]
int char4TOint(char p[4]);//char[4]转换为int


int FileToArray()
{
    int i,j,k;
    fseek(Disk,0,SEEK_SET);
    for(i=0; i<C; i++)
    {
        for(j=0; j<H; j++)
        {
            for(k=0; k<B; k++)
            {
                if(fread(&ldisk[i][j][k],512,1,Disk)!=1)
                {
                    printf("文件内容恢复到数组失败！\n");
                    k=B;
                    j=H;
                    i=C;//退出
                }
            }
        }
    }
    return 1;
}
int ArrayToFile()
{
    int i,j,k;
    fseek(Disk,0,SEEK_SET);
    for(i=0; i<C; i++)
    {
        for(j=0; j<H; j++)
        {
            for(k=0; k<B; k++)
            {
                if(fwrite(&ldisk[i][j][k],512,1,Disk)!=1)
                {
                    printf("数组存储到文件失败！\n");
                    k=B;
                    j=H;
                    i=C;//退出
                }
            }
        }
    }
    return 1;
}
int read_block(int x, char *p, int strlen)
{
    int i,j,k;
    i=x/(H*B);
    j=(x-i*H*B)/B;
    k=(x-i*H*B-j*B);
    strncpy(p, &ldisk[i][j][k], strlen);
    return 0;
}
int write_block(int x, char *p, int strlen)
{
    int i,j,k;
    i=x/(H*B);
    j=(x-i*H*B)/B;
    k=(x-i*H*B-j*B);
    strncpy(&ldisk[i][j][k], p, strlen);
    return 0;
}
void intTOchar4(int x,char p[4])
{
    int i=0;
    i=x%10;
    switch(i)
    {
    case 0:
        p[0]='0';
        break;
    case 1:
        p[0]='1';
        break;
    case 2:
        p[0]='2';
        break;
    case 3:
        p[0]='3';
        break;
    case 4:
        p[0]='4';
        break;
    case 5:
        p[0]='5';
        break;
    case 6:
        p[0]='6';
        break;
    case 7:
        p[0]='7';
        break;
    case 8:
        p[0]='8';
        break;
    case 9:
        p[0]='9';
        break;
    default:
        break;
    }
    x/=10;
    i=x%10;
    switch(i)
    {
    case 0:
        p[1]='0';
        break;
    case 1:
        p[1]='1';
        break;
    case 2:
        p[1]='2';
        break;
    case 3:
        p[1]='3';
        break;
    case 4:
        p[1]='4';
        break;
    case 5:
        p[1]='5';
        break;
    case 6:
        p[1]='6';
        break;
    case 7:
        p[1]='7';
        break;
    case 8:
        p[1]='8';
        break;
    case 9:
        p[1]='9';
        break;
    default:
        break;
    }
    x/=10;
    i=x%10;
    switch(i)
    {
    case 0:
        p[2]='0';
        break;
    case 1:
        p[2]='1';
        break;
    case 2:
        p[2]='2';
        break;
    case 3:
        p[2]='3';
        break;
    case 4:
        p[2]='4';
        break;
    case 5:
        p[2]='5';
        break;
    case 6:
        p[2]='6';
        break;
    case 7:
        p[2]='7';
        break;
    case 8:
        p[2]='8';
        break;
    case 9:
        p[2]='9';
        break;
    default:
        break;
    }
    x/=10;
    i=x%10;
    switch(i)
    {
    case 0:
        p[3]='0';
        break;
    case 1:
        p[3]='1';
        break;
    case 2:
        p[3]='2';
        break;
    case 3:
        p[3]='3';
        break;
    case 4:
        p[3]='4';
        break;
    case 5:
        p[3]='5';
        break;
    case 6:
        p[3]='6';
        break;
    case 7:
        p[3]='7';
        break;
    case 8:
        p[3]='8';
        break;
    case 9:
        p[3]='9';
        break;
    default:
        break;
    }
}
int char4TOint(char p[4])
{
    int x=0;
    x=(((p[3]-48)*10+(p[2]-48))*10+(p[1]-48))*10+p[0]-48;
    return x;
}
#endif // IO_H_INCLUDED
