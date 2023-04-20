#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QString>
#include <QByteArray>
#include <QMessageBox>
#include <QTimer>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QFile>
#include <windows.h>//要用到WindowsAPI
#include <QDebug>
QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

#define PVZADDRESS 0X731C50

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    void InitForm();
    void ReadMessage();//读取游戏必要信息
    void VerifyGame();//检测游戏状态
    void AddSun();//阳光增加
    void PlantSet(DWORD x,DWORD y,DWORD id);//种植植物
    void ZombieSet(DWORD x,DWORD y,DWORD id);

private slots:
    void on_SunBtn_clicked();

    void on_ColdBtn_clicked();

    void on_PU_stateChanged(int arg1);

    void on_PS_stateChanged(int arg1);

    void on_PSbtn_clicked();

    void on_ZSbtn_clicked();

    void on_OHBTN_clicked();

private:
    Ui::Widget *ui;
    QStringList Llist;
    QTimer *timer;
    HWND hwnd;//窗口句柄
    DWORD PID;//植物大战僵尸线程ID
    HANDLE handle;
    QSqlDatabase Plant;
    QSqlDatabase Zombie;

    int CardNum;
    DWORD CardAddress;
    QStringList PLIST,ZLIST,PCOLD,xList,yList;
};

#endif // WIDGET_H
