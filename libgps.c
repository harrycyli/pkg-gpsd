/* libgps.c -- client interface library for the gpsd daemon */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/time.h>

#include "gpsd.h"

struct gps_data_t *gps_open(const char *host, const char *port)
/* open a connection to a gpsd daemon */
{
    struct gps_data_t *gpsdata = (struct gps_data_t *)calloc(sizeof(struct gps_data_t), 1);

    if (!gpsdata)
	return NULL;
    if (!host)
	host = "localhost";
    if (!port)
	port = DEFAULT_GPSD_PORT;

    if ((gpsdata->gps_fd = netlib_connectsock(host, port, "tcp")) < 0) {
	errno = gpsdata->gps_fd;
	free(gpsdata);
	return NULL;
    }

    gpsdata->mode = MODE_NOT_SEEN;
    gpsdata->status = STATUS_NO_FIX;
    gpsdata->track = TRACK_NOT_VALID;
    return gpsdata;
}

int gps_close(struct gps_data_t *gpsdata)
/* close a gpsd connection */
{
    int retval = close(gpsdata->gps_fd);
    if (gpsdata->gps_id)
	free(gpsdata->gps_id);
    free(gpsdata);
    return retval;
}

void gps_set_raw_hook(struct gps_data_t *gpsdata, void (*hook)(struct gps_data_t *, char *))
{
    gpsdata->raw_hook = hook;
}

/*
 * return: 0, no changes
 *         1, something changed
 */
