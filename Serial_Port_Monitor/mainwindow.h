#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QString>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void recieve();
    void send();
    void refresh();
    void start();
    void send_data(QString);
    void add_line(QString);

private:
    Ui::MainWindow *ui;

    QTimer *timer;
    QSerialPort serial;
    QString buffer;
};
#endif // MAINWINDOW_H
