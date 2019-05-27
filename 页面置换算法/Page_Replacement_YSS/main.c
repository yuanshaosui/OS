#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct item
{
    int num;		//页号
    int time;		//等待时间，LRU算法会用到这个属性
    int A;          //访问位，Clock算法会用到这个属性
    int M;          //修改位，Clock算法会用到这个属性
} Pro;
Pro *page;			//作业页面集
Pro *memory;		//内存页面集

int record[10000];   //页面访问序列
int pageNum;		//系统分配给作业的主存中的页面数
int memoryNum;		//可用内存页面数
int curmemory;		//调入内存中的页面个数
int missNum;		//缺页次数
float missRate;		//缺页率

//子函数
int SequenceCreation();     //创建页面访问序列
int OPT();                  //最佳置换算法
int FIFO();                 //先进先出页面置换算法
int LRU();                  //最近最久未使用置换算法
int Clock();                //改进型Clock置换算法
int PBA();                  //页面缓冲算法(PBA)
void print(Pro *page1);		//打印当前主存中的页面
int  Search(int num1, Pro *memory1);	//在页面集memory1中查找num1，如果找到，返回其在memory1中的下标，否则返回-1

int main(void)//主函数
{
    int i;
    char c;		//得到用户的输入字符，来选择相应的置换算法
    pageNum = SequenceCreation();   //创建页面访问序列
    printf("输入系统分配内存页面数:\n");
    scanf("%d", &memoryNum);
    page = (Pro*)malloc(sizeof(Pro)*pageNum);
    memory = (Pro*)malloc(sizeof(Pro)*memoryNum);

    for (i = 0; i<pageNum; i++)
    {
        page[i].num = record[i];
        page[i].time = 0;
        page[i].A = 0;
        page[i].M = 0;
    }
    do
    {
        for (i = 0; i<memoryNum; i++)		//初始化内存中页面
        {
            memory[i].num = -1;				//页面为空用-1表示
            memory[i].time = -1;
            memory[i].A = 0;
            memory[i].M = 0;
        }
        printf("请根据提示选择以下页面置换算法：\n");
        printf("     o:最佳（Optimal)置换算法\n");
        printf("     f:先进先出(FIFO)置换算法\n");
        printf("     l:最近最久未使用(Least Recently Used)置换算法\n");
        printf("     c:改进型Clock置换算法\n");
        printf("     p:页面缓冲算法(PBA)\n");
        printf("*****请选择操作类型(o,f,l,c,p),按其它键结束******\n");
        getchar();
        scanf("%c", &c);
        curmemory = 0;
        if (c == 'o')			//最佳（Optimal)置换算法
        {
            OPT();
        }
        if (c == 'f')           //先进先出(FIFO)置换算法
        {
            FIFO();
        }
        if (c == 'l')			//最近最久未使用(Least Recently Used)置换算法
        {
            LRU();
        }
        if (c == 'c')			//改进型Clock置换算法
        {
            Clock();
        }
        if (c == 'p')			//页面缓冲算法(PBA)
        {
            PBA();
        }
    }
    while (c == 'o' || c == 'f' || c == 'l' || c == 'c' || c == 'p');
    free(page);
    free(memory);
    return 0;
}

int FIFO()//先进先出页面置换算法
{
    int i;
    missNum = 0;
    printf("先进先出页面置换算法页面置换情况:   \n");
    for (i = 0; i<pageNum; i++)
    {
        if (Search(page[i].num, memory)<0)//若在内存中没有找到该页面
        {
            missNum++;
            memory[curmemory].num = page[i].num;
            print(memory);
            curmemory = (curmemory + 1) % memoryNum;   //找出最先进入内存的页面
        }
    }
    missRate = (float)missNum / pageNum;
    printf("缺页次数：%d   缺页率:  %f\n\n", missNum, missRate);
    return 0;
}

