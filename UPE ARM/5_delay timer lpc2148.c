/*
  Delay using Timer in LPC2148(ARM7)
  http://www.electronicwings.com/arm7/lpc2148-timercounter
*/

#include <lpc214x.h>

__irq void T0_ISR (void)
{
	IO0PIN = ( IO0PIN ^ (0x00000100) );	/* Toggle P0.8 pin */ 
	T0IR = ( T0IR | (0x01) ); 
	VICVectAddr = 0x00;
}


int main (void)
{
	VPBDIV = 0x00000002; /* For Pclk = 30MHz */
	/* We have configured Cclk=60MHz. Above instruction makes Pclk = Cclk/2 = 30MHz */
	PINSEL0 = PINSEL0 | 0x00000020; /* Configure P0.2 as Capture 0.0 */
	IO0DIR = ( IO0DIR | (0x00000100) ); /* 8 P0.8-P0.15 as output pins for LED port */
	IO0PIN = IO0PIN | 0x00000100; /* Writing 1 to LED pin P0.8 */
	VICVectAddr0 = (unsigned) T0_ISR; /* T0 ISR Address */
	VICVectCntl0 = 0x00000024; /* Enable T0 IRQ slot */
	VICIntEnable = 0x00000010; /* Enable T0 interrupt */
	VICIntSelect = 0x00000000; /* T0 configured as IRQ */
	T0TCR = 0x02; /* Reset TC and PR */
	T0CTCR = 0x00; /* Timer mode, increment on every rising edge */
	T0PR = 0x1D; /* Load Pre-Scalar counter with 29 (0 to 29 = 30), so that timer counts every 1usec */
	T0MR0 = 100000; /* Load timer counter for 100msec delay, 1usec*1000*100 */
	T0MCR = 0x0003; /* Interrupt generate on match and reset timer */
	T0TCR = 0x01; /* Enable timer */

	while (1);
}
