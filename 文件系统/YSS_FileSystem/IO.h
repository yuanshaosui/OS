#ifndef IO_H_INCLUDED
#define IO_H_INCLUDED

#include <stdio.h>
/*�Զ��峣��*/
#define C	                8//��������
#define H	                8//��ͷ����
#define B	                16//һ���ŵ���������
#define L		            1024//��������;L=C*H*B
#define DISK 		        "disk.txt"//�����ļ�
/*�ļ�ָ��*/
FILE* Disk;
/*�ṹ��*/
typedef struct
{
    char record[512];//ÿ��������СΪ512�ֽ�
} Sector; //����
/*ģ���������*/
Sector ldisk[C][H][B];//CHB �ֱ��ʾ����ţ���ͷ�ź�������
/*IO����*/
int FileToArray(void);//���ļ����ݻָ�������
int ArrayToFile(void);//������ldisk �洢���ļ�
int read_block(int x, char *p, int strlen);//���߼���i�����ݶ��뵽ָ��pָ����ڴ�λ�ã��������ַ�����Ϊstrlen
int write_block(int i, char *p, int strlen);//��ָ��pָ�������д���߼���i���������ַ�����Ϊstrlen
void intTOchar4(int x,char p[4]);//int����ת��Ϊchar[4]
int char4TOint(char p[4]);//char[4]ת��Ϊint


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
                    printf("�ļ����ݻָ�������ʧ�ܣ�\n");
                    k=B;
                    j=H;
                    i=C;//�˳�
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
                    printf("����洢���ļ�ʧ�ܣ�\n");
                    k=B;
                    j=H;
                    i=C;//�˳�
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
