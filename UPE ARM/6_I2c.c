#include <lpc214x.h>
#include <stdint.h>
void I2C_INIT(void)
{
	PINSEL0 = PINSEL0 | 0x00000050; /* Configure P0.2 and P0.3 as SCL0 and SDA0 respectively */
	I2C0CONSET = 0x40; /* Enable I2C  */
	I2C0SCLL = 0x32; /* I2C data rate 300Khz and 50% duty cycle */
	I2C0SCLH = 0x32; /* I2C data rate 300Khz and 50% duty cycle */
}
void I2C_START(void)
{
	I2C0CONSET = 0x20; /* Set Start bit for Start condition */
	while ( (I2C0CONSET & 0x08) == 0 ); /* Wait till SI = 1 */
	I2C0CONCLR = 0x28; /* Clear Start bit and SI bit */
}
void I2C_WRITE( char data )
{
	I2C0DAT = data; /* Load data to be written into the data register */
	I2C0CONSET = 0x40; /* Enable I2C */
	while( (I2C0CONSET & 0x08) == 0 ); /* Wait till SI = 1 */
	I2C0CONCLR = 0x08;  /* Clear SI bit */
}
unsigned char I2C_READ_ACK( void )
{
	I2C0CONSET = 0x44; /* Enable I2C with Acknowledge */
	while( (I2C0CONSET & 0x08) == 0 ); /* Wait till SI = 1 */
	I2C0CONCLR = 0x0C; /* Clear SI and Acknowledge bits */
	return I2C0DAT; /* Return received data */
}
unsigned char I2C_READ_NACK( void )
{
	I2C0CONSET = 0x40; /* Enable I2C without Acknowledge */
	while( (I2C0CONSET & 0x08) == 0 ); /* Wait till SI = 1 */
	I2C0CONCLR = 0x08;   /* Clear SI bit */
	return I2C0DAT; /* Return received data */
}
void I2C_MULTIREAD( char* arr , int bytes ) /* For reading multiple bytes at a time */
{
	uint8_t i = 0;
	while( ( bytes - 1 ) != 0 )
	{	
		I2C0CONSET = 0x44; /* Enable I2C with Acknowledge */
		while( (I2C0CONSET & 0x08) == 0 ); /* Wait till SI = 1 */
		I2C0CONCLR = 0x0C;  /* Clear SI and Acknowledge bits */
		*( arr + i ) = I2C0DAT ;
		bytes--;
		i++;
	}
	I2C0CONSET = 0x40; /* Enable I2C without Acknowledge */
	while( (I2C0CONSET & 0x08) == 0 ); /* Wait till SI = 1 */
	I2C0CONCLR = 0x08; /* Clear SI bit */
	*( arr + i ) = I2C0DAT ;
}
void I2C_STOP( void )
{
	I2C0CONSET = 0x50; /* Set Stop bit for Stop condition */
}
int main()
{
	I2C_INIT();
	I2C_START();
	I2C_WRITE('c');
	I2C_READ_ACK();
	I2C_READ_NACK();
	I2C_MULTIREAD("b",5);
	I2C_STOP();
	}


