/* json.c - unit test for JSON partsing into fixed-extent structures */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "json.h"
#include "gps.h"

#define ASSERT_CASE(num, status) \
    if (status < 0) \
    { \
	(void)fprintf(stderr, "case %d FAILED, status %d.\n", num, status); \
	exit(1); \
    }

#define ASSERT_STRING(attr, fld, val)		\
    if (strcmp(fld, val)) \
    {\
	(void)fprintf(stderr, "'%s' attribute eval failed, value = %s.\n", attr, fld); \
	exit(1);\
    }

#define ASSERT_INTEGER(attr, fld, val)		\
    if (fld != val)\
    {\
	(void)fprintf(stderr, "'%s' attribute eval failed, value = %d.\n", attr, fld); \
	exit(1);\
    }

#define ASSERT_BOOLEAN(attr, fld, val)		\
    if (fld != val)\
    {\
	(void)fprintf(stderr, "'%s' attribute eval failed, value = %s.\n", attr, fld ? "true" : "false"); \
	exit(1);\
    }

/*
 * Floating point comparisons are iffy, but at least if any of these fail
 * the output will make it clear whether it was a precision issue
 */
#define ASSERT_REAL(attr, fld, val)		\
    if (fld != val)\
    {\
	(void)fprintf(stderr, "'%s' attribute eval failed, value = %f.\n", attr, fld); \
	exit(1);\
    }

const char *json_str1 = "{\"device\":\"GPS#1\",\"tag\":\"MID2\",\
    \"time\":1119197561.890,\"lon\":46.498203637,\"lat\":7.568074350,\
    \"alt\":1327.780,\"eph\":21.000,\"epv\":124.484,\"mode\":3,\
    \"flag1\":true,\"flag2\":false}";

static char buf1[JSON_VAL_MAX+1];
static char buf2[JSON_VAL_MAX+1];
static struct gps_fix_t fix;
bool flag1, flag2;

const struct json_attr_t json_attrs_1[] = {
    {"device", string,  .addr.string = buf1},
    {"tag",    string,  .addr.string = buf2},
    {"time",   real,    .addr.real = &fix.time,      .dflt.real = 0},
    {"lon",    real,    .addr.real = &fix.longitude, .dflt.real = 0},
    {"lat",    real,    .addr.real = &fix.latitude,  .dflt.real = 0},
    {"alt",    real,    .addr.real = &fix.altitude,  .dflt.real = 0},
    {"eph",    real,    .addr.real = &fix.eph,       .dflt.real = 0},
    {"epv",    real,    .addr.real = &fix.epv,       .dflt.real = 0},
    {"mode",   integer, .addr.integer = &fix.mode,   .dflt.integer = -1},
    {"flag1",  boolean, .addr.boolean = &flag1,},
    {"flag2",  boolean, .addr.boolean = &flag2,},
    {NULL},
};

int main(int argc, char *argv[])
{
    int status;

    (void)fprintf(stderr, "JSON unit test ");

    status = parse_json(json_str1, json_attrs_1);
    ASSERT_CASE(1, status);
    ASSERT_STRING("device", buf1, "GPS#1");
    ASSERT_STRING("tag", buf2, "MID2");
    ASSERT_INTEGER("mode", fix.mode, 3);
    ASSERT_REAL("time", fix.time, 1119197561.890);
    ASSERT_REAL("lon", fix.longitude, 46.498203637);
    ASSERT_REAL("lat", fix.latitude, 7.568074350);
    ASSERT_BOOLEAN("flag1", flag1, true);
    ASSERT_BOOLEAN("flag2", flag2, false);

    (void)fprintf(stderr, "succeeded.\n");
    exit(0);
}
