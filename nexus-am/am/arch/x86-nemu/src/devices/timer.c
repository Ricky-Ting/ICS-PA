#include <am.h>
#include <x86.h>
#include <amdev.h>

_UptimeReg myoldtime;
_UptimeReg systemtime;

size_t timer_read(uintptr_t reg, void *buf, size_t size) {
  switch (reg) {
    case _DEVREG_TIMER_UPTIME: {
      _UptimeReg *uptime = (_UptimeReg *)buf;
      uptime->lo = inl(0x48);
			if(uptime->lo<myoldtime.lo)
							myoldtime.hi++;
			myoldtime.lo=uptime->lo;
			
			if(uptime->lo>=systemtime.lo) {
					uptime->lo=uptime->lo - systemtime.lo;
					uptime->hi=myoldtime.hi;
			}
			else {
					uptime->lo=0xffffffff-systemtime.lo+uptime->lo;
					uptime->hi=myoldtime.hi-1;
			}


      return sizeof(_UptimeReg);
    }
    case _DEVREG_TIMER_DATE: {
      _RTCReg *rtc = (_RTCReg *)buf;
      rtc->second = 0;
      rtc->minute = 0;
      rtc->hour   = 0;
      rtc->day    = 0;
      rtc->month  = 0;
      rtc->year   = 2018;
      return sizeof(_RTCReg);
    }
  }
  return 0;
}

void timer_init() {
	systemtime.lo=inl(0x48);
	systemtime.hi=0;
	myoldtime.lo=0;
	myoldtime.hi=0;
}
