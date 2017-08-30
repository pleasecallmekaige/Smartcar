/*
 * File:		io.c
 * Purpose:		Serial Input/Output routines
 *
 * Notes:       TERMINAL_PORT defined in <board>.h
 */

#include "common.h"
#include "MK60_uart.h"

/********************************************************************/
char
in_char (void)
{
    char ch;
    uart_getchar(VCAN_PORT,&ch);
    return ch;
}
/********************************************************************/
void
out_char (char ch)
{
    uart_putchar(VCAN_PORT, ch);
}
/********************************************************************/
int
char_present (void)
{
    return uart_query(VCAN_PORT);
}
/********************************************************************/
