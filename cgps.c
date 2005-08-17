/*
 * Copyright (c) 2005 Jeff Francis <jeff@gritch.org>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

/*
  Jeff Francis
  jeff@gritch.org
  $Id$

  Kind of a curses version of xgps for use with gpsd.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <math.h>
#include <errno.h>

#include <ncurses.h>                                                         
#include <signal.h>

#include "gps.h"

/* Macro for declaring function arguments unused. */
#if defined(__GNUC__)
#  define UNUSED __attribute__((unused)) /* Flag variable as unused */
#else /* not __GNUC__ */
#  define UNUSED
#endif

static struct gps_data_t *gpsdata;
static time_t timer;	/* time of last state change */
static int state = 0;	/* or MODE_NO_FIX=1, MODE_2D=2, MODE_3D=3 */
static float altfactor = METERS_TO_FEET;
static float speedfactor = MPS_TO_MPH;
static char *altunits = "ft";
static char *speedunits = "mph";

/* Function to call when we're all done.  Does a bit of clean-up. */
static void die(void) 
{
    /* Ignore signals. */
    signal(SIGINT,SIG_IGN);

    /* Move the cursor to the bottom left corner. */
    mvcur(0,COLS-1,LINES-1,0);

    /* Put input attributes back the way they were. */
    echo();
    noraw();

    /* Done with curses. */
    endwin();

    /* We're done talking to gpsd. */
    (void)gps_close(gpsdata);

    /* Bye! */
    exit(0);
}

/* This gets called once for each new sentence. */
static void update_panel(struct gps_data_t *gpsdata, 
			 char *message,
			 size_t len UNUSED, 
			 int level UNUSED)
{
    unsigned int i;
    int newstate;
    char s[128];

    /* Do the initial field label setup. */
    move(0,5);
    printw("Time:");
    move(1,5);
    printw("Latitude:");
    move(2,5);
    printw("Longitude:");
    move(3,5);
    printw("Altitude:");
    move(4,5);
    printw("Speed:");
    move(5,5);
    printw("Heading:");
    move(6,5);
    printw("HPE:");
    move(7,5);
    printw("VPE:");
    move(8,5);
    printw("Climb:");
    move(9,5);
    printw("Status:");
    move(10,5);
    printw("Change:");
    move(0,45);
    printw("PRN:   Elev:  Azim:  SNR:  Used:");


    /* This is for the satellite status display.  Lifted almost verbatim
       from xgps.c. */
    if (gpsdata->satellites) {
	for (i = 0; i < MAXCHANNELS; i++) {
	    if (i < (unsigned int)gpsdata->satellites) {
		move(i+1,45);
		printw(" %3d    %02d    %03d    %02d      %c    ",
		       gpsdata->PRN[i],
		       gpsdata->elevation[i], gpsdata->azimuth[i], 
		       gpsdata->ss[i],	gpsdata->used[i] ? 'Y' : 'N');
	    } else {
		move(i+1,45);
		printw("                                          ");
	    }
	}
    }
  
/* TODO: Make this work. */
    if (isnan(gpsdata->fix.time)==0) {
	move(0,17);
	printw("%s",unix_to_iso8601(gpsdata->fix.time, s, (int)sizeof(s)));
    } else {
	move(0,17);
	printw("n/a         ");
    }

    /* Fill in the latitude. */
    if (gpsdata->fix.mode >= MODE_2D) {
	move(1,17);
	printw("%lf %c     ", fabs(gpsdata->fix.latitude), (gpsdata->fix.latitude < 0) ? 'S' : 'N');
    } else {
	move(1,17);
	printw("n/a         ");
    }

    /* Fill in the longitude. */
    if (gpsdata->fix.mode >= MODE_2D) {
	move(2,17);
	printw("%lf %c     ", fabs(gpsdata->fix.longitude), (gpsdata->fix.longitude < 0) ? 'W' : 'E');
    } else {
	move(2,17);
	printw("n/a         ");
    }

    /* Fill in the altitude. */
    if (gpsdata->fix.mode == MODE_3D) {
	move(3,17);
	printw("%.1f %s     ",gpsdata->fix.altitude*altfactor, altunits);
    } else {
	move(3,17);
	printw("n/a         ");
    }

    /* Fill in the speed */
    if (gpsdata->fix.mode >= MODE_2D && isnan(gpsdata->fix.track)==0) {
	move(4,17);
	printw("%.1f %s     ", gpsdata->fix.speed*speedfactor, speedunits);
    } else {
	move(4,17);
	printw("n/a         ");
    }

    /* Fill in the heading. */
    if (gpsdata->fix.mode >= MODE_2D && isnan(gpsdata->fix.track)==0) {
	move(5,17);
	printw("%.1f degrees     \n", gpsdata->fix.track);
    } else {
	move(5,17);
	printw("n/a         ");
    }

    /* Fill in the estimated horizontal position error. */
    if (isnan(gpsdata->fix.eph)==0) {
	move(6,17);
	printw("%d ft     ", (int) (gpsdata->fix.eph * altfactor));
    } else {
	move(6,17);
	printw("n/a         ");
    }

    /* Fill in the estimated vertical position error. */
    if (isnan(gpsdata->fix.epv)==0) {
	move(7,17);
	printw("%d ft     ", (int)(gpsdata->fix.epv * altfactor));
    } else {
	move(7,17);
	printw("n/a         ");
    }

    /* Fill in the rate of climb. */
    /* TODO: Factor are probably wrong. */
    if (gpsdata->fix.mode == MODE_3D && isnan(gpsdata->fix.climb)==0) {
	move(8,17);
	printw("%.1f ft/min     ", gpsdata->fix.climb * METERS_TO_FEET * 60);
    } else {
	move(8,17);
	printw("n/a         ");
    }
  
    /* Fill in the GPS status */
    if (gpsdata->online == 0) {
	newstate = 0;
	move(9,17);
	printw("OFFLINE          ");
    } else {
	newstate = gpsdata->fix.mode;
	switch (gpsdata->fix.mode) {
	case MODE_2D:
	    move(9,17);
	    printw("2D %sFIX     ",(gpsdata->status==STATUS_DGPS_FIX)?"DIFF ":"");
	    break;
	case MODE_3D:
	    move(9,17);
	    printw("3D %sFIX     ",(gpsdata->status==STATUS_DGPS_FIX)?"DIFF ":"");
	    break;
	default:
	    move(9,17);
	    printw("NO FIX               ");
	    break;
	}
    }

    /* Fill in the time since the last state change. */
    if (newstate != state) {
	timer = time(NULL);
	state = newstate;
    }
    move(10,17);
    printw("(%d secs)          ", (int) (time(NULL) - timer));

    /* Update the screen. */
    refresh();
}

