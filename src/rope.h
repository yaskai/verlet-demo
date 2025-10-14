#include <stdint.h>
#include "raylib.h"

#ifndef ROPE_H_
#define ROPE_H_

#define RLEN		255
#define RN_PINNED	0x01

typedef struct {
	uint8_t flags;
	
	Vector2 pos_prev;	
	Vector2 pos_curr;
} RopeNode;

typedef struct {
	float max_dist;
} RopeSegment;

typedef struct {
	uint8_t length;
	uint8_t iteration_steps;
	
	int16_t selected_node;

	float segment_dist;
	float dampening;

	RopeNode *nodes;
	//RopeNode nodes[RLEN];
} Rope;

void RopeInit(Rope *rope, Vector2 pos);
void RopeIntegrate(Rope *rope, float dt);
void RopeSolveConstraints(Rope *rope, float dt);
void RopeUpdate(Rope *rope, float dt);
void RopeDraw(Rope *rope);
void RopeClose(Rope *rope);

#endif
