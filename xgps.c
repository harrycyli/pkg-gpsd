#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <math.h>
#include <errno.h>
#include <Xm/Xm.h>
#include <Xm/MwmUtil.h>
#include <Xm/PushB.h>
#include <Xm/Form.h>
#include <Xm/RowColumn.h>
#include <Xm/Label.h>
#include <Xm/TextF.h>
#include <Xm/List.h>
#include <Xm/DrawingA.h>
#include <Xm/Protocols.h>
#include <X11/Shell.h>

#include "config.h"
#include "gps.h"
#include "display.h"

static Widget toplevel, form, left, right, quitbutton;
static Widget satellite_list, satellite_diagram, status;
static Widget rowColumn_11, rowColumn_12, rowColumn_13, rowColumn_14;
static Widget rowColumn_15, rowColumn_16, rowColumn_17, rowColumn_18;
static Widget rowColumn_19, rowColumn_20, rowColumn_21;
static Widget text_1, text_2, text_3, text_4, text_5;
static Widget text_6, text_7, text_8, text_9, text_10;
static Widget label_1, label_2, label_3, label_4, label_5;
static Widget label_6, label_7, label_8, label_9, label_10;
static GC gc;

static XrmOptionDescRec options[] = {
{"-altunits",  "*altunits",            XrmoptionSepArg,        NULL},
{"-speedunits","*speedunits",          XrmoptionSepArg,        NULL},
};
String fallback_resources[] = {NULL};

struct unit_t {
    char *legend;
    float factor;
};
static struct unit_t speedtable[] = {
    {"knots",	1},
    {"mph",		KNOTS_TO_MPH},
    {"kph",		KNOTS_TO_KPH},
}, *speedunits = speedtable;
static struct unit_t  alttable[] = {
    {"feet",		METERS_TO_FEET},
    {"meters",		1},
}, *altunits = alttable;

static void quit_cb(void)
{
    exit(0);	/* closes the GPS along with other fds */
}

static Pixel get_pixel(Widget w, char *resource_value)
{
    Colormap colormap;
    Boolean status;
    XColor exact, color;

    colormap = DefaultColormapOfScreen(DefaultScreenOfDisplay(XtDisplay(w)));
    status = XAllocNamedColor(XtDisplay(w), colormap, resource_value, &color, &exact);
    if (status == 0) {
	fprintf(stderr, "Unknown color: %s", resource_value);
	color.pixel = BlackPixelOfScreen(DefaultScreenOfDisplay(XtDisplay(w)));
    };
    return (color.pixel);
}

