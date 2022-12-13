#include <stdio.h>
#include <stdint.h>
#include <wiringPi.h>

#define BLINKING_RATE_MS 500
#define BUFFERDSIZE 5

/*RawSerial serial_servo(D1,D0);
RawSerial pc(USBTX,USBRX);*/
static UnbufferedSerial serial_servo(D1, D0, 115200);
static UnbufferedSerial pc(USBTX,USBRX, 115200);

unsigned short mask_h = 65408; //マスク処理上8bits(0b1111111110000000)
unsigned short mask_l = 127;   //マスク処理下8bits(0b0000000001111111)

int j;
int n;
int p=1;
int open_count = 0;
int count = 0;
char ch,flag_open_crawl,flag_close,flag_open_trot,flag_left_turn,flag_right_turn;
unsigned char tx[3];
unsigned char rx[10];
int open_crawl_max=10;
int open_trot_max=50;
int crawl_max=100;
int trot_max=40;
int turn_max=100;
int pose_max=100;
int cross_max=100;

unsigned short servo_position(unsigned char id,int pos){
    tx[0] = 0x80|id;
    tx[1] = (unsigned char)((pos & mask_h)>>7);
    tx[2] = (unsigned char)(pos & mask_l);
    thread_sleep_for(2);
    for(int i=0;i<3;i++)
    {
        serial_servo.write(&tx[i], sizeof(tx[i]));
    }
    return 0;
}

unsigned short servo_speed(unsigned char id,int speed){
        tx[0] = 0xC0|id;
        tx[1] = 0x02;
        tx[2] = 0x00|speed;
        thread_sleep_for(2);
        for(int i=0;i<3;i++)
        {
            serial_servo.write(&tx[i], sizeof(tx[i]));
        }
        return 0;
}

void pc_command(){
    //ch = pc.getc();
    pc.read(&ch,  sizeof(ch));
    //pc.putc(ch);
    pc.write(&ch, sizeof(ch));
    return;
}

void comment(int a){
    char comment[] = "muve c \r\n";
    pc.write(&comment,sizeof(comment));

    for(int i = 0; i < a; i++){
        pc.write("1", sizeof(comment));
    }
    pc.write("\r\n", sizeof(comment));
    return;
}

int main(){
		if(wiringPiSetupGpio() == -1)
			return 1;

    int po = 1, i;
    thread_sleep_for(50);
    serial_servo.baud(115200);
    serial_servo.format(8,SerialBase::Even,1);
    
    pc.baud(115200);
    pc.format(8,SerialBase::None,1);
    char start_comment[] = "press c \r\n";
    
    
    pc.write(&start_comment,sizeof(start_comment));

    while(1){
        pc.attach(pc_command,SerialBase::RxIrq);

        switch(ch){

            case 'q':
            for(i = 3; i <= 12; i +=3){
                servo_speed(i,50);
                servo_position(i,10110);
            }
            i = 9;
            servo_speed(i,50);
            servo_position(i,6500);
            thread_sleep_for(20);
            comment(9);
            ch=0;
            break;

            default:
            break;
        }
    }
		digitalWrite(18, 0);
		return 0;
}

