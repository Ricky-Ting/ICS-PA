#include <am.h>
#include <x86.h>
#include <amdev.h>

#define I8042_DATA_PORT 0x60

size_t input_read(uintptr_t reg, void *buf, size_t size) {
  switch (reg) {
    case _DEVREG_INPUT_KBD: {
      _KbdReg *kbd = (_KbdReg *)buf;
      uint32_t info = inl(I8042_DATA_PORT);
      kbd->keydown = (info & 0x8000) > 0;
      kbd->keycode = info | 0x8000;
      return sizeof(_KbdReg);
    }
  }
  return 0;
}
