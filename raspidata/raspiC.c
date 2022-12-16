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
    for(int i = 0;i < 10; i++){ 
		for(int i=0;i<3;i++)
    	{
        	write(fd, &tx[i], sizeof(tx[i]));
    	}
    }
	return 0;
}

unsigned short servo_speed(int fd, unsigned char id,int speed){
    tx[0] = 0xC0|id;
    tx[1] = 0x02;
    tx[2] = 0x00|speed;
	for(int i = 0;i < 10; i++){
		for(int i=0;i<3;i++)
   		{
        	write(fd, &tx[i], sizeof(tx[i]));
    	}
	}
    return 0;
}

void close_motion(int fd){
	int j;
	printf("moob1\n"); 
    for(j=1;j<13;j++){
        servo_speed(fd, j, 30);
    }
	servo_position(fd, 1,10167);
   	servo_position(fd, 4,10167);
   	servo_position(fd, 7,10167);
   	servo_position(fd, 10,10167);

	sleepms(1000);

	printf("moob2\n"); 
    for(j=1;j<13;j++){
        servo_speed(fd, j,30);
    }
   	servo_position(fd, 1,10167);
   	servo_position(fd, 4,10167);
   	servo_position(fd, 7,10167);
   	servo_position(fd, 10,10167);
   	servo_position(fd, 2,7600);
   	servo_position(fd, 5,7600);
   	servo_position(fd, 8,7600);
   	servo_position(fd, 11,7600);
    
	sleepms(1000);

	printf("moob3\n"); 
    for(j=1;j<13;j++){
        servo_speed(fd, j,30);
    }
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
	
	sleepms(1000);

	printf("moob4\n"); 
    for(j=1;j<13;j++){
        servo_speed(fd, j,30);
    }	
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
	sleepms(1000);
}

void open_motion_crawl(int fd){
    int open_crawl[10][12]={{8780,7600,10277,8780,7600,10277,8780,7600,10277,8780,7600,10277},
    {8741,7600,10260,8741,7600,10260,8741,7600,10260,8741,7600,10260},
    {8701,7600,10243,8701,7600,10243,8701,7600,10243,8701,7600,10243},
    {8662,7600,10225,8662,7600,10225,8662,7600,10225,8662,7600,10225},
    {8622,7600,10207,8622,7600,10207,8622,7600,10207,8622,7600,10207},
    {8583,7600,10188,8583,7600,10188,8583,7600,10188,8583,7600,10188},
    {8544,7600,10169,8544,7600,10169,8544,7600,10169,8544,7600,10169},
    {8505,7600,10150,8505,7600,10150,8505,7600,10150,8505,7600,10150},
    {8466,7600,10130,8466,7600,10130,8466,7600,10130,8466,7600,10130},
    {8428,7600,10110,8428,7600,10110,8428,7600,10110,8428,7600,10110}};
	int open_count = 0;
	int open_crawl_max = 10;	
	int j;

    for(j=1;j<13;j++){
        servo_speed(fd, j,50);
    }
    servo_position(fd, 2,7600);
    servo_position(fd, 5,7600);
    servo_position(fd, 8,7600);
    servo_position(fd, 11,7600);
	sleepms(500);
    
    for(j=1;j<13;j++){
        servo_speed(fd, j,50);
    }
    servo_position(fd, 3,10294);
    servo_position(fd, 6,10294);
    servo_position(fd, 9,10294);
    servo_position(fd, 12,10294);
	sleepms(60);

    for(j=1;j<13;j++){
        servo_speed(fd, j,50);
    }
    servo_position(fd, 1,8820);
    servo_position(fd, 4,8820);
    servo_position(fd, 7,8820);
    servo_position(fd, 10,8820);
	sleepms(100);
    
    while(open_count<open_crawl_max){
        for(j=1;j<13;j++){
        	servo_speed(fd, j,50);
        }
        servo_position(fd, 10,open_crawl[open_count][0]);
        servo_position(fd, 11,open_crawl[open_count][1]);
        servo_position(fd, 12,open_crawl[open_count][2]);
        servo_position(fd, 1,open_crawl[open_count][3]);
        servo_position(fd, 2,open_crawl[open_count][4]);
        servo_position(fd, 3,open_crawl[open_count][5]);
        servo_position(fd, 7,open_crawl[open_count][6]);
        servo_position(fd, 8,open_crawl[open_count][7]);
        servo_position(fd, 9,open_crawl[open_count][8]);
        servo_position(fd, 4,open_crawl[open_count][9]);
        servo_position(fd, 5,open_crawl[open_count][10]);
        servo_position(fd, 6,open_crawl[open_count][11]);
        open_count++;
		sleepms(20);
    }
    open_count=0;
}

