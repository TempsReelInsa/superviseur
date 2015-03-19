#ifndef DEBUG_H
#define DEBUG_H

#include "term_colors.h"

#define DBG_SEND_MONITOR (1<<0)
#define DBG_CONNECT_ROBOT (1<<1)
#define DBG_RECV_MONITOR (1<<2)
#define DBG_MOVE_ROBOT (1<<3)
#define DBG_BATTERY_STATE (1<<4)
#define DBG_IMAGE (1<<5)
#define DBG_ALL (DBG_SEND_MONITOR | DBG_CONNECT_ROBOT | DBG_RECV_MONITOR | DBG_MOVE_ROBOT | DBG_BATTERY_STATE | DBG_IMAGE)

#define DEBUG_WHAT DBG_ALL

#define DPRINTF(fmt, ...) \
	rt_printf("[%s:%d] " fmt, __func__, __LINE__, ## __VA_ARGS__)

#define DPRINTF_C(color, fmt, ...) \
	rt_printf("[" COLOR("%s", color) ":" COLOR("%d", GREEN) "] " fmt, __func__, __LINE__, ## __VA_ARGS__)

#define BEGIN_THREAD() DPRINTF_C(BRIGHT_BLACK, "starting thread ! \n")

#if DEBUG_WHAT & DBG_SEND_MONITOR
	#define LOG_SEND_MONITOR(fmt, ...) DPRINTF_C(BRIGHT_RED, fmt, ## __VA_ARGS__)
#else
	#define LOG_SEND_MONITOR(fmt, ...)
#endif

#if DEBUG_WHAT & DBG_CONNECT_ROBOT
	#define LOG_CONNECT_ROBOT(fmt, ...) DPRINTF_C(BRIGHT_YELLOW, fmt, ## __VA_ARGS__)
#else
	#define LOG_CONNECT_ROBOT(fmt, ...)
#endif

#if DEBUG_WHAT & DBG_RECV_MONITOR
	#define LOG_RECV_MONITOR(fmt, ...) DPRINTF_C(BRIGHT_BLUE, fmt, ## __VA_ARGS__)
#else
	#define LOG_RECV_MONITOR(fmt, ...)
#endif

#if DEBUG_WHAT & DBG_MOVE_ROBOT
	#define LOG_MOVE_ROBOT(fmt, ...) DPRINTF_C(BRIGHT_CYAN, fmt, ## __VA_ARGS__)
#else
	#define LOG_MOVE_ROBOT(fmt, ...)
#endif

#if DEBUG_WHAT & DBG_BATTERY_STATE
	#define LOG_BATTERY_STATE(fmt, ...) DPRINTF_C(BRIGHT_WHITE, fmt, ## __VA_ARGS__)
#else
	#define LOG_BATTERY_STATE(fmt, ...)
#endif

#if DEBUG_WHAT & DBG_IMAGE
	#define LOG_IMAGE(fmt, ...) DPRINTF_C(BRIGHT_VIOLET, fmt, ## __VA_ARGS__)
#else
	#define LOG_IMAGE(fmt, ...)
#endif

#endif