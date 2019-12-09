#include "ros/ros.h"
#include "std_msgs/String.h"
#include "std_msgs/Float64.h"
#include <std_msgs/Float64MultiArray.h>
#include <std_msgs/UInt8MultiArray.h>
#include "lc_xsdrive/XsDriveCAN.hpp"
#include <sensor_msgs/JointState.h>


double len1;
double len2;
double len3;
double len4;
double slewAngle;


void getLenMsg( double length,uint16_t (&lenMsg)[8]);
void getSlewToLenMsg( double length,uint16_t (&lenMsg)[8]);
void callback(const sensor_msgs::JointStateConstPtr& msg)
{

	slewAngle  =(double)msg->position[20]; // this needs to be updated if added more joints, this is last in joint state topic






	// joint_state.position[1]  =msg->position[1];
	// joint_state.position[2]  =msg->position[2];
 
	// joint_state.position[3]  =msg->position[3];
    // joint_state.position[4]  =msg->position[4];
	// joint_state.position[5]  =msg->position[5];


	// joint_state.position[6]  =msg->position[6];
	// joint_state.position[7]  =msg->position[7];
	// joint_state.position[8]  =msg->position[8];


	len1 =(double)msg->position[12]*10000; //LEN_MLLL
	len2 =(double)msg->position[13]*10000; //LEN_MLLE
	len3 =(double)msg->position[14]*10000; //LEN_MRLL
	len4 =(double)msg->position[15]*10000; //LEN_MRLE

    // std::cout
    //     <<" len1 = "<<len1
    //     <<"\t len2 = "<<len2
    //     <<"\t len3 = "<<len3
    //     <<"\t len4 = "<<len4
    //     <<"\n";
	// joint_state.position[13] = msg->position[13];
	// joint_state.position[14] = msg->position[14];
	// joint_state.position[15] = msg->position[15];
	// joint_state.position[16] = msg->position[16];
	// joint_state.position[17] = msg->position[17];
	

	// joint_state.position[18] = msg->position[18];
	// joint_state.position[19] = msg->position[19];
	// joint_state.position[20] = msg->position[20];
	// joint_state.position[21] = msg->position[21];
	// joint_state.position[22] = msg->position[22];
	// joint_state.position[23] = msg->position[23];

    // joint_state.position[24] = msg->position[24];
	
    

}
// %EndTag(CALLBACK)%

int main(int argc, char **argv)
{
  ros::init(argc, argv, "xsDrive");
  ros::NodeHandle n;

/*---------------------------------------*/
  // Holds a handle to the CAN channel
  canHandle hnd;
  // Status returned by the Canlib calls
  canStatus stat;
  // The CANlib channel number we would like to use
  int channel_number = 0;
  long len1ID=1921;
  long len2ID=1922;
  long lenSlewID=1927;

  
  uint16_t lenMsg[8]={1,2,0,0,0,0,0,0};
  
  
  
  XsDriveCAN XsDriveobj; 

/*---------------------------------------*/

  /* subscribe to adc topic from Gazebo */
    ros::Subscriber sub = n.subscribe<sensor_msgs::JointState>("/loadercrane/joint_states", 1, callback);
       
  XsDriveobj.openCanChannel(hnd,stat,channel_number);
  XsDriveobj.Check("canBusOn",stat);    



  
  ros::Rate loop_rate(1000);
  int count = 0;
	while (ros::ok()) {
         


        getSlewToLenMsg(slewAngle,lenMsg);
        XsDriveobj.txLenSensorMsg(hnd,stat,lenSlewID,lenMsg);


        getLenMsg(len2,lenMsg); //MLLE
        XsDriveobj.txLenSensorMsg(hnd,stat,len1ID,lenMsg);


        getLenMsg(len4,lenMsg); //MRLE
        XsDriveobj.txLenSensorMsg(hnd,stat,len2ID,lenMsg);
        

        ros::spinOnce();
	    loop_rate.sleep();
         ++count;
	}
  
  XsDriveobj.closeCanChannel(hnd,stat);


  return 0;
}
void getSlewToLenMsg( double slew,uint16_t (&lenMsg)[8]){

    double uMin = 41.5;
    double uMax = 830;

    double angMin = -203;
    double angMax = 203;

    double length = ((((slew*180/3.1416-angMin)*(uMax-uMin))/(angMax-angMin))+uMin)*10;

    std::cout<<"slew[deg]="<<slew*180/3.1416<<"    converted="<<length<<"\n";
    
    
    lenMsg[0] = (uint16_t)length & 0xFF; 
    lenMsg[1] = (((uint16_t)length & 0xFF00)>>8); 
    lenMsg[2] = 0; 
    lenMsg[3] = 0; 
    lenMsg[4] = 0; 
    lenMsg[5] = 0; 
    lenMsg[6] = 0; 
    lenMsg[7] = 0; 
}

void getLenMsg( double length,uint16_t (&lenMsg)[8]){

        lenMsg[0] = (uint16_t)length & 0xFF; 
        lenMsg[1] = (((uint16_t)length & 0xFF00)>>8); 
        lenMsg[2] = 0; 
        lenMsg[3] = 0; 
        lenMsg[4] = 0; 
        lenMsg[5] = 0; 
        lenMsg[6] = 0; 
        lenMsg[7] = 0; 
}