static void build_gui(Widget toplevel)
{
    Arg args[100];
    XGCValues gcv;
    Atom delw;
    int i;
    XmString string;

    /* the root application window */
    XtSetArg(args[0], XmNgeometry, "700x570");
    XtSetArg(args[1], XmNresizePolicy, XmRESIZE_NONE);
    XtSetArg(args[2], XmNallowShellResize, False);
    XtSetArg(args[3], XmNdeleteResponse, XmDO_NOTHING);
    XtSetArg(args[4], XmNmwmFunctions,
	     MWM_FUNC_RESIZE | MWM_FUNC_MOVE | MWM_FUNC_MINIMIZE | MWM_FUNC_MAXIMIZE);
    XtSetValues(toplevel, args, 5);

    /* a form to assist with geometry negotiation */
    form = XtVaCreateManagedWidget("form", xmFormWidgetClass, toplevel, NULL);
    /* the left half of the screen */
    left = XtVaCreateManagedWidget("left", xmRowColumnWidgetClass, form,
				   XmNleftAttachment, XmATTACH_FORM,
				   XmNtopAttachment, XmATTACH_FORM,
				   NULL);
    /* the right half of the screen */
    right = XtVaCreateManagedWidget("right", xmRowColumnWidgetClass, form,
				    XmNleftAttachment, XmATTACH_WIDGET,
				    XmNleftWidget, left,
				    XmNtopAttachment, XmATTACH_FORM,
				    NULL);
    /* the application status bar */
    status = XtVaCreateManagedWidget("status", xmTextFieldWidgetClass, form,
				     XmNcursorPositionVisible, False,
				     XmNeditable, False,
				     XmNmarginHeight, 1,
				     XmNhighlightThickness, 0,
				     XmNshadowThickness, 1,
				     XmNleftAttachment, XmATTACH_FORM,
				     XmNrightAttachment, XmATTACH_FORM,
				     XmNtopAttachment, XmATTACH_WIDGET,
				     XmNtopWidget, left,
				     NULL);
    /* satellite location and SNR data panel */
#define FRAMEHEIGHT	220
#define LEFTSIDE_WIDTH	205
    satellite_list =
      XtVaCreateManagedWidget("satellite_list", xmListWidgetClass, left,
			      XmNbackground, get_pixel(toplevel, "snow"),
			      XmNheight, FRAMEHEIGHT,
			      XmNwidth, LEFTSIDE_WIDTH,
			      XmNlistSizePolicy, XmCONSTANT,
			      XmNhighlightThickness, 0,
			      XmNlistSpacing, 4,
			      NULL);
    /* the satellite diagram */
#define SATDIAG_SIZE	400
    satellite_diagram = 
      XtVaCreateManagedWidget("satellite_diagram",
			      xmDrawingAreaWidgetClass, right, 
			      XmNbackground, get_pixel(toplevel, "snow"),
			      XmNheight, SATDIAG_SIZE, XmNwidth, SATDIAG_SIZE,
			      NULL);
    gcv.foreground = BlackPixelOfScreen(XtScreen(satellite_diagram));
    gc = XCreateGC(XtDisplay(satellite_diagram),
	RootWindowOfScreen(XtScreen(satellite_diagram)), GCForeground, &gcv);
    register_canvas(satellite_diagram, gc);
    XtAddCallback(satellite_diagram, XmNexposeCallback, (XtPointer)redraw, NULL);
    /* the data display */
    XtSetArg(args[0], XmNorientation, XmHORIZONTAL);
    rowColumn_11 = XtCreateManagedWidget("time", xmRowColumnWidgetClass, left, args, 1);

    rowColumn_12 = XtCreateManagedWidget("latitude", xmRowColumnWidgetClass, left, args, 1);
    rowColumn_13 = XtCreateManagedWidget("longitude", xmRowColumnWidgetClass, left, args, 1);
    rowColumn_14 = XtCreateManagedWidget("altitude", xmRowColumnWidgetClass, left, args, 1);
    rowColumn_15 = XtCreateManagedWidget("speed", xmRowColumnWidgetClass, left, args, 1);
    rowColumn_16 = XtCreateManagedWidget("track", xmRowColumnWidgetClass, left, args, 1);
    rowColumn_17 = XtCreateManagedWidget("eph", xmRowColumnWidgetClass, left, args, 1);
    rowColumn_18 = XtCreateManagedWidget("epv", xmRowColumnWidgetClass, left, args, 1);
    rowColumn_19 = XtCreateManagedWidget("climb", xmRowColumnWidgetClass, left, args, 1);
    rowColumn_20 = XtCreateManagedWidget("status", xmRowColumnWidgetClass, left, args, 1);
    rowColumn_21 = XtCreateManagedWidget("quit", xmRowColumnWidgetClass, left, args, 1);

    label_1 = XtCreateManagedWidget("Time     ", xmLabelWidgetClass, rowColumn_11, args, 0);
    label_2 = XtCreateManagedWidget("Latitude ", xmLabelWidgetClass, rowColumn_12, args, 0);
    label_3 = XtCreateManagedWidget("Longitude", xmLabelWidgetClass, rowColumn_13, args, 0);
    label_4 = XtCreateManagedWidget("Altitude ", xmLabelWidgetClass, rowColumn_14, args, 0);
    label_5 = XtCreateManagedWidget("Speed    ", xmLabelWidgetClass, rowColumn_15, args, 0);
    label_6 = XtCreateManagedWidget("Course   ", xmLabelWidgetClass, rowColumn_16, args, 0);
    label_7 = XtCreateManagedWidget("EPH      ", xmLabelWidgetClass, rowColumn_17, args, 0);
    label_8 = XtCreateManagedWidget("EPV      ", xmLabelWidgetClass, rowColumn_18, args, 0);
    label_9 = XtCreateManagedWidget("Climb    ", xmLabelWidgetClass, rowColumn_19, args, 0);
    label_10= XtCreateManagedWidget("Status   ", xmLabelWidgetClass, rowColumn_20, args, 0);

    XtSetArg(args[0], XmNcursorPositionVisible, False);
    XtSetArg(args[1], XmNeditable, False);
    XtSetArg(args[2], XmNmarginHeight, 2);
    XtSetArg(args[3], XmNhighlightThickness, 0);
    XtSetArg(args[4], XmNshadowThickness, 1);
    XtSetArg(args[5], XmNcolumns, 23);
    text_1 = XtCreateManagedWidget("text_1", xmTextFieldWidgetClass,
				   rowColumn_11, args, 6);
    text_2 = XtCreateManagedWidget("text_2", xmTextFieldWidgetClass,
				   rowColumn_12, args, 6);
    text_3 = XtCreateManagedWidget("text_3", xmTextFieldWidgetClass,
				   rowColumn_13, args, 6);
    text_4 = XtCreateManagedWidget("text_4", xmTextFieldWidgetClass,
				   rowColumn_14, args, 6);
    text_5 = XtCreateManagedWidget("text_5", xmTextFieldWidgetClass,
				   rowColumn_15, args, 6);
    text_6 = XtCreateManagedWidget("text_6", xmTextFieldWidgetClass,
				   rowColumn_16, args, 6);
    text_7 = XtCreateManagedWidget("text_7", xmTextFieldWidgetClass,
				   rowColumn_17, args, 6);
    text_8 = XtCreateManagedWidget("text_8", xmTextFieldWidgetClass,
				   rowColumn_18, args, 6);
    text_9 = XtCreateManagedWidget("text_9", xmTextFieldWidgetClass,
				   rowColumn_19, args, 6);
    text_10 = XtCreateManagedWidget("text_10", xmTextFieldWidgetClass,
				   rowColumn_20, args, 6);

    quitbutton = XtCreateManagedWidget("Quit",
			 xmPushButtonWidgetClass, rowColumn_21, args, 0);
    XtAddCallback(quitbutton, XmNactivateCallback, (XtPointer)quit_cb, NULL);

    XtRealizeWidget(toplevel);
    delw = XmInternAtom(XtDisplay(toplevel), "WM_DELETE_WINDOW", False);
    XmAddWMProtocolCallback(toplevel, delw,
			    (XtCallbackProc)quit_cb, (XtPointer)NULL);

    /* create empty list items to be replaced on update */
    string = XmStringCreateSimple(" ");
    for (i = 0; i < MAXCHANNELS; i++)
	XmListAddItem(satellite_list, string, i+1);
    XmStringFree(string);
}

