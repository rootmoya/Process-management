#include "basic.h"
pnode *proot; //system process tree root 
pnode *plink; //system process link head

//create process
int createpc(int *para)
{   
//add your code here 
    pnode *p,*p1,*pp; 
    int pflag;    ///判断是否有父进程 
    pflag=0;
    for(p=plink;p;p=p->next)
    {
        if(p->node->pid == para[0]) //check if this pid is already exist
        {
            printf("pid %d is already exist!\n",para[0]);
            return -1; 
        }
        if(p->node->pid == para[1]) //find parent pcb
        { 
            pflag=1;
            pp = p; 
        } 
    }
    if(!pflag)
    {
        printf("parent id %d is not exist!\n",para[1]);
        return -2; 
    }
//init new pcb 
    p1 = new pnode;
    p1->node=new pcb;
    p1->node->pid =  para[0]; 
    p1->node->ppid = para[1];
    p1->node->prio = para[2]; 
    p1->sub=NULL;
    p1->next=NULL;
    p1->brother=NULL;
    
    //add to process tree 
    if(!pp->sub)          //当前节点没有子节点 
        pp->sub=p1;
        
    else
    {
        for(p=pp->sub;p->brother;p=p->brother);    //加入当前节点的兄弟节点 
        p->brother=p1;
    }
    // add to process link 
    for(p=plink;p->next;p=p->next); 
        p->next=p1;
    printf("创建进程成功!!!\n");
    return 0;
}



void revokepc(int num)
{
    pnode *p,*p1,*p2,*p3,*rp,*pp;
    p=plink;
    // 判断是否有进程，与进程是否存在
    if(p->next==NULL)
    {
    	printf("当前没有进程！\n");
		return ;
	}	
	else
	{
		while(p->node->pid!=num)
		{
			if(p->next==NULL)
			{
				printf("此进程不存在!\n");
				return ;	
			}
			p=p->next;
		}
		
	}
    for(p=plink;p;p=p->next)
    {
        rp=p->next;  //当前节点的前一个结点，依次编历
        if(rp&&rp->node->pid==num)
        {

            if(rp->next)
                p->next=rp->next;
                // delete rp;
            else
                p->next=NULL;
        }
        p1=p->sub;//指向子节点
        if(p1&&p1->node->pid==num)
        {
            p2=p1->sub;//孙节点
            for(p2=p1->sub;p2;p2=p2->brother)
                revokepc(p2->node->pid);
            if(p1->brother)
                p->sub=p1->brother;
            else
                p->sub=NULL;
        }
        else
        {
            for(;p1;p1=p1->brother)
            {
                p3=p1->brother;//遍历兄弟节点
                if(p3&&p3->node->pid==num)
                {
                    p2=p3->sub;
                    for(;p2;p2=p2->brother) 
                        revokepc(p2->node->pid);
                    if(p3->brother)
                    {
                        pp=p3->brother;
                        p1->brother=pp;
                    }
                    else
                        p1->brother=NULL;
                }
            }
        }
    }
    printf("删除成功!\n");
}



//show process detail 
void showdetail()
{
    //add your code here 
    pnode *p,*p1; 
    p=plink;
    for(;p;) //print all pcb info     当前节点的id,优先级，子节点，兄弟节点 
    {
        printf("pid=%d(prio=%d): ",p->node->pid,p->node->prio);
        p1 = p->sub;
        for(;p1;) //print sub pcb
        {
            printf("pid=%d(prio=%d) ",p1->node->pid,p1->node->prio);
            p1 = p1->brother;
        }
        printf("\n");
        p = p->next;
    }
    printf("\n");
}


//don't change 
int main()
{
    initerror();
    short flag;
    short cflag,pflag; 
    char cmdstr[32]; 
    proot = new pnode;
    proot->node=new pcb; 
    proot->node->pid=0; 
    proot->node->ppid=-1;
    proot->node->prio=0; 
    proot->next=NULL; 
    proot->sub=NULL; 
    proot->brother=NULL; 
    plink=proot;
    for(;;)
    {
        cflag=0; pflag=0;    //cflag输出是否正确。 pflag 
        printf("-----------please input you are choose-------------\n");
        printf("1.exit                    2.showdetail\n"); 
        printf("3.createpc(pid,ppid,prio) 4. revokepc(pid)\n");                             
		scanf("%s",cmdstr);
		if(!strcmp(cmdstr,"1")) //exit the program 返回值为0相等 
			break;			
        if(!strcmp(cmdstr,"2"))
        {
            cflag = 1; 
            pflag = 1; 
            showdetail();
        }

        if(!strcmp(cmdstr,"3"))
        {
			scanf("%s",cmdstr);
	        int *para;
	        char *s,*s1;
	        s = strstr(cmdstr,"createpc"); //create process 在字符中第一次出现createpc的位置。 
	//            printf("%s\n",s);
	        if(s)
	        {
	            cflag=1;
	            para = (int *)malloc(3);   //0,pid  1 ppid  2prio 
	                //getparameter获取参数 
	            s1 = substr(s,instr(s,'(')+1,strlen(s)-2); //get param string   获取id,pid,prio 
	//                printf("%s\n",s1);
	            para=strtoarray(s1); //get parameter 
	            createpc(para);  
	            pflag=1;
                free(para);
	        }
        }

        if(!strcmp(cmdstr,"4"))
        {

            int num;
            cflag=1;
            printf("输入想要删除进程的id号:");
            scanf("%d",&num);
            revokepc(num);
            pflag=1;
		}
    	if(!cflag)
        	geterror(0);
    	else if(!pflag)
        	geterror(1); 
    }
    free(errormsg[0]);
    free(errormsg[1]);
    delete plink;
    delete proot;
    delete proot->node;
    return 0;
}