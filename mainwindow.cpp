#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initVar();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initVar(){
    ui->graphicsView->setScene(&scene);
    //set ms reg
    QRegExp regExp("[0-9]{1,2}");
    ui->producerMs->setValidator(new QRegExpValidator(regExp,this));
    ui->consumerMs->setValidator(new QRegExpValidator(regExp,this));
    ui->producerTs->setValidator(new QRegExpValidator(regExp,this));
    ui->consumerTs->setValidator(new QRegExpValidator(regExp,this));
    ui->storeMax->setValidator(new QRegExpValidator(regExp,this));
}

void MainWindow::initGraphicsView(int pCount,int cCount){
    scene.clear();
    viewWidth = ui->graphicsView->width();
    viewHeight = ui->graphicsView->height();
    storeWidth = viewWidth/8;
    storeHeight = storeWidth;
    storeTextPix = storeWidth/5;
    int count = pCount>cCount?pCount:cCount;
    int fcount = count+1;
    int element = viewHeight/(fcount+count*2);
    element = element>storeHeight/2?storeHeight/2:element;
    int frameHeight = element;
    int height = element*2;
    int by = 0;
    int bx = -viewWidth/2;
    //draw producer
    {
        by = -(element*(pCount*2+pCount+1))/2;
        pl = new QGraphicsEllipseItem*[pCount];
        pll = new QGraphicsLineItem*[pCount];
        plt = new QGraphicsTextItem*[pCount];
        for(int i=0;i<pCount;i++){
            //ellipse
            pl[i] = new QGraphicsEllipseItem();
            pl[i]->setRect(bx+viewWidth/6-height/2,by+(i+1)*frameHeight+i*height,height,height);
            pl[i]->setPen(QPen(QColor(0,0,0),2));
            pl[i]->setBrush(QBrush(QColor(0,0,255)));
            //line
            pll[i] = new QGraphicsLineItem(bx+viewWidth/6,by+(i+1)*frameHeight+i*height+height/2,0,0);
            //text
            plt[i] = new QGraphicsTextItem("等待",pl[i]);
            plt[i]->setFont(QFont("幼圆",height/4));
            QFontMetrics* fm = new QFontMetrics(plt[i]->font());
            plt[i]->setPos(bx+viewWidth/6-fm->width(QString("等待"))/2,by+(i+1)*frameHeight+i*height+height/2-fm->height()/2);
            //insert
            scene.addItem(pll[i]);
            scene.addItem(pl[i]);
        }
    }
    //draw consumer
    {
        by = -(element*(cCount*2+cCount+1))/2;
        cl = new QGraphicsEllipseItem*[cCount];
        cll = new QGraphicsLineItem*[cCount];
        clt = new QGraphicsTextItem*[cCount];
        for(int i=0;i<cCount;i++){
            //ellipse
            cl[i] = new QGraphicsEllipseItem();
            cl[i]->setRect(bx+viewWidth/6*5-height/2,by+(i+1)*frameHeight+i*height,height,height);
            cl[i]->setPen(QPen(QColor(0,0,0),2));
            cl[i]->setBrush(QBrush(QColor(0,192,0)));
            //line
            cll[i] = new QGraphicsLineItem(bx+viewWidth/6*5,by+(i+1)*frameHeight+i*height+height/2,0,0);
            //text
            clt[i] = new QGraphicsTextItem("等待",cl[i]);
            clt[i]->setFont(QFont("幼圆",height/4));
            QFontMetrics* fm = new QFontMetrics(clt[i]->font());
            clt[i]->setPos(bx+viewWidth/6*5-fm->width(QString("等待"))/2,by+(i+1)*frameHeight+i*height+height/2-fm->height()/2);
            scene.addItem(cll[i]);
            scene.addItem(cl[i]);
        }
    }
    {
        //draw store place
        store = new QGraphicsRectItem(-storeWidth/2,-storeHeight/2,storeWidth,storeHeight);
        store->setPen(QPen(QColor(0,0,0),2));
        store->setBrush(QBrush(QColor(255,0,0)));
        scene.addItem(store);
        //draw store text
        storeText = new QGraphicsTextItem(QString("仓库"),store);
        storeText->setFont(QFont("幼圆",storeTextPix));
        QFontMetrics* fm = new QFontMetrics(storeText->font());
        storeText->setPos(-storeWidth/2+storeWidth/2-fm->width(QString("仓库"))/2,-storeHeight/2+storeHeight/2-fm->height()/2);
        //draw status text
        statusText = new QGraphicsTextItem(QString(""));
        statusText->setFont(QFont("幼圆",storeTextPix));
        statusText->setPos(-storeWidth,storeHeight);
        scene.addItem(statusText);
    }
}

