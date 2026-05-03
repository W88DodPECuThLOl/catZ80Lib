#ifndef INCL_x1_gra_setVRAMAccessBank__h
#define INCL_x1_gra_setVRAMAccessBank__h

__sfr __banked __at(0x1FD0) IoPort_1FD0;

/**
 * @brief バンク0アクセス
 */
static inline x1_graSetVRAMAccessBank0() { IoPort_1FD0 = (IoPort_1FD0 & 0xEF); }

/**
 * @brief バンク1アクセス
 */
static inline x1_graSetVRAMAccessBank1() { IoPort_1FD0 = (IoPort_1FD0 | 0x10); }

#endif // INCL_x1_gra_setVRAMAccessBank__h
