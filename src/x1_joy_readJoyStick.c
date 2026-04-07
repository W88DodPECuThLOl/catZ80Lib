#include "x1_joy_readJoyStick.h"

__sfr __banked __at(0x1B00) PSG_DATA;
__sfr __banked __at(0x1C00) PSG_REGISTER_NO;

u8
x1_readJoyStick(u8 index)
{
    __asm__ ("di"); // 割り込み禁止

	// ジョイスティックを入力設定に
	PSG_REGISTER_NO = 7; // PSG Reg7
	PSG_DATA = PSG_DATA & 0x3F;

	// 値を読み込む
	if(index == 0) {
		PSG_REGISTER_NO = 14; // PSG Reg14
	} else {
		PSG_REGISTER_NO = 15; // PSG Reg15
	}
	u8 data = PSG_DATA;

	__asm__ ("ei"); // 割り込み許可
	return data;
}