static int gps_unpack(char *buf, struct gps_data_t *gpsdata)
/* unpack a daemon response into a status structure */
{
    char *ns, *sp, *tp;
    double d1, d2, d3;
    int i1, i2, changed = 0;

    for (ns = buf; ns; ns = strstr(ns+1, "GPSD")) {
	if (!strncmp(ns, "GPSD", 4)) {
	    for (sp = ns + 5; ; sp = tp+1) {
		if (!(tp = strchr(sp, ',')))
		    tp = strchr(sp, '\r');
		if (!tp) break;
		*tp = '\0';

		if (sp[2] == '?')
		    continue;

		switch (*sp) {
		case 'A':
		    sscanf(sp, "A=%lf", &d1);
		    gpsdata->altitude_stamp.changed = (gpsdata->altitude != d1);
		    gpsdata->altitude = d1;
		    REFRESH(gpsdata->altitude_stamp);
		    break;
		case 'B':
		    sscanf(sp, "B=%d %*d %*s %d", &gpsdata->baudrate, &gpsdata->stopbits);
		    break;
		case 'C':
		    sscanf(sp, "C=%d", &gpsdata->cycle);
		    break;
		case 'D':
		    strcpy(gpsdata->utc, sp+2);
		    break;
		case 'E':
		    sscanf(sp, "E=%lf %lf %lf", &d1, &d2, &d3);
		    gpsdata->fix_quality_stamp.changed = \
			(gpsdata->epe != d1) || (gpsdata->eph != d2) || (gpsdata->epv != d3);
		    gpsdata->epe = d1; gpsdata->eph = d2; gpsdata->epv = d3;
		    REFRESH(gpsdata->epe_quality_stamp);
		    break;
		case 'I':
		    if (gpsdata->gps_id)
			free(gpsdata->gps_id);
		    gpsdata->gps_id = strdup(sp+2);
		case 'M':
		    gpsdata->mode = atoi(sp+2);
		    break;
		case 'N':
		    gpsdata->driver_mode = atoi(sp+2);
		    break;
		case 'P':
		    sscanf(sp, "P=%lf %lf", &d1, &d2);
		    gpsdata->latlon_stamp.changed = (gpsdata->latitude != d1) || (gpsdata->longitude != d2);
		    gpsdata->latitude = d1; gpsdata->longitude = d2;
		    REFRESH(gpsdata->latlon_stamp);
		    break;
		case 'Q':
		    sscanf(sp, "Q=%d %lf %lf %lf", &i1, &d1, &d2, &d3);
		    gpsdata->fix_quality_stamp.changed = \
			(gpsdata->satellites_used != i1)
			|| (gpsdata->pdop != d1)
			|| (gpsdata->hdop != d2)
			|| (gpsdata->vdop != d3);
		    gpsdata->satellites_used = i1;
		    gpsdata->pdop = d1; gpsdata->hdop = d2; gpsdata->vdop = d3;
		    REFRESH(gpsdata->fix_quality_stamp);
		    break;
		case 'S':
		    gpsdata->status = atoi(sp+2);
		    break;
		case 'T':
		    sscanf(sp, "T=%lf", &gpsdata->track);
		    break;
		case 'U':
		    sscanf(sp, "U=%lf", &gpsdata->climb);
		    break;
		case 'V':
		    sscanf(sp, "V=%lf", &gpsdata->speed);
		    break;
		case 'X':
		    if (!strncmp(sp, "X=1", 3)) {
			gpsdata->online_stamp.changed = gpsdata->online != 1;
			gpsdata->online = 1;
			REFRESH(gpsdata->online_stamp);
		    }
		    else if (!strncmp(sp, "X=0", 3)) {
			gpsdata->online_stamp.changed = gpsdata->online != 0;
			gpsdata->online = 0;
			REFRESH(gpsdata->online_stamp);
		    }
		    break;
		case 'Y':
		    i1 = atoi(sp+2);
		    gpsdata->satellite_stamp.changed = (gpsdata->satellites != i1);
		    gpsdata->satellites = i1;
		    if (gpsdata->satellites) {
			int j, i3, i4, i5;
			int PRN[MAXCHANNELS];
			int elevation[MAXCHANNELS], azimuth[MAXCHANNELS];
			int ss[MAXCHANNELS], used[MAXCHANNELS];

			for (j = 0; j < gpsdata->satellites; j++) {
			    PRN[j]=elevation[j]=azimuth[j]=ss[j]=used[j]=0;
			}
			for (j = 0; j < gpsdata->satellites; j++) {
			    sp = strchr(sp, ':') + 1;
			    sscanf(sp, "%d %d %d %d %d", &i1, &i2, &i3, &i4, &i5);
			    PRN[j] = i1;
			    elevation[j] = i2; azimuth[j] = i3;
			    ss[j] = i4; used[j] = i5;
			}
			/*
			 * This won't catch the case where all values are identical
			 * but rearranged.  We can live with that.
			 */
			gpsdata->satellite_stamp.changed |= \
			    memcmp(gpsdata->PRN, PRN, sizeof(PRN)) ||
			    memcmp(gpsdata->elevation, elevation, sizeof(elevation)) ||
			    memcmp(gpsdata->azimuth, azimuth,sizeof(azimuth)) ||
			    memcmp(gpsdata->ss, ss, sizeof(ss)) ||
			    memcmp(gpsdata->used, used, sizeof(used));
			memcpy(gpsdata->PRN, PRN, sizeof(PRN));
			memcpy(gpsdata->elevation, elevation, sizeof(elevation));
			memcpy(gpsdata->azimuth, azimuth,sizeof(azimuth));
			memcpy(gpsdata->ss, ss, sizeof(ss));
			memcpy(gpsdata->used, used, sizeof(used));
		    }
		    REFRESH(gpsdata->satellite_stamp);
		    break;
		case 'Z':
		    sscanf(sp, "Z=%d", &gpsdata->profiling);
		    break;
		case '$':
		    sscanf(sp, "$=%s %d %lf %lf %lf %lf %lf %lf", 
			   gpsdata->tag,
			   &gpsdata->sentence_length,
			   &gpsdata->gps_time, 
			   &gpsdata->d_xmit_time, 
			   &gpsdata->d_recv_time, 
			   &gpsdata->d_decode_time, 
			   &gpsdata->poll_time, 
			   &gpsdata->emit_time);
		    break;
		}
	    }
	}
	changed |=
	    gpsdata->online_stamp.changed
	|| gpsdata->latlon_stamp.changed 
	|| gpsdata->altitude_stamp.changed 
	|| gpsdata->fix_quality_stamp.changed 
	|| gpsdata->epe_quality_stamp.changed 
	|| gpsdata->satellite_stamp.changed 
	;
    }

    if (gpsdata->raw_hook)
	gpsdata->raw_hook(gpsdata, buf);

    return changed;
}

/*
 * return: 0, no changes or no read
 *         1, something changed
 *        -1, read error
 */
int gps_poll(struct gps_data_t *gpsdata)
/* wait for and read data being streamed from the daemon */ 
{
    char	buf[BUFSIZ];
    int		n;
    double received = 0;

    /* the daemon makes sure that every read is NUL-terminated */
    n = read(gpsdata->gps_fd, buf, sizeof(buf)-1);
    if (n < 0) {
        /* error */
	return -1;
    } else if ( n == 0 ) {
        /* nothing read */
	return 0;
    }
    buf[n] = '\0';
    if (gpsdata->profiling)
	received = timestamp();
    n = gps_unpack(buf, gpsdata);
    if (gpsdata->profiling)
    {
	gpsdata->c_decode_time = received - gpsdata->gps_time;
	gpsdata->c_recv_time = timestamp() - gpsdata->gps_time;
    }
    return n;
}

