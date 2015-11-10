#ifndef __LCD_H
#define __LCD_H

#define Sil 		 1		// Ekrani temizler
#define BasaDon 	 2		// Imleci sol �st k�seye getirir
#define SolaYaz 	 4  	// Imlecin belirttigi adres azalarak gider
#define SagaYaz 	 6   	// Imlecin belirttigi adres artarak gider
#define ImlecGizle	 12		// G�stergeyi ac, kursor g�r�nmesin
#define ImlecAltta	 14		// Yanip s�nen blok kursor
#define ImlecYanSon	 15		// Yanip s�nen blok kursor
#define ImlecGeri	 16		// Kursor� bir karakter geri kaydir
#define KaydirSaga	 24		// G�stergeyi bir karakter saga kaydir                   
#define KaydirSola   28		// G�stergeyi bir karakter sola kaydir   
#define EkraniKapat	 8		// G�stergeyi kapat (veriler silinmez)
#define BirinciSatir 128	// LCD'nin ilk satir baslangic adresi       
                          	// (DDRAM adres)
#define IkinciSatir  192	// Ikinci satirin baslangic adresi
#define KarakUretAdres 64	// Karakter �reteci adresini belirle  
                          	// (CGRAM adres)
/* LCD'de Kullanilan Fonksiyon Se�imi */
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

