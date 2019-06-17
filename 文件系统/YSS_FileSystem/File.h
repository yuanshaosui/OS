#ifndef FILE_H_INCLUDED
#define FILE_H_INCLUDED

#include "IO.h"

/*自定义常量*/
#define BlkNum		    512//磁盘块的数目(编号从0开始)
#define BlkSize		    1024//磁盘块大小为1K
#define InodeNum	    112//文件描述符数目
#define BlkPerNode	    4//每个文件包含的最大的磁盘块数目
#define K	            4//超级块占磁盘块数目(0\1\2\3)
#define SuperBlkBeg	    0//超级块起始磁盘块(0)
#define InodeBeg	    1//文件描述符起始磁盘块(1\2\3)
#define BlockDirBeg	    4//目录区起始磁盘块（4/5/6/7）
#define BlockFileBeg	8//文件区起始磁盘块
#define MaxDirNum	    112//目录所支持的最大的文件数
#define CommanNum	    (sizeof(command)/sizeof(char*))//指令数目
#define OpenFileMaximumNum  10//文件系统同时打开文件最大数目
/*指令集合*/
char* command[]= {"format","quit","directory","create","destroy","open","close","lseek","read","write","help","sbm","sim","sOF"};
/*路径名*/
char path[20]="file_system(yss)$ ";//文件系统（yss 袁少随）
/*读写缓冲区*/
char buffer[5000];
/*结构体*/
typedef struct
{
    char  blk_map[BlkNum];//磁盘块位图
    char  inode_map[InodeNum];//文件描述符位图
    char  blk_used[4];//已被使用的磁盘块数目
    char  inode_used[4];//已被使用的文件描述符数目
} SuperBlk;//超级块（保留区）
typedef struct
{
    char  blk_identifier[BlkPerNode][4];//占用的磁盘块编号
    char  blk_num[4];//占用的磁盘块数目
    char  file_size[4];//文件的大小
} Inode;//文件描述符
typedef struct
{
    char  name[20];//文件名
    char  inode_num[4];//文件描述符序号
} Dir;//目录
typedef struct
{
    int  pointer1;//读写指针(选择盘块）
    int  pointer2;//读写指针(盘块内偏移）
    char  name[30];//文件名
    int  inode_num;//文件描述符序号
} OpenFile;//已打开文件
/*自定义变量*/
SuperBlk	super_blk;//文件系统的超级块
Inode 	record_inode[InodeNum];//文件描述符使用情况
Dir 	dir_table[MaxDirNum];//目录使用情况
OpenFile    OpenFile_table[OpenFileMaximumNum];//已打开文件
int OpenFileNum=0;//已打开文件数目
/*子函数*/
void init_Dick();//初始化磁盘
int init_fs();//初始化文件系统
int close_fs();//关闭文件系统
int format_fs();//格式化文件系统
int create(char *filename);//根据指定的文件名创建新文件
int destroy(char *filename);//删除指定文件
int open_file(char *filename);//打开文件。该函数返回的索引号可用于后续的read, write, lseek,或close操作
int close_file(int index);//关闭指定文件
int read_file(int index, int number);//从指定文件顺序读入number个字节到读写缓冲区buffer。读操作从文件的读写指针指示的位置开始
int write_file(int index, int number);//把读写缓冲区buffer的number个字节顺序写入指定文件。写操作从文件的读写指针指示的位置开始
/*把文件的读写指针移动到pos指定的位置。pos是一个整数，表示从文件开始位置的偏移量。文件打开时，读写指针自动设置为0。每次读写操作之后，
它指向最后被访问的字节的下一个位置。lseek能够在不进行读写操作的情况下改变读写指针能位置*/
int lseek_file(int index, int pos);
int directory();//列表显示所有文件及其长度
void help();//求助函数
void show_blk_map();//显示磁盘块使用情况，0代表未被使用，1代表已使用
void show_inode_map();//显示文件描述符使用情况，0代表未被使用，1代表已使用
void show_OpenFile();//显示已打开文件

