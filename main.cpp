#include "basic.h"
pnode *proot; //system process tree root 
pnode *plink; //system process link head

//create process
int createpc(int *para)
{   
//add your code here 
    pnode *p,*p1,*pp; 
    int pflag;    ///�ж��Ƿ��и����� 
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
    if(!pp->sub)          //��ǰ�ڵ�û���ӽڵ� 
        pp->sub=p1;
        
    else
    {
        for(p=pp->sub;p->brother;p=p->brother);    //���뵱ǰ�ڵ���ֵܽڵ� 
        p->brother=p1;
    }
    // add to process link 
    for(p=plink;p->next;p=p->next); 
        p->next=p1;
    printf("�������̳ɹ�!!!\n");
    return 0;
}



void revokepc(int num)
{
    pnode *p,*p1,*p2,*p3,*rp,*pp;
    p=plink;
    // �ж��Ƿ��н��̣�������Ƿ����
    if(p->next==NULL)
    {
    	printf("��ǰû�н��̣�\n");
		return ;
	}	
	else
	{
		while(p->node->pid!=num)
		{
			if(p->next==NULL)
			{
				printf("�˽��̲�����!\n");
				return ;	
			}
			p=p->next;
		}
		
	}
    for(p=plink;p;p=p->next)
    {
        rp=p->next;  //��ǰ�ڵ��ǰһ����㣬���α���
        if(rp&&rp->node->pid==num)
        {

            if(rp->next)
                p->next=rp->next;
                // delete rp;
            else
                p->next=NULL;
        }
        p1=p->sub;//ָ���ӽڵ�
        if(p1&&p1->node->pid==num)
        {
            p2=p1->sub;//��ڵ�
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
                p3=p1->brother;//�����ֵܽڵ�
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
    printf("ɾ���ɹ�!\n");
}



//show process detail 
void showdetail()
{
    //add your code here 
    pnode *p,*p1; 
    p=plink;
    for(;p;) //print all pcb info     ��ǰ�ڵ��id,���ȼ����ӽڵ㣬�ֵܽڵ� 
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
        cflag=0; pflag=0;    //cflag����Ƿ���ȷ�� pflag 
        printf("-----------please input you are choose-------------\n");
        printf("1.exit                    2.showdetail\n"); 
        printf("3.createpc(pid,ppid,prio) 4. revokepc(pid)\n");                             
		scanf("%s",cmdstr);
		if(!strcmp(cmdstr,"1")) //exit the program ����ֵΪ0��� 
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
	        s = strstr(cmdstr,"createpc"); //create process ���ַ��е�һ�γ���createpc��λ�á� 
	//            printf("%s\n",s);
	        if(s)
	        {
	            cflag=1;
	            para = (int *)malloc(3);   //0,pid  1 ppid  2prio 
	                //getparameter��ȡ���� 
	            s1 = substr(s,instr(s,'(')+1,strlen(s)-2); //get param string   ��ȡid,pid,prio 
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
            printf("������Ҫɾ�����̵�id��:");
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