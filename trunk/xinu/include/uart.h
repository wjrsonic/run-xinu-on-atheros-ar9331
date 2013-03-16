/* uart.h - definintions for the NS16550 uart serial hardware */

#define UART_BAUD   115200  /* Default console baud rate.       */
#define UART_OUT_IDLE   0x0016  /* determine if transmit idle       */
#define UART_FIFO_SIZE  16  /* chars in UART onboard output FIFO    */

/*
 * Control and Status Register (CSR) definintions for the 16550 UART.
 * The code maps the structure structure directly onto the base address
 * CSR address for the device.
 */
struct  uart_csreg
{
    volatile uint32 uart_data;  /* Transmit and Rx FIFO     */
    volatile uint32 uart_cs;    /* Configuration and Status */
    volatile uint32 uart_clock; /* Clock        */
    volatile uint32 uart_int;   /* Interrupt    */
    volatile uint32 uart_int_en;    /* Interrupt enable     */
};

#define UART_TX_CSR                 (1<<9)
#define UART_RX_CSR                 (1<<8)

#define UART_HOST_INT_EN            (1<<13)
#define UART_HOST_INT               (1<<12)


#define UART_TX_EMPTY_INT           (1<<9)
#define UART_RX_VALID_INT           (1<<0)

#define UART_TX_EMPTY_INT_EN        (1<<9)
#define UART_RX_VALID_INT_EN        (1<<0)

