#include <stdint.h>
#include "raylib.h"
#include "rope.h"

#ifndef UI_H_
#define UI_H_

#define UI_FOCUSED		0x01

#define SLIDER_HOVERED	0x01
#define SLIDER_PRESSED	0x02
#define SLIDER_BG_HOV	0x04

typedef struct {
	uint8_t flags;
	
	Rectangle rec_base;
	Rectangle rec_ctrl;

	float control_bounds[2];
	float *control;

	float percent;

	char *text;
} Slider;

typedef struct {
	uint8_t flags;
	uint8_t slider_count;

	Font font;
	
	Slider *sliders;
} Ui;

void UiInit(Ui *ui, Rope *rope);
void UiUpdate(Ui *ui);
void UiDraw(Ui *ui);
void UiClose(Ui *ui);

void UiMakeSliders(Ui *ui, Rope *rope);

void SliderUpate(Slider *slider);
void SliderDraw(Slider *slider);

float HandlePosFromVal(Slider *slider);

#endif
