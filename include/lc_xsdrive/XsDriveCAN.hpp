#include <stdio.h>
// To use CANlib, we need to include canlib.h and also link to canlib32.lib
// when compiling.
#include "canlib.h"
#include <curses.h>
#include <string>
#include <iostream>

    static bool x=false;
class XsDriveCAN{
private:

public:  
    XsDriveCAN(/* args */);
    ~XsDriveCAN();
    void openCanChannel(canHandle&hnd, canStatus&stat,int channelNumber);
    void closeCanChannel(canHandle&hnd, canStatus&stat);
    void txAliveMsg(canHandle&hnd, canStatus&stat);
    void printRxMsg(canHandle hnd);
    void getLeverMsg(canHandle hnd,long id,int (&msgOut)[8]);
    void getBtnMsg(canHandle hnd,long id,bool (&msgOut)[8]);
    void Check(std::string id, canStatus stat);
    void dumpMessageLoop(canHandle hnd);

};