[TOC]

### 槽函数理解：

槽函数本身是QT的一种机制，从QT4的signal和SLOT理解connect里面的槽函数，是一种触发型处理函数，可以被一个或多个信号链接触发，QT5里面少了参数，但是只要信号函数和槽函数一样（编译器会做参数检查，不像回调函数本身就是不安全的，它的类型匹配保证了安全性），那么就可以带参数传递。其中界面里面的提升为槽也是一种触发型处理函数的机制。

```
https://www.cnblogs.com/tgycoder/p/5274681.html//槽函数
https://blog.csdn.net/SiriusWilliam/article/details/117483663（Qt中double与float与16进制字符串之间的转换）
```

#### float数据的传输

直接强转不适用于C++的强制转换，可以把float转换成uint*形式

```
f_uint = *(uint*)&z_angle
```

同样都是4个字节，然后再转换成

```
QString("%1").arg(f_uint, 4, 16, QLatin1Char('1'))
```

QString形式的十六进制字符串。

最后再利用下面函数将十六进制字符串转换成字符串形式。

```
QString Client::qstrhex_to_qstr(QString string)
{
    QStringList string3=string.split(" ");
    QString string4;
    for(int i=0;i<string3.size();i++)
    {
        unsigned char value=string3.at(i).toInt(NULL,16);
        string4.append(value);
    }
    return string4;
}
```

### 网络（QNetworkInterface）

#### TCP

##### client

```
tcpSocket_client=new QTcpSocket(this);

tcpSocket_client->connectToHost(addr_str, targetport);//可以UDP获得
   connect(tcpSocket_client ,&QTcpSocket::connected,
                [=]()
                {
                });
    connect(tcpSocket_client ,&QTcpSocket::disconnected,
                [=]()
                {
                });
    //读取数据信号处理
    connect(tcpSocket_client ,&QTcpSocket::readyRead ,
            [=]()
            {
            QByteArray dataGram = tcpSocket_client->readAll();
            //从通讯套接字中，取出数据
            //ui->text_server->append(dataGram);
            //tcpSocket_server->readAll().toHex()
            QByteArray hexData = dataGram.toHex();
         });
```

##### server

```
udpSocket_server=new QUdpSocket(this);
tcp_server = new QTcpServer(this);
tcp_server->listen(QHostAddress::Any,PORT);
connect(tcp_server ,&QTcpServer::newConnection ,
                 [=]()
                 {
                     //新的连接产生，获取Tcp连接套接字
                     tcpSocket_server = tcp_server->nextPendingConnection();
                     QString temp = QString("[%1-%2]:tcp成功连接").arg(targetaddr.toString()).arg(QString::number(targetport));
                     //显示连接信息
                     ui->text_server->append(temp);
                     //服务器端连接数据接收信号
                     connect(tcpSocket_server ,&QTcpSocket::readyRead ,
                             [=]()
                             {
                                 QByteArray dataGram = tcpSocket_server->readAll();
                                 //从通讯套接字中，取出数据
                                 //ui->text_server->append(dataGram);
                                 //tcpSocket_server->readAll().toHex()
                                 QByteArray hexData = dataGram.toHex();//取出十六进制数据
                                 if(hex_int_registerOrHeart == 256)//应答接受注册0100
                                         {
                                             tcpSocket_server->write(datastr,len);
                                         }
								}
					});
```

#### UDP

##### client

```
udpSocket_client=new QUdpSocket(this);
udpSocket_client->bind(3601, QUdpSocket::ShareAddress);
connect(udpSocket_client,&QUdpSocket::readyRead,this,&Client::udp_processData);
len = udpSocket_client->writeDatagram(datastr,
                                   qstrlen(datastr),
                                   QHostAddress::Broadcast,    //udp广播地址
                                   3602);
void Client::udp_processData()
{
    //有未处理的数据报
    while(udpSocket_client->hasPendingDatagrams()){
        QByteArray dataGram;
        //读取的数据报大小
        dataGram.resize(udpSocket_client->pendingDatagramSize());
        udpSocket_client->readDatagram(dataGram.data(), dataGram.size(),&targetaddr,&targetport);
        /*IP处理*/
        if(first == true)
        {
            connect_to_TCP();
            first = false;
        }
        ui->text_client->append(dataGram.data());   //将接收到的数据显示到标签上
    }
}
```

##### server

```
udpSocket_server=new QUdpSocket(this);
QHostAddress address = QNetworkInterface().allAddresses().at(1);//分配网络地址
udpSocket_server->bind(address,PORT);//#define PORT 3602
connect(udpSocket_server,&QUdpSocket::readyRead,this,&Server::udp_processData);
```



### 图表（QChart）

