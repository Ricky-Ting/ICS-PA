#include <assert.h>
#include <stdlib.h>
#include <ndl.h>
#include <stdio.h>

int main() {
  NDL_Bitmap *bmp = (NDL_Bitmap*)malloc(sizeof(NDL_Bitmap));
  NDL_LoadBitmap(bmp, "/share/pictures/projectn.bmp");
  assert(bmp->pixels);
	printf("l\n");
  NDL_OpenDisplay(bmp->w, bmp->h);
	printf("ll\n");
  NDL_DrawRect(bmp->pixels, 0, 0, bmp->w, bmp->h);
	printf("lll\n");
  NDL_Render();
	printf("llll\n");
  NDL_CloseDisplay();
  while (1);
  return 0;
}
