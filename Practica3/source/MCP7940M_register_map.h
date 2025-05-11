/*
 * MCP7940M_register_map.h
 *
 *  Created on: Apr 26, 2021
 *      Author: Fernando
 */

#ifndef MCP7940M_REGISTER_MAP_H_
#define MCP7940M_REGISTER_MAP_H_

/**
 *@brief Address for RTC
 * */

typedef enum {RTCSEC,
			  RTCMIN,
			  RTCHOUR,
			  RTCWKDAY,
			  RTCDATE,
			  RTCMTH,
			  RTCYEAR,
			  CONTROL,
	          OSCTRIM,
	          reserved_1,
			  ALM0SEC,
			  ALM0MIN,
			  ALM0HOUR,
			  ALM0WKDAY,
			  ALM0DATE,
			  ALM0MTH,
			  reserved_2,
			  ALM1SEC,
			  ALM1MIN,
			  ALM1HOUR,
			  ALM1WDAY,
			  ALM1DATE,
			  ALM1MTH
			} map_register_t;


#endif /* MCP7940M_REGISTER_MAP_H_ */
