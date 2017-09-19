
#include "canreader.h"

CanReader::CanReader(QObject *parent)
    : QThread(parent)
{
    system("sudo ip link set can0 up type can bitrate 500000");
    system("sudo ifconfig can0 txqueuelen 100");

    openPort("can0");
    abort = false;
}


CanReader::~CanReader()
{
    mutex.lock();
    abort = true;
    condition.wakeOne();
    mutex.unlock();

    wait();
    system("sudo ip link set can0 down");
}

bool CanReader::init()
{
    if (!isRunning()) {
        start(HighestPriority);
    }

    return true;
}

void CanReader::run()
{
    QTextStream out(stdout);
    QList<int> mtrCntrlTemps;

    struct can_frame frame_rd;
    int recvbytes = 0;

    int RPM             = 0;
    int SOC             = 0;
    int mtrTemp         = 0;
    int moduleA         = 0;
    int moduleB         = 0;
    int moduleC         = 0;
    int gateDrvrBrd     = 0;
    int highCellTemp    = 0;

    forever
    {
        /*
        mutex.lock();
        condition.wait(&mutex);
        mutex.unlock();
        */

        if ( this->abort )
            return;

        struct timeval timeout = {1, 0};
        fd_set readSet;
        FD_ZERO(&readSet);
        FD_SET(this->soc, &readSet);
        if (select((this->soc + 1), &readSet, NULL, NULL, &timeout) >= 0)
        {
            if (FD_ISSET(this->soc, &readSet))
            {
                recvbytes = read(this->soc, &frame_rd, sizeof(struct can_frame));
                if(recvbytes)
                {
                    switch(frame_rd.can_id)
                    {
                    case 0xA0:
                        moduleA         = (int16_t)(( frame_rd.data[1] << 8 ) + ( frame_rd.data[0] )) * 0.1;
                        moduleB         = (int16_t)(( frame_rd.data[3] << 8 ) + ( frame_rd.data[2] )) * 0.1;
                        moduleC         = (int16_t)(( frame_rd.data[5] << 8 ) + ( frame_rd.data[4] )) * 0.1;
                        gateDrvrBrd     = (int16_t)(( frame_rd.data[7] << 8 ) + ( frame_rd.data[6] )) * 0.1;

                        mtrCntrlTemps.append(moduleA);
                        mtrCntrlTemps.append(moduleB);
                        mtrCntrlTemps.append(moduleC);
                        mtrCntrlTemps.append(gateDrvrBrd);

                        emit setMtrCntrlTemp( *std::max_element(mtrCntrlTemps.begin(), mtrCntrlTemps.end()) );

                        mtrCntrlTemps.clear();

                        break;

                    case 0xA2:
                        mtrTemp = (int16_t)(( frame_rd.data[5] << 8 ) + ( frame_rd.data[4] )) * 0.1;
                        emit setMtrTemp( mtrTemp );

                        break;

                    case 0xA5:
                        RPM = (int16_t)(( frame_rd.data[3] << 8 ) + ( frame_rd.data[2] ));
                        emit setRPM( RPM );

                        break;

                    case 0x181:
                        highCellTemp = (int16_t)(( frame_rd.data[2] << 8 ) + ( frame_rd.data[1] )) * 0.1;
                        emit setHighBattTemp( highCellTemp );

                        break;

                    case 0x183:
                        SOC = (int16_t)(( frame_rd.data[5] << 8 ) + ( frame_rd.data[4] )) * 0.5;
                        emit setSOC( SOC );

                        break;

                    default:
                        break;
                    }
                }
            }
        }
    }
}

int CanReader::openPort(const char *port)
{
    struct ifreq ifr;
    struct sockaddr_can addr;
    /* open socket */
    this->soc = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if(this->soc < 0)
    {
        return (-1);
    }
    addr.can_family = AF_CAN;
    strcpy(ifr.ifr_name, port);
    if (ioctl(this->soc, SIOCGIFINDEX, &ifr) < 0)
    {
        return (-1);
    }
    addr.can_ifindex = ifr.ifr_ifindex;
    fcntl(this->soc, F_SETFL, O_NONBLOCK);
    if (bind(this->soc, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        return (-1);
    }
    return 0;
}