static void handle_time_out(XtPointer client_data UNUSED,
			    XtIntervalId *ignored UNUSED)
/* runs when there is no data for a while */
{
    XmTextFieldSetString(status, "no data arriving");
    XmTextFieldSetString(text_10, "UNKNOWN");
}

/*
 * No dependencies on the session structure above this point.
 */

static struct gps_data_t *gpsdata;
static time_t timer;	/* time of last state change */
static int state = 0;	/* or MODE_NO_FIX=1, MODE_2D=2, MODE_3D=3 */
XtAppContext app;
XtIntervalId timeout;

static void handle_input(XtPointer client_data UNUSED, int *source UNUSED,
			 XtInputId *id UNUSED)
{
    gps_poll(gpsdata);
}

static void update_panel(struct gps_data_t *gpsdata, char *message)
/* runs on each sentence */
{
    unsigned int i;
    int newstate;
    XmString string[MAXCHANNELS+1];
    char s[128], *sp;

    if (message[0])
	while (isspace(*(sp = message + strlen(message) - 1)))
	    *sp = '\0';
    XmTextFieldSetString(status, message);
    string[0] = XmStringCreateSimple("PRN:   Elev:  Azim:  SNR:  Used:");
    /* This is for the satellite status display */
    if (gpsdata->satellites) {
	for (i = 0; i < MAXCHANNELS; i++) {
	    if (i < (unsigned int)gpsdata->satellites) {
		snprintf(s, sizeof(s), " %3d    %02d    %03d    %02d      %c", 
			gpsdata->PRN[i],
			gpsdata->elevation[i], gpsdata->azimuth[i], 
			gpsdata->ss[i],	gpsdata->used[i] ? 'Y' : 'N'
		    );
	    } else
		sprintf(s, "                  ");
	    string[i+1] = XmStringCreateSimple(s);
	}
	XmListReplaceItemsPos(satellite_list, string, sizeof(string), 1);
	for (i = 0; i < sizeof(string)/sizeof(string[0]); i++)
	    XmStringFree(string[i]);
    }
    /* here are the value fields */
    if (gpsdata->fix.time != TIME_NOT_VALID)
	unix_to_iso8601(gpsdata->fix.time, s);
    else
	strcpy(s, "n/a");
    XmTextFieldSetString(text_1, s);
    if (gpsdata->fix.mode >= MODE_2D)
	sprintf(s, "%f %c", fabsf(gpsdata->fix.latitude), (gpsdata->fix.latitude < 0) ? 'S' : 'N');
    else
	strcpy(s, "n/a");
    XmTextFieldSetString(text_2, s);
    if (gpsdata->fix.mode >= MODE_2D)
	sprintf(s, "%f %c", fabsf(gpsdata->fix.longitude), (gpsdata->fix.longitude < 0) ? 'W' : 'E');
    else
	strcpy(s, "n/a");
    XmTextFieldSetString(text_3, s);
    if (gpsdata->fix.mode == MODE_3D)
	sprintf(s, "%f %s",gpsdata->fix.altitude*altunits->factor, altunits->legend);
    else
	strcpy(s, "n/a");
    XmTextFieldSetString(text_4, s);
    if (gpsdata->fix.mode >= MODE_2D && gpsdata->fix.track != TRACK_NOT_VALID)
	sprintf(s, "%f %s", gpsdata->fix.speed*speedunits->factor, speedunits->legend);
    else
	strcpy(s, "n/a");
    XmTextFieldSetString(text_5, s);
    if (gpsdata->fix.mode >= MODE_2D && gpsdata->fix.track != TRACK_NOT_VALID)
	sprintf(s, "%f degrees", gpsdata->fix.track);
    else
	strcpy(s, "n/a");
    XmTextFieldSetString(text_6, s);
    if (gpsdata->fix.eph != UNCERTAINTY_NOT_VALID)
	sprintf(s, "%f %s", gpsdata->fix.eph * altunits->factor, altunits->legend);
    else
	strcpy(s, "n/a");
    XmTextFieldSetString(text_7, s);
    if (gpsdata->fix.epv != UNCERTAINTY_NOT_VALID)
	sprintf(s, "%f %s", gpsdata->fix.epv * altunits->factor, altunits->legend);
    else
	strcpy(s, "n/a");
    XmTextFieldSetString(text_8, s);
    if (gpsdata->fix.mode == MODE_3D)
	sprintf(s, "%f %s/sec", gpsdata->fix.climb * altunits->factor, altunits->legend);
    else
	strcpy(s, "n/a");
    XmTextFieldSetString(text_9, s);

    if (!gpsdata->online) {
	newstate = 0;
	sprintf(s, "OFFLINE");
    } else {
	newstate = gpsdata->fix.mode;
	switch (gpsdata->fix.mode) {
	case MODE_2D:
	    sprintf(s, "2D %sFIX",(gpsdata->status==STATUS_DGPS_FIX)?"DIFF ":"");
	    break;
	case MODE_3D:
	    sprintf(s, "3D %sFIX",(gpsdata->status==STATUS_DGPS_FIX)?"DIFF ":"");
	    break;
	default:
	    sprintf(s, "NO FIX");
	    break;
	}
    }
    if (newstate != state) {
	timer = time(NULL);
	state = newstate;
    }
    sprintf(s + strlen(s), " (%d secs)", (int) (time(NULL) - timer));
    XmTextFieldSetString(text_10, s);
    draw_graphics(gpsdata);

    XtRemoveTimeOut(timeout);
    timeout = XtAppAddTimeOut(app, 2000, handle_time_out, NULL);
}

