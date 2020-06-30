/*#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "IVSHMEM.h"
#include <QDebug>
#include <stdlib.h>
#include <string.h>
#include "waitevent.h"
#include "debug.h"

static IVSHMEM* ivshmem = new IVSHMEM();
static void* memory = nullptr;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_cmdInit_clicked()
{
    qDebug() << __FUNCTION__ << ivshmem->Initialize();
}

void MainWindow::on_cmdGetSize_clicked()
{
    qDebug() << __FUNCTION__ << ivshmem->GetSize();
}

void MainWindow::on_cmdGetPeerID_clicked()
{
    qDebug() << __FUNCTION__ << ivshmem->GetPeerID();
}

void MainWindow::on_cmdGetVectors_clicked()
{
    qDebug() << __FUNCTION__ << ivshmem->GetVectors();
}

void MainWindow::on_cmdDeinit_clicked()
{
    if(ivshmem)
        delete ivshmem;
    qDebug() << __FUNCTION__;
}

void MainWindow::on_cmdGetMemory_clicked()
{
    getReady();
    qDebug() << __FUNCTION__;

    QString s = "";
    int i = 0;
    while(1)
    {
        if(*((static_cast<char*>(memory)) + i) != 0)
        {
            s.append( *((static_cast<char*>(memory)) + i++));
        }
        else
            break;
    }
    ui->plainTextEdit->setPlainText(s);
}

void MainWindow::getReady()
{
    if(!ivshmem->IsInitialized())
        ivshmem->Initialize();
    if(memory == nullptr)
        memory = ivshmem->GetMemory();
}

void MainWindow::on_cmdWriteMemory_clicked()
{
    getReady();
    strcpy(static_cast<char*>(memory), "Hello World!\n");
}

void MainWindow::on_cmdWriteMemoryEdit_clicked()
{
    getReady();
    strcpy(static_cast<char*>(memory), ui->plainTextEdit->toPlainText().toLocal8Bit());
}

void MainWindow::on_cmdRegisterVector0_clicked()
{
    getReady();
    waitEvent* w = ivshmem->CreateVectorEvent(0);
    qRegisterMetaType<uint16_t>("uint16_t");
    qRegisterMetaType<HANDLE>("HANDLE");

    connect(w, SIGNAL(Event(uint16_t, HANDLE)), this, SLOT(eventVector(uint16_t, HANDLE)), Qt::UniqueConnection);
}

void MainWindow::eventVector(uint16_t vector, HANDLE  event)
{
    DEBUG_INFO("Event from Vector %d received!", vector);
}

void MainWindow::on_cmdRing_clicked()
{
    ivshmem->RingDoorbell(ui->linePeer->text().toInt(), 0);
}
*/