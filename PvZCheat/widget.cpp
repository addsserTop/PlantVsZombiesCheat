#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    InitForm();
}

Widget::~Widget()
{
    CloseHandle(handle);
    delete ui;
}

void Widget::InitForm()
{
    setWindowTitle(tr("植物大战僵尸作弊器"));
    setFixedSize(570,510);

    timer=new QTimer;
    timer->start(1000);

    hwnd=FindWindow(NULL,L"Plants vs. Zombies");//类名置为NULL
    GetWindowThreadProcessId(hwnd,&PID);
    handle=OpenProcess(PROCESS_ALL_ACCESS,false,PID);

    xList<<"0"<<"1"<<"2"<<"3"<<"4"<<"5"<<"6"<<"7"<<"8";
    yList<<"0"<<"1"<<"2"<<"3"<<"4";
    PLIST<<"豌豆射手"<<"向日葵"<<"樱桃炸弹"<<"坚果"<<"土豆地雷"<<"寒冰射手"<<"大嘴花"<<"双重射手"
        <<"小喷菇"<<"阳光菇"<<"大喷菇"<<"咬咬碑"<<"迷糊菇"<<"胆小菇"<<"冰川菇"<<"末日菇"
          <<"莲叶"<<"窝瓜"<<"三重射手"<<"缠绕水草"<<"火爆辣椒"<<"地刺"<<"火炬树桩"<<"高坚果"
            <<"水兵菇"<<"路灯花"<<"仙人掌"<<"三叶草"<<"双向射手"<<"星星果"<<"南瓜头"<<"磁力菇"
              <<"卷心菜投手"<<"花盆"<<"玉米投手"<<"咖啡豆"<<"大蒜"<<"萝卜伞"<<"金盏花"<<"西瓜投手";
    PCOLD<<"750"<<"750"<<"3500"<<"2000"<<"2000"<<"750"<<"750"<<"750"
           <<"750"<<"750"<<"750"<<"750"<<"2000"<<"750"<<"3500"<<"3500"
             <<"750"<<"2000"<<"750"<<"2000"<<"3500"<<"750"<<"750"<<"2000"
               <<"2000"<<"2000"<<"750"<<"750"<<"750"<<"750"<<"2000"<<"750"
                <<"750"<<"750"<<"750"<<"750"<<"750"<<"750"<<"750"<<"750";
    ZLIST<<"普通僵尸"<<"旗子僵尸"<<"路障僵尸"<<"撑杆僵尸"<<"铁桶僵尸"
           <<"读报僵尸"<<"铁网门僵尸"<<"橄榄球僵尸"<<"舞者僵尸"<<"伴舞僵尸"
             <<"鸭子僵尸"<<"潜水僵尸"<<"雪橇车僵尸"<<"雪橇车小队僵尸"<<"海豚僵尸"
               <<"小丑僵尸"<<"气球僵尸"<<"矿工僵尸"<<"蹦蹦僵尸"<<"雪人僵尸"
                 <<"飞贼僵尸"<<"梯子僵尸"<<"投石车僵尸"<<"巨人僵尸"<<"小鬼僵尸";
    ui->PID->addItems(PLIST);
    ui->ZID->addItems(ZLIST);
    ui->PX->addItems(xList);
    ui->PY->addItems(yList);
    ui->ZX->addItems(xList);
    ui->ZY->addItems(yList);

    ui->C1Edit->setEnabled(false);
    ui->C2Edit->setEnabled(false);
    ui->C3Edit->setEnabled(false);
    ui->C4Edit->setEnabled(false);
    ui->C5Edit->setEnabled(false);
    ui->C6Edit->setEnabled(false);
    ui->C7Edit->setEnabled(false);
    ui->C8Edit->setEnabled(false);
    ui->C9Edit->setEnabled(false);
    ui->C10Edit->setEnabled(false);

    CardNum=0;
    QStringList List;
    List<<"卡槽1"<<"卡槽2"<<"卡槽3"<<"卡槽4"<<"卡槽5"<<"卡槽6"<<"卡槽7"<<"卡槽8"<<"卡槽9"<<"卡槽10";
    ui->CardSelect->addItems(List);

    QFile pfile(qApp->applicationDirPath()+"/Plant.db");
    QFile zfile(qApp->applicationDirPath()+"/Zombie.db");
    if(!(pfile.exists()&&zfile.exists()))
    {
        Plant=QSqlDatabase::addDatabase("QSQLITE","P");
        Plant.setDatabaseName(qApp->applicationDirPath()+"/Plant.db");
        Plant.setHostName("192.168.1.1");
        Plant.setUserName("root");
        Plant.setPort(3306);
        Plant.setPassword("Plant");
        Plant.open();

        Zombie=QSqlDatabase::addDatabase("QSQLITE","Z");
        Zombie.setDatabaseName(qApp->applicationDirPath()+"/Zombie.db");
        Zombie.setHostName("192.168.1.1");
        Zombie.setUserName("root");
        Zombie.setPort(3306);
        Zombie.setPassword("Zombie");
        Zombie.open();
        QSqlQuery PQuery(Plant);
        PQuery.exec("create table Plant(id int,name varchar(20),cold int)");
        for(int i=0;i<40;i++)
        {
            QString str=QString("insert into Plant values(%1,'%2',%3)").arg(i).arg(PLIST[i]).arg(PCOLD[i]);
            PQuery.exec(str);
        }
        QSqlQuery ZQuery(Zombie);
        ZQuery.exec("create table Zombie(id int,name varchar(20))");
        for(int i=0;i<25;i++)
        {
            QString str=QString("insert into Plant values(%1,'%2',%3)").arg(i).arg(ZLIST[i]);
            ZQuery.exec(str);
        }
    }
    else
    {
        Plant=QSqlDatabase::addDatabase("QSQLITE","Z");
        Plant.setDatabaseName(qApp->applicationDirPath()+"/Plant.db");
        Plant.setHostName("192.168.1.1");
        Plant.setUserName("root");
        Plant.setPort(3306);
        Plant.setPassword("Plant");
        Plant.open();

        Zombie=QSqlDatabase::addDatabase("QSQLITE","P");
        Zombie.setDatabaseName(qApp->applicationDirPath()+"/Zombie.db");
        Zombie.setHostName("192.168.1.1");
        Zombie.setUserName("root");
        Zombie.setPort(3306);
        Zombie.setPassword("Zombie");
        Zombie.open();
    }
    ReadMessage();

    connect(timer,&QTimer::timeout,this,[=](){
        //
        if(hwnd==0)
        {
            QMessageBox::warning(this,tr("警告"),tr("未检测到游戏进程"));
            qApp->closeAllWindows();
        }
        else
        {
            setWindowTitle(tr("植物大战僵尸作弊器:游戏运行中"));
            VerifyGame();
        }
    });
}

