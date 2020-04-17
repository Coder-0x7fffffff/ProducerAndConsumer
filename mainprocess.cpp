#include "mainprocess.h"

QMutex *mutex = nullptr;
QSemaphore *full = nullptr,*empty = nullptr;
int producetCount;
int consumerCount;
int producerMs;
int consumerMs;
int producerTs;
int consumerTs;
int storeMax;
int goodMax;
int usedGood;
int producedGood;
int inStore;
bool running;

mainProcess::mainProcess(
        QMainWindow *view,
        int pCount,
        int cCount,
        int pMs,
        int cMs,
        int pTs,
        int cTs,
        int sMax
)
{
    this->view = view;
    producetCount = pCount;
    consumerCount = cCount;
    producerMs = pMs;
    consumerMs = cMs;
    producerTs = pTs;
    consumerTs = cTs;
    storeMax = sMax;
    usedGood = 0;
    producedGood = 0;
    inStore = 0;
    running = true;
    mutex = new QMutex();
    full = new QSemaphore(0);
    empty = new QSemaphore(storeMax);
}

mainProcess::~mainProcess(){
    producetCount = 0;
    consumerCount = 0;
    producerMs = 0;
    consumerMs = 0;
    producerTs = 0;
    consumerTs = 0;
    storeMax = 0;
    goodMax = 0;
    usedGood = 0;
    producedGood = 0;
    inStore = 0;
    running = false;
    delete mutex;
    delete full;
    delete empty;
}

void mainProcess::run(){
    pl = new producer*[producetCount];
    cl = new consumer*[consumerCount];
    for(int i=0;i<producetCount;i++){
        pl[i] = new producer(view,i);
        pl[i]->start();
    }
    for(int i=0;i<consumerCount;i++){
        cl[i] = new consumer(view,i);
        cl[i]->start();
    }
    for(int i =0;i<producetCount;i++)
        pl[i]->wait();
    for(int i =0;i<consumerCount;i++)
        cl[i]->wait();
}

int mainProcess::getProducet(){
    return producedGood;
}

int mainProcess::getConsume(){
    return usedGood;
}

int mainProcess::getInStore(){
    return inStore;
}

void mainProcess::stop(){
    for(int i =0;i<producetCount;i++)
        pl[i]->terminate();
    for(int i =0;i<consumerCount;i++)
        cl[i]->terminate();
}