int gps_query(struct gps_data_t *gpsdata, const char *requests)
/* query a gpsd instance for new data */
{
    if (write(gpsdata->gps_fd, requests, strlen(requests)) <= 0)
	return -1;
    return gps_poll(gpsdata);
}

#ifdef TESTMAIN
/*
 * A simple command-line exerciser for the library.
 * Not really useful for anything but debugging.
 */
void data_dump(struct gps_data_t *collect, time_t now)
{
    char *status_values[] = {"NO_FIX", "FIX", "DGPS_FIX"};
    char *mode_values[] = {"", "NO_FIX", "MODE_2D", "MODE_3D"};

    if (collect->online_stamp.changed)
	printf("online: %d\n", collect->online);
    if (collect->latlon_stamp.changed) {
	printf("P: lat/lon: %lf %lf", collect->latitude, collect->longitude);
	printf("(lr=%ld, changed=%d)\n",
	       collect->latlon_stamp.last_refresh,
	       collect->latlon_stamp.changed);
    }
    if (collect->altitude_stamp.changed) {
	printf("A: altitude: %lf  U: climb: %lf", 
	       collect->altitude, collect->climb);
	printf("(lr=%ld, changed=%d)\n",
	       collect->altitude_stamp.last_refresh,
	       collect->altitude_stamp.changed);
    }
    if (collect->track_stamp.changed) {
	printf("T: track: %lf  V: speed: %lf ", 
	       collect->track, collect->speed);
	printf("(lr=%ld, changed=%d)\n",
	       collect->track_stamp.last_refresh,
	       collect->track_stamp.changed);
    }
    if (collect->status_stamp.changed) {
	printf("S: status: %d (%s) ", 
	       collect->status,status_values[collect->status]);
	printf("(lr=%ld, changed=%d)\n",
	       collect->status_stamp.last_refresh,
	       collect->status_stamp.changed);
    }
    if (collect->mode_stamp.changed) {
	printf("M: mode: %d (%s) ", collect->mode, mode_values[collect->mode]);
	printf("(lr=%ld, changed=%d)",
	       collect->mode_stamp.last_refresh,
	       collect->mode_stamp.changed);
    }
    if (collect->fix_quality_stamp.changed) {
	printf("Q: satellites %d, pdop=%lf, hdop=%lf, vdop=%lf ",
	      collect->satellites_used, 
	      collect->pdop, collect->hdop, collect->vdop);
	printf("(lr=%ld, changed=%d)\n",
	       collect->fix_quality_stamp.last_refresh,
	       collect->fix_quality_stamp.changed);
    }
    if (collect->satellite_stamp.changed) {
	int i;

	printf("Y: satellites in view: %d\n", collect->satellites);
	for (i = 0; i < collect->satellites; i++) {
	    printf("    %2.2d: %2.2d %3.3d %3.3d %c\n", collect->PRN[i], collect->elevation[i], collect->azimuth[i], collect->ss[i], collect->used[i]? 'Y' : 'N');
	}
	printf("(lr=%ld, changed=%d)\n",
	       collect->satellite_stamp.last_refresh,
	       collect->satellite_stamp.changed);
    }
}

static void dumpline(char *buf)
{
    puts(buf);
}

main(int argc, char *argv[])
{
    struct gps_data_t *collect;
    char buf[BUFSIZ];

    collect = gps_open(NULL, 0);
    gps_set_raw_hook(collect, dumpline);
    if (argc > 1) {
	strcpy(buf, argv[1]);
	strcat(buf,"\n");
	gps_query(collect, buf);
	data_dump(collect, time(NULL));
    } else {
	int	tty = isatty(0);

	if (tty)
	    fputs("This is the gpsd exerciser.\n", stdout);
	for (;;) {
	    if (tty)
		fputs("> ", stdout);
	    if (fgets(buf, sizeof(buf), stdin) == NULL) {
		if (tty)
		    putchar('\n');
		break;
	    }
	    if (!gps_query(collect, buf))
		fputs("No changes.\n", stdout);
	    data_dump(collect, time(NULL));
	}
    }

    gps_close(collect);
}

#endif /* TESTMAIN */
