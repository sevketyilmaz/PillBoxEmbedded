//PA8

#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
#include "delay.h"
#include "sound.h"
#include "string.h"

BitAction LOW_S = Bit_RESET, HIGH_S = Bit_SET;

char notes[] = "EDEDEbDCaceabegbCeEDEDEbDCaceabeCbaEDEDEbDC";//aceabegbCeEDEDEbDCaceabeCbabCDEgFEDfEDCeDCbeEeEDEDEDEDEDEDEbDCaceabegbCeEDEDEbDCaceabeCba"; 

//              E D E D E b D C a c e a b e g b C e E D|E D E b D C a c e a b e C b a E D E D E b D C|a c e a b e g b C e E D E D E b D C a c e a|b e C b a b C D E g F E D f E D C e D C|b e E e E D E D E D E D E D E D E b D C a c e a b e g b C e E D E D E b D C a c e a b e C b a
int beats[] = { 1,1,1,1,1,1,1,1,3,1,1,1,3,1,1,1,3,1,1,1,1,1,1,1,1,1,3,1,1,1,3,1,1,1,3,1,1,1,1,1,1,1,1,3,1,1,1,3,1,1,1,3,1,1,1,1,1,1,1,1,1,3,1,1,1,3,1,1,1,3,1,1,1,2,1,1,1,2,1,1,1,2,1,1,1,3,1,5,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};//,3,1,1,1,3,1,1,1,3,1,1,1,1,1,1,1,1,1,3,1,1,1,3,1,1,1,6};
int tempo = 175;

void Sound_Init(void) {
	GPIO_InitTypeDef GPIO_InitStruct;
	  
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);		
	GPIO_Init(GPIOA, &GPIO_InitStruct);
}
	
void playTone(int tone, int duration) {
	int i = 0;
  for (i = 0; i < duration * 1000; i += tone * 2) {
		GPIO_WriteBit(GPIOA, GPIO_Pin_6, HIGH_S); 
    delay_us(tone);
		GPIO_WriteBit(GPIOA, GPIO_Pin_6, LOW_S); 
    delay_us(tone);
  }
}

void playNote(char note, int duration) {
  char names[] = {'c', 'd','e', 'f', 'g', 'a',  'b', 'C',  'D',  'E',  'F',  'G',  'A',  'B',  'U'};
  int tones[] = {3822,3424,3033,2864,2551,2272,2024, 1915, 1700, 1519, 1432, 1275, 1136, 1014, 956};
	int i = 0;
// play the tone corresponding to the note name
  for (i = 0; i < 15; i++) {
    if (names[i] == note) {
		//if(strcmp(&names[i], &note) == 1){
      playTone(tones[i], duration);
    }
  }
}
void stopMusic(){

	GPIO_WriteBit(GPIOA, GPIO_Pin_6, LOW_S); 
}

void playMusic() {
	int i;
	STM_EVAL_LEDOff(LED3);

  for (i = 0; i < sizeof(notes); i++) {
    if (notes[i] == ' ') {
      delay_us(beats[i] * tempo*1000); // rest
    } else {
      playNote(notes[i], beats[i] * tempo);
    }
    // pause between notes
    delay_us((tempo / 2)*1000); 
    //if (i == sizeof(notes)-1){
    //  delay_ms(100000);
    //}
  }
}
