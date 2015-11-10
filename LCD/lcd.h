#ifndef __LCD_H
#define __LCD_H

#define Sil 		 1		// Ekrani temizler
#define BasaDon 	 2		// Imleci sol üst köseye getirir
#define SolaYaz 	 4  	// Imlecin belirttigi adres azalarak gider
#define SagaYaz 	 6   	// Imlecin belirttigi adres artarak gider
#define ImlecGizle	 12		// Göstergeyi ac, kursor görünmesin
#define ImlecAltta	 14		// Yanip sönen blok kursor
#define ImlecYanSon	 15		// Yanip sönen blok kursor
#define ImlecGeri	 16		// Kursorü bir karakter geri kaydir
#define KaydirSaga	 24		// Göstergeyi bir karakter saga kaydir                   
#define KaydirSola   28		// Göstergeyi bir karakter sola kaydir   
#define EkraniKapat	 8		// Göstergeyi kapat (veriler silinmez)
#define BirinciSatir 128	// LCD'nin ilk satir baslangic adresi       
                          	// (DDRAM adres)
#define IkinciSatir  192	// Ikinci satirin baslangic adresi
#define KarakUretAdres 64	// Karakter üreteci adresini belirle  
                          	// (CGRAM adres)
/* LCD'de Kullanilan Fonksiyon Seçimi */
#define CiftSatir8Bit  56	 // 8 bit ara birim, 2 satir, 5*7 piksel
#define TekSatir8Bit   48	 // 8 bit ara birim, 1 satir, 5*7 piksel
#define CiftSatir4Bit  40	 // 4 bit ara birim, 2 satir, 5*7 piksel
#define TekSatir4Bit   32	 // 4 bit ara birim, 1 satir, 5*7 piksel

void veri_yolla(unsigned char);
void lcd_clear(void);
void lcd_yaz(const char *s);
void lcd_gotoxy(unsigned char x, unsigned char y);
void lcd_init(void);
void lcd_komut(unsigned char c);
void gecikme(int say);

#endif /* __LCD_H */

