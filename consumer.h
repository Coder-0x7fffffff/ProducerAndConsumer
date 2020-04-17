#ifndef CONSUMER_H
#define CONSUMER_H

#include <QThread>
#include <QMutex>
#include <QSemaphore>
#include <QDebug>
#include <QMainWindow>

class consumer : public QThread
{
    Q_OBJECT
signals:
    void signal_consume_dispatch(int id);
    void signam_consume_dispatch_over(int id);
    void signal_consume(int id);
    void signal_consume_over(int id);
public:
    int id;
    consumer(QMainWindow *view,int id);
    void run();
};

#endif // CONSUMER_H
