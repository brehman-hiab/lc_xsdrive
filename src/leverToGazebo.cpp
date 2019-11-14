#include "ros/ros.h"
#include "std_msgs/String.h"
#include "std_msgs/Float64.h"
#include <std_msgs/Float64MultiArray.h>
#include <std_msgs/UInt8MultiArray.h>
// #include "rosserial_arduino/Adc.h"

std_msgs::Float64 slewAngleRef;
std_msgs::Float64 alphaAngleRef;
std_msgs::Float64 betaAngleRef;

std_msgs::Float64 ext1Ref;
std_msgs::Float64 ext2Ref;
std_msgs::Float64 ext3Ref;
std_msgs::Float64 ext4Ref;
std_msgs::Float64 ext5Ref;
std_msgs::Float64 ext6Ref;


std_msgs::Float64 rStabEXTRef;
std_msgs::Float64 rStabEXTLegRef;
std_msgs::Float64 lStabEXTRef;
std_msgs::Float64 lStabEXTLegRef;

const double  SLEW_MIN=-3.14159;
const double  SLEW_MAX=3.14159;

const double  ALPHA_MIN=-0.9; //1b -60
const double  ALPHA_MAX=1.57;

const double  BETA_MIN=-2.87; //2b 
const double  BETA_MAX=0.123;

const double  EXT6_MAX=2.3;
const double  EXT5_MAX=2.2;
const double  EXT4_MAX=2.1;
const double  EXT3_MAX=2.05;
const double  EXT2_MAX=1.09;
const double  EXT1_MAX=1.85;

const double  rStabEXT_MAX=2.0;
const double  rStabEXTLeg_MAX=1.0;
const double  lStabEXT_MAX=2.0;
const double  lStabEXTLeg_MAX=1.0;


double scalingFunction(double raw, double rawMin, double rawMax, double angMin , double angMax);
double scalingFunctionExt(double raw, double MaxExt);

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

void ramp(double gain, double bias,double Min, double Max, double remIn, double time, double &ramped);


void chatterCallback(const std_msgs::Float64MultiArray& msg) 
{
    // std::cout<<"raw="<< (double)msg.data[0]<<" conveted="<<msg.data[1]<<"\n"
  lever1= (double)msg.data[0];
  lever2= (double)msg.data[1];
  lever3= (double)msg.data[2];
  lever4= (double)msg.data[3];
  lever5= (double)msg.data[4];
  lever6= (double)msg.data[5];
}
void chatterCallbackBtn(const std_msgs::UInt8MultiArray& msg) 
{
//    std::cout<<"raw="<< (double)msg.data[0]<<" conveted="<<msg.data[1]<<"\n";
  btn1= (double)msg.data[0];
  btn2= (double)msg.data[1];
  btn3= (double)msg.data[2];
  btn4= (double)msg.data[3];
  btn5= (double)msg.data[4];
  btn6= (double)msg.data[5];
  btn7= (double)msg.data[6];
}

// %EndTag(CALLBACK)%

