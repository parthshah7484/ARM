#include <lpc214x.h>

__irq void PWM_ISR (void)
{
	if ( PWMIR & 0x0001 )	/* If interrupt due to PWM0 */
	{
		PWMIR = 0x0001;	/* Clear PWM0 interrupt */
	}
	
	if ( PWMIR & 0x0002 )	/* If interrupt due to PWM1 */
	{
		PWMIR = 0x0002;	/* Clear PWM1 interrupt */
	}
	
	if ( PWMIR & 0x0004 )	/* If interrupt due to PWM2 */
	{
		PWMIR = 0x0004;	/* Clear PWM2 interrupt */
	}
	
	if ( PWMIR & 0x0008 )	/* If interrupt due to PWM3 */
	{
		PWMIR = 0x0008;	/* Clear PWM3 interrupt */
	}	
	VICVectAddr = 0x00000000;
}


int main (void)
{
	VPBDIV		= 0x00000002;
	PINSEL0 = PINSEL0 | 0x00008008; /* Configure P0.1 and P0.7 as PWM3 and PWM2 respectively */
	VICVectAddr0 = (unsigned) PWM_ISR; /* PWM ISR Address */
	VICVectCntl0 = (0x00000020 | 8); /* Enable PWM IRQ slot */
	VICIntEnable = VICIntEnable | 0x00000100; /* Enable PWM interrupt */
	VICIntSelect = VICIntSelect | 0x00000000; /* PWM configured as IRQ */


// For PWM3 double edge
	PWMTCR = 0x02;	/* Reset and disable counter for PWM */
	PWMPR = 0x1D;	/* Prescale value for 1usec, Pclk=30MHz*/
	PWMMR0 = 1000000;	/* Time period of PWM wave, 100msec */
	PWMMR2 = 40000;	/* Rising edge of double edge controlled PWM */
	PWMMR3 = 80000;	/* Falling edge of double edge controlled PWM */
	PWMMCR = 0x00000243;	/* Reset and interrupt on MR0 match, interrupt on MR2 and MR3 match */
	PWMLER = 0x0D;	/* Latch enable for PWM3, PWM2 and PWM0 */
	PWMPCR = 0x0C08;	/* Enable PWM3, PWM2 and PWM0, double edge controlled PWM on PWM3 */
	PWMTCR = 0x09;	/* Enable PWM and counter */

	while (1);
}
