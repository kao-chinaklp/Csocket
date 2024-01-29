#ifndef THREAD_H_
#define THREAD_H_

#include <pthread.h>

#include "Cstring.h"
#include "Deque.h"

class CTask{
    protected:
        String TaskName; // 任务内容
        int connfd; // 编号

    public:
        CTask()=default;
        CTask(String &taskName):TaskName(taskName), connfd(0){}
        virtual ~CTask(){}
        int GetConnFd();
        virtual int Run()=0; // 任务的具体实现
        String GetTaskName(); // 获得任务内容，调试用
        void SetConnFd(int data);
        void SetTaskName(const String _taskname);
};

class CThreadPool{
    private:
        int TaskNum; // 最大同时运行的线程数
        bool shutdown; // 线程池是否被关闭
        pthread_t *pthread_id;
        Deque<CTask*> queTaskList; // 任务队列
        Deque<pthread_t>BusyQue; // 忙碌队列
        pthread_mutex_t pthreadMutex; // 互斥锁
        pthread_cond_t pthreadCond; // 条件变量

    protected:
        int Create();// 创建
        void Sleep(int ms); // 线程休眠
        int MoveToIdle(pthread_t tid); // 回收空闲线程
        int MoveToBusy(pthread_t tid); // 加入忙碌队列
        Deque<pthread_t>::iterator find(pthread_t tid); //查找线程 
        friend void* ThreadFunc(void* threadData); // 回调函数

    public:
        CThreadPool(int threadNum=0);
        ~CThreadPool();
        int StopAll(); // 关闭线程池
        int GetTaskSize(); // 获取线程数
        int GetConnFd(int index); // 获取编号
        int AddTask(CTask* task); // 添加任务
};

#endif
