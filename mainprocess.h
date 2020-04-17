#ifndef MAINPROCESS_H
#define MAINPROCESS_H
#include <QThread>
#include <QMutex>
#include <QSemaphore>
#include <QDebug>
#include <QGraphicsScene>
#include <QMainWindow>
#include <producer.h>
#include <consumer.h>

class mainProcess : public QThread
{
    Q_OBJECT
public:
    QMainWindow *view;
    consumer **cl;
    producer **pl;
    mainProcess(QMainWindow *view,int pCount,int cCount,int pMs,int cMs, int pTs,int cTs,int sMax);
    ~mainProcess();
    void run();
    int getProducet();
    int getConsume();
    int getInStore();
    void stop();
};

#endif // MAINPROCESS_H