void Widget::VerifyGame()
{
    ui->C1Edit->clear();
    ui->C2Edit->clear();
    ui->C3Edit->clear();
    ui->C4Edit->clear();
    ui->C5Edit->clear();
    ui->C6Edit->clear();
    ui->C7Edit->clear();
    ui->C8Edit->clear();
    ui->C9Edit->clear();
    ui->C10Edit->clear();
    DWORD CardCold=CardAddress+0x5C;
    for(int i=0;i<10;i++)
    {
        DWORD VALUE;
        ReadProcessMemory(handle,(LPVOID)CardCold, &VALUE,sizeof(int),0);
        if(VALUE>50||VALUE<0)
        {
            Llist<<"无";
        }
        else
        {
            QSqlQuery query(Plant);
            QString str=QString("select * from Plant where id = '%1'").arg(VALUE);
            query.exec(str);
            query.next();
            Llist<<query.value(1).toString();
        }
        CardCold+=0x50;
    }
    ui->C1Edit->setText(Llist[0]);
    ui->C2Edit->setText(Llist[1]);
    ui->C3Edit->setText(Llist[2]);
    ui->C4Edit->setText(Llist[3]);
    ui->C5Edit->setText(Llist[4]);
    ui->C6Edit->setText(Llist[5]);
    ui->C7Edit->setText(Llist[6]);
    ui->C8Edit->setText(Llist[7]);
    ui->C9Edit->setText(Llist[8]);
    ui->C10Edit->setText(Llist[9]);
}
void Widget::ReadMessage()
{
    //读取信息
    CardAddress=0X0;
    ReadProcessMemory(handle,(LPVOID)PVZADDRESS, &CardAddress,sizeof(int),0);
    CardAddress = CardAddress + 0x868;
    ReadProcessMemory(handle, (LPVOID)CardAddress, &CardAddress, sizeof(int), 0);
    CardAddress = CardAddress + 0x15C;
    ReadProcessMemory(handle, (LPVOID)CardAddress, &CardAddress, sizeof(int), 0);
}
void Widget::on_SunBtn_clicked()
{
    if(ui->SunEdit->text().isEmpty())
    {
        QMessageBox::warning(this,tr("警告"),tr("未检测到输入"));
        return;
    }
    DWORD SUNVALUE=ui->SunEdit->text().toULong();
    DWORD SUNCURRUNTVALUE=0x0;
    ReadProcessMemory(handle,(LPVOID)PVZADDRESS, &SUNCURRUNTVALUE,sizeof(int),0);
    SUNCURRUNTVALUE = SUNCURRUNTVALUE + 0x868;
    ReadProcessMemory(handle, (LPVOID)SUNCURRUNTVALUE, &SUNCURRUNTVALUE, sizeof(int), 0);
    SUNCURRUNTVALUE = SUNCURRUNTVALUE + 0x5578;
    WriteProcessMemory(handle, (LPVOID)SUNCURRUNTVALUE, &SUNVALUE, sizeof(int), 0);
    ui->SunEdit->clear();
}

void Widget::on_ColdBtn_clicked()
{
    //冷却
    DWORD ADDRESS=CardAddress+0x4C;
    DWORD COLD=0X2710;
    if(Llist[ui->CardSelect->currentIndex()]=="无")
    {
        QMessageBox::warning(this,tr("警告"),tr("查无此槽"));
    }
    else
    {
        //冷却处理
        for(int i=0;i<ui->CardSelect->currentIndex();i++)
        {
            ADDRESS+=0X50;
        }
        WriteProcessMemory(handle, (LPVOID)ADDRESS, &COLD, sizeof(int), 0);
    }
}

