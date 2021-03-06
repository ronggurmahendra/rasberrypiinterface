#include "vimana.h"
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <time.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
using namespace std;

#define MAX7219_REG_NOOP 0x00
#define MAX7219_REG_DIGIT0 0x01
#define MAX7219_REG_DIGIT1 0x02
#define MAX7219_REG_DIGIT2 0x03
#define MAX7219_REG_DIGIT3 0x04
#define MAX7219_REG_DIGIT4 0x05
#define MAX7219_REG_DIGIT5 0x06
#define MAX7219_REG_DIGIT6 0x07
#define MAX7219_REG_DIGIT7 0x08
#define MAX7219_REG_DECODEMODE 0x09
#define MAX7219_REG_INTENSITY 0x0A
#define MAX7219_REG_SCANLIMIT 0x0B
#define MAX7219_REG_SHUTDOWN 0x0C
#define MAX7219_REG_DISPLAYTEST 0x0F

#define CMD_WRITE 0x40

typedef struct
{

 int digits[8];

} max7219;


max7219 header;

int spi_fd;
struct sigaction act;
static char *spiDevice = "/dev/spidev0.1";
static uint8_t spiBPW = 8;
static uint32_t spiSpeed = 5000000;
static uint16_t spiDelay = 0;

int spiOpen(char* dev)
{
  if((spi_fd = open(dev, O_RDWR)) < 0)
  {
    printf("error opening %s\n",dev);
    return -1;
  }
  return 0;
}

void writeByte(uint8_t reg, uint8_t data)
{
  uint8_t spiBufTx [3];
  uint8_t spiBufRx [3];
  struct spi_ioc_transfer spi;
  memset (&spi, 0, sizeof(spi));
  spiBufTx [0] = CMD_WRITE;
  spiBufTx [1] = reg;
  spiBufTx [2] = data;
  spi.tx_buf =(unsigned long)spiBufTx;
  spi.rx_buf =(unsigned long)spiBufRx;
  spi.len = 3;
  spi.delay_usecs = spiDelay;
  spi.speed_hz = spiSpeed;
  spi.bits_per_word = spiBPW;
  ioctl(spi_fd, SPI_IOC_MESSAGE(1), &spi);
}

void writeBytes(max7219 *header, int cmd, int data)
{
  if(!header) return;
  writeByte(cmd, data);
}

void setBrightness(max7219 *header, int bright)
{
  if(!header) return;
  if (bright < 0) bright = 0;
  if (bright > 15) bright = 15;

  writeBytes(header, MAX7219_REG_INTENSITY, bright);
}

void initialiseDisplay(max7219 *header)
{
  spiOpen(spiDevice);

  writeBytes(header, MAX7219_REG_SCANLIMIT, 7);
  writeBytes(header, MAX7219_REG_DECODEMODE, 0xff);
  writeBytes(header, MAX7219_REG_SHUTDOWN, 1);
  writeBytes(header, MAX7219_REG_DISPLAYTEST, 0);
  setBrightness(header, 5);
}

void byte_to_binary(int x)
{
    static char b[9];
    b[0] = '\0';

    int z;
    for (z = 128; z > 0; z >>= 1)
    {
        strcat(b, ((x & z) == z) ? "1" : "0");
    }
    printf("%s", b);
}

void digitDisplay(max7219 *header)
{
  int i;
  if(!header) return;
  for (i=0; i<8; i++)
  {
    writeBytes(header, i+1, header->digits[i]);
  }
}

void clearDisplay(max7219 *header)
{
  int i;
  if(!header) return;
  for (i=0; i<8; i++) {
    header->digits[i] = 0x0F;
  }
  digitDisplay(header);
}

uint8_t asciiToBCD (uint8_t chr)
{
  uint8_t bcd_value;
  bcd_value = (chr - 48);
  return bcd_value;
}

void writeDigits(max7219 *header, char chars[12])
{
  int i,j;
  if (!header) return;
  i = 7;
  j = 0;
  while (j <= 11 && i >= 0)
  {
    switch ((int)chars[j]) {
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':

      header->digits[i] = asciiToBCD(chars[j]);
      j++;
      i--;
      break;
    case ' ':
      header->digits[i] = 0x0F;
      j++;
      i--;
      break;
    case '-':
      header->digits[i] = 0x0a;
      j++;
      i--;
      break;
    case ':':
      header->digits[i] = 0x8f;
      j++;
      i--;
      break;
    case '.':
      header->digits[i+1] = header->digits[i+1] + 0x80;
      j++;
      break;
    case 0x00:
      j = 12;
      i = -1;
      break;
    default:
      j++;
      break;
    }
  }
  digitDisplay(header);
}

void sig_handler(int signum, siginfo_t *info, void *ptr)
{
  clearDisplay(&header);
  printf("Received signal %d\n", signum);
  printf("Signal originated from process %lu\n",
    (unsigned long)info->si_pid);
  exit(0);
}

#define SPI_CHANNEL 0
#define SPI_CLOCK_SPEED 1000000
//constructor, destructor
Vimana::Vimana(){
    //attitude
     this->pitch = 0;
     this->roll = 0;
     this->yaw = 0;
     this->rollspeed = 0;
     this->pitchspeed = 0;
     this->yawspeed = 0;
    
    //global post int
     this->heading = 0;
     this->lat = 0;
     this->lon = 0;
     this->relative_alt = 0;
     this->vx = 0;
     this->vy = 0;
     this->vz = 0;
    
    
} //vimana object constructor
Vimana::~Vimana(){
} //vimana object destructor
    
//data retrival
double Vimana::getPitch(){
    return this->pitch;

} // returns vimana's Pitch in rad
double Vimana::getRoll(){
    return this->roll;
    
} // returns vimana's Roll in rad
double Vimana::getYaw(){
    return this->yaw;
} // returns vimana's Yaw in degE7
double Vimana::getPitchSpeed(){
    return this->pitchspeed;
} // returns vimana's Pitch in rad
double Vimana::getRollSpeed(){
    return this->rollspeed;
} // returns vimana's Roll in rad
double Vimana::getYawSpeed(){
    return this->yawspeed;
} // returns vimana's Yaw in degE7

//get global post int
long[] Vimana::getGPS(){
    return [this->lat,this->lon];
} // returns vimana's Positio 
long Vimana::getRelativeAlt(){
    return this->relative_alt;
} // returns vimana's relative altitude in mm
long Vimana::getSpeed(){
    return this->vx;
} // returns vimana's Speed in mm/s
long Vimana::getVx(){
    return this->vx;
}
long Vimana::getVy(){
    return this->vy;
}
long Vimana::getVz(){
    return this->vz;
}
double Vimana::heading(){
    return this->heading;
}

//data transmit
int Vimana::commandGoTo(long longitude, long latitude,long altitude){
    return 1;
} // sends command to vimana to go to longitude, latitude, altitude provided returns command ack
int Vimana::commandLoiter(long longitude, long latitude,long altitude,double radius){
    return 1;
} // 
int Vimana::commandSetHeading(double Heading){
    return 1;
} // command vimana to go certain heading returns command ack

//utilities 
double distanceToCoor(long longitude, long latitude){
    return 123;
}