int main(int argc, char **argv)
{
  ros::init(argc, argv, "rem_levers");

  ros::NodeHandle n;

  /* subscribe to adc topic from arduino */
  ros::Subscriber sub = n.subscribe("/xsDrive/lever", 1000, chatterCallback);
  ros::Subscriber subBtn = n.subscribe("/xsDrive/button", 1000, chatterCallbackBtn);

  ros::Publisher slewRef_pub = n.advertise<std_msgs::Float64>("/loadercrane/slew_joint_position_controller/command",1000);
  ros::Publisher alphaRef_pub = n.advertise<std_msgs::Float64>("/loadercrane/alpha_joint_position_controller/command",1000);
  ros::Publisher betaRef_pub = n.advertise<std_msgs::Float64>("/loadercrane/beta_joint_position_controller/command",1000);
  
  ros::Publisher ext1Ref1_pub = n.advertise<std_msgs::Float64>("/loadercrane/ext1_joint_position_controller/command",1000);
  ros::Publisher ext1Ref2_pub = n.advertise<std_msgs::Float64>("/loadercrane/ext2_joint_position_controller/command",1000);
  ros::Publisher ext1Ref3_pub = n.advertise<std_msgs::Float64>("/loadercrane/ext3_joint_position_controller/command",1000);
  ros::Publisher ext1Ref4_pub = n.advertise<std_msgs::Float64>("/loadercrane/ext4_joint_position_controller/command",1000);
  ros::Publisher ext1Ref5_pub = n.advertise<std_msgs::Float64>("/loadercrane/ext5_joint_position_controller/command",1000);
  ros::Publisher ext1Ref6_pub = n.advertise<std_msgs::Float64>("/loadercrane/ext6_joint_position_controller/command",1000);

  ros::Publisher rStabExt_pub = n.advertise<std_msgs::Float64>("/loadercrane/r_slider_joint_position_controller/command",1000);
  ros::Publisher rStabExtLeg_pub = n.advertise<std_msgs::Float64>("/loadercrane/r_leg_stroke_joint_position_controller/command",1000);
  ros::Publisher lStabExt_pub = n.advertise<std_msgs::Float64>("/loadercrane/l_slider_joint_position_controller/command",1000);
  ros::Publisher lStabExtLeg_pub = n.advertise<std_msgs::Float64>("/loadercrane/l_leg_stroke_joint_position_controller/command",1000);

  
  
  
  

  double slewBias = 0;
  double alphaBias = 0;
  double betaBias = 0;
  double extBias = 0;
  double rStabextBias=0;
  double rStabextLegBias=0;
  double lStabextBias=0;
  double lStabextLegBias=0;

  double ramped =0;
  double gain=1000000;

  double rawExt=0;
  double ext1;
  double ext2;
  double ext3;
  double ext4;
  double ext5;
  double ext6;

  double extMax = EXT1_MAX+EXT2_MAX+EXT3_MAX+EXT4_MAX+EXT5_MAX+EXT6_MAX;
  int slew_count=0;
  int alpha_count=0;
  int beta_count=0;
  int ext_count=0;
  int rStabext_count=0;
  int rStabextLeg_count=0;
  int lStabext_count=0;
  int lStabextLeg_count=0;
  ros::Rate loop_rate(100);

	while (ros::ok()) {
	  //update 
    if (btn1==1)
      {
        ramp(gain/10, rStabextBias,0,rStabEXT_MAX,-1*lever2, rStabext_count,ramped);
        rStabextBias = ramped;
        rStabEXTRef.data = ramped;   

        if (lever2==(double)0)
           {
               rStabext_count=0;       
           }
        rStabext_count+=1;    

        ramp(gain/10, rStabextLegBias,0,rStabEXTLeg_MAX,-1*lever3, rStabextLeg_count,ramped);
        rStabextLegBias = ramped;
        rStabEXTLegRef.data = ramped;   
        
        
        
        if (lever3==(double)0)
           {
               rStabextLeg_count=0;       
           }
        rStabextLeg_count+=1;    

        ramp(gain/10, lStabextBias,0,lStabEXT_MAX,-1*lever5, lStabext_count,ramped);
        lStabextBias = ramped;
        lStabEXTRef.data = ramped;   
      
         
        
        if (lever5==(double)0)
           {
               lStabext_count=0;       
           }
        lStabext_count+=1;   

        ramp(gain/10, lStabextLegBias,0,lStabEXTLeg_MAX,-1*lever4, lStabextLeg_count,ramped);
        lStabextLegBias = ramped;
        lStabEXTLegRef.data = ramped;   
        
        if (lever3==(double)0)
           {
               lStabextLeg_count=0;       
           }
        lStabextLeg_count+=1;     

        // std::cout<<"Btn 1 is active"<<"\n";
      }else
      {
        ramp(gain, slewBias, SLEW_MIN,SLEW_MAX, lever1, slew_count, ramped);    
        slewBias = ramped;
        slewAngleRef.data = ramped;   
             

        ramp(gain, alphaBias,ALPHA_MIN,ALPHA_MAX, lever2, alpha_count,ramped);    
        alphaBias = ramped;
        alphaAngleRef.data = ramped;     
        

        ramp(gain, betaBias,BETA_MIN,BETA_MAX,lever3, beta_count,ramped);    
        betaBias = ramped;
        betaAngleRef.data = ramped;
        

        ramp(gain, extBias,0,extMax,-1*lever4, ext_count,ramped);    
        extBias = ramped;
        rawExt =extBias;
        ext6 = scalingFunctionExt(rawExt,EXT6_MAX)/1;
        rawExt=rawExt-ext6;
        ext5 = scalingFunctionExt(rawExt,EXT5_MAX)/1; 
        rawExt = rawExt - ext5;
        ext4 = scalingFunctionExt(rawExt,EXT4_MAX)/1;   
        rawExt = rawExt - ext4;
        ext3 = scalingFunctionExt(rawExt,EXT3_MAX)/1;   
        rawExt = rawExt - ext3;
        ext2 = scalingFunctionExt(rawExt,EXT2_MAX)/1;   
        rawExt = rawExt - ext2;
        ext1 = scalingFunctionExt(rawExt,EXT1_MAX)/1;  

        ext1Ref.data=ext1/1;   
        ext2Ref.data=ext2/1;
        ext3Ref.data=ext3/1;
        ext4Ref.data=ext4/1;
        ext5Ref.data=ext5/1;
        ext6Ref.data=ext6/1;

        

         if (lever1==(double)0)
            {
                slew_count=0;       
            }
         if (lever2==(double)0)
            {
                alpha_count=0;       
            }
        if (lever3==(double)0)
            {
                beta_count=0;       
            } 
         if (lever4==(double)0)
            {
                ext_count=0;       
            }         
      slew_count +=1;
      alpha_count +=1;
      beta_count+=1;
      ext_count+=1;

        rStabEXTRef.data = rStabextBias;   
        rStabEXTLegRef.data = rStabextLegBias;   
        lStabEXTRef.data = lStabextBias;  
        lStabEXTLegRef.data = lStabextLegBias;
      }
      
        slewRef_pub.publish(slewAngleRef);
        alphaRef_pub.publish(alphaAngleRef);
        betaRef_pub.publish(betaAngleRef);
    
        ext1Ref1_pub.publish(ext1Ref);
        ext1Ref2_pub.publish(ext2Ref);
        ext1Ref3_pub.publish(ext3Ref);
        ext1Ref4_pub.publish(ext4Ref);
        ext1Ref5_pub.publish(ext5Ref);
        ext1Ref6_pub.publish(ext6Ref);    
        rStabExt_pub.publish(rStabEXTRef);     
        rStabExtLeg_pub.publish(rStabEXTLegRef);     
        lStabExt_pub.publish(lStabEXTRef);     
        lStabExtLeg_pub.publish(lStabEXTLegRef);     

         ros::spinOnce();
	    loop_rate.sleep();
	}
  
  


  return 0;
}

