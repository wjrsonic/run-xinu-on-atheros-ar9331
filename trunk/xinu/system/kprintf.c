/* kprintf.c -  kputc, kgetc, kprintf */

#include <xinu.h>
#include <stdarg.h>

/*------------------------------------------------------------------------
 * kputc - use polled I/O to write a character to the console serial line
 *------------------------------------------------------------------------
 */
syscall kputc(byte c)
{
    int uart_data;
    volatile struct uart_csreg *regptr;
    struct  dentry  *devptr;

    devptr = (struct dentry *) &devtab[CONSOLE];
    regptr = (struct uart_csreg *)devptr->dvcsr;

    do {
        uart_data = regptr->uart_data;
    }while((uart_data & UART_TX_CSR) == 0);

    uart_data = c;
    uart_data |= UART_TX_CSR;

    regptr->uart_data = uart_data;

    return c;
}

/*------------------------------------------------------------------------
 * kgetc - use polled I/O to read a character from the console serial line
 *------------------------------------------------------------------------
 */
syscall kgetc(void)
{
    int uart_data;
    volatile struct uart_csreg *regptr;
    byte c;
    struct  dentry  *devptr;

    devptr = (struct dentry *) &devtab[CONSOLE];
    regptr = (struct uart_csreg *)devptr->dvcsr;

    uart_data = regptr->uart_data;
    if( uart_data & UART_RX_CSR )
    {
        c = uart_data & 0xff;
        uart_data = UART_RX_CSR;

        regptr->uart_data = uart_data;      
    }
    return c;
}

extern  void    _doprnt(char *,int (*)(int), ...);

/*------------------------------------------------------------------------
 *  kprintf - kernel printf using unbuffered, polled output to CONSOLE
 *------------------------------------------------------------------------
 */
syscall kprintf(char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    _doprnt(fmt, ap, (int (*)(int))kputc, (int)&devtab[CONSOLE]);
    va_end(ap);
    return OK;
}
