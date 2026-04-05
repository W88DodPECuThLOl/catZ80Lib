#ifndef INCL_catZ80Lib__h
#define INCL_catZ80Lib__h

#include "catBasicTypes.h"
#include "z80_inp.h"
#include "z80_outp.h"

#if !defined(CAT_Z80_LIB_DISABLE_SOS_FUNCTIONS)
#include "sos_msx.h"
#include "sos_printf.h"
#endif // !defined(CAT_Z80_LIB_DISABLE_SOS_FUNCTIONS)

#if !defined(CAT_Z80_LIB_DISABLE_X1_FUNCTIONS)
#include "x1_emm0_copyFromEmm0.h"
#include "x1_emm0_copyToBank0FromEmm0.h"
#include "x1_emm0_copyToBank1FromEmm0.h"
#include "x1_emm0_setEmm0Address.h"
#include "x1_gra_setPaletteZ.h"
#include "x1_portAddress.h"
#endif // !defined(CAT_Z80_LIB_DISABLE_X1_FUNCTIONS)

#endif // INCL_catZ80Lib__h
