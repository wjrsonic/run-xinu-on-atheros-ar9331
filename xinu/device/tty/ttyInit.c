/* ttyInit.c - ttyInit */

#include <xinu.h>

struct	ttycblk	ttytab[Ntty];

/*------------------------------------------------------------------------
 *  ttyInit - initialize buffers and modes for a tty line
 *------------------------------------------------------------------------
 */
devcall	ttyInit(
	  struct dentry	*devptr		/* entry in device switch table	*/
	)
{
	struct	ttycblk	*typtr;		/* pointer to ttytab entry	*/
	struct	uart_csreg *uptr;	/* address of UART's CSRs	*/

	typtr = &ttytab[ devptr->dvminor ];

	/* Initialize values in the tty control block */

	typtr->tyihead = typtr->tyitail = 	/* set up input queue	*/
		&typtr->tyibuff[0];		/*    as empty		*/
	typtr->tyisem = semcreate(0);		/* input semaphore	*/
	typtr->tyohead = typtr->tyotail = 	/* set up output queue	*/
		&typtr->tyobuff[0];		/*    as empty		*/
	typtr->tyosem = semcreate(TY_OBUFLEN);	/* output semaphore	*/
	typtr->tyehead = typtr->tyetail = 	/* set up echo queue	*/
		&typtr->tyebuff[0];		/*    as empty		*/
	typtr->tyimode = TY_IMCOOKED;		/* start in cooked mode	*/
	typtr->tyiecho = TRUE;			/* echo console input	*/
	typtr->tyieback = TRUE;			/* honor erasing bksp	*/
	typtr->tyevis = TRUE;			/* visual control chars	*/
	typtr->tyecrlf = TRUE;			/* echo CRLF for NEWLINE*/
	typtr->tyicrlf = TRUE;			/* map CR to NEWLINE	*/
	typtr->tyierase = TRUE;			/* do erasing backspace	*/
	typtr->tyierasec = TY_BACKSP;		/* erase char is ^H	*/
	typtr->tyeof = TRUE;			/* honor eof on input	*/
	typtr->tyeofch = TY_EOFCH;		/* end-of-file character*/
	typtr->tyikill = TRUE;			/* allow line kill	*/
	typtr->tyikillc = TY_KILLCH;		/* set line kill to ^U	*/
	typtr->tyicursor = 0;			/* start of input line	*/
	typtr->tyoflow = TRUE;			/* handle flow control	*/
	typtr->tyoheld = FALSE;			/* output not held	*/
	typtr->tyostop = TY_STOPCH;		/* stop char is ^S	*/
	typtr->tyostart = TY_STRTCH;		/* start char is ^Q	*/
	typtr->tyocrlf = TRUE;			/* send CRLF for NEWLINE*/
	typtr->tyifullc = TY_FULLCH;		/* send ^G when buffer	*/
						/*   is full		*/

	/* Initialize the UART */

	uptr = (struct uart_csreg *)devtab[CONSOLE].dvcsr;

	/* Set baud rate */
	/* Not set the band rate again,rely the code in u-boot */

	/* Enable interrupts */
	uptr->uart_cs |= UART_HOST_INT_EN;
	uptr->uart_int_en = UART_RX_VALID_INT_EN;

	/* Register the interrupt handler for the dispatcher */

	interruptVector[devptr->dvirq] = (void *)devptr->dvintr;

	/* Ready to enable interrupts on the UART hardware */

	enable_irq(devptr->dvirq);

	ttyKickOut(typtr, uptr);

	return OK;
}
