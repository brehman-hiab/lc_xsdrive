#include "lc_xsdrive/XsDriveCAN.hpp"

XsDriveCAN::XsDriveCAN(/* args */)
{
  
}

XsDriveCAN::~XsDriveCAN()
{
}


void XsDriveCAN::Check(std::string id, canStatus stat)
{
  if (stat != canOK) {
    char buf[50];
    buf[0] = '\0';
    canGetErrorText(stat, buf, sizeof(buf));
    std::cout<<" failed, stat= "<< id<< (int)stat<<  buf<< "\n";
  }
}


void XsDriveCAN::openCanChannel(canHandle&hnd, canStatus&stat,int channelNumber){
    canInitializeLibrary();
    hnd = canOpenChannel(channelNumber, canOPEN_ACCEPT_VIRTUAL);
    if(hnd < 0){
    // To check for errors and print any possible error message, we can use the
    // Check method.
    Check("canOpenChannel", stat);
    exit(1);
    }
    stat = canSetBusParams(hnd, canBITRATE_125K, 1, 0, 0, 0, 0);
    Check("canSetBusParams", stat);
    // Next, take the channel on bus using the canBusOn method. This needs to be
    // done before we can send a message.
    stat = canBusOn(hnd);
    Check("canBusOn", stat);

}

void XsDriveCAN::closeCanChannel(canHandle&hnd, canStatus&stat){
     // Once we are done using the channel, we go off bus using the
    // canBusOff method. It take the handle as the only argument.
    stat = canBusOff(hnd);
    Check("canBusOff", stat);
    // We also close the channel using the canCloseChannel method, which take the
    // handle as the only argument.
    stat = canClose(hnd);
    Check("canClose", stat);
}


void XsDriveCAN::txAliveMsg(canHandle&hnd, canStatus&stat){
    char msg[8] = {1,1,1,1,1,1,1,1};
    unsigned char msg2[8] = {3,1,1,1,1,1,0x98,1};
    stat = canWrite(hnd, 1024, msg, 8, 0);
    stat = canWrite(hnd, 1025, msg, 8, 0);
    stat = canWrite(hnd, 1040, msg, 8, 0);
    stat = canWrite(hnd, 1048, msg, 8, 0);
    stat = canWrite(hnd, 1312, msg2, 8, 0);
    Check("canWrite", stat);
    stat = canWriteSync(hnd, 100);

}

void XsDriveCAN::txLenSensorMsg(canHandle&hnd, canStatus&stat,long id,uint16_t (msgIn)[8]){
    
    unsigned char msg[8] = {0,0,0,0,0,0,0,0};
    for (int i=0 ; i<8 ; i++){
          msg[i] =  (unsigned char)msgIn[i];
    }
    stat = canWrite(hnd, id, msg, 8, 0);

    Check("canWrite", stat);
    stat = canWriteSync(hnd, 100);

}

void XsDriveCAN::printRxMsg(canHandle hnd){
    dumpMessageLoop(hnd);
    
}