int main(int argc, char *argv[])
{
    int option;
    char *arg = NULL, *colon1, *colon2, *device = NULL, *server = NULL, *port = DEFAULT_GPSD_PORT;
    char *err_str = NULL;

    /* Process the options.  Print help if requested. */
    while ((option = getopt(argc, argv, "hv")) != -1) {
	switch (option) {
	case 'v':
	    (void)fprintf(stderr, "SVN ID: $Id$ \n");
	    exit(0);
	case 'h': default:
	    (void)fprintf(stderr, "Usage: %s [-h] [-v] [server[:port:[device]]]\n", argv[0]);
	    exit(1);
	}
    }

    /* Grok the server, port, and device. */
    if (optind < argc) {
	arg = strdup(argv[optind]);
	colon1 = strchr(arg, ':');
	server = arg;
	if (colon1 != NULL) {
	    if (colon1 == arg)
		server = NULL;
	    else
		*colon1 = '\0';
	    port = colon1 + 1;
	    colon2 = strchr(port, ':');
	    if (colon2 != NULL) {
		if (colon2 == port)
		    port = NULL;
		else
		    *colon2 = '\0';
		device = colon2 + 1;
	    }
	}
	colon1 = colon2 = NULL;
    }

    switch (gpsd_units())
    {
    case imperial:
	altfactor = METERS_TO_FEET;
	altunits = "ft";
	speedfactor = MPS_TO_MPH;
	speedunits = "mph";
	break;
    case nautical:
	altfactor = METERS_TO_FEET;
	altunits = "ft";
	speedfactor = MPS_TO_KNOTS;
	speedunits = "knots";
	break;
    case metric:
	altfactor = 1;
	altunits = "m";
	speedfactor = MPS_TO_KPH;
	speedunits = "kph";
	break;
    default:
	/* leave the default alone */
	break;
    }

    /* Open the stream to gpsd. */
    gpsdata = gps_open(server, port);
    if (!gpsdata) {
	switch ( errno ) {
	case NL_NOSERVICE: 	err_str = "can't get service entry"; break;
	case NL_NOHOST: 	err_str = "can't get host entry"; break;
	case NL_NOPROTO: 	err_str = "can't get protocol entry"; break;
	case NL_NOSOCK: 	err_str = "can't create socket"; break;
	case NL_NOSOCKOPT: 	err_str = "error SETSOCKOPT SO_REUSEADDR"; break;
	case NL_NOCONNECT: 	err_str = "can't connect to host"; break;
	default:             	err_str = "Unknown"; break;
	}
	(void)fprintf( stderr, 
		       "xgps: no gpsd running or network error: %d, %s\n", 
		       errno, err_str);
	exit(2);
    }

    /* Update the timestamp (used to keep track of time since last state
       change). */
    timer = time(NULL);

    /* Set up the curses screen (if using curses). */
    initscr();
    raw();
    noecho();
    /* signal(SIGINT,die); */

    /* Here's where updates go. */
    gps_set_raw_hook(gpsdata, update_panel);

    /* If the user requested a specific device, try to change to it. */
    if (device) {
	char *channelcmd = (char *)malloc(strlen(device)+3);

	if (channelcmd) {
	    (void)strcpy(channelcmd, "F=");
	    (void)strcpy(channelcmd+2, device);
	    (void)gps_query(gpsdata, channelcmd);
	    (void)free(channelcmd);
	}
    }

    /* Request "w+x" data from gpsd. */
    (void)gps_query(gpsdata, "w+x\n");

    /* Loop and poll once per second (this could be less than optimal
       for a receiver that updates > 1hz, or for a user using a *really*
       slow ancient serial terminal). */
    while(1) {
	gps_poll(gpsdata);
	sleep(1);
    }

    die();
}