void open_motion(int fd){
	int j;
    for(j=1;j<13;j++){
        servo_speed(fd, j,50);
    }
	printf("motion1\n");
    servo_position(fd, 2,7600);
    servo_position(fd, 5,7600);
    servo_position(fd, 8,7600);
    servo_position(fd, 11,7600);
	sleepms(50);   
 
    for(j=1;j<13;j++){
        servo_speed(fd, j,50);
    }
	printf("motion2\n");
    servo_position(fd, 3,10294);
    servo_position(fd, 6,10294);
    servo_position(fd, 9,10294);
    servo_position(fd, 12,10294);
	sleepms(60);   

    for(j=1;j<13;j++){
        servo_speed(fd, j,50);
    }
    servo_position(fd, 1,8820);
    servo_position(fd, 4,8820);
    servo_position(fd, 7,8820);
    servo_position(fd, 10,8820);
	sleepms(100);   
}

void stand_set_motion(int fd){
    for(int j=1;j<13;j++){
        servo_speed(fd, j,30);

    }
	printf("motion1\n");
    servo_position(fd, 2,7500);
    servo_position(fd, 5,7500);
    servo_position(fd, 8,7500);
    servo_position(fd, 11,7500);
	sleepms(500);  
	printf("motion2\n"); 
    servo_position(fd, 1,9870);
    servo_position(fd, 3,10922);
    servo_position(fd, 4,9870);
    servo_position(fd, 6,10922);
    servo_position(fd, 7,9870);
    servo_position(fd, 9,10922);
    servo_position(fd, 10,9870);
    servo_position(fd, 12,10922);
	sleepms(500);   
}

void stand_motion(int fd){
    for(int j=1;j<13;j++){
        servo_speed(fd, j,30);
    }
	printf("motion1\n");    
    servo_position(fd, 1,9870);
    servo_position(fd, 3,8922);
    servo_position(fd, 4,9870);
    servo_position(fd, 6,8922);
    servo_position(fd, 7,9870);
    servo_position(fd, 9,8922);
    servo_position(fd, 10,9870);
    servo_position(fd, 12,8922);
	sleepms(500);  
	printf("motion2\n"); 
    servo_position(fd, 1,6820);
    servo_position(fd, 4,6820);
    servo_position(fd, 7,6820);
    servo_position(fd, 10,6820);
	sleepms(500);  
	printf("motion3\n"); 
    servo_position(fd, 3,4922);
    servo_position(fd, 6,4922);
    servo_position(fd, 9,4922);
    servo_position(fd, 12,4922);
	sleepms(500);   
    printf("motion4");
    servo_position(fd, 1,7820);
    servo_position(fd, 3,5522);
    servo_position(fd, 4,7820);
    servo_position(fd, 6,5522);
    servo_position(fd, 7,7820);
    servo_position(fd, 9,5522);
    servo_position(fd, 10,7820);
    servo_position(fd, 12,5522);
	sleepms(1000);   
}

void stand_close(int fd){
    for(int j=1;j<13;j++){
        servo_speed(fd, j,30);
    }   
	printf("motion1\n"); 
    servo_position(fd, 1,6820);
    servo_position(fd, 4,6820);
    servo_position(fd, 7,6820);
    servo_position(fd, 10,6820);
	sleepms(500);   
   
	printf("motion2\n"); 
    servo_position(fd, 3,8922);
    servo_position(fd, 6,8922);
    servo_position(fd, 9,8922);
    servo_position(fd, 12,8922);
	sleepms(100);   
   
	printf("motion3\n"); 
	servo_position(fd, 1,9870);
    servo_position(fd, 4,9870);
    servo_position(fd, 7,9870);
    servo_position(fd, 10,9870);
	sleepms(100);   
   
	printf("motion4\n"); 
    servo_position(fd, 2,7500);
    servo_position(fd, 5,7500);
    servo_position(fd, 8,7500);
    servo_position(fd, 11,7500);
	sleepms(100);   
   
	printf("motino5\n"); 
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
	sleepms(2000);   
   
 
	printf("motino6\n"); 
    servo_position(fd, 2,4933);
    servo_position(fd, 5,4933);
    servo_position(fd, 8,4933);
    servo_position(fd, 11,4933);
	sleepms(100);   
}