int OPT()//最佳置换算法
{
    int i;
    missNum = 0;
    curmemory = 0;
    printf("最佳置换算法页面置换情况:   \n");
    for (i = 0; i<pageNum; i++)
    {
        if (Search(page[i].num, memory) < 0)//若在内存中没有找到该页面
        {
            //找出未来最长时间内不再被访问的页面
            int tem;
            int opt = 0;
            int k;
            for (k = 0; k < memoryNum; k++)
            {
                if (memory[k].num == -1)
                {
                    curmemory = k;
                    break;
                }
                tem = 0;       //页面k在未来tem时间内不会出现
                int j;
                for (j = i+1; j < pageNum; j++)
                {
                    if (page[j].num == memory[k].num)
                    {
                        if (tem > opt)
                        {
                            opt = tem;
                            curmemory = k;
                        }
                        break;
                    }
                    else tem++;
                }
                if (j == pageNum)
                {
                    opt = tem;
                    curmemory = k;
                }
            }
            missNum++;
            memory[curmemory].num = page[i].num;
            print(memory);
        }
    }
    missRate = (float)missNum / pageNum;
    printf("缺页次数：%d   缺页率:  %f\n\n", missNum, missRate);
    return 0;
}

int LRU()//最近最久未使用置换算法
{
    int i;
    missNum = 0;
    curmemory = 0;
    printf("最近最久未使用置换算法页面置换情况:   \n");
    for (i = 0; i<pageNum; i++)
    {
        int rec=Search(page[i].num, memory);
        if (rec < 0)    //若在内存中没有找到该页面
        {
            missNum++;
            int j;
            for (j = 0; j<memoryNum; j++)     //找出最近最久未使用的页面
                if (memory[j].time == -1)
                {
                    curmemory = j;
                    break;
                }
                else if (memory[j].time > memory[curmemory].time)
                    curmemory = j;

            memory[curmemory].num = page[i].num;
            memory[curmemory].time = 0;
            print(memory);

        }
        else memory[rec].time = 0;
        int j;
        for (j = 0; j<memoryNum; j++)     //内存中的所有页面等待时间+1
            if (memory[j].num != -1)
                memory[j].time++;

    }//end for
    missRate = (float)missNum / pageNum;
    printf("缺页次数：%d 缺页率: %f\n\n", missNum, missRate);
    return 0;
}

int Clock()//改进型Clock置换算法
{
    int i;
    int seek=0;
    missNum = 0;
    curmemory = 0;
    printf("改进型Clock置换算法页面置换情况:   \n");

    for (i = 0; i<pageNum; i++)
    {
        seek = 0;
        int rec=Search(page[i].num, memory);
        if (rec < 0)    //若在内存中没有找到该页面
        {
            int j;
            missNum++;
            for (j = 0; j<memoryNum; j++)		//随机选择修改位
            {
                memory[j].M = rand()%2;
            }
            do
            {
                for (j = 0; j<memoryNum; j++)     //寻找是否有空闲物理块
                {
                    if (memory[j].num == -1)
                    {
                        curmemory = j;
                        seek = 1;
                        memory[j].num = 0;
                        break;
                    }

                }
                if(seek != 1)
                {
                    for (j = 0; j<memoryNum; j++)     //寻找最近即未被访问，又为被修改的页面
                    {
                        if (memory[j].A == 0 && memory[j].M == 0)
                        {
                            curmemory = j;
                            seek = 1;
                            break;
                        }
                    }
                    if(seek != 1)
                    {
                        for (j = 0; j<memoryNum; j++)     //寻找最近即未被访问，但已被修改的页面
                        {
                            if (memory[j].A == 0 && memory[j].M == 1)
                            {
                                curmemory = j;
                                seek = 1;
                                break;
                            }
                            memory[j].A = 0;
                        }
                    }
                }
            }
            while(seek == 0);
        }
        memory[curmemory].num = page[i].num;
        print(memory);
    }
    missRate = (float)missNum / pageNum;
    printf("缺页次数：%d 缺页率: %f\n\n", missNum, missRate);
    return 0;
}

