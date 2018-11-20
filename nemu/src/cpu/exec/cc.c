#include "cpu/rtl.h"

/* Condition Code */

void rtl_setcc(rtlreg_t* dest, uint8_t subcode) {
  bool invert = subcode & 0x1;
  enum {
    CC_O, CC_NO, CC_B,  CC_NB,
    CC_E, CC_NE, CC_BE, CC_NBE,
    CC_S, CC_NS, CC_P,  CC_NP,
    CC_L, CC_NL, CC_LE, CC_NLE
  };

  // TODO: Query EFLAGS to determine whether the condition code is satisfied.
  // dest <- ( cc is satisfied ? 1 : 0)
  rtlreg_t term, term1, term2;
  switch (subcode & 0xe) {
    case CC_O: rtl_get_OF(dest); break;
    case CC_B: rtl_get_CF(dest); break;
    case CC_E: rtl_get_ZF(dest); break;
    case CC_BE:
      rtl_get_ZF(&term);
      rtl_get_CF(dest);
      *dest = *dest | term;
      break;
    case CC_S: rtl_get_SF(dest); break;
    case CC_L:
      rtl_get_SF(&term);
      rtl_get_OF(dest);
      *dest = (*dest != term);
      break;
    case CC_LE:
      rtl_get_SF(&term1);
      rtl_get_OF(&term2);
      rtl_get_ZF(dest);
      *dest = (*dest || (term1 != term2));
      break;
      TODO();
    default: panic("should not reach here");
    case CC_P: panic("n86 does not have PF");
  }

  if (invert) {
    rtl_xori(dest, dest, 0x1);
  }
}
