#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct item
{
    int num;		//ҳ��
    int time;		//�ȴ�ʱ�䣬LRU�㷨���õ��������
    int A;          //����λ��Clock�㷨���õ��������
    int M;          //�޸�λ��Clock�㷨���õ��������
} Pro;
Pro *page;			//��ҵҳ�漯
Pro *memory;		//�ڴ�ҳ�漯

int record[10000];   //ҳ���������
int pageNum;		//ϵͳ�������ҵ�������е�ҳ����
int memoryNum;		//�����ڴ�ҳ����
int curmemory;		//�����ڴ��е�ҳ�����
int missNum;		//ȱҳ����
float missRate;		//ȱҳ��

//�Ӻ���
int SequenceCreation();     //����ҳ���������
int OPT();                  //����û��㷨
int FIFO();                 //�Ƚ��ȳ�ҳ���û��㷨
int LRU();                  //������δʹ���û��㷨
int Clock();                //�Ľ���Clock�û��㷨
int PBA();                  //ҳ�滺���㷨(PBA)
void print(Pro *page1);		//��ӡ��ǰ�����е�ҳ��
int  Search(int num1, Pro *memory1);	//��ҳ�漯memory1�в���num1������ҵ�����������memory1�е��±꣬���򷵻�-1

int main(void)//������
{
    int i;
    char c;		//�õ��û��������ַ�����ѡ����Ӧ���û��㷨
    pageNum = SequenceCreation();   //����ҳ���������
    printf("����ϵͳ�����ڴ�ҳ����:\n");
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
        for (i = 0; i<memoryNum; i++)		//��ʼ���ڴ���ҳ��
        {
            memory[i].num = -1;				//ҳ��Ϊ����-1��ʾ
            memory[i].time = -1;
            memory[i].A = 0;
            memory[i].M = 0;
        }
        printf("�������ʾѡ������ҳ���û��㷨��\n");
        printf("     o:��ѣ�Optimal)�û��㷨\n");
        printf("     f:�Ƚ��ȳ�(FIFO)�û��㷨\n");
        printf("     l:������δʹ��(Least Recently Used)�û��㷨\n");
        printf("     c:�Ľ���Clock�û��㷨\n");
        printf("     p:ҳ�滺���㷨(PBA)\n");
        printf("*****��ѡ���������(o,f,l,c,p),������������******\n");
        getchar();
        scanf("%c", &c);
        curmemory = 0;
        if (c == 'o')			//��ѣ�Optimal)�û��㷨
        {
            OPT();
        }
        if (c == 'f')           //�Ƚ��ȳ�(FIFO)�û��㷨
        {
            FIFO();
        }
        if (c == 'l')			//������δʹ��(Least Recently Used)�û��㷨
        {
            LRU();
        }
        if (c == 'c')			//�Ľ���Clock�û��㷨
        {
            Clock();
        }
        if (c == 'p')			//ҳ�滺���㷨(PBA)
        {
            PBA();
        }
    }
    while (c == 'o' || c == 'f' || c == 'l' || c == 'c' || c == 'p');
    free(page);
    free(memory);
    return 0;
}

int FIFO()//�Ƚ��ȳ�ҳ���û��㷨
{
    int i;
    missNum = 0;
    printf("�Ƚ��ȳ�ҳ���û��㷨ҳ���û����:   \n");
    for (i = 0; i<pageNum; i++)
    {
        if (Search(page[i].num, memory)<0)//�����ڴ���û���ҵ���ҳ��
        {
            missNum++;
            memory[curmemory].num = page[i].num;
            print(memory);
            curmemory = (curmemory + 1) % memoryNum;   //�ҳ����Ƚ����ڴ��ҳ��
        }
    }
    missRate = (float)missNum / pageNum;
    printf("ȱҳ������%d   ȱҳ��:  %f\n\n", missNum, missRate);
    return 0;
}