/*File函数*/
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
    printf("    如果你是第一次使用此文件系统，请输入1！否则输入其它任意键！\n");
    scanf("%c",&judge);
    if(judge=='1')
    {
        format_fs();
        printf("\n");
    }
    FileToArray();//把文件内容恢复到数组
    //把数组内容恢复到程序内存中
    read_block(0,&super_blk,632);
    read_block(2,&record_inode,2688);
    read_block(8,&dir_table,4032);
    printf("\n");
    return 0;
}
int close_fs()
{
    //把程序内存中内容写入模拟磁盘数组
    write_block(0,&super_blk,632);
    write_block(2,&record_inode,2688);
    write_block(8,&dir_table,4032);
    ArrayToFile();//把数组ldisk 存储到文件
    return 0;
}
int format_fs()//格式化文件系统
{
    init_Dick();////初始化磁盘数组
    char use[512];
    int i=0;
    memset(&super_blk.blk_map, '0', BlkNum);//初始化超级块
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
    for(i=0; i<InodeNum; i++)//初始化文件描述符使用情况
    {
        intTOchar4(BlkNum,record_inode[i].blk_identifier[0]);
        intTOchar4(BlkNum,record_inode[i].blk_identifier[1]);
        intTOchar4(BlkNum,record_inode[i].blk_identifier[2]);
        intTOchar4(BlkNum,record_inode[i].blk_identifier[3]);
        intTOchar4(0,record_inode[i].blk_num);
        intTOchar4(0,record_inode[i].file_size);
    }
    strncpy(dir_table[0].name,"yss_contents########",sizeof("yss_contents########")-1);//创建目录文件
    intTOchar4(0,dir_table[0].inode_num);
    intTOchar4(0,record_inode[0].blk_num);
    intTOchar4(0,record_inode[0].file_size);
    super_blk.inode_map[0]='1';
    for(i=1; i<MaxDirNum; i++)//初始化目录使用情况
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
    printf("    格式化文件系统成功！\n");
    return 0;
}
int create(char *filename)//根据指定的文件名创建新文件
{
    int i;
    //printf("%d\n",strlen(filename));
    if(strlen(filename)>=20)
    {
        printf("    文件名过长！\n");
        return -1;
    }
    for(i=0; i<MaxDirNum; i++)
    {
        if(strncmp(dir_table[i].name, filename,strlen(filename))==0&&dir_table[i].name[strlen(filename)]=='!')
        {
            printf("    文件系统中已存在该文件，请更换文件名或将已存在文件删除！\n");
            return -1;
        }
    }
    if(char4TOint(super_blk.inode_used)>=InodeNum)
    {
        printf("    文件系统中存储文件已达上限，请先删除一些不必要的文件后再创建新文件！\n");
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
    printf("    文件%s创建成功！\n",filename);
    return 0;
}
int destroy(char *filename)//删除指定文件
{
    int i,j,k;
    char use[512];
    for(i=0; i<OpenFileNum; i++)
    {
        if(strcmp(OpenFile_table[i].name, filename)==0)
        {
            printf("    %s文件已打开，请先关闭文件后再进行删除。\n",filename);
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

            printf("    %s文件已删除！\n",filename);
            break;
        }
    }
    if(i==MaxDirNum)
    {
        printf("    没有在目录中找到该文件，请确保输入的文件名正确！\n");
    }
    return 0;
}
int open_file(char *filename)//打开文件。该函数返回的索引号可用于后续的read, write, lseek,或close操作
{
    int i;
    if(OpenFileNum>OpenFileMaximumNum)
    {
        printf("    此时已打开文件数目达到文件系统最大要求，如要继续打开此文件，请先关闭一些不再使用的文件！\n");
        return -1;
    }
    for(i=0; i<MaxDirNum; i++)
    {
        if(strncmp(dir_table[i].name, filename,strlen(filename))==0&&dir_table[i].name[strlen(filename)]=='!')
        {
            strcpy(OpenFile_table[OpenFileNum].name, filename);
            OpenFile_table[OpenFileNum].inode_num = char4TOint(dir_table[i].inode_num);
            OpenFileNum++;
            printf("    %s文件已打开，文件索引号为%d。\n",filename,char4TOint(dir_table[i].inode_num));
            break;
        }
    }
    if(i==MaxDirNum)
    {
        printf("    没有在目录中找到该文件，请确保输入的文件名正确！\n");
    }
    return char4TOint(dir_table[i].inode_num);
}
int close_file(int index)//关闭指定文件
{
    int i=0,j=0,judge=0;
    for(i=0; i<OpenFileNum; i++)
    {
        if(OpenFile_table[i].inode_num == index)
        {
            printf("    %s文件已关闭。\n",OpenFile_table[i].name);
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
        printf("    没有在已打开文件中找到该文件，请确保输入的索引号正确！\n");
    }
    return 0;
}
/*把文件的读写指针移动到pos指定的位置。pos是一个整数，表示从文件开始位置的偏移量。文件打开时，读写指针自动设置为0。每次读写操作之后，
它指向最后被访问的字节的下一个位置。lseek能够在不进行读写操作的情况下改变读写指针能位置*/
int lseek_file(int index, int pos)
{
    int i=0;
    for(i=0; i<OpenFileNum; i++)
    {
        if(OpenFile_table[i].inode_num == index)
        {
            if(char4TOint(record_inode[index].file_size) == 0)
            {
                printf("    该文件尚未存储任何内容,不允许偏移读写指针！\n");
            }
            else
            {
                if(char4TOint(record_inode[index].file_size) < pos)
                {
                    printf("    该文件允许偏移读写指针最大为%d,指针偏移失败 ！\n",char4TOint(record_inode[index].file_size));
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
        printf("    索引号所对应的文件没有被打开，请先打开该文件！\n");
    }
    return 0;
}
int read_file(int index, int number)//从指定文件顺序读入number个字节到读写缓冲区buffer。读操作从文件的读写指针指示的位置开始
{
    int i=0,j=0,x=0,y=0;
    char use[8][512];
    for(i=0; i<OpenFileNum; i++)
    {
        if(OpenFile_table[i].inode_num == index)
        {
            if(char4TOint(record_inode[index].file_size) == 0)
            {
                printf("    该文件为空！\n");
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
                    printf("    文件内容已读入到末尾,共读取%d个字节!\n",j);
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
                printf("    文件内容已读入到读写缓冲区buffer中！内容为：\n\t\t%s\n",&buffer);
                lseek_file(index,OpenFile_table[i].pointer1*1024+OpenFile_table[i].pointer2);
            }
            break;
        }
    }
    if(i==OpenFileNum)
    {
        printf("    索引号所对应的文件没有被打开，请先打开该文件！\n");
    }
    return 0;
}
int write_file(int index, int number)//把读写缓冲区buffer的number个字节顺序写入指定文件。写操作从文件的读写指针指示的位置开始
{
    int i=0,j=0,k=0,x=0,y=0,z=0,w;
    char use[8][512];
    for(i=0; i<OpenFileNum; i++)
    {
        if(OpenFile_table[i].inode_num == index)
        {
            printf("    请输入你想写入该文件的内容：\n\t");
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
                printf("    文件内容未全部写入指定文件中，文件内容已达到最大值，写入%d个字节！\n",j);
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
                printf("    文件内容已全部写入指定文件中！\n");
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
        printf("    索引号所对应的文件没有被打开，请先打开该文件！\n");
    }
    return 0;
}
int directory()//显示所有文件及其长度，以及占用磁盘号
{
    int i=0,j=0,k=0,x=0;
    if(char4TOint(super_blk.inode_used)==1)
    {
        printf("    文件系统未含有文件！\n");
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
                printf("    文件名为:");
                for(x=0; x<20; x++)
                {
                    if(dir_table[j].name[x]!='!')
                        printf("%c",dir_table[j].name[x]);
                }
                printf("     文件大小为:%d字节\n",char4TOint(record_inode[i].file_size));
                if(char4TOint(record_inode[i].blk_num)==0)
                {
                    printf("    文件未占用磁盘块！");
                }
                else
                {
                    printf("    文件占用的磁盘块编号为: ");
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
    printf("* 下面是本文件系统所支持的命令：                                                  *\n");
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
    printf("***********************************************************************************\n");
}
void show_blk_map()//显示磁盘块使用情况，0代表未被使用，1代表已使用
{
    int i=0;
    for(i=0; i<BlkNum; i++)
    {
        printf("%c   ",super_blk.blk_map[i]);
        if((i+1)%24==0)
            printf("\n");
    }
    printf("\n\t已被使用的磁盘块数目为：%d\n",char4TOint(super_blk.blk_used));
}
void show_inode_map()//显示文件描述符使用情况，0代表未被使用，1代表已使用
{
    int i=0;
    for(i=0; i<InodeNum; i++)
    {
        printf("%c   ",super_blk.inode_map[i]);
        if((i+1)%24==0)
            printf("\n");
    }
    printf("\n\t已被使用的文件描述符数目为：%d\n",char4TOint(super_blk.inode_used));
}
void show_OpenFile()//显示已打开文件
{
    int i=0;
    if(OpenFileNum == 0)
    {
        printf("\t未打开任何文件！\n");
    }
    else
    {
        printf("\t已打开%d个文件,分别是：\n",OpenFileNum);
        for(i=0; i<OpenFileNum; i++)
        {
            printf("\t\t文件名为%s,该文件索引号为%d.",OpenFile_table[i].name,OpenFile_table[i].inode_num);
        }
    }
}
#endif // FILE_H_INCLUDED
