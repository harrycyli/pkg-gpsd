#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>

#if defined (HAVE_SYS_TERMIOS_H)
#include <sys/termios.h>
#else
#if defined (HAVE_TERMIOS_H)
#include <termios.h>
#endif
#endif

#include "outdata.h"
#include "nmea.h"
#include "gpsd.h"

#define DEFAULTPORT "2947"

/* define global variables */
static int ttyfd = -1;
static struct termios ttyset, ttyset_old;

int serial_open(char *device_name, int device_speed)
{
    char *temp;
    char *p;

    temp = strdup(device_name);

    if ( (p = strchr(temp, ':')) ) {
	char *port = DEFAULTPORT;

	if (*(p + 1))
	    port = p + 1;
	*p = '\0';

	/* temp now holds the HOSTNAME portion and port the port number. */
	ttyfd = connectTCP(temp, port);
	port = 0;

	if (write(ttyfd, "r\n", 2) != 2)
	    errexit("Can't write to socket");
    } else {
	ttyfd = open(temp, O_RDWR | O_NONBLOCK);

	if (ttyfd < 0)
	    return (-1);

	if (isatty(ttyfd)) {
            /* Save original terminal parameters */
            if (tcgetattr(ttyfd,&ttyset_old) != 0)
              return (-1);

	    memcpy(&ttyset, &ttyset_old, sizeof(ttyset));

	    cfsetispeed(&ttyset, (speed_t)device_speed);
	    cfsetospeed(&ttyset, (speed_t)device_speed);

	    ttyset.c_cflag &= ~(PARENB | CRTSCTS);
	    ttyset.c_cflag |= (CSIZE & CS8) | CREAD | CLOCAL;
	    ttyset.c_iflag = ttyset.c_oflag = ttyset.c_lflag = (tcflag_t) 0;
	    ttyset.c_oflag = (ONLCR);
            if (tcsetattr(ttyfd, TCSANOW, &ttyset) != 0)
		return (-1);
	}
    }
    free(temp);
    return ttyfd;
}

void serial_close()
{
    if (ttyfd != -1) {
	if (isatty(ttyfd)) {
#if defined (USE_TERMIO)
	    ttyset.c_cflag = CBAUD & B0;
#else
	    ttyset.c_ispeed = B0;
	    ttyset.c_ospeed = B0;
#endif
            tcsetattr(ttyfd, TCSANOW, &ttyset);
	}
	/* Restore original terminal parameters */
	/* but make sure DTR goes down */
	ttyset_old.c_cflag |= HUPCL;
	tcsetattr(ttyfd,TCSANOW,&ttyset_old);

	close(ttyfd);
	ttyfd = -1;
    }
}
