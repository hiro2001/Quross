#include <wiringPi.h>
#include <wiringSerial.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <termio.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>
#include <sys/select.h>

#define BLINKING_RATE_MS 500
#define BUFFERDSIZE 5

/*RawSerial serial_servo(D1,D0);
RawSerial pc(USBTX,USBRX);*/
//static UnbufferedSerial serial_servo(D1, D0, 115200);
unsigned short mask_h = 65408; //マスク処理上8bits(0b1111111110000000)
unsigned short mask_l = 127;   //マスク処理下8bits(0b0000000001111111)
int j;
int n;
int p=1;
char ch;
unsigned char tx[3];
unsigned char rx[10];

int opens(){
        int fd;
	if((fd = serialOpen("/dev/ttyAMA0",115200)) < 0)
	{
        	fprintf(stderr, "open error\n");
        	exit(1);
        }
//	wiringPiSerup();
	fflush(stdout);
	return fd;
}

unsigned short servo_position(int fd, unsigned char id,int pos){
    tx[0] = 0x80|id;
    tx[1] = (unsigned char)((pos & mask_h)>>7);
    tx[2] = (unsigned char)(pos & mask_l);
    usleep(1.5*100000);
    for(int i=0;i<3;i++)
    {
        write(fd, &tx[i], sizeof(tx[i]));
    }
    return 0;
}

unsigned short servo_speed(int fd, unsigned char id,int speed){
        tx[0] = 0xC0|id;
        tx[1] = 0x02;
        tx[2] = 0x00|speed;
        usleep(1.5*100000);
	for(int i=0;i<3;i++)
        {
            write(fd, &tx[i], sizeof(tx[i]));
        }
        return 0;
}

int main(){
    int po = 1, i, fd = 0;
    usleep(50*1000);
    //serial_servo.baud(115200);
    //serial_servo.format(8,SerialBase::Even,1);
    fd = opens(); 
    
    
    while(1){
	scanf("%c",&ch);
        switch(ch){

            case 'q':
	    printf("start");
/*            for(i = 3; i <= 12; i +=3){
                servo_speed(fd, i,50);
                servo_position(fd, i,10110);
            }*/
            i = 9;
            servo_speed(fd, i,50);
            servo_position(fd, i,6500);
            usleep(0.5*1000000);
	    ch=0;
            break;
            case 's':
	    exit(1);
	    break;
            
	    default:
            break;
        }
    }
		digitalWrite(18, 0);
		return 0;
}