static char *get_resource(Widget w, char *name, char *default_value)
{
  XtResource xtr;
  char *value = NULL;

  xtr.resource_name = name;
  xtr.resource_class = "AnyClass";
  xtr.resource_type = XmRString;
  xtr.resource_size = sizeof(String);
  xtr.resource_offset = 0;
  xtr.default_type = XmRImmediate;
  xtr.default_addr = default_value;
  XtGetApplicationResources(w, &value, &xtr, 1, NULL, 0);
  if (value) return value;
  return default_value;
}

int main(int argc, char *argv[])
{
    int option;
    char *colon, *server = NULL, *port = DEFAULT_GPSD_PORT;
    char *su, *au;
    char *err_str = NULL;

    toplevel = XtVaAppInitialize(&app, "xgps", 
			       options, XtNumber(options), 
			       &argc,argv, fallback_resources,NULL);

    su = get_resource(toplevel, "speedunits", "mph");
    for (speedunits = speedtable; 
	 speedunits < speedtable + sizeof(speedtable)/sizeof(speedtable[0]);
	 speedunits++)
	if (strcmp(speedunits->legend, su) == 0)
	    goto speedunits_ok;
    speedunits = speedtable;
    fprintf(stderr, "xgps: unknown speed unit, defaulting to %s\n", speedunits->legend);
speedunits_ok:;

    au = get_resource(toplevel, "altunits",   "feet");
    for (altunits = alttable; 
	 altunits < alttable + sizeof(alttable)/sizeof(alttable[0]);
	 altunits++)
	if (strcmp(altunits->legend, au) == 0)
	    goto altunits_ok;
    altunits = alttable;
    fprintf(stderr, "xgps: unknown altitude unit, defaulting to %s\n", altunits->legend);
altunits_ok:;

    while ((option = getopt(argc, argv, "?hv")) != -1) {
	switch (option) {
	case 'v':
	    printf("xgps %s\n", VERSION);
	    exit(0);
	case 'h': case '?': default:
	    fputs("usage:  xgps [-?hv] [-speedunits {mph,kph,knots}] [-altunits {ft,meters}] [server[:port]]\n", stderr);
	    exit(1);
	}
    }
    if (optind < argc) {
	server = strdup(argv[optind]);
	colon = strchr(server, ':');
	if (colon != NULL) {
	    server[colon - server] = '\0';
	    port = colon + 1;
	}
    }

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
	fprintf( stderr, "xgps: no gpsd running or network error: %d, %s\n"
		, errno, err_str);
	exit(2);
    }

    build_gui(toplevel);

    timeout = XtAppAddTimeOut(app, 2000, handle_time_out, app);
    timer = time(NULL);

    gps_set_raw_hook(gpsdata, update_panel);
    gps_query(gpsdata, "w+x\n");

    XtAppAddInput(app, gpsdata->gps_fd, 
		  (XtPointer)XtInputReadMask, handle_input, NULL);
    XtAppMainLoop(app);

    gps_close(gpsdata);
    return 0;
}
