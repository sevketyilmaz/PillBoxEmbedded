#include	"lcd.h"		// lcd.h dosyasi tanimlanip, degerler aliniyor
#include "stm32f4xx.h"
#include <stdio.h>
#include <stdarg.h>
#include "delay.h"


#define PORT_LCD GPIOE

#define LCD_D4 GPIO_Pin_7
#define LCD_D5 GPIO_Pin_8
#define LCD_D6 GPIO_Pin_9
#define LCD_D7 GPIO_Pin_10

#define LCD_RS GPIO_Pin_11
#define LCD_EN GPIO_Pin_12

typedef struct{
	
	GPIO_InitTypeDef LCD_GPIO;
	GPIO_TypeDef *LCD_PORT;
	u32 LCD_CLOCK_GPIO;

}LCDAyarlari;
struct{
	LCDAyarlari rs,e,d4,d5,d6,d7;
}Ayarlar;

void lcd_busy(void)		// 500us bekleme
{
	delay_us(250);
	delay_us(250);
}

void lcd_komut(unsigned char c)		// Komut gönderme fonksiyonu
{
	GPIO_ResetBits(PORT_LCD, LCD_RS);	//rs=0;							// LCD'ye komut gönderilecek
	GPIO_SetBits(PORT_LCD, LCD_EN);			//e=1;							// Düsen kenar tetikleme oldugu için E önce 1
//High bits
	GPIO_ResetBits(PORT_LCD, LCD_D4);			//e=1;							// Düsen kenar tetikleme oldugu için E önce 1
	GPIO_ResetBits(PORT_LCD, LCD_D5);			//e=1;							// Düsen kenar tetikleme oldugu için E önce 1
	GPIO_ResetBits(PORT_LCD, LCD_D6);			//e=1;							// Düsen kenar tetikleme oldugu için E önce 1
	GPIO_ResetBits(PORT_LCD, LCD_D7);			//e=1;							// Düsen kenar tetikleme oldugu için E önce 1
	if ((c & 0x10) == 0x10){
		GPIO_WriteBit(PORT_LCD, LCD_D4, Bit_SET);			
	}
	if ((c & 0x20) == 0x20){
		GPIO_WriteBit(PORT_LCD, LCD_D5, Bit_SET);			
	}	
  if ((c & 0x40) == 0x40){
		GPIO_WriteBit(PORT_LCD, LCD_D6, Bit_SET);			
	}
  if ((c & 0x80) == 0x80){
		GPIO_WriteBit(PORT_LCD, LCD_D7, Bit_SET);			
	}
	
	GPIO_WriteBit(PORT_LCD, LCD_EN, Bit_RESET);			
	lcd_busy();			//e=1;							// Düsen kenar tetikleme oldugu için E önce 1
	GPIO_WriteBit(PORT_LCD, LCD_EN, Bit_SET);			

//Low bits
	GPIO_WriteBit(PORT_LCD, LCD_D4, Bit_RESET);			//e=1;							// Düsen kenar tetikleme oldugu için E önce 1
	GPIO_WriteBit(PORT_LCD, LCD_D5, Bit_RESET);			//e=1;							// Düsen kenar tetikleme oldugu için E önce 1
	GPIO_WriteBit(PORT_LCD, LCD_D6, Bit_RESET);			//e=1;							// Düsen kenar tetikleme oldugu için E önce 1
	GPIO_WriteBit(PORT_LCD, LCD_D7, Bit_RESET);			//e=1;							// Düsen kenar tetikleme oldugu için E önce 1
	if ((c & 0x01) == 0x01){
		GPIO_WriteBit(PORT_LCD, LCD_D4, Bit_SET);			
	}
	if ((c & 0x02) == 0x02){
		GPIO_WriteBit(PORT_LCD, LCD_D5, Bit_SET);			
	}	
  if ((c & 0x04) == 0x04){
		GPIO_WriteBit(PORT_LCD, LCD_D6, Bit_SET);			
	}
  if ((c & 0x08) == 0x08){
		GPIO_WriteBit(PORT_LCD, LCD_D7, Bit_SET);			
	}
	
	GPIO_WriteBit(PORT_LCD, LCD_EN, Bit_RESET);			
	lcd_busy();
}

