#include "include.h"

extern uint8 img_handle[CAMERA_H][CAMERA_W];



/***************œÒÀÿµ„1±∂œ‘ æ****************/
void LCD_img(void)
{
  int i,j;
  Address_set(0,0,OV7725_EAGLE_W-1,20);
  for(i=40;i<60;i++)
  {
    for(j=0;j<CAMERA_W;j++)
    {
      if(img_handle[i][j]==255)
      {
        LCD_Write_Data2(WHITE);
      }
      if(img_handle[i][j]==0)
      {
        LCD_Write_Data2(BLACK);
      }
    }
  }