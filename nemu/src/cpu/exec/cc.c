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

	uint32_t myZF,myOF,myCF,mySF;
	
	rtl_get_ZF(&myZF);
	rtl_get_OF(&myOF);
	rtl_get_CF(&myCF);
	rtl_get_SF(&mySF);
	
  // TODO: Query EFLAGS to determine whether the condition code is satisfied.
  // dest <- ( cc is satisfied ? 1 : 0)
  switch (subcode & 0xe) {
    case CC_O:
    case CC_B:
    case CC_E: *dest=(myZF?1:0); break;
    case CC_BE:
    case CC_S:
    case CC_L: *dest=((mySF!=myOF)?1:0); break;
    case CC_LE:
      TODO();
    default: panic("should not reach here");
    case CC_P: panic("n86 does not have PF");
  }

  if (invert) {
    rtl_xori(dest, dest, 0x1);
  }
}
