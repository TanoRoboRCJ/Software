#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include <Arduino.h>
#include <TFT_eSPI.h>
#include <PNGdec.h>

#define ARR_SIZE 76800
#define MAX_IMAGE_WIDTH 240

class DISPLAY_DEVICE {
   public:
    TFT_eSPI* tftPtr;
    TFT_eSprite* spritePtr;


    PNG png;

    DISPLAY_DEVICE(TFT_eSPI* tftPtr, TFT_eSprite* spritePtr);

    void init(void);
    void setSPIClockFast(void);

    void pngDraw(PNGDRAW* pDraw);

    void publish(int x = 0, int y = 0);
    void setBackgroundImage(const uint16_t* imagePtr);

    void createSprite(int x = 240, int y = 240);

    void setBackgroundImagePNG(uint8_t* imagePtr, int imageLen);

   

   private:
    const int backlightPin = D6;
};

#endif