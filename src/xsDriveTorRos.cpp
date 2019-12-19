#include "ros/ros.h"
#include "std_msgs/String.h"
#include "std_msgs/Float64.h"
#include <std_msgs/Float64MultiArray.h>
#include <std_msgs/UInt8MultiArray.h>
#include "lc_xsdrive/XsDriveCAN.hpp"


std_msgs::Float64MultiArray leverArray;
std_msgs::UInt8MultiArray buttonArray;

double lever1;
double lever2;
double lever3;
double lever4;
double lever5;
double lever6;

uint8_t btn1;
uint8_t btn2;
uint8_t btn3;
uint8_t btn4;
uint8_t btn5;
uint8_t btn6;
uint8_t btn7;

static bool buttonState1 = false;
static bool buttonState2 = false;
static bool buttonState3 = false;
static bool buttonState4 = false;
static bool buttonState5 = false;
// %EndTag(CALLBACK)%

int main(int argc, char **argv)
{
  ros::init(argc, argv, "xsDrive");
  ros::NodeHandle n;
  leverArray.data.resize(8);
  buttonArray.data.resize(8);

/*---------------------------------------*/
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

/*---------------------------------------*/

  /* subscribe to adc topic from arduino */

  ros::Publisher lever_msg = n.advertise<std_msgs::Float64MultiArray>("/xsDrive/lever",1000);
  ros::Publisher button_msg = n.advertise<std_msgs::UInt8MultiArray>("/xsDrive/button",1000);
       
  XsDriveobj.openCanChannel(hnd,stat,channel_number);
  XsDriveobj.Check("canBusOn",stat);    




   leverArray.data[0]=0;
   leverArray.data[1]=0;
   leverArray.data[2]=0;
   leverArray.data[3]=0;
   leverArray.data[4]=0;
   leverArray.data[5]=0;
   leverArray.data[6]=0;
   leverArray.data[7]=0;
  
  ros::Rate loop_rate(1000);
  int count = 0;
  int btn2Toggel = 0;
  int btn3Toggel = 0;
	while (ros::ok()) {
         
        // leverCmd[0]=leverCmd[0];
        // leverCmd[1]=leverCmd[1];
        // leverCmd[2]=leverCmd[2];
        // leverCmd[3]=leverCmd[3];
        // leverCmd[4]=leverCmd[4];
        // leverCmd[5]=leverCmd[5];
        // leverCmd[6]=leverCmd[6];
        // leverCmd[7]=leverCmd[7];

         XsDriveobj.txAliveMsg(hnd,stat);
         XsDriveobj.getLeverMsg(hnd,leverID,leverCmd);
         XsDriveobj.getBtnMsg(hnd,btnID,btnState);

        
        if(btnState[0]==1){
          buttonState1=!buttonState1;
        }
        if(btnState[1]==1){
          buttonState2=!buttonState2;
        }
        if(btnState[2]==1){
          buttonState3=!buttonState3;
        }
        
        if(btnState[3]==1){
          buttonState4=!buttonState4;
        }
        if(btnState[4]==1){
          buttonState5=!buttonState5;
        }

    



        leverArray.data[0]=leverCmd[0]; buttonArray.data[0]=(uint8_t)buttonState1;
        leverArray.data[1]=leverCmd[1]; buttonArray.data[1]=(uint8_t)btnState[1];
        leverArray.data[2]=leverCmd[2]; buttonArray.data[2]=(uint8_t)btnState[2];
        leverArray.data[3]=leverCmd[3]; buttonArray.data[3]=(uint8_t)btnState[3];
        leverArray.data[4]=leverCmd[4]; buttonArray.data[4]=(uint8_t)btnState[4];
        leverArray.data[5]=leverCmd[5]; buttonArray.data[5]=(uint8_t)btnState[5];
        leverArray.data[6]=leverCmd[6]; buttonArray.data[6]=(uint8_t)btnState[6];
        leverArray.data[7]=leverCmd[7]; buttonArray.data[7]=(uint8_t)btnState[7];
        
        lever_msg.publish(leverArray);    
        button_msg.publish(buttonArray);    
        ros::spinOnce();
	    loop_rate.sleep();
         ++count;
	}
  
  XsDriveobj.closeCanChannel(hnd,stat);


  return 0;
}




