#ifndef DEBUG_H
#define DEBUG_H

#define DBG_SEND_MONITOR (1<<0)
#define DBG_CONNECT_ROBOT (1<<1)
#define DBG_RECV_MONITOR (1<<2)
#define DBG_MOVE_ROBOT (1<<3)
#define DBG_BATTERY_STATE (1<<4)
#define DBG_IMAGE (1<<5)
#define DBG_ALL (DBG_SEND_MONITOR | DBG_CONNECT_ROBOT | DBG_RECV_MONITOR | DBG_MOVE_ROBOT | DBG_BATTERY_STATE | DBG_IMAGE)

#define DEBUG_WHAT DBG_ALL & ~DBG_BATTERY_STATE

#define DPRINTF(fmt, ...) \
	rt_printf("[%s:%d] " fmt, __func__, __LINE__, ## __VA_ARGS__); \
	rt_printf("%08x", DEBUG_WHAT)

#define BEGIN_THREAD() DPRINTF("starting thread ! \n")

#if DEBUG_WHAT & DBG_SEND_MONITOR
	#define LOG_SEND_MONITOR(fmt, ...) DPRINTF(fmt, ## __VA_ARGS__)
#else
	#define LOG_SEND_MONITOR(fmt, ...)
#endif

#if DEBUG_WHAT & DBG_CONNECT_ROBOT
	#define LOG_CONNECT_ROBOT(fmt, ...) DPRINTF(fmt, ## __VA_ARGS__)
#else
	#define LOG_CONNECT_ROBOT(fmt, ...)
#endif

#if DEBUG_WHAT & DBG_RECV_MONITOR
	#define LOG_RECV_MONITOR(fmt, ...) DPRINTF(fmt, ## __VA_ARGS__)
#else
	#define LOG_RECV_MONITOR(fmt, ...)
#endif

#if DEBUG_WHAT & DBG_MOVE_ROBOT
	#define LOG_MOVE_ROBOT(fmt, ...) DPRINTF(fmt, ## __VA_ARGS__)
#else
	#define LOG_MOVE_ROBOT(fmt, ...)
#endif

#if DEBUG_WHAT & DBG_BATTERY_STATE
	#define LOG_BATTERY_STATE(fmt, ...) DPRINTF(fmt, ## __VA_ARGS__)
#else
	#define LOG_BATTERY_STATE(fmt, ...)
#endif

#if DEBUG_WHAT & DBG_IMAGE
	#define LOG_IMAGE(fmt, ...) DPRINTF(fmt, ## __VA_ARGS__)
#else
	#define LOG_IMAGE(fmt, ...)
#endif

#endif