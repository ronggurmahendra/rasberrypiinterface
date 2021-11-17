#ifndef vimana_H
#define vimana_H

#include <iostream>
#include <wiringPiSPI.h>



class Vimana {
public:
    //constructor, destructor
    Vimana(); //vimana object constructor
    ~Vimana(); //vimana object destructor

    //data retrival
    double getPitch(); // returns vimana's Pitch in rad
    double getRoll(); // returns vimana's Roll in rad
    double getYaw(); // returns vimana's Yaw in degE7
    double getPitchSpeed(); // returns vimana's Pitch in rad
    double getRollSpeed(); // returns vimana's Roll in rad
    double getYawSpeed(); // returns vimana's Yaw in degE7

    //get global post int
    long[2] getGPS(); // returns vimana's Positio 
    long getRelativeAlt(); // returns vimana's relative altitude in mm
    long getSpeed(); // returns vimana's Speed in mm/s
    long getVx();
    long getVy();
    long getVz();
    double heading();

    //data transmit
    int commandGoTo(long longitude, long latitude,long altitude); // sends command to vimana to go to longitude, latitude, altitude provided returns command ack
    int commandLoiter(long longitude, long latitude,long altitude,double radius); // 
    int commandSetHeading(double Heading); // command vimana to go certain heading returns command ack

private:
    //attitude
    double pitch;
    double roll;
    double yaw;
    double rollspeed;
    double pitchspeed;
    double yawspeed;
    
    //global post int
    double heading;
    long lat;
    long lon;
    long relative_alt;
    long vx;
    long vy;
    long vz;
    
    
}
#endif