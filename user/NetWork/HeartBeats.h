#include "linkc_network_protocol.h"
#include <qt4/QtCore/QThread>

class HeartBeats : public QThread{
    Q_OBJECT
public:
    HeartBeats(int sockfd, QThread *parent = 0);
    void run();
signals:
    void SendError(int);        // Use ErrorCode
public slots:
    void HeartBeatsStop(void);  // 心跳停止【好吓人】
protected:
    int ConnFd;
    char *SendBuffer;
};