void set_motion(int fd){
    for(int j=1;j<13;j++){
        servo_speed(fd, j,30);
    }
    servo_position(fd, 2,7500);
	sleepms(2);   
    
    servo_position(fd, 5,7500);
	sleepms(2);   
    
    servo_position(fd, 11,7500);
	sleepms(500);   
    
    servo_position(fd, 1,9870);
	sleepms(2);   
    
    servo_position(fd, 3,8922);
	sleepms(2);   
}

void flip_motion(int fd){
    for(int j=1;j<13;j++){
        servo_speed(fd, j,50);
    }
    int first = 8158;
    int second = 7600;
    int third = 9116;
    
    servo_position(fd, 4,first);
	sleepms(2);   
    
    servo_position(fd, 5,second);
	sleepms(2);   
    
    servo_position(fd, 6,third);
	sleepms(2);   
    
    servo_position(fd, 10,first);
	sleepms(2);   
    
    servo_position(fd, 11,second);
	sleepms(2);   
    
    servo_position(fd, 12,third);
	sleepms(1000);   
    
    servo_position(fd, 3,7500);
	sleepms(500);   
    
    servo_position(fd, 1,first);
	sleepms(2);   
    servo_position(fd, 2,second);
	sleepms(2);   
    servo_position(fd, 3,third);
	sleepms(500);   
    
    servo_position(fd, 1,8907);
	sleepms(2);   
    servo_position(fd, 3,10563);
	sleepms(2);   
    servo_position(fd, 8,7500);
	sleepms(500);   
    servo_position(fd, 7,8907);
	sleepms(2);   
    servo_position(fd, 9,10563);
	sleepms(2);   
}

void end_motion(int fd){
    for(int j=1;j<13;j++){
        servo_speed(fd, j,30);
    }
    int first = 9459;
    int third = 10495;
    
    servo_position(fd, 4,first);
	sleepms(2);   
    
    servo_position(fd, 6,third);
	sleepms(2);   
    
    servo_position(fd, 10,first);
	sleepms(2);   
    
    servo_position(fd, 12,third);
	sleepms(300);   
    
    first = 8907;
    third = 10565;
    
    servo_position(fd, 4,first);
	sleepms(2);   
    
    servo_position(fd, 6,third);
	sleepms(2);   
    
    servo_position(fd, 10,first);
	sleepms(2);   
    
    servo_position(fd, 12,third);
	sleepms(1000);   
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
            		printf("fin c\n\n");
					break;	
            
				case 'o':
					open_motion_crawl(fd);
					sleepms(50);
					ch = 0;
					printf("fin o\n\n");
					break;

				case 'p':
					open_motion(fd);
					sleepms(50);
					ch = 0;
					printf("fin p\n\n");
					break;
				
				case '1':
					stand_set_motion(fd);
					sleepms(50);
					ch = 0;
					printf("fin stand_set_motion\n\n");
					break;
				
				case '2':
					stand_motion(fd);
					sleepms(50);
					ch = 0;
					printf("fin stand_motion\n\n");
					break;

				case '3':
					stand_close(fd);
					sleepms(50);
					ch = 0;
					printf("fin stand_closei\n\n");
					break;
	
				case '4':
					set_motion(fd);
					sleepms(50);
					ch = 0;
					printf("fin set_motion\n\n");
					break;

				case '5':
					flip_motion(fd);
					sleepms(50);
					ch = 0;
					printf("fin flip_motion\n\n");
					break;

				case '6':
					end_motion(fd);
					sleepms(50);
					ch = 0;
					printf("fin end_motion\n\n");
					break;

				case 'q':
	    			printf("start\n");
            		for(i = 3; i <= 12; i +=3){
                		servo_speed(fd, i,50);
                		servo_position(fd, i,10110);
            		}
	    			sleepms(50);
					ch=0;
            		break;
            		
				case 'f':
					printf("Close raspiC.c\n");
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

