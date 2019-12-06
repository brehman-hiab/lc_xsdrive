#include "ros/ros.h"
#include "std_msgs/String.h"
#include "std_msgs/Int8.h"
#include "std_msgs/Float64.h"
#include <std_msgs/Float64MultiArray.h>
#include <std_msgs/UInt8MultiArray.h>
#include <sensor_msgs/JointState.h>

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

std_msgs::Float64 _slewAngleRef;
std_msgs::Float64 _alphaAngleRef;
std_msgs::Float64 _betaAngleRef;
std_msgs::Float64 _ext1Ref;
std_msgs::Float64 _ext2Ref;
std_msgs::Float64 _ext3Ref;
std_msgs::Float64 _ext4Ref;
std_msgs::Float64 _ext5Ref;
std_msgs::Float64 _ext6Ref;
std_msgs::Float64 _rStabEXTRef;
std_msgs::Float64 _rStabEXTLegRef;
std_msgs::Float64 _lStabEXTRef;
std_msgs::Float64 _lStabEXTLegRef;


sensor_msgs::JointState joint_state;

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
int controlMode;

void ramp(double gain, double bias,double Min, double Max, double remIn, double time, double &ramped);



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

void chatterCallbackMode(const std_msgs::Int8& msg) 
{
//    std::cout<<"raw="<< (double)msg.data[0]<<" conveted="<<msg.data[1]<<"\n";
  controlMode= (int)msg.data;
   std::cout<<"raw="<< (int)msg.data<<" conveted="<<controlMode<<"\n";
  
}


void chatterCallbackscmd(const std_msgs::Float64MultiArray& msg) 
{
    // std::cout<<"raw="<< (double)msg.data[0]<<" conveted="<<msg.data[1]<<"\n"
  lever1= (double)msg.data[0];
  lever2= (double)msg.data[1];
  lever3= (double)msg.data[2];
  lever4= (double)msg.data[3];
  lever5= (double)msg.data[4];
  lever6= (double)msg.data[5];
}


void chatterCallbackjoint(const sensor_msgs::JointState& msg) 
{
    std::cout<<"_alphaAngleRef="<< msg.position[1]<<"\n";
    _slewAngleRef.data  =msg.position[0];
    _alphaAngleRef.data =msg.position[1];
    _betaAngleRef.data  =msg.position[2];
    _ext1Ref.data       =msg.position[3];
    _ext2Ref.data       =msg.position[4];
    _ext3Ref.data       =msg.position[5];
    _ext4Ref.data       =msg.position[6];
    _ext5Ref.data       =msg.position[7];
    _ext6Ref.data       =msg.position[8];
    _rStabEXTRef.data   =msg.position[9];
    _rStabEXTLegRef.data=msg.position[10];
    _lStabEXTRef.data   =msg.position[11];
    _lStabEXTLegRef.data=msg.position[12];
}
// %EndTag(CALLBACK)%
  

