#include <stdint.h>
#include "raylib.h"

#ifndef UI_H_
#define UI_H_

typedef struct {
	uint8_t flags;

	Font font;
} Ui;

void UiInit(Ui *ui);
void UiUpdate(Ui *ui);
void UiDraw(Ui *ui);
void UiClose(Ui *ui);

#endif
