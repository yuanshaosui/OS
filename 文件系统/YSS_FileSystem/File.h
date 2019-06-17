#ifndef FILE_H_INCLUDED
#define FILE_H_INCLUDED

#include "IO.h"

/*�Զ��峣��*/
#define BlkNum		    512//���̿����Ŀ(��Ŵ�0��ʼ)
#define BlkSize		    1024//���̿��СΪ1K
#define InodeNum	    112//�ļ���������Ŀ
#define BlkPerNode	    4//ÿ���ļ����������Ĵ��̿���Ŀ
#define K	            4//������ռ���̿���Ŀ(0\1\2\3)
#define SuperBlkBeg	    0//��������ʼ���̿�(0)
#define InodeBeg	    1//�ļ���������ʼ���̿�(1\2\3)
#define BlockDirBeg	    4//Ŀ¼����ʼ���̿飨4/5/6/7��
#define BlockFileBeg	8//�ļ�����ʼ���̿�
#define MaxDirNum	    112//Ŀ¼��֧�ֵ������ļ���
#define CommanNum	    (sizeof(command)/sizeof(char*))//ָ����Ŀ
#define OpenFileMaximumNum  10//�ļ�ϵͳͬʱ���ļ������Ŀ
/*ָ���*/
char* command[]= {"format","quit","directory","create","destroy","open","close","lseek","read","write","help","sbm","sim","sOF"};
/*·����*/
char path[20]="file_system(yss)$ ";//�ļ�ϵͳ��yss Ԭ���棩
/*��д������*/
char buffer[5000];
/*�ṹ��*/
typedef struct
{
    char  blk_map[BlkNum];//���̿�λͼ
    char  inode_map[InodeNum];//�ļ�������λͼ
    char  blk_used[4];//�ѱ�ʹ�õĴ��̿���Ŀ
    char  inode_used[4];//�ѱ�ʹ�õ��ļ���������Ŀ
} SuperBlk;//�����飨��������
typedef struct
{
    char  blk_identifier[BlkPerNode][4];//ռ�õĴ��̿���
    char  blk_num[4];//ռ�õĴ��̿���Ŀ
    char  file_size[4];//�ļ��Ĵ�С
} Inode;//�ļ�������
typedef struct
{
    char  name[20];//�ļ���
    char  inode_num[4];//�ļ����������
} Dir;//Ŀ¼
typedef struct
{
    int  pointer1;//��дָ��(ѡ���̿飩
    int  pointer2;//��дָ��(�̿���ƫ�ƣ�
    char  name[30];//�ļ���
    int  inode_num;//�ļ����������
} OpenFile;//�Ѵ��ļ�
/*�Զ������*/
SuperBlk	super_blk;//�ļ�ϵͳ�ĳ�����
Inode 	record_inode[InodeNum];//�ļ�������ʹ�����
Dir 	dir_table[MaxDirNum];//Ŀ¼ʹ�����
OpenFile    OpenFile_table[OpenFileMaximumNum];//�Ѵ��ļ�
int OpenFileNum=0;//�Ѵ��ļ���Ŀ
/*�Ӻ���*/
void init_Dick();//��ʼ������
int init_fs();//��ʼ���ļ�ϵͳ
int close_fs();//�ر��ļ�ϵͳ
int format_fs();//��ʽ���ļ�ϵͳ
int create(char *filename);//����ָ�����ļ����������ļ�
int destroy(char *filename);//ɾ��ָ���ļ�
int open_file(char *filename);//���ļ����ú������ص������ſ����ں�����read, write, lseek,��close����
int close_file(int index);//�ر�ָ���ļ�
int read_file(int index, int number);//��ָ���ļ�˳�����number���ֽڵ���д������buffer�����������ļ��Ķ�дָ��ָʾ��λ�ÿ�ʼ
int write_file(int index, int number);//�Ѷ�д������buffer��number���ֽ�˳��д��ָ���ļ���д�������ļ��Ķ�дָ��ָʾ��λ�ÿ�ʼ
/*���ļ��Ķ�дָ���ƶ���posָ����λ�á�pos��һ����������ʾ���ļ���ʼλ�õ�ƫ�������ļ���ʱ����дָ���Զ�����Ϊ0��ÿ�ζ�д����֮��
��ָ����󱻷��ʵ��ֽڵ���һ��λ�á�lseek�ܹ��ڲ����ж�д����������¸ı��дָ����λ��*/
int lseek_file(int index, int pos);
int directory();//�б���ʾ�����ļ����䳤��
void help();//��������
void show_blk_map();//��ʾ���̿�ʹ�������0����δ��ʹ�ã�1������ʹ��
void show_inode_map();//��ʾ�ļ�������ʹ�������0����δ��ʹ�ã�1������ʹ��
void show_OpenFile();//��ʾ�Ѵ��ļ�

