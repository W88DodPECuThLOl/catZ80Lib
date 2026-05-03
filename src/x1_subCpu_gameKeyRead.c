#include "x1_subCpu_gameKeyRead.h"

__sfr __banked __at(0x1A01) IoPort_1A01;
__sfr __banked __at(0x1900) IoPort_1900;

static inline void
subCpuWaitReadyWrite(void)
{
	while(IoPort_1A01 & 0x40) {}
}

static inline void
subCpuWaitReadyRead(void)
{
	while(IoPort_1A01 & 0x20) {}
}

static inline void
subCpuSend1(const u8 data)
{
	subCpuWaitReadyWrite();
	IoPort_1900 = data;
}

static inline u8
subCpuGet1(void)
{
	subCpuWaitReadyRead();
	return IoPort_1900;
}

void
x1_subCpuGameKeyRead(u8* keyData)
{
	__asm__("di");
	subCpuSend1(0xE3);
	subCpuWaitReadyWrite();
	*keyData++ = subCpuGet1();
	*keyData++ = subCpuGet1();
	*keyData   = subCpuGet1();
	__asm__("ei");
}
