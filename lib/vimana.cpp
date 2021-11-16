#include "vimana.h"
#include <iostream>

using namespace std;

//constructor, destructor
Vimana::Vimana(){
} //vimana object constructor
Vimana::~Vimana(){
} //vimana object destructor
   
//data retrival
double Vimana::getPitch(){

} // returns vimana's Pitch in rad
double Vimana::getRoll(){
    
} // returns vimana's Roll in rad
double Vimana::getYaw(){

} // returns vimana's Yaw in degE7
double Vimana::getPitchSpeed(){

} // returns vimana's Pitch in rad
double Vimana::getRollSpeed(){

} // returns vimana's Roll in rad
double Vimana::getYawSpeed(){

} // returns vimana's Yaw in degE7

//get global post int
long[2] Vimana::getGPS(){

} // returns vimana's Positio 
long Vimana::getRelativeAlt(){

} // returns vimana's relative altitude in mm
long Vimana::getSpeed(){

} // returns vimana's Speed in mm/s
long Vimana::getVx(){

}
long Vimana::getVy(){

}
long Vimana::getVz(){

}
double Vimana::heading(){

}

//data transmit
int Vimana::commandGoTo(long longitude, long latitude,long altitude){

} // sends command to vimana to go to longitude, latitude, altitude provided returns command ack
int Vimana::commandLoiter(long longitude, long latitude,long altitude,double radius){

} // 
int Vimana::commandSetHeading(double Heading){
    
} // command vimana to go certain heading returns command ack
