#include<iostream>
#include<stdio.h>
#include<queue>
using namespace std;
//进程控制块
struct PCB
{
    int id;
    int priority;
    int alltime;
    int cputime;
    int state;//0:wait,1:run,2:finish
    PCB(int i=0,int p=0,int a=0,int c=0,int s=0)
    {
        id=i;priority=p;a=alltime;c=cputime;state=s;
    }
    bool operator<(const PCB &p) const
    {
        return priority<p.priority;
    }
};

int id[6]={0,1,2,3,4,5};
int priority[6]={0,10,6,8,3,15};
int alltime[6]={0,8,4,3,1,5};
int cputime[6]={0,0,0,0,0,0};
char state[6]={'W','W','W','W','W','W'};

//打印处于就绪状态的PCB
void print_waiting_queue(priority_queue<PCB> q)
{
    while(!q.empty())
    {
        PCB w=q.top();
        q.pop();
        cout<<w.id<<" ";
    }
    cout<<endl;
}
void print_waiting_queue2(queue<PCB> q)
{
    while(!q.empty())
    {
        PCB w=q.front();
        q.pop();
        cout<<w.id<<" ";
    }
    cout<<endl;
}

//输出id,priority,cputime,alltime,state
void print_status()
{
    cout<<"ID\t\t\t\t";
    for(int i=1;i<=5;i++)
        //cout<<id[i]<<"  ";
        printf("%3d ",id[i]);
    cout<<endl;
    cout<<"PRIORITY\t\t\t";
    for(int i=1;i<=5;i++)
        //cout<<priority[i]<<"  ";
        printf("%3d ",priority[i]);
    cout<<endl;
    cout<<"CPUTIME\t\t\t\t";
    for(int i=1;i<=5;i++)
        //cout<<cputime[i]<<"  ";
        printf("%3d ",cputime[i]);
    cout<<endl;
    cout<<"ALLTIME\t\t\t\t";
    for(int i=1;i<=5;i++)
        //cout<<alltime[i]<<"  ";
        printf("%3d ",alltime[i]);
    cout<<endl;
    cout<<"STATE\t\t\t\t";
    for(int i=1;i<=5;i++)
        //cout<<state[i]<<"  ";
        printf("%3c ",state[i]);
    cout<<endl;
}

//判断是否所有进程都已经完成
bool isEnded()
{
    for(int i=1;i<=5;i++)
    {
        if(state[i]!='F')
            return false;
    }
    return true;
}

int main()
{
    //创建5个进程
    PCB pcb1(1,10,8);
    PCB pcb2(2,6,4);
    PCB pcb3(3,8,3);
    PCB pcb4(4,3,1);
    PCB pcb5(5,15,5);
    string algorithm;
    cout<<"TYPE THE ALGORITHM (PRIORITY/RR):";
    cin>>algorithm;
    if(algorithm=="PRIORITY")
    {
        cout<<"OUTPUT OF "<<algorithm<<endl;
        //将进程放入优先队列
        priority_queue<PCB> waiting_queue;
        waiting_queue.push(pcb1);
        waiting_queue.push(pcb2);
        waiting_queue.push(pcb3);
        waiting_queue.push(pcb4);
        waiting_queue.push(pcb5);
        int count=0;//记录总共占用多少时间片
        while(!waiting_queue.empty())//当就绪队列非空时
        {
            count++;
            cout<<"============================================"<<endl;
            cout<<"RUNNING PROC.\t\t\tWAITING QUEUE"<<endl;
            PCB running_pcb=waiting_queue.top();
            waiting_queue.pop();
            running_pcb.state=1;
            state[running_pcb.id]='R';

            cout<<running_pcb.id<<"\t\t\t\t";
            print_waiting_queue(waiting_queue);
            cout<<"============================================"<<endl;
            print_status();

            //更改running_pcb的状态
            running_pcb.priority-=3;
            priority[running_pcb.id]-=3;
            alltime[running_pcb.id]-=1;
            if(alltime[running_pcb.id]>0)
            {
                running_pcb.alltime-=1;
                running_pcb.state=0;
                running_pcb.cputime+=1;
                cputime[running_pcb.id]+=1;
                state[running_pcb.id]='W';
                waiting_queue.push(running_pcb);
            }
            else
            {
                alltime[running_pcb.id]=0;
                running_pcb.alltime=0;
                running_pcb.state=2;
                state[running_pcb.id]='F';
                running_pcb.cputime+=1;
                cputime[running_pcb.id]+=1;
            }
            cout<<endl<<endl;
        }
        cout<<"============================================"<<endl;
        cout<<"RUNNING PROC.\t\t\tWAITING QUEUE"<<endl<<endl;
        cout<<"============================================"<<endl;
        print_status();
        cout<<"============================================"<<endl;
        cout<<"SYSTEM FINISHED"<<endl;
        cout<<"ALL TIME:"<<count<<endl;
    }
    else if(algorithm=="RR")
    {
        cout<<"OUTPUT OF "<<algorithm<<endl;
        //将进程放入队列
        queue<PCB> waiting_queue;
        waiting_queue.push(pcb1);
        waiting_queue.push(pcb2);
        waiting_queue.push(pcb3);
        waiting_queue.push(pcb4);
        waiting_queue.push(pcb5);
        int mark=1;
        PCB running_pcb;
        int count=0;
        while(!isEnded())//当就绪队列非空时
        {
            count++;
            cout<<"============================================"<<endl;
            cout<<"RUNNING PROC.\t\t\tWAITING QUEUE"<<endl;
            if(mark==1)//在mark=1的情况下从waiting_queue中pop出一个进程进行运行
            {
                running_pcb=waiting_queue.front();
                waiting_queue.pop();
                running_pcb.state=1;
                state[running_pcb.id]='R';
                running_pcb.cputime=0;//清空为0
                cputime[running_pcb.id]=0;
                mark=0;
            }

            cout<<running_pcb.id<<"\t\t\t\t";
            print_waiting_queue2(waiting_queue);
            cout<<"============================================"<<endl;
            print_status();

            //更改running_pcb的状态
            alltime[running_pcb.id]-=1;
            if(alltime[running_pcb.id]==0)//该进程已经运行完毕
            {
                running_pcb.alltime=0;
                running_pcb.state=2;
                state[running_pcb.id]='F';
                running_pcb.cputime+=1;
                cputime[running_pcb.id]+=1;
                mark=1;//要求从waiting_queue中pop出一个进程来运行
            }
            else if(alltime[running_pcb.id]>0&&priority[running_pcb.id]==cputime[running_pcb.id]+1)//还没运行完，但需要进行调度
            {
                running_pcb.cputime+=1;
                cputime[running_pcb.id]+=1;
                running_pcb.state=0;
                state[running_pcb.id]='W';
                running_pcb.alltime-=1;
                waiting_queue.push(running_pcb);//将该进程push进队列
                mark=1;//要求从waiting_queue中pop出一个进程来运行
            }
            else
            {
                running_pcb.alltime-=1;
                //alltime[running_pcb.id]-=1;
                running_pcb.cputime+=1;
                cputime[running_pcb.id]+=1;
            }
            cout<<endl<<endl;
        }
        cout<<"============================================"<<endl;
        cout<<"RUNNING PROC.\t\t\tWAITING QUEUE"<<endl<<endl;
        cout<<"============================================"<<endl;
        print_status();
        cout<<"============================================"<<endl;
        cout<<"SYSTEM FINISHED"<<endl;
        cout<<"ALL TIME:"<<count<<endl;
    }
    return 0;
}
