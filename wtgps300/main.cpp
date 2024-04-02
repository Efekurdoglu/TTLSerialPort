#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);

    foreach (const QSerialPortInfo &portInfo, QSerialPortInfo::availablePorts())
    {
        qDebug() << "Port Name:" << portInfo.portName();
    }

    QSerialPort *serialPort = new QSerialPort();

    // Replace "COMx" with your actual serial port name, e.g., "COM3" on Windows or "/dev/ttyUSB0" on Linux
    serialPort->setPortName("/dev/ttyUSB0");
    serialPort->setBaudRate(QSerialPort::Baud115200); // Adjust baud rate as needed
    serialPort->setDataBits(QSerialPort::Data8);
    serialPort->setParity(QSerialPort::NoParity);
    serialPort->setStopBits(QSerialPort::OneStop);
    serialPort->setFlowControl(QSerialPort::NoFlowControl);

    if (serialPort->open(QIODevice::ReadOnly))
    {
        qDebug() << "Serial port opened successfully";
    }
    else
    {
        qCritical() << "Error opening serial port:" << serialPort->errorString();
    }

    QByteArray data = serialPort->readAll();
    qDebug() << "Received data:" << data;


    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreated,
        &app,
        [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