void XsDriveCAN::getLeverMsg(canHandle hnd,long id,int (&msgOut)[8]){
  canStatus stat = canOK;
  unsigned int dlc, flags;
  unsigned char msg[8];
  unsigned long timestamp;
  int lever1=0;
  int lever2=0;
  int lever3=0;
  int lever4=0;
  int lever5=0;
  int lever6=0;
  int lever7=0;
  int lever8=0;

    // Call the canReadWait method to wait for a message on the channel. This
    // method has a timeout parameter which in this case is set to 100 ms. If a
    // message is received during this time, it will return the status code
    // canOK and the message will be written to the output parameters. If no
    // message is received, it will return canERR_NOMSG.
    stat = canReadWait(hnd, &id, &msg, &dlc, &flags, &timestamp, 100);
    // Check that the returned status is OK (which means that a message has been received)
    if (stat == canOK){
      // If the message contains an error flag (which implies a different kind
      // of error than if an error signal had been returned), an error message
      // will be shown.
      if (flags & canMSG_ERROR_FRAME){
        // printf("***ERROR FRAME RECEIVED***");
        std::cout<<"***ERROR FRAME RECEIVED***"<<std::endl;
      }
      // If no error flag was found, the program prints the message.
      else {
        /**/
        if(id==1280)
        {
          lever1 = (int)msg[0];
          lever2 = (int)msg[1];
          lever3 = (int)msg[2];
          lever4 = (int)msg[3];
          lever5 = (int)msg[4];
          lever6 = (int)msg[5];
          lever7 = (int)msg[6];
          lever8 = (int)msg[7];

          if ((int)msg[0]>100){ lever1 = (int)msg[0]-256;}
          if ((int)msg[1]>100){ lever2 = (int)msg[1]-256;}
          if ((int)msg[2]>100){ lever3 = (int)msg[2]-256;}
          if ((int)msg[3]>100){ lever4 = (int)msg[3]-256;}
          if ((int)msg[4]>100){ lever5 = (int)msg[4]-256;}
          if ((int)msg[5]>100){ lever6 = (int)msg[5]-256;}
          if ((int)msg[6]>100){ lever7 = (int)msg[6]-256;}
          if ((int)msg[7]>100){ lever8 = (int)msg[7]-256;}
          // std::cout<<"Id: "<<id<<" dlc= "<<dlc<<" Msg: "<<lever1<<" , "<<lever2<<" , "<<lever3<<" , "<<lever4<<" , "<<lever5<<" , "<<lever6<<" , "<< lever7<<" , "<<lever8<<" , timeStamp: "<<timestamp<<"\n";
        }else
        {
          
          lever1=msgOut[0];
          lever2=msgOut[1];
          lever3=msgOut[2];
          lever4=msgOut[3];
          lever5=msgOut[4];
          lever6=msgOut[5];
          lever7=msgOut[6];
          lever8=msgOut[7];
        }
        
      }
    }
     else if (stat != canERR_NOMSG){
      Check("canRead", stat);
    //   break;
    }
    
    msgOut[0]=lever1;
    msgOut[1]=lever2;
    msgOut[2]=lever3;
    msgOut[3]=lever4;
    msgOut[4]=lever5;
    msgOut[5]=lever6;
    msgOut[6]=lever7;
    msgOut[7]=lever8;
    


}
void XsDriveCAN::getBtnMsg(canHandle hnd,long id,bool (&msgOut)[8]){

  canStatus stat = canOK;
  unsigned int dlc, flags;
  unsigned char msg[8];
  unsigned long timestamp;
  
  bool button1=false;
  bool button2=false;
  bool button3=false;
  bool button4=false;
  bool button5=false;
  bool button6=false;
  bool button7=false;
  bool button8=false;

  // Call the canReadWait method to wait for a message on the channel. This
    // method has a timeout parameter which in this case is set to 100 ms. If a
    // message is received during this time, it will return the status code
    // canOK and the message will be written to the output parameters. If no
    // message is received, it will return canERR_NOMSG.
    stat = canReadWait(hnd, &id, &msg, &dlc, &flags, &timestamp, 100);
    // Check that the returned status is OK (which means that a message has been received)
    if (stat == canOK){
      // If the message contains an error flag (which implies a different kind
      // of error than if an error signal had been returned), an error message
      // will be shown.
      if (flags & canMSG_ERROR_FRAME){
        // printf("***ERROR FRAME RECEIVED***");
        std::cout<<"***ERROR FRAME RECEIVED***"<<std::endl;
      }
      // If no error flag was found, the program prints the message.
      else {
         if(id==1296)
        {
            
          if((int)msg[4]==8)
          {
            button1=true;
          }else if((int)msg[4]==16){
              button2=true;
          }else if((int)msg[4]==32){
              button3=true;
          }

          if((int)msg[5]==1)
          {
            button4=true;
          }else if((int)msg[5]==2){
              button5=true;
          }else if((int)msg[5]==4){
              button6=true;
          }else if((int)msg[5]==8){
              button7=true;
          }
          else if((int)msg[5]==16){
              button8=true;
          }

          // if (button1==true)
          // {
          //   x=!x;
          // }else
          // {
          //   x=x;
          // }
          


        //std::cout<<"x "<<x<<"Id: "<<id<<" Msg: btn1="<<button1<<" , btn2="<<button2<<" ,btn3= "<<button3<<", btn4="<<button4<<" ,btn5="<<button5<<", btn6="<<button6<<" ,btn7="<<button7<<",btn8="<<button8<<", timeStamp: "<<timestamp<<"\n";
        // std::cout<<"x "<<x<<"\n";
        }
      }
    }
     else if (stat != canERR_NOMSG){
      Check("canRead", stat);
    //   break;
    }

    msgOut[0]=button1;
    msgOut[1]=button2;
    msgOut[2]=button3;
    msgOut[3]=button4;
    msgOut[4]=button5;
    msgOut[5]=button6;
    msgOut[6]=button7;
    msgOut[7]=button8;




}