int main(int argc, char **argv)
{
  ros::init(argc, argv, "rem_levers");

  ros::NodeHandle n;

  /* subscribe  */
  ros::Subscriber subCMode = n.subscribe("/SimulinkRviz/control_mode", 1000, chatterCallbackMode);
  ros::Subscriber sub = n.subscribe("/SimulinkRviz/leverCmd", 1000, chatterCallbackscmd);
  ros::Subscriber subjointRef = n.subscribe("/SimulinkRviz/my_joint", 1000, chatterCallbackjoint);

  ros::Subscriber subBtn = n.subscribe("/xsDrive/button", 1000, chatterCallbackBtn);
  ros::Publisher joint_pub = n.advertise<sensor_msgs::JointState>("/my_joint",1);



    joint_state.name.resize(25);
	joint_state.position.resize(25);



    joint_state.name[0] ="slew";
	joint_state.name[1] ="alpha";
	joint_state.name[2] ="beta";
	joint_state.name[3] ="ext1";
	joint_state.name[4] ="ext2";
	joint_state.name[5] ="ext3";
    joint_state.name[6] ="ext4";
	joint_state.name[7] ="ext5";
	joint_state.name[8] ="ext6";

	joint_state.name[9] ="r_slider";
	joint_state.name[10] ="r_leg_stroke";
	joint_state.name[11] ="l_slider";
    joint_state.name[12] ="l_leg_stroke";
	


	joint_state.name[13] ="fw1";
	joint_state.name[14] ="fw2";
    joint_state.name[15] ="rw1";
	joint_state.name[16] ="rw2";

	joint_state.name[17] ="left_rear_slider";
	joint_state.name[18] ="left_rear_leg";
	joint_state.name[19] ="right_rear_slider";
    joint_state.name[20] ="right_rear_leg";
	joint_state.name[21] ="rear_left_stab_leg_joint";


	joint_state.name[22] ="rear_right_stab_leg_joint";
	joint_state.name[23] ="front_left_stab_leg_joint";
    joint_state.name[24] ="front_right_stab_leg_joint";

 
	

	
  
  

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


        if (controlMode==1)
        {
            slewBias = _slewAngleRef.data;
            alphaBias = _alphaAngleRef.data;
            betaBias = _betaAngleRef.data;
            extBias = _ext1Ref.data+_ext2Ref.data+
                        _ext3Ref.data+_ext4Ref.data+
                        _ext5Ref.data+_ext6Ref.data;
        }
        

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


      if (controlMode==1)
    {
        joint_state.position[0] = _slewAngleRef.data;
        joint_state.position[1] = _alphaAngleRef.data;
        joint_state.position[2]  =_betaAngleRef.data;
 	    joint_state.position[3]  =_ext1Ref.data;
        joint_state.position[4]  =_ext2Ref.data;
	    joint_state.position[5]  =_ext3Ref.data;
	    joint_state.position[6]  =_ext4Ref.data;
	    joint_state.position[7]  =_ext5Ref.data;
	    joint_state.position[8]  =_ext6Ref.data;
        joint_state.position[9]  =_rStabEXTRef.data ;
	    joint_state.position[10] =_rStabEXTLegRef.data;
	    joint_state.position[11] =_lStabEXTRef.data ;
	    joint_state.position[12] =_lStabEXTLegRef.data;

        // slewAngleRef.data   =
        // alphaAngleRef.data  =
        // betaAngleRef.data   =
        // ext1Ref.data        =
        // ext2Ref.data        =
        // ext3Ref.data        =
        // ext4Ref.data        =
        // ext5Ref.data        =
        // ext6Ref.data        =

        // rStabEXTRef.data    =
        // rStabEXTLegRef.data =
        // lStabEXTRef.data    =
        // lStabEXTLegRef.data =

        std::cout<<"controllMode"<<controlMode<<"sss"<<joint_state.position[1]<<"\n";

    }else
    {
               
	    joint_state.position[0] = slewAngleRef.data;
        joint_state.position[1] = alphaAngleRef.data;
        joint_state.position[2]  =betaAngleRef.data;
 	    joint_state.position[3]  =ext1Ref.data;
        joint_state.position[4]  =ext2Ref.data;
	    joint_state.position[5]  =ext3Ref.data;
	    joint_state.position[6]  =ext4Ref.data;
	    joint_state.position[7]  =ext5Ref.data;
	    joint_state.position[8]  =ext6Ref.data;
	    
        joint_state.position[9]  =rStabEXTRef.data ;
	    joint_state.position[10] =rStabEXTLegRef.data;
	    joint_state.position[11] =lStabEXTRef.data ;
	    joint_state.position[12] =lStabEXTLegRef.data;
    }
	    joint_state.position[13] = 0;
	    joint_state.position[14] = 0;
	    joint_state.position[15] = 0;
	    joint_state.position[16] = 0;
	
        joint_state.position[17] = 0;//"left_rear_slider";
        joint_state.position[18] = 0;//"left_rear_leg";
	    joint_state.position[19] = 0;//"right_rear_slider";
	    joint_state.position[20] = 0;//"right_rear_leg";
	    joint_state.position[21] = 0;//"rear_left_stab_leg_joint";
	    joint_state.position[22] = 0;//"rear_right_stab_leg_joint";
	    joint_state.position[23] = 0;//"front_left_stab_leg_joint";
        joint_state.position[24] =0; //"front_right_stab_leg_joint";

        joint_pub.publish(joint_state);
   

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