void MainWindow::on_startButton_clicked()
{
    //get parameters
    int producetCount = ui->producerCount->text().toInt();
    int consumerCount = ui->consumerCount->text().toInt();
    int producerMs = ui->producerMs->text().toInt();
    int consumerMs = ui->consumerMs->text().toInt();
    int producerTs = ui->producerTs->text().toInt();
    int consumerTs = ui->consumerTs->text().toInt();
    int storeMax = ui->storeMax->text().toInt();
    if(!(producerMs>=1 && producerMs<=99)){
        QMessageBox::critical(this, "错误", "请输入正确的生产用时", QMessageBox::Yes, QMessageBox::Yes);
        return;
    }
    if(!(consumerMs>=1 && consumerMs<=99)){
        QMessageBox::critical(this, "错误", "请输入正确的消费用时", QMessageBox::Yes, QMessageBox::Yes);
        return;
    }
    if(!(storeMax>=1 && storeMax<=99)){
        QMessageBox::critical(this, "错误", "请输入正确的仓库容量", QMessageBox::Yes, QMessageBox::Yes);
        return;
    }
    if(!(producerTs>=0 && producerTs<=99)){
        QMessageBox::critical(this, "错误", "请输入正确的消费用时", QMessageBox::Yes, QMessageBox::Yes);
        return;
    }
    if(!(consumerTs>=0 && consumerTs<=99)){
        QMessageBox::critical(this, "错误", "请输入正确的仓库容量", QMessageBox::Yes, QMessageBox::Yes);
        return;
    }
    //ban input
    ui->producerMs->setEnabled(false);
    ui->producerTs->setEnabled(false);
    ui->producerCount->setEnabled(false);
    ui->consumerMs->setEnabled(false);
    ui->consumerTs->setEnabled(false);
    ui->consumerCount->setEnabled(false);
    ui->storeMax->setEnabled(false);
    ui->startButton->setEnabled(false);
    //draw producer, consumer and store
    initGraphicsView(producetCount,consumerCount);
    //start
    mp =new mainProcess(this,producetCount,consumerCount,producerMs,consumerMs,producerTs,consumerTs,storeMax);
    mp->start();
}

void MainWindow::produce(int id){
    plt[id]->setPlainText("生产");
    statusText->setPlainText(QString("仓库中:")+QString::number(mp->getInStore())+QString("\n生产了:")+QString::number(mp->getProducet())+QString("\n使用了:")+QString::number(mp->getConsume()));
}

void MainWindow::produceOver(int id){
    plt[id]->setPlainText("等待");
    statusText->setPlainText(QString("仓库中:")+QString::number(mp->getInStore())+QString("\n生产了:")+QString::number(mp->getProducet())+QString("\n使用了:")+QString::number(mp->getConsume()));
}

void MainWindow::dispatchProduct(int id){
    pll[id]->setPen(QPen(QColor(255,0,0),3));
    statusText->setPlainText(QString("仓库中:")+QString::number(mp->getInStore())+QString("\n生产了:")+QString::number(mp->getProducet())+QString("\n使用了:")+QString::number(mp->getConsume()));
}

void MainWindow::dispatchProductOver(int id){
    pll[id]->setPen(QPen(QColor(0,0,0),1));
    statusText->setPlainText(QString("仓库中:")+QString::number(mp->getInStore())+QString("\n生产了:")+QString::number(mp->getProducet())+QString("\n使用了:")+QString::number(mp->getConsume()));
}

void MainWindow::dispatchConsume(int id){
    cll[id]->setPen(QPen(QColor(255,0,0),3));
    statusText->setPlainText(QString("仓库中:")+QString::number(mp->getInStore())+QString("\n生产了:")+QString::number(mp->getProducet())+QString("\n使用了:")+QString::number(mp->getConsume()));
}

void MainWindow::dispatchConsumeOver(int id){
    cll[id]->setPen(QPen(QColor(0,0,0),1));
    statusText->setPlainText(QString("仓库中:")+QString::number(mp->getInStore())+QString("\n生产了:")+QString::number(mp->getProducet())+QString("\n使用了:")+QString::number(mp->getConsume()));
}

void MainWindow::consume(int id){
    clt[id]->setPlainText("使用");
    statusText->setPlainText(QString("仓库中:")+QString::number(mp->getInStore())+QString("\n生产了:")+QString::number(mp->getProducet())+QString("\n使用了:")+QString::number(mp->getConsume()));
}

void MainWindow::consumeOver(int id){
    clt[id]->setPlainText("等待");
    statusText->setPlainText(QString("仓库中:")+QString::number(mp->getInStore())+QString("\n生产了:")+QString::number(mp->getProducet())+QString("\n使用了:")+QString::number(mp->getConsume()));
}
void MainWindow::on_stopButton_clicked()
{
    scene.clear();
    mp->stop();
    mp->terminate();
    delete mp;
    mp = nullptr;
    ui->producerMs->setEnabled(true);
    ui->producerTs->setEnabled(true);
    ui->producerCount->setEnabled(true);
    ui->consumerMs->setEnabled(true);
    ui->consumerTs->setEnabled(true);
    ui->consumerCount->setEnabled(true);
    ui->storeMax->setEnabled(true);
    ui->startButton->setEnabled(true);
}