void Widget::on_PU_stateChanged(int arg1)
{
    BYTE VALUE;
    DWORD ADDRESS=0X5447A3;
    if(arg1==2)
    {
        //无敌
        VALUE=0X00;
        WriteProcessMemory(handle, (LPVOID)ADDRESS, &VALUE, sizeof(BYTE), 0);
    }
    else
    {
        VALUE=0XFC;
        WriteProcessMemory(handle, (LPVOID)ADDRESS, &VALUE, sizeof(BYTE), 0);
    }
}

void Widget::on_PS_stateChanged(int arg1)
{
    DWORD ADDRESS=0X545E04;
    if(arg1==2)
    {
        //无敌
        unsigned char VALUE[]={0XC7,0X87,0XC8,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X90};
        WriteProcessMemory(handle, (LPVOID)ADDRESS, &VALUE, sizeof(BYTE)*11, 0);
    }
    else
    {
        unsigned char VALUE[]={0X89,0XAF,0XC8,0X00,0X00,0X00,0XE8,0X61,0XC3,0XFF,0XFF};
        WriteProcessMemory(handle, (LPVOID)ADDRESS, &VALUE, sizeof(BYTE)*11, 0);
    }
}

void Widget::PlantSet(DWORD x, DWORD y, DWORD id)
{
    unsigned char VALUE[]={0x60,0x8B,0X15,0X50,0X1C,0X73,0X00,0X8B,0X92,0X68,0X08,0X00,0X00,0X68,0XFF,0XFF,0XFF,0XFF,0X6A,0x00,0XB8,
                           0X00,0X00,0X00,0X00,0X6A,0x00,0x52,0xB9,0XA0,0X05,0X41,0X00,0xFF,0XD1,0x61,0XC3,0X90};
    VALUE[19]=id;
    VALUE[21]=y;
    VALUE[26]=x;
    LPVOID lpRemoteMemory = VirtualAllocEx(handle, NULL, 4096, MEM_COMMIT, PAGE_READWRITE);
    WriteProcessMemory(handle, lpRemoteMemory, VALUE, sizeof(BYTE)*38, NULL);
    HANDLE hThread=CreateRemoteThread(handle, NULL, 0, (LPTHREAD_START_ROUTINE)lpRemoteMemory, NULL, 0, NULL);
    WaitForSingleObject(hThread, INFINITE);
    VirtualFreeEx(handle, lpRemoteMemory, 0, MEM_RELEASE);
}

void Widget::ZombieSet(DWORD x, DWORD y, DWORD id)
{
    unsigned char VALUE[]={0x60,0x8B,0X15,0X50,0X1C,0X73,0X00,0X8B,0X92,0X68,0X08,0X00,0X00,0X8B,0X92,0X78,
                           0X01,0X00,0X00,0X6A,0X02,0X6A,0X00,0XB8,0X02,0X00,0X00,0X00,0X8B,0xCA,0xBB,0xE0,
                           0XDC,0X42,0X00,0XFF,0xD3,0x61,0XC3,0X90};
    VALUE[20]=x;
    VALUE[22]=id;
    VALUE[24]=y;
    LPVOID lpRemoteMemory = VirtualAllocEx(handle, NULL, 4096, MEM_COMMIT, PAGE_READWRITE);
    WriteProcessMemory(handle, lpRemoteMemory, VALUE, sizeof(BYTE)*40, NULL);
    HANDLE hThread=CreateRemoteThread(handle, NULL, 0, (LPTHREAD_START_ROUTINE)lpRemoteMemory, NULL, 0, NULL);
    WaitForSingleObject(hThread, INFINITE);
    VirtualFreeEx(handle, lpRemoteMemory, 0, MEM_RELEASE);
}

void Widget::on_PSbtn_clicked()
{
    //放置植物
    DWORD x=ui->PX->currentText().toULong();
    DWORD y=ui->PY->currentText().toULong();
    DWORD id=ui->PID->currentIndex();
    PlantSet(x,y,id);
}

void Widget::on_ZSbtn_clicked()
{
    //放置僵尸
    DWORD x=ui->ZX->currentText().toULong();
    DWORD y=ui->ZY->currentText().toULong();
    DWORD id=ui->ZID->currentIndex();
    ZombieSet(x,y,id);
}

void Widget::on_OHBTN_clicked()
{
    //狂热模式
    //全体无冷却+全屏炸弹
    DWORD ADDRESS=CardAddress+0x4C;
    DWORD COLD=0X2710;
    for(int i=0;i<ui->CardSelect->currentIndex();i++)
    {
        WriteProcessMemory(handle, (LPVOID)ADDRESS, &COLD, sizeof(int), 0);
        ADDRESS+=0X50;
    }
    for(int i=0;i<9;i++)
    {
        for(int j=0;j<5;j++)
        {
            PlantSet(i,j,2);
        }
    }
}
