/*
 * Author		： Junfeng Zhang <564691478@qq.com>
 * Last-Change		： April 5, 2014
 */
#include "HeartBeats.h"

HeartBeats::HeartBeats(UDP_csocket sk, QThread *parent):
    QThread(parent){
    Dest = sk;
    SendBuffer = new char[128];
}

HeartBeats::~HeartBeats(){
	delete [] SendBuffer;
}

void HeartBeats::run(){
    int length = pack_message(HEART_BEATS,NULL,0,SendBuffer);
    int status;
    LinkC_Debug("UDP Heart Beats",LINKC_STARTED);
    while(1){
        status = Dest.Send_msg(SendBuffer,length,0);
        if(status == LINKC_FAILURE){
            LinkC_Debug("Heart Beats",LINKC_WARNING);
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
