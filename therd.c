#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <termio.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>
#include <sys/select.h>

#define SERIAL_PORT "/dev/ttyAMA0"

static char w[2] = "A";

// -----------------------------------------------------------------------
int main(int argc,char *argv[])
{
    int fd;
    int it;
    unsigned char buf[255];
    struct termios tio;

    fprintf(stderr, "*** start ***\n");
    fprintf(stderr, "*** Sep/30/2020 ***\n");

    for (it = 0; it < sizeof(buf); it++)
        {
        buf[it] = 0;
        }

    int rv; 
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

    fprintf(stderr, "*** check *** aaa ***\n");

    bzero(&tio, sizeof(tio));

    /* 115200bps, フロー制御有り, ８ビット，DTR/DSR無効，受信可能 */
    tio.c_cflag = B115200 | CRTSCTS | CS8 | CLOCAL | CREAD;
    tio.c_cc[VMIN] = 1; /* 入力データをバッファしない */
    tcsetattr(fd, TCSANOW, &tio); /* アトリビュートのセット */

    fprintf(stderr, "*** check *** bbb ***\n");
	while(1){
    for(it = 0; it < 10; it++)
        {
        write(fd, w, 1);

        rv = select(fd + 1, &set, NULL, NULL, &timeout);

        if(rv == -1)
            fprintf(stderr,"select\n"); /* an error accured */
        else if(rv == 0)
            fprintf(stderr,"timeout\n"); /* a timeout occured */
        else
            {
            int len = read(fd, buf, sizeof(buf));
            if (0 < len)
                {
                printf("read = %s\n", buf);
                }
            else
                {
                printf("len = %d\n", len);
                }
            }

        w[0]++;
    }
	}
    close(fd);

    fprintf(stderr, "*** end ***\n");

    exit(0);
}
