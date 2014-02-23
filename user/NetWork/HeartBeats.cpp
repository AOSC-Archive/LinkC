#include "HeartBeats.h"

HeartBeats::HeartBeats(UDP_csocket sk, QThread *parent):
    QThread(parent){
    Dest = sk;
    SendBuffer = new char[128];
}

void HeartBeats::run(){
    int length = pack_message(HEART_BEATS,NULL,0,SendBuffer);
    int status;
    printf("Debug >> Heart Beats\t= [STARTED]\n");
    while(1){
        status = Dest.Send_msg(SendBuffer,length,0);
        if(status == LINKC_FAILURE){
            printf("Debug >> Heart Beats\t= [ERROR]\n");
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