```
void Server::chart_init()
{
    series_x = new QSplineSeries();   // 创建一个样条曲线对象
    series_x->setName("x agnle");
    series_x->setColor(0x00BBFF);

    series_z = new QSplineSeries();   // 创建一个样条曲线对象
    series_z->setName("z angle");
    series_z->setColor(0x00AA00);
    series_x->append(0,0);
    series_z->append(0,0);

    scatterSeries_x = new QScatterSeries();

    scatterSeries_x->setPointLabelsFormat("@yPoint");
    scatterSeries_x->setMarkerSize(6);
    scatterSeries_x->setPointLabelsColor(0x00BBFF);
    scatterSeries_x->setColor(Qt::red);
    scatterSeries_x->setPointLabelsVisible(true);
    scatterSeries_x->setPointsVisible(true);
    scatterSeries_x->append(0, 0);

    scatterSeries_z = new QScatterSeries();
    scatterSeries_z->setPointLabelsFormat("@yPoint");
    scatterSeries_z->setPointLabelsColor(0x00AA00);
    scatterSeries_z->setColor(Qt::red);
    scatterSeries_z->setMarkerSize(6);
    scatterSeries_z->setPointsVisible(true);
    scatterSeries_z->setPointLabelsVisible(true);
    scatterSeries_z->append(0, 0);

    // 添加轴
    xAxis = new QValueAxis();
    yAxis = new QValueAxis();

    // 坐标轴整体
    xAxis->setRange(0, 10);
    yAxis->setRange(0, 360);

    // 轴标题设置
    xAxis->setTitleText("次数");
    yAxis->setTitleText("度数/°");

    // 轴标签设置
    yAxis->setLabelFormat("%.1f");
//    yAxis->setLabelsAngle(45);
    //xAxis->setLabelsAngle(45);

    // 轴线和刻度线设置
    xAxis->setLabelFormat("%d");
    xAxis->setTickCount(2);
    yAxis->setTickCount(2);
    //xAxis->setLinePenColor(Qt::blue);

    // 主网格线设置
    //xAxis->setGridLineVisible(true);
    //xAxis->setGridLineColor(Qt::black);

    // 次刻度和次网格线设置
    xAxis->setMinorTickCount(9);
    xAxis->setMinorGridLinePen(QPen(QBrush(0xDDDDDD),1,Qt::DashLine));
    yAxis->setMinorTickCount(3);
    yAxis->setMinorGridLinePen(QPen(QBrush(0xDDDDDD),1,Qt::DashLine));
    //xAxis->setMinorGridLineColor(Qt::red);

    chart = new QChart();
    // 添加创建好的曲线图对象
    chart->addSeries(scatterSeries_x);
    chart->addSeries(scatterSeries_z);
    chart->addSeries(series_x);
    chart->addSeries(series_z);

    chart->addAxis(xAxis, Qt::AlignBottom);
    chart->addAxis(yAxis, Qt::AlignLeft);

    scatterSeries_x->attachAxis(xAxis);
    scatterSeries_z->attachAxis(xAxis);
    scatterSeries_x->attachAxis(yAxis);
    scatterSeries_z->attachAxis(yAxis);
    series_x->attachAxis(xAxis);
    series_z->attachAxis(xAxis);
    series_x->attachAxis(yAxis);
    series_z->attachAxis(yAxis);
    chart->legend()->markers().at(0)->setVisible(false);
    chart->legend()->markers().at(1)->setVisible(false);
    //chart->legend()->setAlignment(Qt::AlignBottom);
    //chart->legend()->setGeometry(this->width()-100,this->height()-100,30,30);
    //chart->legend()->hide();                //关闭图例
    //chart = ui->chart_view->chart();
    chart->setTitle("采集数据");
    ui->chart_view->setChart(chart);
    ui->chart_view->setRenderHint(QPainter::Antialiasing, true); // 设置抗锯齿
}
void Server::refresh_chart()
{
//        x_data << x_angle;
//        z_data << z_angle;
//        // 数据个数超过了最大数量,删除首个，数据往前移，添加最后一个
//        if (x_data.size() > 10) {
//            x_data.pop_front();
//            z_data.pop_front();
//            count++;
//            xAxis->setRange(count-10, count);
//        }
//        //清空数据
//        series_x->clear();
//        series_z->clear();
//        scatterSeries_x->clear();
//        scatterSeries_z->clear();
//        //以1为步长
//        for (int i = 0; i < x_data.size(); ++i) {
//            series_z->append(i, x_data.at(i));
//            series_x->append(i, z_data.at(i));
//            scatterSeries_x->append(i, x_data.at(i));
//            scatterSeries_z->append(i, z_data.at(i));
//        }
        x_data << x_angle;
        z_data << z_angle;
        static int count = 0;
        if(count > 10)
        {
            series_x->remove(0);
            series_z->remove(0);
            scatterSeries_x->remove(0);
            scatterSeries_z->remove(0);
            xAxis->setRange(count-10, count);                    // 更新X轴范围
        }
        series_x->append(QPointF(count,x_data.at(count) ));
        series_z->append(QPointF(count,z_data.at(count) ));
        scatterSeries_x->append(QPointF(count,x_data.at(count) ));
        scatterSeries_z->append(QPointF(count,z_data.at(count) ));
        count++;
}
```

若对图表的图例操作，可以对类型也可以对单个进行操作，但是要加上对应的头文件。

| Constant                                   | Value | Description                                            |
| ------------------------------------------ | ----- | ------------------------------------------------------ |
| QLegendMarker::LegendMarkerTypeArea        | 0     | A legend marker for an area series.                    |
| QLegendMarker::LegendMarkerTypeBa          | 1     | A legend marker for a bar set.                         |
| QLegendMarker::LegendMarkerTypePie         | 2     | A legend marker for a pie slice.                       |
| QLegendMarker::LegendMarkerTypeXY          | 3     | A legend marker for a line, spline, or scatter series. |
| QLegendMarker::LegendMarkerTypeBoxPlot     | 4     | A legend marker for a box plot series.                 |
| QLegendMarker::LegendMarkerTypeCandlestick | 5     | A legend marker for a candlestick series.              |

```
   #include <QLegendMarker>
   chart->legend()->markers().at(1)->setVisible(false);
   
   foreach (QLegendMarker* marker, chart->legend()->markers())
  	{
     	if(marker->type() == QLegendMarker::LegendMarkerTypeXY)
     	{
        	marker->setVisible(false);
     	}
 	}
```

### 串口（QSerialport）
