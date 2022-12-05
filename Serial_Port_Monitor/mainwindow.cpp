#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    refresh();
    connect(ui->pb_refresh, SIGNAL(clicked()), this, SLOT(refresh()));
    connect(ui->pb_connect, SIGNAL(clicked()), this, SLOT(start()));
    connect(ui->pb_send, SIGNAL(clicked()), this, SLOT(send()));


    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(recieve()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::recieve(){
    QString buffer = serial.readAll();
    QString buffer2;
    if(buffer == ""){
        return;
    }

    for(int i = 0; i < buffer.size(); i++){
        QString str = QString::number(buffer.at(i).unicode());
        int nValue = str.toInt();
        if(nValue < 32 || nValue > 122){
            buffer2.append("\\x");
            if(nValue < 16){
                buffer2.append("0");
            }
            buffer2.append(QString::number(nValue,16));

        }else{
            buffer2.append(buffer.at(i));
        }

    }

    add_line("r - " + buffer2);
}

void MainWindow::send(){
    send_data(ui->le_input->text());
    ui->le_input->clear();
}

void MainWindow::refresh(){
    ui->cb_com->disconnect();
    ui->cb_com->clear();

    const QList infos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &info : infos){
        ui->cb_com->addItem(info.portName());
    }
}

void MainWindow::start(){
    //opening com
    QString currentPortName;
    currentPortName = ui->cb_com->currentText();

    serial.close();
    serial.setPortName(currentPortName);

    ui->pb_connect->setStyleSheet("");

    if(currentPortName == ""){
    }else if(!serial.open(QIODevice::ReadWrite)) {
        ui->pb_connect->setStyleSheet("color: red");
        timer->stop();
        return;
    }else{
        ui->pb_connect->setStyleSheet("color: green");
        add_line("i - comport opened");
        timer->start(1000);
    }

}

//Send data over open serial
void MainWindow::send_data(QString datatobewritten){
    QString buffer;
    for(int i = 0; i < datatobewritten.size(); i++){
        if(datatobewritten.mid(i,2) == "\\x" && datatobewritten.size() > i+3){

            QByteArray array = QByteArray::fromHex(datatobewritten.mid(i+2,2).toUtf8());
            serial.write(array);
            buffer += array;
            i += 3;

        }else{
            serial.write(datatobewritten.mid(i,1).toUtf8());
            buffer += datatobewritten.mid(i,1).toUtf8();
        }
    }
    add_line("w - " + datatobewritten);
}

void MainWindow::add_line(QString add){
    QTextCursor textCursor = ui->pte_output->textCursor();
    textCursor.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor,1);
    ui->pte_output->setTextCursor(textCursor); // The line to add
    ui->pte_output->setPlainText(add + "\n" + ui->pte_output->toPlainText());
}
