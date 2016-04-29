#ifndef __SOUND_H
#define __SOUND_H

void Sound_Init(void);
void playMusic(void);
void stopMusic(void);
void playNote(char note, int duration);
void playTone(int tone, int duration);

#endif /* __SOUND_H */

