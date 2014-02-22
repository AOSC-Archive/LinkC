#include "HeartBeats.h"

HeartBeats::HeartBeats(UDP_csocket sk, QThread *parent):
    QThread(parent){
    Dest = sk;
    SendBuffer = new char[128];
}

void HeartBeats::run(){
    printf("Heart Beats Started!\n");
    int length = pack_message(HEART_BEATS,NULL,0,SendBuffer);
    int status;
    while(1){
        status = Dest.Send_msg(SendBuffer,length,0);
        if(status == LINKC_FAILURE){
            printf("Heart Beats Error\n");
            emit SendError(status);
        }
        sleep(HEART_BEATS_TIME);
    }
}

void HeartBeats::HeartBeatsStop(){
    if(this->isRunning() == true)
        this->terminate();
    // Do Something
    delete SendBuffer;
}
