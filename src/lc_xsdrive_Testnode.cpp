
#include "lc_xsdrive/XsDriveCAN.hpp"
int main(int argc, char* argv[]){
       
    // Holds a handle to the CAN channel
    canHandle hnd;
    // Status returned by the Canlib calls
    canStatus stat;
    // The CANlib channel number we would like to use
    int channel_number = 0;
    long leverID=1280;
    int leverCmd[8]={0,0,0,0,0,0,0,0};
    long btnID=1296;
    bool btnState[8]={false,false,false,false,false,false,false,false};
    XsDriveCAN XsDriveobj;

    XsDriveobj.openCanChannel(hnd,stat,channel_number);
    XsDriveobj.Check("canBusOn",stat);
    int count = 0;
    while (count<1000)
    {
        leverCmd[0]=leverCmd[0]; btnState[0]=btnState[0];
        leverCmd[1]=leverCmd[1]; btnState[1]=btnState[1];
        leverCmd[2]=leverCmd[2]; btnState[2]=btnState[2];
        leverCmd[3]=leverCmd[3]; btnState[3]=btnState[3];
        leverCmd[4]=leverCmd[4]; btnState[4]=btnState[4];
        leverCmd[5]=leverCmd[5]; btnState[5]=btnState[5];
        leverCmd[6]=leverCmd[6]; btnState[6]=btnState[6];
        leverCmd[7]=leverCmd[7]; btnState[7]=btnState[7];


        XsDriveobj.txAliveMsg(hnd,stat);       
        XsDriveobj.Check("canWriteSync", stat);
        // XsDriveobj.printRxMsg(hnd);      
        // XsDriveobj.getLeverMsg(hnd,leverID,leverCmd);
        XsDriveobj.getBtnMsg(hnd,btnID,btnState);
        // std::cout<<" leverCmd: "<<leverCmd[0]<<" , "<<leverCmd[1]<<" , "<<leverCmd[2]<<" , "<<leverCmd[3]<<" , "<<leverCmd[4]<<" , "<<leverCmd[5]<<" , "<<leverCmd[6]<<" , "<<leverCmd[7]<<"\n";
        // std::cout<<" btnState: "<<btnState[0]<<" , "<<btnState[1]<<" , "<<btnState[2]<<" , "<<btnState[3]<<" , "<<btnState[4]<<" , "<<btnState[5]<<" , "<<btnState[6]<<" , "<<btnState[7]<<"\n";
        count+=1;
    }

    XsDriveobj.closeCanChannel(hnd,stat);
    
    



    std::cout<<"hello"<<std::endl;
    return 0;
}