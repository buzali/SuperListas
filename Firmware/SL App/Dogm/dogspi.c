#include <p24FJ256GA106.h>
#include "HardwareProfile.h"
#include <libpic30.h>


extern void dog_Delay(int val);

void dog_spi_init(void)
{

    LCD_A0_TRIS=0;
    LCD_SCL_TRIS =0;
    LCD_SO_TRIS =0;
    LCD_CS_TRIS =0;
    

    LCD_SPICON1 = 0;
    LCD_SPICON1bits.MSTEN = 1; //MAster
    LCD_SPICON1bits.CKE = 1;
    LCD_SPICON1bits.DISSDO = 0;
    LCD_SPICON1bits.SMP = 0;

    //Para beta 1
//    LCD_SPICON1bits.SPRE1 = 1;
//    LCD_SPICON1bits.SPRE2 = 1;
//    LCD_SPICON1bits.SPRE0 = 0;
//    LCD_SPICON1bits.PPRE0 = 1;
//    LCD_SPICON1bits.PPRE1 = 0;
//    LCD_SPISTATbits.SPIEN = 1;
//Para beta 2
    LCD_SPICON1bits.SPRE1 = 1;
    LCD_SPICON1bits.SPRE2 = 1;
    LCD_SPICON1bits.SPRE0 = 0;
    LCD_SPICON1bits.PPRE0 = 0;
    LCD_SPICON1bits.PPRE1 = 1;
    LCD_SPISTATbits.SPIEN = 1;



}

unsigned char dog_spi_out(unsigned char data)
{
                volatile unsigned char vDummy;
//                IO1_O = 0;
                LCD_SSPBUF=data;
                //while (LCD_SPISTATbits.SPITBF == 1);
                //IO1_O = 0;
                __delay_us(60);
                //WaitForDataByte();
                //LCD_SPI_IF=0;
                vDummy = LCD_SSPBUF;
                //LD1_O = 0;
                return vDummy;

}

void dog_spi_enable_client(void)
{
LCD_CS_O=0;
}

void dog_spi_disable_client(void)
{
LCD_CS_O=1;
}

void dog_cmd_mode(void)
{
  LCD_A0_O=0;
}

void dog_data_mode(void)
{
  LCD_A0_O=1;
}



void dog_init_display(void)
{

  dog_cmd_mode();

  dog_spi_out(0x40);		/* set display start line to 0 */
  dog_spi_out(0xa1);		/* ADC set to reverse */
  dog_spi_out(0xc0);		/* common output mode */
  dog_spi_out(0xa6);		/* display normal, bit val 0: LCD pixel off. */
  dog_spi_out(0xa2);		/* LCD bias 1/9 */
  dog_spi_out(0x2f);		/* all power  control circuits on */
  dog_spi_out(0xf8);		/* set booster ratio to */
  dog_spi_out(0x00);		/* 4x */
  dog_spi_out(0x27);		/* set V0 voltage resistor ratio to large */
  dog_spi_out(0x81);		/* set contrast */
  dog_spi_out(0x18);		/* contrast value, EA default: 0x016 */
  dog_spi_out(0xac);		/* indicator */
  dog_spi_out(0x00);		/* disable */
  dog_spi_out(0xaf);		/* display on */


  dog_spi_out(0xa5);		/* display all points, ST7565, UC1610 */
  dog_Delay(300);
  dog_spi_out(0xa4);		/* normal display  */
  dog_Delay(300);
  //Serial.println("exiting dog_init_display");
}