void veri_yolla(unsigned char c)	// Komut yolladan tek farki, RS'nin 1 olmasi
{
	GPIO_WriteBit(PORT_LCD, LCD_RS, Bit_SET);	//rs=0;							// LCD'ye komut gönderilecek
	GPIO_WriteBit(PORT_LCD, LCD_EN, Bit_SET);			//e=1;							// Düsen kenar tetikleme oldugu için E önce 1
//High bits
	GPIO_WriteBit(PORT_LCD, LCD_D4, Bit_RESET);			//e=1;							// Düsen kenar tetikleme oldugu için E önce 1
	GPIO_WriteBit(PORT_LCD, LCD_D5, Bit_RESET);			//e=1;							// Düsen kenar tetikleme oldugu için E önce 1
	GPIO_WriteBit(PORT_LCD, LCD_D6, Bit_RESET);			//e=1;							// Düsen kenar tetikleme oldugu için E önce 1
	GPIO_WriteBit(PORT_LCD, LCD_D7, Bit_RESET);			//e=1;							// Düsen kenar tetikleme oldugu için E önce 1
	if ((c & 0x10) == 0x10){
		GPIO_WriteBit(PORT_LCD, LCD_D4, Bit_SET);			
	}
	if ((c & 0x20) == 0x20){
		GPIO_WriteBit(PORT_LCD, LCD_D5, Bit_SET);			
	}	
  if ((c & 0x40) == 0x40){
		GPIO_WriteBit(PORT_LCD, LCD_D6, Bit_SET);			
	}
  if ((c & 0x80) == 0x80){
		GPIO_WriteBit(PORT_LCD, LCD_D7, Bit_SET);			
	}
	
	GPIO_WriteBit(PORT_LCD, LCD_EN, Bit_RESET);			
	lcd_busy();			//e=1;							// Düsen kenar tetikleme oldugu için E önce 1
	GPIO_WriteBit(PORT_LCD, LCD_EN, Bit_SET);			

//Low bits
	GPIO_WriteBit(PORT_LCD, LCD_D4, Bit_RESET);			//e=1;							// Düsen kenar tetikleme oldugu için E önce 1
	GPIO_WriteBit(PORT_LCD, LCD_D5, Bit_RESET);			//e=1;							// Düsen kenar tetikleme oldugu için E önce 1
	GPIO_WriteBit(PORT_LCD, LCD_D6, Bit_RESET);			//e=1;							// Düsen kenar tetikleme oldugu için E önce 1
	GPIO_WriteBit(PORT_LCD, LCD_D7, Bit_RESET);			//e=1;							// Düsen kenar tetikleme oldugu için E önce 1
	if ((c & 0x01) == 0x01){
		GPIO_WriteBit(PORT_LCD, LCD_D4, Bit_SET);			
	}
	if ((c & 0x02) == 0x02){
		GPIO_WriteBit(PORT_LCD, LCD_D5, Bit_SET);			
	}	
  if ((c & 0x04) == 0x04){
		GPIO_WriteBit(PORT_LCD, LCD_D6, Bit_SET);			
	}
  if ((c & 0x08) == 0x08){
		GPIO_WriteBit(PORT_LCD, LCD_D7, Bit_SET);			
	}
	
	GPIO_WriteBit(PORT_LCD, LCD_EN, Bit_RESET);			
	lcd_busy();
}

void lcd_clear(void)		// LCD siliniyor
{
	lcd_komut(0x1);
	delay_ms(2);
}

void lcd_yaz(const char * s)	// LCD'ye string ifade gönderiliyor
{
	lcd_busy();
	while(*s)
		veri_yolla(*s++);
}

void lcd_gotoxy(unsigned char x,unsigned char y)	// LCD'nin belli bölgesine gidiliyor
{
	if(x==1)
		lcd_komut(0x80+((y-1)%16));
	else
		lcd_komut(0xC0+((y-1)%16));
}
	
