/*
 * This is code generated by jsongen.py. Do not hand-hack it!
 */
 
/*@ -fullinitblock */


    const struct json_attr_t json_ais1[] = {
	AIS_HEADER,
	{"status",        t_uinteger,  .addr.uinteger = &ais->type1.status,
                                       .dflt.uinteger = 0},
	{"turn",          t_integer,   .addr.integer = &ais->type1.turn,
                                       .dflt.integer = AIS_TURN_NOT_AVAILABLE},
	{"speed",         t_uinteger,  .addr.uinteger = &ais->type1.speed,
                                       .dflt.uinteger = AIS_SPEED_NOT_AVAILABLE},
	{"accuracy",      t_boolean,   .addr.boolean = &ais->type1.accuracy,
                                       .dflt.boolean = false},
	{"lon",           t_integer,   .addr.integer = &ais->type1.lon,
                                       .dflt.integer = AIS_LON_NOT_AVAILABLE},
	{"lat",           t_integer,   .addr.integer = &ais->type1.lat,
                                       .dflt.integer = AIS_LAT_NOT_AVAILABLE},
	{"course",        t_uinteger,  .addr.uinteger = &ais->type1.course,
                                       .dflt.uinteger = AIS_COURSE_NOT_AVAILABLE},
	{"heading",       t_uinteger,  .addr.uinteger = &ais->type1.heading,
                                       .dflt.uinteger = AIS_HEADING_NOT_AVAILABLE},
	{"second",        t_uinteger,  .addr.uinteger = &ais->type1.second,
                                       .dflt.uinteger = AIS_SEC_NOT_AVAILABLE},
	{"maneuver",      t_uinteger,  .addr.uinteger = &ais->type1.maneuver,
                                       .dflt.uinteger = AIS_SEC_INOPERATIVE},
	{"raim",          t_boolean,   .addr.boolean = &ais->type1.raim,
                                       .dflt.boolean = false},
	{"radio",         t_uinteger,  .addr.uinteger = &ais->type1.radio,
                                       .dflt.uinteger = 0},
	{NULL},
    };

    char timestamp[JSON_VAL_MAX+1];
    const struct json_attr_t json_ais4[] = {
	AIS_HEADER,
	{"timestamp",     t_string,    .addr.string = timestamp,
                                       .len = sizeof(timestamp)},
	{"accuracy",      t_boolean,   .addr.boolean = &ais->type4.accuracy,
                                       .dflt.boolean = true},
	{"lon",           t_integer,   .addr.integer = &ais->type4.lon,
                                       .dflt.integer = AIS_LON_NOT_AVAILABLE},
	{"lat",           t_integer,   .addr.integer = &ais->type4.lat,
                                       .dflt.integer = AIS_LAT_NOT_AVAILABLE},
	{"epfd",          t_uinteger,  .addr.uinteger = &ais->type4.epfd,
                                       .dflt.uinteger = 0},
	{"raim",          t_boolean,   .addr.boolean = &ais->type4.raim,
                                       .dflt.boolean = false},
	{"radio",         t_uinteger,  .addr.uinteger = &ais->type4.radio,
                                       .dflt.uinteger = 0},
	{NULL},
    };

    char eta[JSON_VAL_MAX+1];
    const struct json_attr_t json_ais5[] = {
	AIS_HEADER,
	{"imo",           t_uinteger,  .addr.uinteger = &ais->type5.imo,
                                       .dflt.uinteger = 0},
	{"ais_version",   t_uinteger,  .addr.uinteger = &ais->type5.ais_version,
                                       .dflt.uinteger = 0},
	{"callsign",      t_string,    .addr.string = ais->type5.callsign,
                                       .len = sizeof(ais->type5.callsign)},
	{"shipname",      t_string,    .addr.string = ais->type5.shipname,
                                       .len = sizeof(ais->type5.shipname)},
	{"shiptype",      t_uinteger,  .addr.uinteger = &ais->type5.shiptype,
                                       .dflt.uinteger = 0},
	{"to_bow",        t_uinteger,  .addr.uinteger = &ais->type5.to_bow,
                                       .dflt.uinteger = 0},
	{"to_stern",      t_uinteger,  .addr.uinteger = &ais->type5.to_stern,
                                       .dflt.uinteger = 0},
	{"to_port",       t_uinteger,  .addr.uinteger = &ais->type5.to_port,
                                       .dflt.uinteger = 0},
	{"to_starboard",  t_uinteger,  .addr.uinteger = &ais->type5.to_starboard,
                                       .dflt.uinteger = 0},
	{"epfd",          t_uinteger,  .addr.uinteger = &ais->type5.epfd,
                                       .dflt.uinteger = 0},
	{"eta",           t_string,    .addr.string = eta,
                                       .len = sizeof(eta)},
	{"draught",       t_uinteger,  .addr.uinteger = &ais->type5.draught,
                                       .dflt.uinteger = 0},
	{"destination",   t_string,    .addr.string = ais->type5.destination,
                                       .len = sizeof(ais->type5.destination)},
	{"dte",           t_uinteger,  .addr.uinteger = &ais->type5.dte,
                                       .dflt.uinteger = 1},
	{NULL},
    };

    char data[JSON_VAL_MAX+1];
    const struct json_attr_t json_ais6[] = {
	AIS_HEADER,
	{"seqno",         t_uinteger,  .addr.uinteger = &ais->type6.seqno,
                                       .dflt.uinteger = 0},
	{"dest_mmsi",     t_uinteger,  .addr.uinteger = &ais->type6.dest_mmsi,
                                       .dflt.uinteger = 0},
	{"retransmit",    t_boolean,   .addr.boolean = &ais->type6.retransmit,
                                       .dflt.boolean = false},
	{"app_id",        t_uinteger,  .addr.uinteger = &ais->type6.app_id,
                                       .dflt.uinteger = 0},
	{"data",          t_string,    .addr.string = data,
                                       .len = sizeof(data)},
	{NULL},
    };

    const struct json_attr_t json_ais7[] = {
	AIS_HEADER,
	{"mmsi1",         t_uinteger,  .addr.uinteger = &ais->type7.mmsi1,
                                       .dflt.uinteger = 0},
	{"mmsi2",         t_uinteger,  .addr.uinteger = &ais->type7.mmsi2,
                                       .dflt.uinteger = 0},
	{"mmsi3",         t_uinteger,  .addr.uinteger = &ais->type7.mmsi3,
                                       .dflt.uinteger = 0},
	{"mmsi4",         t_uinteger,  .addr.uinteger = &ais->type7.mmsi4,
                                       .dflt.uinteger = 0},
	{NULL},
    };

    const struct json_attr_t json_ais8[] = {
	AIS_HEADER,
	{"app_id",        t_uinteger,  .addr.uinteger = &ais->type8.app_id,
                                       .dflt.uinteger = 0},
	{"data",          t_string,    .addr.string = data,
                                       .len = sizeof(data)},
	{NULL},
    };

    const struct json_attr_t json_ais9[] = {
	AIS_HEADER,
	{"alt",           t_uinteger,  .addr.uinteger = &ais->type9.alt,
                                       .dflt.uinteger = AIS_ALT_NOT_AVAILABLE},
	{"speed",         t_uinteger,  .addr.uinteger = &ais->type9.speed,
                                       .dflt.uinteger = AIS_SPEED_NOT_AVAILABLE},
	{"accuracy",      t_boolean,   .addr.boolean = &ais->type9.accuracy,
                                       .dflt.boolean = false},
	{"lon",           t_integer,   .addr.integer = &ais->type9.lon,
                                       .dflt.integer = AIS_LON_NOT_AVAILABLE},
	{"lat",           t_integer,   .addr.integer = &ais->type9.lat,
                                       .dflt.integer = AIS_LAT_NOT_AVAILABLE},
	{"course",        t_uinteger,  .addr.uinteger = &ais->type9.course,
                                       .dflt.uinteger = AIS_COURSE_NOT_AVAILABLE},
	{"second",        t_uinteger,  .addr.uinteger = &ais->type9.second,
                                       .dflt.uinteger = AIS_SEC_NOT_AVAILABLE},
	{"regional",      t_uinteger,  .addr.uinteger = &ais->type9.regional,
                                       .dflt.uinteger = 0},
	{"dte",           t_uinteger,  .addr.uinteger = &ais->type9.dte,
                                       .dflt.uinteger = 1},
	{"raim",          t_boolean,   .addr.boolean = &ais->type9.raim,
                                       .dflt.boolean = false},
	{"radio",         t_uinteger,  .addr.uinteger = &ais->type9.radio,
                                       .dflt.uinteger = 0},
	{NULL},
    };

    const struct json_attr_t json_ais10[] = {
	AIS_HEADER,
	{"dest_mmsi",     t_uinteger,  .addr.uinteger = &ais->type10.dest_mmsi,
                                       .dflt.uinteger = 0},
	{NULL},
    };

    const struct json_attr_t json_ais12[] = {
	AIS_HEADER,
	{"seqno",         t_uinteger,  .addr.uinteger = &ais->type12.seqno,
                                       .dflt.uinteger = 0},
	{"dest_mmsi",     t_uinteger,  .addr.uinteger = &ais->type12.dest_mmsi,
                                       .dflt.uinteger = 0},
	{"retransmit",    t_boolean,   .addr.boolean = &ais->type12.retransmit,
                                       .dflt.boolean = 0},
	{"text",          t_string,    .addr.string = ais->type12.text,
                                       .len = sizeof(ais->type12.text)},
	{NULL},
    };

    const struct json_attr_t json_ais14[] = {
	AIS_HEADER,
	{"text",          t_string,    .addr.string = ais->type14.text,
                                       .len = sizeof(ais->type14.text)},
	{NULL},
    };

    const struct json_attr_t json_ais15[] = {
	AIS_HEADER,
	{"mmsi1",         t_uinteger,  .addr.uinteger = &ais->type15.mmsi1,
                                       .dflt.uinteger = 0},
	{"type1_1",       t_uinteger,  .addr.uinteger = &ais->type15.type1_1,
                                       .dflt.uinteger = 0},
	{"offset1_1",     t_uinteger,  .addr.uinteger = &ais->type15.offset1_1,
                                       .dflt.uinteger = 0},
	{"type1_2",       t_uinteger,  .addr.uinteger = &ais->type15.type1_2,
                                       .dflt.uinteger = 0},
	{"offset1_2",     t_uinteger,  .addr.uinteger = &ais->type15.offset1_2,
                                       .dflt.uinteger = 0},
	{"mmsi2",         t_uinteger,  .addr.uinteger = &ais->type15.mmsi2,
                                       .dflt.uinteger = 0},
	{"type2_1",       t_uinteger,  .addr.uinteger = &ais->type15.type2_1,
                                       .dflt.uinteger = 0},
	{"offset2_1",     t_uinteger,  .addr.uinteger = &ais->type15.offset2_1,
                                       .dflt.uinteger = 0},
	{NULL},
    };

    const struct json_attr_t json_ais16[] = {
	AIS_HEADER,
	{"mmsi1",         t_uinteger,  .addr.uinteger = &ais->type16.mmsi1,
                                       .dflt.uinteger = 0},
	{"offset1",       t_uinteger,  .addr.uinteger = &ais->type16.offset1,
                                       .dflt.uinteger = 0},
	{"increment1",    t_uinteger,  .addr.uinteger = &ais->type16.increment1,
                                       .dflt.uinteger = 0},
	{"mmsi2",         t_uinteger,  .addr.uinteger = &ais->type16.mmsi2,
                                       .dflt.uinteger = 0},
	{"offset2",       t_uinteger,  .addr.uinteger = &ais->type16.offset2,
                                       .dflt.uinteger = 0},
	{"increment2",    t_uinteger,  .addr.uinteger = &ais->type16.increment2,
                                       .dflt.uinteger = 0},
	{NULL},
    };

    const struct json_attr_t json_ais17[] = {
	AIS_HEADER,
	{"lon",           t_integer,   .addr.integer = &ais->type17.lon,
                                       .dflt.integer = AIS_GNS_LON_NOT_AVAILABLE},
	{"lat",           t_integer,   .addr.integer = &ais->type17.lat,
                                       .dflt.integer = AIS_GNS_LAT_NOT_AVAILABLE},
	{"data",          t_string,    .addr.string = data,
                                       .len = sizeof(data)},
	{NULL},
    };

    const struct json_attr_t json_ais18[] = {
	AIS_HEADER,
	{"reserved",      t_uinteger,  .addr.uinteger = &ais->type18.reserved,
                                       .dflt.uinteger = 0},
	{"speed",         t_uinteger,  .addr.uinteger = &ais->type18.speed,
                                       .dflt.uinteger = AIS_SPEED_NOT_AVAILABLE},
	{"accuracy",      t_boolean,   .addr.boolean = &ais->type18.accuracy,
                                       .dflt.boolean = false},
	{"lon",           t_integer,   .addr.integer = &ais->type18.lon,
                                       .dflt.integer = AIS_LON_NOT_AVAILABLE},
	{"lat",           t_integer,   .addr.integer = &ais->type18.lat,
                                       .dflt.integer = AIS_LAT_NOT_AVAILABLE},
	{"course",        t_uinteger,  .addr.uinteger = &ais->type18.course,
                                       .dflt.uinteger = AIS_COURSE_NOT_AVAILABLE},
	{"heading",       t_uinteger,  .addr.uinteger = &ais->type18.heading,
                                       .dflt.uinteger = AIS_HEADING_NOT_AVAILABLE},
	{"second",        t_uinteger,  .addr.uinteger = &ais->type18.second,
                                       .dflt.uinteger = AIS_SEC_NOT_AVAILABLE},
	{"regional",      t_uinteger,  .addr.uinteger = &ais->type18.regional,
                                       .dflt.uinteger = 0},
	{"cs",            t_boolean,   .addr.boolean = &ais->type18.cs,
                                       .dflt.boolean = false},
	{"display",       t_boolean,   .addr.boolean = &ais->type18.display,
                                       .dflt.boolean = false},
	{"dsc",           t_boolean,   .addr.boolean = &ais->type18.dsc,
                                       .dflt.boolean = false},
	{"band",          t_boolean,   .addr.boolean = &ais->type18.band,
                                       .dflt.boolean = false},
	{"msg22",         t_boolean,   .addr.boolean = &ais->type18.msg22,
                                       .dflt.boolean = false},
	{"raim",          t_boolean,   .addr.boolean = &ais->type18.raim,
                                       .dflt.boolean = false},
	{"radio",         t_uinteger,  .addr.uinteger = &ais->type18.radio,
                                       .dflt.uinteger = 0},
	{NULL},
    };

    const struct json_attr_t json_ais19[] = {
	AIS_HEADER,
	{"reserved",      t_uinteger,  .addr.uinteger = &ais->type19.reserved,
                                       .dflt.uinteger = 0},
	{"speed",         t_uinteger,  .addr.uinteger = &ais->type19.speed,
                                       .dflt.uinteger = AIS_SPEED_NOT_AVAILABLE},
	{"accuracy",      t_boolean,   .addr.boolean = &ais->type19.accuracy,
                                       .dflt.boolean = false},
	{"lon",           t_integer,   .addr.integer = &ais->type19.lon,
                                       .dflt.integer = AIS_LON_NOT_AVAILABLE},
	{"lat",           t_integer,   .addr.integer = &ais->type19.lat,
                                       .dflt.integer = AIS_LAT_NOT_AVAILABLE},
	{"course",        t_uinteger,  .addr.uinteger = &ais->type19.course,
                                       .dflt.uinteger = AIS_COURSE_NOT_AVAILABLE},
	{"heading",       t_uinteger,  .addr.uinteger = &ais->type19.heading,
                                       .dflt.uinteger = AIS_HEADING_NOT_AVAILABLE},
	{"second",        t_uinteger,  .addr.uinteger = &ais->type19.second,
                                       .dflt.uinteger = AIS_SEC_NOT_AVAILABLE},
	{"regional",      t_uinteger,  .addr.uinteger = &ais->type19.regional,
                                       .dflt.uinteger = 0},
	{"shipname",      t_string,    .addr.string = ais->type19.shipname,
                                       .len = sizeof(ais->type19.shipname)},
	{"shiptype",      t_uinteger,  .addr.uinteger = &ais->type19.shiptype,
                                       .dflt.uinteger = 0},
	{"to_bow",        t_uinteger,  .addr.uinteger = &ais->type19.to_bow,
                                       .dflt.uinteger = 0},
	{"to_stern",      t_uinteger,  .addr.uinteger = &ais->type19.to_stern,
                                       .dflt.uinteger = 0},
	{"to_port",       t_uinteger,  .addr.uinteger = &ais->type19.to_port,
                                       .dflt.uinteger = 0},
	{"to_starboard",  t_uinteger,  .addr.uinteger = &ais->type19.to_starboard,
                                       .dflt.uinteger = 0},
	{"epfd",          t_uinteger,  .addr.uinteger = &ais->type19.epfd,
                                       .dflt.uinteger = 0},
	{"raim",          t_boolean,   .addr.boolean = &ais->type19.raim,
                                       .dflt.boolean = false},
	{"dte",           t_uinteger,  .addr.uinteger = &ais->type19.dte,
                                       .dflt.uinteger = 1},
	{"assigned",      t_boolean,   .addr.boolean = &ais->type19.assigned,
                                       .dflt.boolean = false},
	{NULL},
    };

    const struct json_attr_t json_ais20[] = {
	AIS_HEADER,
	{"offset1",       t_uinteger,  .addr.uinteger = &ais->type20.offset1,
                                       .dflt.uinteger = 0},
	{"number1",       t_uinteger,  .addr.uinteger = &ais->type20.number1,
                                       .dflt.uinteger = 0},
	{"timeout1",      t_uinteger,  .addr.uinteger = &ais->type20.timeout1,
                                       .dflt.uinteger = 0},
	{"increment1",    t_uinteger,  .addr.uinteger = &ais->type20.increment1,
                                       .dflt.uinteger = 0},
	{"offset2",       t_uinteger,  .addr.uinteger = &ais->type20.offset2,
                                       .dflt.uinteger = 0},
	{"number2",       t_uinteger,  .addr.uinteger = &ais->type20.number2,
                                       .dflt.uinteger = 0},
	{"timeout2",      t_uinteger,  .addr.uinteger = &ais->type20.timeout2,
                                       .dflt.uinteger = 0},
	{"increment2",    t_uinteger,  .addr.uinteger = &ais->type20.increment2,
                                       .dflt.uinteger = 0},
	{"offset3",       t_uinteger,  .addr.uinteger = &ais->type20.offset3,
                                       .dflt.uinteger = 0},
	{"number3",       t_uinteger,  .addr.uinteger = &ais->type20.number3,
                                       .dflt.uinteger = 0},
	{"timeout3",      t_uinteger,  .addr.uinteger = &ais->type20.timeout3,
                                       .dflt.uinteger = 0},
	{"increment3",    t_uinteger,  .addr.uinteger = &ais->type20.increment3,
                                       .dflt.uinteger = 0},
	{"offset4",       t_uinteger,  .addr.uinteger = &ais->type20.offset4,
                                       .dflt.uinteger = 0},
	{"number4",       t_uinteger,  .addr.uinteger = &ais->type20.number4,
                                       .dflt.uinteger = 0},
	{"timeout4",      t_uinteger,  .addr.uinteger = &ais->type20.timeout4,
                                       .dflt.uinteger = 0},
	{"increment4",    t_uinteger,  .addr.uinteger = &ais->type20.increment4,
                                       .dflt.uinteger = 0},
	{NULL},
    };

    const struct json_attr_t json_ais21[] = {
	AIS_HEADER,
	{"aid_type",      t_uinteger,  .addr.uinteger = &ais->type21.aid_type,
                                       .dflt.uinteger = 0},
	{"name",          t_string,    .addr.string = ais->type21.name,
                                       .len = sizeof(ais->type21.name)},
	{"accuracy",      t_boolean,   .addr.boolean = &ais->type21.accuracy,
                                       .dflt.boolean = false},
	{"lon",           t_integer,   .addr.integer = &ais->type21.lon,
                                       .dflt.integer = AIS_LON_NOT_AVAILABLE},
	{"lat",           t_integer,   .addr.integer = &ais->type21.lat,
                                       .dflt.integer = AIS_LAT_NOT_AVAILABLE},
	{"to_bow",        t_uinteger,  .addr.uinteger = &ais->type21.to_bow,
                                       .dflt.uinteger = 0},
	{"to_stern",      t_uinteger,  .addr.uinteger = &ais->type21.to_stern,
                                       .dflt.uinteger = 0},
	{"to_port",       t_uinteger,  .addr.uinteger = &ais->type21.to_port,
                                       .dflt.uinteger = 0},
	{"to_starboard",  t_uinteger,  .addr.uinteger = &ais->type21.to_starboard,
                                       .dflt.uinteger = 0},
	{"epfd",          t_uinteger,  .addr.uinteger = &ais->type21.epfd,
                                       .dflt.uinteger = 0},
	{"second",        t_uinteger,  .addr.uinteger = &ais->type21.second,
                                       .dflt.uinteger = 0},
	{"regional",      t_uinteger,  .addr.uinteger = &ais->type21.regional,
                                       .dflt.uinteger = 0},
	{"off_position",  t_boolean,   .addr.boolean = &ais->type21.off_position,
                                       .dflt.boolean = false},
	{"raim",          t_boolean,   .addr.boolean = &ais->type21.raim,
                                       .dflt.boolean = false},
	{"virtual_aid",   t_boolean,   .addr.boolean = &ais->type21.virtual_aid,
                                       .dflt.boolean = false},
	{NULL},
    };

    const struct json_attr_t json_ais22[] = {
	AIS_HEADER,
	{"channel_a",     t_uinteger,  .addr.uinteger = &ais->type22.channel_a,
                                       .dflt.uinteger = 0},
	{"channel_b",     t_uinteger,  .addr.uinteger = &ais->type22.channel_b,
                                       .dflt.uinteger = 0},
	{"txrx",          t_uinteger,  .addr.uinteger = &ais->type22.txrx,
                                       .dflt.uinteger = 0},
	{"power",         t_boolean,   .addr.boolean = &ais->type22.power,
                                       .dflt.boolean = false},
	{"ne_lon",        t_integer,   .addr.integer = &ais->type22.area.ne_lon,
                                       .dflt.integer = AIS_GNS_LON_NOT_AVAILABLE},
	{"ne_lat",        t_integer,   .addr.integer = &ais->type22.area.ne_lat,
                                       .dflt.integer = AIS_GNS_LAT_NOT_AVAILABLE},
	{"sw_lon",        t_integer,   .addr.integer = &ais->type22.area.sw_lon,
                                       .dflt.integer = AIS_GNS_LON_NOT_AVAILABLE},
	{"sw_lat",        t_integer,   .addr.integer = &ais->type22.area.sw_lat,
                                       .dflt.integer = AIS_GNS_LAT_NOT_AVAILABLE},
	{"dest1",         t_uinteger,  .addr.uinteger = &ais->type22.mmsi.dest1,
                                       .dflt.uinteger = 0},
	{"dest2",         t_uinteger,  .addr.uinteger = &ais->type22.mmsi.dest2,
                                       .dflt.uinteger = 0},
	{"addressed",     t_boolean,   .addr.boolean = &ais->type22.addressed,
                                       .dflt.boolean = false},
	{"band_a",        t_boolean,   .addr.boolean = &ais->type22.band_a,
                                       .dflt.boolean = false},
	{"band_b",        t_boolean,   .addr.boolean = &ais->type22.band_b,
                                       .dflt.boolean = false},
	{"zonesize",      t_uinteger,  .addr.uinteger = &ais->type22.zonesize,
                                       .dflt.uinteger = 0},
	{NULL},
    };

    const struct json_attr_t json_ais23[] = {
	AIS_HEADER,
	{"ne_lon",        t_integer,   .addr.integer = &ais->type23.ne_lon,
                                       .dflt.integer = AIS_GNS_LON_NOT_AVAILABLE},
	{"ne_lat",        t_integer,   .addr.integer = &ais->type23.ne_lat,
                                       .dflt.integer = AIS_GNS_LAT_NOT_AVAILABLE},
	{"sw_lon",        t_integer,   .addr.integer = &ais->type23.sw_lon,
                                       .dflt.integer = AIS_GNS_LON_NOT_AVAILABLE},
	{"sw_lat",        t_integer,   .addr.integer = &ais->type23.sw_lat,
                                       .dflt.integer = AIS_GNS_LAT_NOT_AVAILABLE},
	{"stationtype",   t_uinteger,  .addr.uinteger = &ais->type23.stationtype,
                                       .dflt.uinteger = 0},
	{"shiptype",      t_uinteger,  .addr.uinteger = &ais->type23.shiptype,
                                       .dflt.uinteger = 0},
	{"txrx",          t_uinteger,  .addr.uinteger = &ais->type23.txrx,
                                       .dflt.uinteger = 0},
	{"interval",      t_uinteger,  .addr.uinteger = &ais->type23.interval,
                                       .dflt.uinteger = 0},
	{"quiet",         t_uinteger,  .addr.uinteger = &ais->type23.quiet,
                                       .dflt.uinteger = 0},
	{NULL},
    };

    const struct json_attr_t json_ais24[] = {
	AIS_HEADER,
	{"shipname",      t_string,    .addr.string = ais->type24.shipname,
                                       .len = sizeof(ais->type24.shipname)},
	{"shiptype",      t_uinteger,  .addr.uinteger = &ais->type24.shiptype,
                                       .dflt.uinteger = 0},
	{"vendorid",      t_string,    .addr.string = ais->type24.vendorid,
                                       .len = sizeof(ais->type24.vendorid)},
	{"callsign",      t_string,    .addr.string = ais->type24.callsign,
                                       .len = sizeof(ais->type24.callsign)},
	{"mothership_mmsi",t_uinteger,  .addr.uinteger = &ais->type24.mothership_mmsi,
                                       .dflt.uinteger = 0},
	{"to_bow",        t_uinteger,  .addr.uinteger = &ais->type24.dim.to_bow,
                                       .dflt.uinteger = 0},
	{"to_stern",      t_uinteger,  .addr.uinteger = &ais->type24.dim.to_stern,
                                       .dflt.uinteger = 0},
	{"to_port",       t_uinteger,  .addr.uinteger = &ais->type24.dim.to_port,
                                       .dflt.uinteger = 0},
	{"to_starboard",  t_uinteger,  .addr.uinteger = &ais->type24.dim.to_starboard,
                                       .dflt.uinteger = 0},
	{NULL},
    };


/*@ +fullinitblock */

/* Generated code ends. */