int PBA()//页面缓冲算法(PBA)
{
    int i,j,k;
    int length = 3;//空闲页面链表
    missNum = 0;
    Pro *memory2;		//空闲页面链表
    memory2 = (Pro*)malloc(sizeof(Pro)*length);
    for (i = 0; i<length; i++)
    {
        page[i].num = -1;
        page[i].time = -1;
        page[i].A = 0;
        page[i].M = 0;
    }
    printf("页面缓冲算法页面置换情况:   \n");

    for (i = 0; i<pageNum; i++)
    {
        if (Search(page[i].num, memory)<0 && Search(page[i].num, memory2)<0)//若在内存及空闲链表中没有找到该页面
        {
            missNum++;
            if(memory[curmemory].num == -1)
            {
                memory[curmemory].num = page[i].num;
                print(memory);
                curmemory = (curmemory + 1) % memoryNum;   //找出最先进入内存的页面
            }
            else
            {
                if(memory[curmemory].M == 0)
                {
                    for (j = 0; j<length; j++)
                    {
                        if(memory2[j].num == -1)
                        {
                            memory2[j].num = memory[curmemory].num;
                            memory2[j].M = memory[curmemory].M;
                            break;
                        }
                    }
                    memory[curmemory].num = page[i].num;
                    print(memory);
                    curmemory = (curmemory + 1) % memoryNum;   //找出最先进入内存的页面
                }
                else
                {
                    memory[curmemory].num = page[i].num;
                    print(memory);
                    curmemory = (curmemory + 1) % memoryNum;
                }
            }
        }
        else
        {
            if (Search(page[i].num, memory2)>=0)
            {
                for (j = 0; j<length; j++)
                {
                    if(memory2[j].num == page[i].num)
                    {
                        for(k=j; k>0; k--)
                        {
                            memory2[k].num = memory2[k-1].num;
                            memory2[k].M = memory2[k-1].M;
                        }
                        memory2[0].num = memory[curmemory].num;
                    }
                }
            }
        }
    }
    missRate = (float)missNum / pageNum;
    printf("缺页次数：%d 缺页率: %f\n\n", missNum, missRate);
    free(memory2);
    return 0;
}

void print(Pro *memory1)//打印当前的页面
{
    int j;
    for (j = 0; j<memoryNum; j++)
        printf("%d ", memory1[j].num);
    printf("\n");
}

int Search(int num1, Pro *memory1)//在页面集memory1中查找num1，如果找到，返回其在memory1中的下标，否则返回-1
{
    int j;
    for (j = 0; j<memoryNum; j++)
    {
        if (num1 == memory1[j].num)
        {
            memory[j].M = 1;
            return j;
        }
    }
    return -1;
}

int SequenceCreation()//创建页面访问序列
{
    int N,P,e,m,i=0,j=0,k=0,judge=0;
    float t,r;
    printf("请输入虚拟内存大小：\n");
    scanf("%d",&N);
    while(N>64)
    {
        printf("虚拟内存限制最大页数为64,请重新输入虚拟内存大小：\n");
        scanf("%d",&N);
    }
    printf("请输入工作集的起始位置(P<N)：\n");
    scanf("%d",&P);
    printf("请输入工作集包含的页数(e<N)：\n");
    scanf("%d",&e);
    printf("请输入工作集移动率(m<N)：\n");
    scanf("%d",&m);
    srand((unsigned)time(NULL));
    t = 0.5;
    do
    {
        for(j=0; j<m; j++)
        {
            record[j+k] = ((rand()%e)+P)%N;
        }
        k+=m;
        r = rand()/32767.0;
        if(r < t)
        {
            P = rand()%N;
        }
        else
        {
            P = (P+1)%N;
        }
        printf("如果想继续加大页面访问序列串的长度,请输入：1，否则输入0；\n");
        printf("此时访问序列串的长度为：%d\n",k);
        scanf("%d",&judge);
    }
    while(judge==1);
    printf("页面访问序列串为：\n");
    for(i = 0; i < k; i++)
    {
        printf("%d ",record[i]);
    }
    printf("\n");
    return k;//返回页面访问序列串的长度
}
