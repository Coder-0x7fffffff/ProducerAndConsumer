#ifndef PRODUCER_H
#define PRODUCER_H

#include <QThread>
#include <QMutex>
#include <QSemaphore>
#include <QDebug>
#include <QMainWindow>

class producer : public QThread
{
    Q_OBJECT
signals:
    void signal_produce(int id);
    void signal_produce_over(int id);
    void signal_produce_dispatch(int id);
    void signam_produce_dispatch_over(int id);
public:
    int id;
    producer(QMainWindow *view,int id);
    void run();
};

#endif // PRODUCER_H