void XsDriveCAN::dumpMessageLoop(canHandle hnd){
  // First declare some variables for holding the incoming messages. The
  // incoming messages consist of the same parameters as an outgoing message,
  // i.e. identifier (id), body (msg), length (dlc), and flags), as well as a
  // timestamp.
  
  canStatus stat = canOK;
  long id;
  unsigned int dlc, flags;
  unsigned char msg[8];
  unsigned long timestamp;
  int lever1;
  int lever2;
  int lever3;
  int lever4;
  int lever5;
  int lever6;
  int lever7;
  int lever8;

  int button1;
  int button2;
  int button3;
  int button4;
  int button5;
  int button6;
  int button7;
  int button8;
//   bool flag=true;
//   printf("Listening for messages on channel 0, press any key to close\n");
  // Start a loop that loops until a key is pressed.
//   while (flag){
    // Call the canReadWait method to wait for a message on the channel. This
    // method has a timeout parameter which in this case is set to 100 ms. If a
    // message is received during this time, it will return the status code
    // canOK and the message will be written to the output parameters. If no
    // message is received, it will return canERR_NOMSG.
    stat = canReadWait(hnd, &id, &msg, &dlc, &flags, &timestamp, 100);
    // Check that the returned status is OK (which means that a message has been received)
    if (stat == canOK){
      // If the message contains an error flag (which implies a different kind
      // of error than if an error signal had been returned), an error message
      // will be shown.
      if (flags & canMSG_ERROR_FRAME){
        // printf("***ERROR FRAME RECEIVED***");
        std::cout<<"***ERROR FRAME RECEIVED***"<<std::endl;
      }
      // If no error flag was found, the program prints the message.
      else {
        /**/
        if(id==1280)
        {
          lever1 = (int)msg[0];
          lever2 = (int)msg[1];
          lever3 = (int)msg[2];
          lever4 = (int)msg[3];
          lever5 = (int)msg[4];
          lever6 = (int)msg[5];
          lever7 = (int)msg[6];
          lever8 = (int)msg[7];

          if ((int)msg[0]>100){ lever1 = (int)msg[0]-256;}
          if ((int)msg[1]>100){ lever2 = (int)msg[1]-256;}
          if ((int)msg[2]>100){ lever3 = (int)msg[2]-256;}
          if ((int)msg[3]>100){ lever4 = (int)msg[3]-256;}
          if ((int)msg[4]>100){ lever5 = (int)msg[4]-256;}
          if ((int)msg[5]>100){ lever6 = (int)msg[5]-256;}
          if ((int)msg[6]>100){ lever7 = (int)msg[6]-256;}
          if ((int)msg[7]>100){ lever8 = (int)msg[7]-256;}

          std::cout<<"Id: "<<id<<" dlc= "<<dlc<<" Msg: "<<lever1<<" , "<<lever2<<" , "<<lever3<<" , "<<lever4<<" , "<<lever5<<" , "<<lever6<<" , "<< lever7<<" , "<<lever8<<" , timeStamp: "<<timestamp<<"\n";
        }

        if(id==1296)
        {
            button1=0;
            button2=0;
            button3=0;
            button4=0;
            button5=0;
            button6=0;
            button7=0;
            button8=0;
          if((int)msg[4]==8)
          {
            button1=1;
          }else if((int)msg[4]==16){
              button2=1;
          }else if((int)msg[4]==32){
              button3=1;
          }

          if((int)msg[5]==1)
          {
            button4=1;
          }else if((int)msg[5]==2){
              button5=1;
          }else if((int)msg[5]==4){
              button6=1;
          }else if((int)msg[5]==8){
              button7=1;
          }
          else if((int)msg[5]==16){
              button8=1;
          }

          if (button1==1)
          {
            x=!x;
          }else
          {
            x=x;
          }
          
          
          


        std::cout<<"x "<<x<<"Id: "<<id<<" Msg: btn1="<<button1<<" , btn2="<<button2<<" ,btn3= "<<button3<<", btn4="<<button4<<" ,btn5="<<button5<<", btn6="<<button6<<" ,btn7="<<button7<<",btn8="<<button8<<", timeStamp: "<<timestamp<<"\n";
        // std::cout<<"Id: "<<id<<" dlc= "<<dlc<<" Msg:"<<button5<<"\t, button6"<<button6<<"\t,button7 "<<button7<<"\t, button8"<<button8<<"\t, "<<(int)msg[5]<<"\t, "<<(int)msg[6]<<"\t, timeStamp: "<<timestamp<<"\n";
        }
        // printf("Id: %ld, Msg: %u %u %u %u %u %u %u %u length: %u Flags: %lu",
        //        id, dlc, msg[0], msg[1], msg[2], msg[3], msg[4],
        //        msg[5], msg[6], msg[7], timestamp);
      }
    }
    // Break the loop if something goes wrong, i.e. if we get a status code
    // that is not canOK (taken care of above) and not canERR_NOMSG
    else if (stat != canERR_NOMSG){
      Check("canRead", stat);
    //   break;
    }
//   }
}