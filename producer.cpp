#include "producer.h"

extern QMutex *mutex;
extern QSemaphore *full,*empty;
extern int producetCount;
extern int consumerCount;
extern int producerMs;
extern int consumerMs;
extern int producerTs;
extern int consumerTs;
extern int storeMax;
extern int goodMax;
extern int usedGood;
extern int producedGood;
extern int inStore;
extern int running;

producer::producer(QMainWindow *view,int id)
{
    this->id = id;
    connect(this,SIGNAL(signal_produce(int)),view,SLOT(produce(int)));
    connect(this,SIGNAL(signal_produce_over(int)),view,SLOT(produceOver(int)));
    connect(this,SIGNAL(signal_produce_dispatch(int)),view,SLOT(dispatchProduct(int)));
    connect(this,SIGNAL(signam_produce_dispatch_over(int)),view,SLOT(dispatchProductOver(int)));
}

void producer::run(){
    while(true){
        emit signal_produce(id);
        sleep(producerMs);
        emit signal_produce_over(id);
        producedGood++;
        empty->acquire();
        mutex->lock();
        emit signal_produce_dispatch(id);
        sleep(producerTs);
        inStore++;
        emit signam_produce_dispatch_over(id);
        mutex->unlock();
        full->release();
    }
}
