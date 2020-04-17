#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QFontMetrics>
#include <QLabel>
#include <QMutex>
#include <QSemaphore>
#include <QScrollBar>
#include <mainprocess.h>
#include <QMessageBox>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_startButton_clicked();

    void on_stopButton_clicked();

private:
    Ui::MainWindow *ui;
    //customer
private:
    int viewWidth,viewHeight;
    int storeWidth = 0;
    int storeHeight = 0;
    int storeTextPix = 0;

    QGraphicsScene scene;
    QGraphicsRectItem *store;
    QGraphicsTextItem *storeText;
    QGraphicsTextItem *statusText;
    QGraphicsEllipseItem **pl,**cl; //ellipse
    QGraphicsLineItem **pll,**cll; //line
    QGraphicsTextItem **plt,**clt; //text
    mainProcess *mp;

    void initVar();
    void initGraphicsView(int pCount,int cCount);
    void reload();
public slots:
    void produce(int id);
    void produceOver(int id);
    void dispatchProduct(int id);
    void dispatchProductOver(int id);
    void dispatchConsume(int id);
    void dispatchConsumeOver(int id);
    void consume(int id);
    void consumeOver(int id);
};

#endif // MAINWINDOW_H
