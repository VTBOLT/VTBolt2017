#include "gpsreader.h"

GpsReader::GpsReader(QObject *parent)
    : QThread(parent){

    QString filename = "gpsOut.txt";
    gps_output = new QFile(filename);
    if ( gps_output->open(QIODevice::ReadWrite) )
    {
        text_stream.setDevice(gps_output);
    }


    abort = false;
}

GpsReader::~GpsReader()
{
    mutex.lock();
    abort = true;
    condition.wakeOne();
    mutex.unlock();

    wait();
}


bool GpsReader::init()
{
    if (!isRunning()) {
        start(LowPriority);
    }

    return true;
}


void GpsReader::run()
{
    QTextStream out(stdout);
    QSerialPort serialPort;

    serialPort.setPortName("ttyS0");
    serialPort.setBaudRate(QSerialPort::Baud9600);
    if (!serialPort.open(QIODevice::ReadOnly))
    {
        out << "Ouch! I'm broken!" << endl;
        out << serialPort.error() << endl;
        return;
    }
    /*
    serialPort.flush();

    // Change Baud Rate to 57600
    //serialPort.write("$PMTK251,57600*2C\r\n");
    //serialPort.setBaudRate(57600);
    //msleep(5000);

    // Change Update Rate to 200 ms
    serialPort.write( "$PMTK220,200*2C\r\n");
    msleep(1000);

    // Change Measurement Rate to 200 ms
    serialPort.write("$PMTK300,200,0,0,0,0*2F\r\n");
    msleep(1000);

    // Limit GPS Output Messages
    serialPort.write("$PMTK314,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28\r\n");
    msleep(1000);
    */
    QString message;
    QString chk_msg;
    QStringList message_split;

    QString lat;
    QString lng;

    uint checksum;
    double latitude;
    double longitude;
    double speed;
    double heading;

    //serialPort.flush();
    serialPort.clear();

    forever
    {
        if ( this->abort )
            return;
        //serialPort.flush();
        message = serialPort.readLine();
        message_split = message.split(',');

        //out << QSerialPortInfo::availablePorts()[1].portName() << endl;
        //out << message.size() << endl;

        if (message_split[0] == "$GPRMC")
        {
            out << "This happened" << endl;
            chk_msg = message.mid(1, message.length()-6);

            // Checksum
            checksum = 0;
            for(int i = 0; i < chk_msg.length(); i++)
            {
                checksum = checksum ^ (uint8_t)(chk_msg[i].toLatin1());
            }

            if ( checksum == message.mid(message.length()-4, 2).toUInt(nullptr, 16))
            {

                if ( message_split[4] == 'N')
                    latitude = message_split[3].mid(0, 2).toUInt(nullptr, 10) + message_split[3].mid(2, -1).toDouble()/60;
                else if ( message_split[4] == 'S')
                    latitude = -1 * (message_split[3].mid(0, 2).toUInt(nullptr, 10) + message_split[3].mid(2, -1).toDouble()/60);
                else
                    continue;

                if ( message_split[6] == 'E')
                    longitude = message_split[5].mid(1, 2).toUInt(nullptr, 10) + message_split[5].mid(3, -1).toDouble()/60;
                else if ( message_split[6] == 'W')
                    longitude = -1 * (message_split[5].mid(1, 2).toUInt(nullptr, 10) + message_split[5].mid(3, -1).toDouble()/60);
                else
                    continue;

                speed   = message_split[7].toDouble();
                heading = message_split[8].toDouble();

                out << latitude << endl;
                out << longitude << endl;

                text_stream << longitude << endl;
                text_stream << latitude << endl;
                text_stream << endl;

                emit newLatitude(latitude);
                emit newLongitude(longitude);
                emit newSpeed(speed);
                emit newHeading(heading);
                emit coordsUpdated();
            }
        }

        msleep(100);
    }
}
