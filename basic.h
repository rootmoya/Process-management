#ifndef basic_h
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define basic_h
char *errormsg[256];
//process control block 
struct pcb 
{ 
    int pid; //process id
    int ppid; //parent process id 
    int prio; //priority   ���ȼ� 
    int state; //state   ����״̬ 
    int lasttime; //last execute time   ���ִ��ʱ�� 
    int tottime; //totle execute time   �ܵ�ִ��ʱ�� 
};


//process node    ���̽ڵ� tree 
struct pnode 
{ 
    pcb *node;  //���� 
    pnode *sub;  //�� 
    pnode *brother; 
    pnode *next;
};



//�ź���		
struct semphore
{
    char name[5]; //����
    int count; //����ֵ
    int curpid; //��ǰ���� id 
    pnode *wlist; //�ȴ�����
};
#define geterror(eno) printf("%s\n",errormsg[eno])


void initerror()
{
    errormsg[0] = (char *)malloc(20);
    errormsg[0]="error command!";
    errormsg[1] = (char *)malloc(20);
    errormsg[1]="error parameter!";        
}


//get a substring in string s   ����Ӵ� 
char * substr(char *s,int start,int end)
{
    char *s1;
    int i;
    int len = strlen(s);
    if(start<0 || end>=len || start>end)
        return NULL;
    s1=(char *)malloc(end-start+2);
    for( i=0;i<=end-start;i++)
        s1[i] = s[i+start];
    s1[i]='\0';
    return s1;
}



//find the location of c in string s 
int instr(char *s,char c)
{
   int i;
    for(i=0;i<strlen(s);i++)
        if(s[i]==c)
            return i;
    return -1; 
}
//change the string to array data 
int * strtoarray(char *s)
{
    int *a,count,i,x1; 
    char c, *s1,*s2; 
    if(!s)
    {
        printf("string can't be null!\n");
        return NULL;
    }
    count=0; s1=s; 
    for(i=0;i<strlen(s1);i++)
        if(s1[i]==',')
            count++;
    count++;
    a = (int *)malloc(count);
    c=',';
    for(i=0;i<count;i++)
    {
        x1 = instr(s1,c);
        if(x1>=0)
            s2=substr(s1,0,x1-1);     //���� 
        else
            s2=s1;
        a[i]=atoi(s2);  //�Ѳ��� str ��ָ����ַ���ת��Ϊһ������ 
        if(c==',')
            s1=substr(s1,x1+1,strlen(s1)-1);   //ÿ��ȥ���Ѿ�����a �еĲ��� 
    }
    return a;
}
#endif