int OPT()//����û��㷨
{
    int i;
    missNum = 0;
    curmemory = 0;
    printf("����û��㷨ҳ���û����:   \n");
    for (i = 0; i<pageNum; i++)
    {
        if (Search(page[i].num, memory) < 0)//�����ڴ���û���ҵ���ҳ��
        {
            //�ҳ�δ���ʱ���ڲ��ٱ����ʵ�ҳ��
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
                tem = 0;       //ҳ��k��δ��temʱ���ڲ������
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
    printf("ȱҳ������%d   ȱҳ��:  %f\n\n", missNum, missRate);
    return 0;
}

int LRU()//������δʹ���û��㷨
{
    int i;
    missNum = 0;
    curmemory = 0;
    printf("������δʹ���û��㷨ҳ���û����:   \n");
    for (i = 0; i<pageNum; i++)
    {
        int rec=Search(page[i].num, memory);
        if (rec < 0)    //�����ڴ���û���ҵ���ҳ��
        {
            missNum++;
            int j;
            for (j = 0; j<memoryNum; j++)     //�ҳ�������δʹ�õ�ҳ��
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
        for (j = 0; j<memoryNum; j++)     //�ڴ��е�����ҳ��ȴ�ʱ��+1
            if (memory[j].num != -1)
                memory[j].time++;

    }//end for
    missRate = (float)missNum / pageNum;
    printf("ȱҳ������%d ȱҳ��: %f\n\n", missNum, missRate);
    return 0;
}

int Clock()//�Ľ���Clock�û��㷨
{
    int i;
    int seek=0;
    missNum = 0;
    curmemory = 0;
    printf("�Ľ���Clock�û��㷨ҳ���û����:   \n");

    for (i = 0; i<pageNum; i++)
    {
        seek = 0;
        int rec=Search(page[i].num, memory);
        if (rec < 0)    //�����ڴ���û���ҵ���ҳ��
        {
            int j;
            missNum++;
            for (j = 0; j<memoryNum; j++)		//���ѡ���޸�λ
            {
                memory[j].M = rand()%2;
            }
            do
            {
                for (j = 0; j<memoryNum; j++)     //Ѱ���Ƿ��п��������
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
                    for (j = 0; j<memoryNum; j++)     //Ѱ�������δ�����ʣ���Ϊ���޸ĵ�ҳ��
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
                        for (j = 0; j<memoryNum; j++)     //Ѱ�������δ�����ʣ����ѱ��޸ĵ�ҳ��
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
    printf("ȱҳ������%d ȱҳ��: %f\n\n", missNum, missRate);
    return 0;
}

int PBA()//ҳ�滺���㷨(PBA)
{
    int i,j,k;
    int length = 3;//����ҳ������
    missNum = 0;
    Pro *memory2;		//����ҳ������
    memory2 = (Pro*)malloc(sizeof(Pro)*length);
    for (i = 0; i<length; i++)
    {
        page[i].num = -1;
        page[i].time = -1;
        page[i].A = 0;
        page[i].M = 0;
    }
    printf("ҳ�滺���㷨ҳ���û����:   \n");

    for (i = 0; i<pageNum; i++)
    {
        if (Search(page[i].num, memory)<0 && Search(page[i].num, memory2)<0)//�����ڴ漰����������û���ҵ���ҳ��
        {
            missNum++;
            if(memory[curmemory].num == -1)
            {
                memory[curmemory].num = page[i].num;
                print(memory);
                curmemory = (curmemory + 1) % memoryNum;   //�ҳ����Ƚ����ڴ��ҳ��
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
                    curmemory = (curmemory + 1) % memoryNum;   //�ҳ����Ƚ����ڴ��ҳ��
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
    printf("ȱҳ������%d ȱҳ��: %f\n\n", missNum, missRate);
    free(memory2);
    return 0;
}

void print(Pro *memory1)//��ӡ��ǰ��ҳ��
{
    int j;
    for (j = 0; j<memoryNum; j++)
        printf("%d ", memory1[j].num);
    printf("\n");
}

int Search(int num1, Pro *memory1)//��ҳ�漯memory1�в���num1������ҵ�����������memory1�е��±꣬���򷵻�-1
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

int SequenceCreation()//����ҳ���������
{
    int N,P,e,m,i=0,j=0,k=0,judge=0;
    float t,r;
    printf("�����������ڴ��С��\n");
    scanf("%d",&N);
    while(N>64)
    {
        printf("�����ڴ��������ҳ��Ϊ64,���������������ڴ��С��\n");
        scanf("%d",&N);
    }
    printf("�����빤��������ʼλ��(P<N)��\n");
    scanf("%d",&P);
    printf("�����빤����������ҳ��(e<N)��\n");
    scanf("%d",&e);
    printf("�����빤�����ƶ���(m<N)��\n");
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
        printf("���������Ӵ�ҳ��������д��ĳ���,�����룺1����������0��\n");
        printf("��ʱ�������д��ĳ���Ϊ��%d\n",k);
        scanf("%d",&judge);
    }
    while(judge==1);
    printf("ҳ��������д�Ϊ��\n");
    for(i = 0; i < k; i++)
    {
        printf("%d ",record[i]);
    }
    printf("\n");
    return k;//����ҳ��������д��ĳ���
}