/*File����*/
void init_Dick()
{
    int i,j,k;
    for(i=0; i<C; i++)
    {
        for(j=0; j<H; j++)
        {
            for(k=0; k<B; k++)
            {
                memset(&ldisk[i][j][k], '#', 512);
            }
        }
    }
}
int init_fs()
{
    char judge;
    printf("    ������ǵ�һ��ʹ�ô��ļ�ϵͳ��������1���������������������\n");
    scanf("%c",&judge);
    if(judge=='1')
    {
        format_fs();
        printf("\n");
    }
    FileToArray();//���ļ����ݻָ�������
    //���������ݻָ��������ڴ���
    read_block(0,&super_blk,632);
    read_block(2,&record_inode,2688);
    read_block(8,&dir_table,4032);
    printf("\n");
    return 0;
}
int close_fs()
{
    //�ѳ����ڴ�������д��ģ���������
    write_block(0,&super_blk,632);
    write_block(2,&record_inode,2688);
    write_block(8,&dir_table,4032);
    ArrayToFile();//������ldisk �洢���ļ�
    return 0;
}
int format_fs()//��ʽ���ļ�ϵͳ
{
    init_Dick();////��ʼ����������
    char use[512];
    int i=0;
    memset(&super_blk.blk_map, '0', BlkNum);//��ʼ��������
    memset(&super_blk.inode_map, '0', InodeNum);
    super_blk.blk_map[0]='1';
    super_blk.blk_map[1]='1';
    super_blk.blk_map[2]='1';
    super_blk.blk_map[3]='1';
    super_blk.blk_map[4]='1';
    super_blk.blk_map[5]='1';
    super_blk.blk_map[6]='1';
    super_blk.blk_map[7]='1';
    intTOchar4(8,super_blk.blk_used);
    super_blk.inode_map[0]='1';
    intTOchar4(1,super_blk.inode_used);
    for(i=0; i<InodeNum; i++)//��ʼ���ļ�������ʹ�����
    {
        intTOchar4(BlkNum,record_inode[i].blk_identifier[0]);
        intTOchar4(BlkNum,record_inode[i].blk_identifier[1]);
        intTOchar4(BlkNum,record_inode[i].blk_identifier[2]);
        intTOchar4(BlkNum,record_inode[i].blk_identifier[3]);
        intTOchar4(0,record_inode[i].blk_num);
        intTOchar4(0,record_inode[i].file_size);
    }
    strncpy(dir_table[0].name,"yss_contents########",sizeof("yss_contents########")-1);//����Ŀ¼�ļ�
    intTOchar4(0,dir_table[0].inode_num);
    intTOchar4(0,record_inode[0].blk_num);
    intTOchar4(0,record_inode[0].file_size);
    super_blk.inode_map[0]='1';
    for(i=1; i<MaxDirNum; i++)//��ʼ��Ŀ¼ʹ�����
    {
        intTOchar4(InodeNum,dir_table[i].inode_num);
        strncpy(dir_table[i].name,"!!!!!!!!!!!!!!!!!!!!",sizeof("!!!!!!!!!!!!!!!!!!!!")-1);
    }
    write_block(0,&super_blk,632);
    write_block(2,&record_inode,2688);
    write_block(8,&dir_table,2688);
    memset(&use, ' ', 512);
    for(i=16; i<L; i++)
    {
        write_block(i,use,512);
    }
    //printf("%d\n",sizeof(super_blk));
    //printf("%d\n",sizeof(record_inode));
    //printf("%d\n",sizeof(dir_table));
    ArrayToFile();
    printf("    ��ʽ���ļ�ϵͳ�ɹ���\n");
    return 0;
}
int create(char *filename)//����ָ�����ļ����������ļ�
{
    int i;
    //printf("%d\n",strlen(filename));
    if(strlen(filename)>=20)
    {
        printf("    �ļ���������\n");
        return -1;
    }
    for(i=0; i<MaxDirNum; i++)
    {
        if(strncmp(dir_table[i].name, filename,strlen(filename))==0&&dir_table[i].name[strlen(filename)]=='!')
        {
            printf("    �ļ�ϵͳ���Ѵ��ڸ��ļ���������ļ������Ѵ����ļ�ɾ����\n");
            return -1;
        }
    }
    if(char4TOint(super_blk.inode_used)>=InodeNum)
    {
        printf("    �ļ�ϵͳ�д洢�ļ��Ѵ����ޣ�����ɾ��һЩ����Ҫ���ļ����ٴ������ļ���\n");
        return -1;
    }
    strncpy(dir_table[char4TOint(super_blk.inode_used)].name,filename,strlen(filename));
    for(i=1; i<InodeNum; i++)
    {
        if(super_blk.inode_map[i]=='0')
        {
            intTOchar4(i,dir_table[char4TOint(super_blk.inode_used)].inode_num);
            intTOchar4(0,record_inode[i].blk_num);
            intTOchar4(0,record_inode[i].file_size);
            super_blk.inode_map[i]='1';
            break;
        }
    }
    i=char4TOint(super_blk.inode_used)+1;
    intTOchar4(i,super_blk.inode_used);
    printf("    �ļ�%s�����ɹ���\n",filename);
    return 0;
}
int destroy(char *filename)//ɾ��ָ���ļ�
{
    int i,j,k;
    char use[512];
    for(i=0; i<OpenFileNum; i++)
    {
        if(strcmp(OpenFile_table[i].name, filename)==0)
        {
            printf("    %s�ļ��Ѵ򿪣����ȹر��ļ����ٽ���ɾ����\n",filename);
            return -1;
        }
    }
    for(i=1; i<MaxDirNum; i++)
    {
        if(strncmp(dir_table[i].name, filename,strlen(filename))==0&&dir_table[i].name[strlen(filename)]=='!')
        {
            strncpy(dir_table[i].name, "!!!!!!!!!!!!!!!!!!!!",sizeof("!!!!!!!!!!!!!!!!!!!!")-1);
            intTOchar4(0,record_inode[char4TOint(dir_table[i].inode_num)].file_size);
            if(char4TOint(record_inode[char4TOint(dir_table[i].inode_num)].blk_num)!=0)
            {
                memset(&use, ' ', 512);
                //printf("%d\n",char4TOint(record_inode[char4TOint(dir_table[i].inode_num)].blk_identifier[0]));
                write_block(char4TOint(record_inode[char4TOint(dir_table[i].inode_num)].blk_identifier[0])*2,use,512);
                write_block(char4TOint(record_inode[char4TOint(dir_table[i].inode_num)].blk_identifier[0])*2+1,use,512);
                write_block(char4TOint(record_inode[char4TOint(dir_table[i].inode_num)].blk_identifier[1])*2,use,512);
                write_block(char4TOint(record_inode[char4TOint(dir_table[i].inode_num)].blk_identifier[1])*2+1,use,512);
                write_block(char4TOint(record_inode[char4TOint(dir_table[i].inode_num)].blk_identifier[2])*2,use,512);
                write_block(char4TOint(record_inode[char4TOint(dir_table[i].inode_num)].blk_identifier[2])*2+1,use,512);
                write_block(char4TOint(record_inode[char4TOint(dir_table[i].inode_num)].blk_identifier[3])*2,use,512);
                write_block(char4TOint(record_inode[char4TOint(dir_table[i].inode_num)].blk_identifier[3])*2+1,use,512);
                k = char4TOint(super_blk.blk_used)-4;
                intTOchar4(k,super_blk.blk_used);
            }
            for(j=0; j<char4TOint(record_inode[char4TOint(dir_table[i].inode_num)].blk_num); j++)
            {
                super_blk.blk_map[char4TOint(record_inode[char4TOint(dir_table[i].inode_num)].blk_identifier[j])]='0';
                intTOchar4(BlkNum,record_inode[char4TOint(dir_table[i].inode_num)].blk_identifier[j]);
            }
            intTOchar4(0,record_inode[char4TOint(dir_table[i].inode_num)].blk_num);
            super_blk.inode_map[char4TOint(dir_table[i].inode_num)]='0';
            intTOchar4(InodeNum,dir_table[i].inode_num);
            i=char4TOint(super_blk.inode_used)-1;
            intTOchar4(i,super_blk.inode_used);

            printf("    %s�ļ���ɾ����\n",filename);
            break;
        }
    }
    if(i==MaxDirNum)
    {
        printf("    û����Ŀ¼���ҵ����ļ�����ȷ��������ļ�����ȷ��\n");
    }
    return 0;
}
int open_file(char *filename)//���ļ����ú������ص������ſ����ں�����read, write, lseek,��close����
{
    int i;
    if(OpenFileNum>OpenFileMaximumNum)
    {
        printf("    ��ʱ�Ѵ��ļ���Ŀ�ﵽ�ļ�ϵͳ���Ҫ����Ҫ�����򿪴��ļ������ȹر�һЩ����ʹ�õ��ļ���\n");
        return -1;
    }
    for(i=0; i<MaxDirNum; i++)
    {
        if(strncmp(dir_table[i].name, filename,strlen(filename))==0&&dir_table[i].name[strlen(filename)]=='!')
        {
            strcpy(OpenFile_table[OpenFileNum].name, filename);
            OpenFile_table[OpenFileNum].inode_num = char4TOint(dir_table[i].inode_num);
            OpenFileNum++;
            printf("    %s�ļ��Ѵ򿪣��ļ�������Ϊ%d��\n",filename,char4TOint(dir_table[i].inode_num));
            break;
        }
    }
    if(i==MaxDirNum)
    {
        printf("    û����Ŀ¼���ҵ����ļ�����ȷ��������ļ�����ȷ��\n");
    }
    return char4TOint(dir_table[i].inode_num);
}
int close_file(int index)//�ر�ָ���ļ�
{
    int i=0,j=0,judge=0;
    for(i=0; i<OpenFileNum; i++)
    {
        if(OpenFile_table[i].inode_num == index)
        {
            printf("    %s�ļ��ѹرա�\n",OpenFile_table[i].name);
            for(j=i; j<OpenFileNum-1; j++)
            {
                OpenFile_table[j].inode_num = OpenFile_table[j+1].inode_num;
                strcpy(OpenFile_table[j].name, OpenFile_table[j+1].name);
                OpenFile_table[j].pointer1 = OpenFile_table[j+1].pointer1;
                OpenFile_table[j].pointer2 = OpenFile_table[j+1].pointer2;
            }
            OpenFile_table[j].inode_num = InodeNum;
            strcpy(OpenFile_table[j].name, "\0");
            OpenFile_table[j].pointer1 = 0;
            OpenFile_table[j].pointer2 = 0;
            OpenFileNum--;
            judge = 1;
            break;
        }
    }
    if(judge==0)
    {
        printf("    û�����Ѵ��ļ����ҵ����ļ�����ȷ���������������ȷ��\n");
    }
    return 0;
}
/*���ļ��Ķ�дָ���ƶ���posָ����λ�á�pos��һ����������ʾ���ļ���ʼλ�õ�ƫ�������ļ���ʱ����дָ���Զ�����Ϊ0��ÿ�ζ�д����֮��
��ָ����󱻷��ʵ��ֽڵ���һ��λ�á�lseek�ܹ��ڲ����ж�д����������¸ı��дָ����λ��*/
int lseek_file(int index, int pos)
{
    int i=0;
    for(i=0; i<OpenFileNum; i++)
    {
        if(OpenFile_table[i].inode_num == index)
        {
            if(char4TOint(record_inode[index].file_size) == 0)
            {
                printf("    ���ļ���δ�洢�κ�����,������ƫ�ƶ�дָ�룡\n");
            }
            else
            {
                if(char4TOint(record_inode[index].file_size) < pos)
                {
                    printf("    ���ļ�����ƫ�ƶ�дָ�����Ϊ%d,ָ��ƫ��ʧ�� ��\n",char4TOint(record_inode[index].file_size));
                }
                else
                {
                    OpenFile_table[i].pointer1 = pos/1024;
                    OpenFile_table[i].pointer2 = pos%1024;
                }
            }
            break;
        }
    }
    if(i==OpenFileNum)
    {
        printf("    ����������Ӧ���ļ�û�б��򿪣����ȴ򿪸��ļ���\n");
    }
    return 0;
}
int read_file(int index, int number)//��ָ���ļ�˳�����number���ֽڵ���д������buffer�����������ļ��Ķ�дָ��ָʾ��λ�ÿ�ʼ
{
    int i=0,j=0,x=0,y=0;
    char use[8][512];
    for(i=0; i<OpenFileNum; i++)
    {
        if(OpenFile_table[i].inode_num == index)
        {
            if(char4TOint(record_inode[index].file_size) == 0)
            {
                printf("    ���ļ�Ϊ�գ�\n");
            }
            else
            {
                read_block(char4TOint(record_inode[index].blk_identifier[0])*2,  use[0],512);
                read_block(char4TOint(record_inode[index].blk_identifier[0])*2+1,use[1],512);
                read_block(char4TOint(record_inode[index].blk_identifier[1])*2,  use[2],512);
                read_block(char4TOint(record_inode[index].blk_identifier[1])*2+1,use[3],512);
                read_block(char4TOint(record_inode[index].blk_identifier[2])*2,  use[4],512);
                read_block(char4TOint(record_inode[index].blk_identifier[2])*2+1,use[5],512);
                read_block(char4TOint(record_inode[index].blk_identifier[3])*2,  use[6],512);
                read_block(char4TOint(record_inode[index].blk_identifier[3])*2+1,use[7],512);
                x = OpenFile_table[i].pointer1*2;
                if(OpenFile_table[i].pointer2>512)
                {
                    x++;
                    y = OpenFile_table[i].pointer2 - 512;
                }
                else
                {
                    y = OpenFile_table[i].pointer2;
                }
                if((char4TOint(record_inode[index].file_size)-(OpenFile_table[i].pointer1*1024+OpenFile_table[i].pointer2)) < number)
                {
                    for(j=0; j<=(char4TOint(record_inode[index].file_size)-(OpenFile_table[i].pointer1*1024+OpenFile_table[i].pointer2)); j++)
                    {
                        buffer[j] = use[x][y];
                        y++;
                        if(y==512)
                        {
                            x++;
                            y=0;
                        }
                    }
                    printf("    �ļ������Ѷ��뵽ĩβ,����ȡ%d���ֽ�!\n",j);
                    buffer[j++]='\0';
                }
                else
                {
                    for(j=0; j<=number; j++)
                    {
                        buffer[j] = use[x][y];
                        y++;
                        if(y==512)
                        {
                            x++;
                            y=0;
                        }
                    }
                    buffer[j++]='\0';
                }
                printf("    �ļ������Ѷ��뵽��д������buffer�У�����Ϊ��\n\t\t%s\n",&buffer);
                lseek_file(index,OpenFile_table[i].pointer1*1024+OpenFile_table[i].pointer2);
            }
            break;
        }
    }
    if(i==OpenFileNum)
    {
        printf("    ����������Ӧ���ļ�û�б��򿪣����ȴ򿪸��ļ���\n");
    }
    return 0;
}
int write_file(int index, int number)//�Ѷ�д������buffer��number���ֽ�˳��д��ָ���ļ���д�������ļ��Ķ�дָ��ָʾ��λ�ÿ�ʼ
{
    int i=0,j=0,k=0,x=0,y=0,z=0,w;
    char use[8][512];
    for(i=0; i<OpenFileNum; i++)
    {
        if(OpenFile_table[i].inode_num == index)
        {
            printf("    ����������д����ļ������ݣ�\n\t");
            for(j=0; j<=number; j++)
            {
                scanf("%c",&buffer[j]);
            }
            if(char4TOint(record_inode[index].blk_num)==0)
            {
                intTOchar4(4,record_inode[index].blk_num);
                for(z=0; z<4; z++)
                {
                    for(k=0; k<BlkNum; k++)
                    {
                        if(super_blk.blk_map[k]=='0')
                        {
                            intTOchar4(k,record_inode[index].blk_identifier[z]);
                            super_blk.blk_map[k]='1';
                            break;
                        }
                    }
                }
                w=char4TOint(super_blk.blk_used)+4;
                intTOchar4(w,super_blk.blk_used);
            }
            read_block(char4TOint(record_inode[index].blk_identifier[0])*2,  use[0],512);
            read_block(char4TOint(record_inode[index].blk_identifier[0])*2+1,use[1],512);
            read_block(char4TOint(record_inode[index].blk_identifier[1])*2,  use[2],512);
            read_block(char4TOint(record_inode[index].blk_identifier[1])*2+1,use[3],512);
            read_block(char4TOint(record_inode[index].blk_identifier[2])*2,  use[4],512);
            read_block(char4TOint(record_inode[index].blk_identifier[2])*2+1,use[5],512);
            read_block(char4TOint(record_inode[index].blk_identifier[3])*2,  use[6],512);
            read_block(char4TOint(record_inode[index].blk_identifier[3])*2+1,use[7],512);
            x = OpenFile_table[i].pointer1*2;
            if(OpenFile_table[i].pointer2>512)
            {
                x++;
                y = OpenFile_table[i].pointer2 - 512;
            }
            else
            {
                y = OpenFile_table[i].pointer2;
            }
            if((4096-(OpenFile_table[i].pointer1*1024+OpenFile_table[i].pointer2)) < number)
            {
                for(j=0; j<=(4096-(OpenFile_table[i].pointer1*1024+OpenFile_table[i].pointer2)); j++)
                {
                    use[x][y] = buffer[j];
                    y++;
                    if(y==512)
                    {
                        x++;
                        y=0;
                    }
                }
                printf("    �ļ�����δȫ��д��ָ���ļ��У��ļ������Ѵﵽ���ֵ��д��%d���ֽڣ�\n",j);
            }
            else
            {
                for(j=0; j<=number; j++)
                {
                    use[x][y] = buffer[j];
                    y++;
                    if(y==512)
                    {
                        x++;
                        y=0;
                    }
                    //printf("%c",use[x][y]);
                }
                printf("    �ļ�������ȫ��д��ָ���ļ��У�\n");
            }
            //printf("%d\n",j);
            w = char4TOint(record_inode[index].file_size)+j;
            intTOchar4(w,record_inode[index].file_size);
            write_block(char4TOint(record_inode[index].blk_identifier[0])*2,  use[0],512);
            write_block(char4TOint(record_inode[index].blk_identifier[0])*2+1,use[1],512);
            write_block(char4TOint(record_inode[index].blk_identifier[1])*2,  use[2],512);
            write_block(char4TOint(record_inode[index].blk_identifier[1])*2+1,use[3],512);
            write_block(char4TOint(record_inode[index].blk_identifier[2])*2,  use[4],512);
            write_block(char4TOint(record_inode[index].blk_identifier[2])*2+1,use[5],512);
            write_block(char4TOint(record_inode[index].blk_identifier[3])*2,  use[6],512);
            write_block(char4TOint(record_inode[index].blk_identifier[3])*2+1,use[7],512);
            lseek_file(index,OpenFile_table[i].pointer1*1024+OpenFile_table[i].pointer2);
        }
        break;
    }
    if(i==OpenFileNum)
    {
        printf("    ����������Ӧ���ļ�û�б��򿪣����ȴ򿪸��ļ���\n");
    }
    return 0;
}
int directory()//��ʾ�����ļ����䳤�ȣ��Լ�ռ�ô��̺�
{
    int i=0,j=0,k=0,x=0;
    if(char4TOint(super_blk.inode_used)==1)
    {
        printf("    �ļ�ϵͳδ�����ļ���\n");
    }
    else
    {
        for(i=1; i<InodeNum; i++)
        {
            if(super_blk.inode_map[i]=='1')
            {
                for(j=0; j<MaxDirNum; j++)
                {
                    if(char4TOint(dir_table[j].inode_num)==i)
                        break;
                }
                printf("    �ļ���Ϊ:");
                for(x=0; x<20; x++)
                {
                    if(dir_table[j].name[x]!='!')
                        printf("%c",dir_table[j].name[x]);
                }
                printf("     �ļ���СΪ:%d�ֽ�\n",char4TOint(record_inode[i].file_size));
                if(char4TOint(record_inode[i].blk_num)==0)
                {
                    printf("    �ļ�δռ�ô��̿飡");
                }
                else
                {
                    printf("    �ļ�ռ�õĴ��̿���Ϊ: ");
                    for(k=0; k<char4TOint(record_inode[i].blk_num); k++)
                        printf("%d ",char4TOint(record_inode[i].blk_identifier[k]));
                }
                printf("\n\n");
            }
        }
    }
    return 0;
}
void help()
{
    printf("***********************************************************************************\n");
    printf("* �����Ǳ��ļ�ϵͳ��֧�ֵ����                                                  *\n");
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
    printf("***********************************************************************************\n");
}
void show_blk_map()//��ʾ���̿�ʹ�������0����δ��ʹ�ã�1������ʹ��
{
    int i=0;
    for(i=0; i<BlkNum; i++)
    {
        printf("%c   ",super_blk.blk_map[i]);
        if((i+1)%24==0)
            printf("\n");
    }
    printf("\n\t�ѱ�ʹ�õĴ��̿���ĿΪ��%d\n",char4TOint(super_blk.blk_used));
}
void show_inode_map()//��ʾ�ļ�������ʹ�������0����δ��ʹ�ã�1������ʹ��
{
    int i=0;
    for(i=0; i<InodeNum; i++)
    {
        printf("%c   ",super_blk.inode_map[i]);
        if((i+1)%24==0)
            printf("\n");
    }
    printf("\n\t�ѱ�ʹ�õ��ļ���������ĿΪ��%d\n",char4TOint(super_blk.inode_used));
}
void show_OpenFile()//��ʾ�Ѵ��ļ�
{
    int i=0;
    if(OpenFileNum == 0)
    {
        printf("\tδ���κ��ļ���\n");
    }
    else
    {
        printf("\t�Ѵ�%d���ļ�,�ֱ��ǣ�\n",OpenFileNum);
        for(i=0; i<OpenFileNum; i++)
        {
            printf("\t\t�ļ���Ϊ%s,���ļ�������Ϊ%d.",OpenFile_table[i].name,OpenFile_table[i].inode_num);
        }
    }
}
#endif // FILE_H_INCLUDED