void lcd_init()		// LCD ilk yükleme ayarlari yapiliyor
{
//LCD bit  
//RS
	Ayarlar.rs.LCD_PORT = PORT_LCD;
	Ayarlar.rs.LCD_CLOCK_GPIO = RCC_AHB1Periph_GPIOE;
	Ayarlar.rs.LCD_GPIO.GPIO_Pin = LCD_RS;
	Ayarlar.rs.LCD_GPIO.GPIO_Mode = GPIO_Mode_OUT;
	Ayarlar.rs.LCD_GPIO.GPIO_OType = GPIO_OType_PP;
	Ayarlar.rs.LCD_GPIO.GPIO_PuPd = GPIO_PuPd_NOPULL;
	Ayarlar.rs.LCD_GPIO.GPIO_Speed = GPIO_Speed_25MHz;
	RCC_AHB1PeriphClockCmd(Ayarlar.rs.LCD_CLOCK_GPIO, ENABLE);
	GPIO_Init(Ayarlar.rs.LCD_PORT, &Ayarlar.rs.LCD_GPIO);
//E
	Ayarlar.e.LCD_PORT = PORT_LCD;
	Ayarlar.e.LCD_CLOCK_GPIO = RCC_AHB1Periph_GPIOE;
	Ayarlar.e.LCD_GPIO.GPIO_Pin = LCD_EN;
	Ayarlar.e.LCD_GPIO.GPIO_Mode = GPIO_Mode_OUT;
	Ayarlar.e.LCD_GPIO.GPIO_OType = GPIO_OType_PP;
	Ayarlar.e.LCD_GPIO.GPIO_PuPd = GPIO_PuPd_NOPULL;
	Ayarlar.e.LCD_GPIO.GPIO_Speed = GPIO_Speed_25MHz;
	RCC_AHB1PeriphClockCmd(Ayarlar.e.LCD_CLOCK_GPIO, ENABLE);
	GPIO_Init(Ayarlar.e.LCD_PORT, &Ayarlar.e.LCD_GPIO);
//D4
	Ayarlar.d4.LCD_PORT = PORT_LCD;
	Ayarlar.d4.LCD_CLOCK_GPIO = RCC_AHB1Periph_GPIOE;
	Ayarlar.d4.LCD_GPIO.GPIO_Pin = LCD_D4;
	Ayarlar.d4.LCD_GPIO.GPIO_Mode = GPIO_Mode_OUT;
	Ayarlar.d4.LCD_GPIO.GPIO_OType = GPIO_OType_PP;
	Ayarlar.d4.LCD_GPIO.GPIO_PuPd = GPIO_PuPd_NOPULL;
	Ayarlar.d4.LCD_GPIO.GPIO_Speed = GPIO_Speed_25MHz;
	RCC_AHB1PeriphClockCmd(Ayarlar.d4.LCD_CLOCK_GPIO, ENABLE);
	GPIO_Init(Ayarlar.d4.LCD_PORT, &Ayarlar.d4.LCD_GPIO);	
//D5
	Ayarlar.d5.LCD_PORT = PORT_LCD;
	Ayarlar.d5.LCD_CLOCK_GPIO = RCC_AHB1Periph_GPIOE;
	Ayarlar.d5.LCD_GPIO.GPIO_Pin = LCD_D5;
	Ayarlar.d5.LCD_GPIO.GPIO_Mode = GPIO_Mode_OUT;
	Ayarlar.d5.LCD_GPIO.GPIO_OType = GPIO_OType_PP;
	Ayarlar.d5.LCD_GPIO.GPIO_PuPd = GPIO_PuPd_NOPULL;
	Ayarlar.d5.LCD_GPIO.GPIO_Speed = GPIO_Speed_25MHz;
	RCC_AHB1PeriphClockCmd(Ayarlar.d5.LCD_CLOCK_GPIO, ENABLE);
	GPIO_Init(Ayarlar.d5.LCD_PORT, &Ayarlar.d5.LCD_GPIO);
//D6
	Ayarlar.d6.LCD_PORT = PORT_LCD;
	Ayarlar.d6.LCD_CLOCK_GPIO = RCC_AHB1Periph_GPIOE;
	Ayarlar.d6.LCD_GPIO.GPIO_Pin = LCD_D6;
	Ayarlar.d6.LCD_GPIO.GPIO_Mode = GPIO_Mode_OUT;
	Ayarlar.d6.LCD_GPIO.GPIO_OType = GPIO_OType_PP;
	Ayarlar.d6.LCD_GPIO.GPIO_PuPd = GPIO_PuPd_NOPULL;
	Ayarlar.d6.LCD_GPIO.GPIO_Speed = GPIO_Speed_25MHz;
	RCC_AHB1PeriphClockCmd(Ayarlar.d6.LCD_CLOCK_GPIO, ENABLE);
	GPIO_Init(Ayarlar.d6.LCD_PORT, &Ayarlar.d6.LCD_GPIO);
//D7
	Ayarlar.d7.LCD_PORT = PORT_LCD;
	Ayarlar.d7.LCD_CLOCK_GPIO = RCC_AHB1Periph_GPIOE;
	Ayarlar.d7.LCD_GPIO.GPIO_Pin = LCD_D7;
	Ayarlar.d7.LCD_GPIO.GPIO_Mode = GPIO_Mode_OUT;
	Ayarlar.d7.LCD_GPIO.GPIO_OType = GPIO_OType_PP;
	Ayarlar.d7.LCD_GPIO.GPIO_PuPd = GPIO_PuPd_NOPULL;
	Ayarlar.d7.LCD_GPIO.GPIO_Speed = GPIO_Speed_25MHz;
	RCC_AHB1PeriphClockCmd(Ayarlar.d7.LCD_CLOCK_GPIO, ENABLE);
	GPIO_Init(Ayarlar.d7.LCD_PORT, &Ayarlar.d7.LCD_GPIO);
//-----------------------------------------------------------------------------//	
	
	GPIO_WriteBit(PORT_LCD, LCD_RS, Bit_RESET);			
	GPIO_WriteBit(PORT_LCD, LCD_EN, Bit_RESET);			
	delay_ms(15);
	GPIO_WriteBit(PORT_LCD, LCD_EN, Bit_SET);			
	lcd_komut(0x02);
  delay_ms(2);

	lcd_komut(CiftSatir4Bit);
	lcd_komut(SagaYaz);
	lcd_komut(ImlecGizle);
	lcd_clear();
  lcd_komut(BirinciSatir);
}
