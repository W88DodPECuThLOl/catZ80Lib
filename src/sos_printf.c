#include <stdio.h>
#include <stdarg.h>
#include "sos_printf.h"
#include "sos_msx.h"

static char sos_printf_text_buffer[SOS_PRINTF_BUFFER_SIZE];

void
sos_printf(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    vsprintf(sos_printf_text_buffer, format, args);
    va_end(args);

    sos_msx(sos_printf_text_buffer);
}
