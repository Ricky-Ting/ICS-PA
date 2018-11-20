#include <am.h>
#include <x86.h>
#include <amdev.h>

#define TIMER_PORT 0x48

static uint64_t timer_base;
static _UptimeReg timer_now;

size_t timer_read(uintptr_t reg, void *buf, size_t size) {
  switch (reg) {
    case _DEVREG_TIMER_UPTIME: {
      _UptimeReg *uptime = (_UptimeReg *)buf;
      uint32_t get_time = inl(TIMER_PORT);
      if(get_time < timer_base)
        timer_now.hi++;
      timer_now.lo = get_time - timer_base;
      uptime->hi = timer_now.hi;
      uptime->lo = timer_now.lo;
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
  //timer_base = add_pio_map(TIMER_PORT, 8, timer_read);
  //*timer_base = clock();
  timer_base = inl(TIMER_PORT);
  timer_now.lo = timer_base;
  timer_now.hi = 0;
}