void ramp(double gain, double bias,double Min, double Max, double remIn, double time, double &ramped){

    double slope=1;
    double finalSetPoint=0;
     if (remIn == (double)0)
    {
        finalSetPoint = bias;
    }else if (remIn>(double)0)
    {
        finalSetPoint = Min;

    }else if (remIn<(double)0)
    {
        finalSetPoint = Max;
    }


    if (remIn == 0)
    {
        ramped = bias;
    }else if (remIn>0)
    {
        // finalSetPoint = min;
        slope = (finalSetPoint-bias)*(remIn)/gain;   
        ramped = bias+(slope*(time));    
        if (ramped < finalSetPoint)
        {
            ramped= finalSetPoint;
        }       
    }
    else
    {
        // finalSetPoint = max;
        slope = (bias-finalSetPoint)*(remIn)/gain;   
        ramped = bias+(slope*(time));    
        if (ramped > finalSetPoint)
        {
            ramped= finalSetPoint;
        }     
    }
    
    
}


double scalingFunction(double raw, double rawMin, double rawMax, double angMin , double angMax)
{

  double conveted = 0;
  conveted = (angMax-angMin)*(raw-rawMin)/(rawMax-rawMin)+angMin;

  if(conveted>angMax)
  {
    return angMax;
  }
  else if(conveted<angMin)
  {
    return angMin;
  }
  else
  {
    return  conveted;
  }
 
}

double scalingFunctionExt(double raw, double MaxExt)
{
  

  if(raw>MaxExt)
  {
    return MaxExt;
  }
  else if(raw<0)
  {
    return 0;
  }
  else
  {
    return raw;
  }


}