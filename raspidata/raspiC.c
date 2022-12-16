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
#define SERIAL_PORT "/dev/ttyAMA0"

unsigned short mask_h = 65408; //マスク処理上8bits(0b1111111110000000)
unsigned short mask_l = 127;   //マスク処理下8bits(0b0000000001111111)
unsigned char tx[3];
unsigned char rx[10];

int opens(){
    int fd;
 	int rv; 
    struct termios tio;
    struct timeval timeout;

    timeout.tv_sec = 0;
    timeout.tv_usec = 10000;

    fd_set set;

    if ((fd = open(SERIAL_PORT, O_RDWR)) < 0)
    {
        fprintf(stderr, "open error\n");
        exit(1);
    }

    FD_ZERO(&set);
    FD_SET(fd, &set);

    bzero(&tio, sizeof(tio));

    /* 115200bps, フロー制御有り, ８ビット，DTR/DSR無効，受信可能 */
    tio.c_cflag = B115200 | CRTSCTS | CS8 | CLOCAL | CREAD | PARENB; 
    tio.c_cc[VMIN] = 1; /* 入力データをバッファしない */
    tcsetattr(fd, TCSANOW, &tio); /* アトリビュートのセット */

    fprintf(stderr, "*** check *** bbb ***\n");	


    return fd;
}

void sleepms(double i){
	usleep(i * 1000);
}

unsigned short servo_position(int fd, unsigned char id,int pos){
    tx[0] = 0x80|id;
    tx[1] = (unsigned char)((pos & mask_h)>>7);
    tx[2] = (unsigned char)(pos & mask_l);
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
	for(int i=0;i<3;i++)
    {
        write(fd, &tx[i], sizeof(tx[i]));
    }
    return 0;
}

void close_motion(int fd){
	int j;
	printf("moob1\n"); 
    for(j=1;j<13;j++){
        servo_speed(fd, j, 30);
    }
    for(j = 0; j < 10; j++){ 
		servo_position(fd, 1,10167);
    	servo_position(fd, 4,10167);
    	servo_position(fd, 7,10167);
    	servo_position(fd, 10,10167);
	}
	sleepms(1000);

	printf("moob2\n"); 
    for(j=1;j<13;j++){
        servo_speed(fd, j,30);
    }
    for(j = 0; j < 10; j++){ 
    	servo_position(fd, 1,10167);
    	servo_position(fd, 4,10167);
    	servo_position(fd, 7,10167);
    	servo_position(fd, 10,10167);
    	servo_position(fd, 2,7600);
    	servo_position(fd, 5,7600);
    	servo_position(fd, 8,7600);
    	servo_position(fd, 11,7600);
    } 
	sleepms(1000);

	printf("moob3\n"); 
    for(j=1;j<13;j++){
        servo_speed(fd, j,30);
    }
    for(j = 0; j < 10; j++){ 
    	servo_position(fd, 1,10167);
    	servo_position(fd, 4,10167);
    	servo_position(fd, 7,10167);
    	servo_position(fd, 10,10167);
    	servo_position(fd, 2,7600);
    	servo_position(fd, 5,7600);
    	servo_position(fd, 8,7600);
    	servo_position(fd, 11,7600);
    	servo_position(fd, 3,11500);
    	servo_position(fd, 6,11500);
    	servo_position(fd, 9,11500);
    	servo_position(fd, 12,11500);
	}	
	sleepms(1000);

	printf("moob4\n"); 
    for(j=1;j<13;j++){
        servo_speed(fd, j,30);
    }	
    for(j = 0; j < 10; j++){ 
    	servo_position(fd, 1,10167);
    	servo_position(fd, 4,10167);
    	servo_position(fd, 7,10167);
    	servo_position(fd, 10,10167);
    	servo_position(fd, 3,11500);
    	servo_position(fd, 6,11500);
    	servo_position(fd, 9,11500);
    	servo_position(fd, 12,11500);
    	servo_position(fd, 2,4933);
    	servo_position(fd, 5,4933);
    	servo_position(fd, 8,4933);
    	servo_position(fd, 11,4933);
	}
	sleepms(1000);
    return;
}

int main(){
    int po = 1, i, fd = 0;
    char ch =0;
    wiringPiSetupGpio();
    pullUpDnControl(14,PUD_UP);
    pullUpDnControl(15,PUD_UP);
    usleep(50*1000);
    fd = opens(); 
    
    while(1){
		scanf(" %c",&ch);
   	    if(ch > 0x2f){ 
			switch(ch){
				case 'c':
					close_motion(fd);
	    			sleepms(50);
            		ch = 0;
            		printf("fin c\n");
					break;	
            
				case 'o':

	
				case '1':
	    			printf("start\n");
            		for(i = 3; i <= 12; i +=3){
                		servo_speed(fd, i,50);
                		servo_position(fd, i,10110);
            		}
	    			sleepms(50);
					ch=0;
            		break;
            		
				case 'f':
	    			close(fd);  
	    			exit(1);
	    			break;
            
	    		default:
            		printf("def\n");
	    			ch = 0;
	    			break;
        	}
		}
    }
	return 0;
}

