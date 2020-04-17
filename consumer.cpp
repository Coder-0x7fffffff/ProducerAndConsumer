#include "consumer.h"

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

consumer::consumer(QMainWindow *view,int id)
{
    this->id = id;
    connect(this,SIGNAL(signal_consume_dispatch(int)),view,SLOT(dispatchConsume(int)));
    connect(this,SIGNAL(signam_consume_dispatch_over(int)),view,SLOT(dispatchConsumeOver(int)));
    connect(this,SIGNAL(signal_consume(int)),view,SLOT(consume(int)));
    connect(this,SIGNAL(signal_consume_over(int)),view,SLOT(consumeOver(int)));
}

void consumer::run(){
    while(true){
        full->acquire();
        mutex->lock();
        emit signal_consume_dispatch(id);
        sleep(consumerTs);
        inStore--;
        emit signam_consume_dispatch_over(id);
        mutex->unlock();
        empty->release();
        emit signal_consume(id);
        sleep(consumerMs);
        emit signal_consume_over(id);
        usedGood++;
    }
}
