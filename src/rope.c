#include <stdlib.h>
#include "raylib.h"
#include "raymath.h"
#include "rope.h"

#define GRAVITY (Vector2){0, 30}
#define dt2		(dt * dt)

void RopeInit(Rope *rope, Vector2 pos) {
	rope->length = RLEN;
	rope->iteration_steps = 16;

	rope->selected_node = -1;
	
	rope->segment_dist = 2.0f;

	rope->nodes = MemAlloc(sizeof(RopeNode) * RLEN); 

	rope->nodes[0] = (RopeNode) {
		.flags = (RN_PINNED),
		.pos_prev = pos,
		.pos_curr = pos
	};

	for(uint8_t i = 1; i < rope->length; i++) {
		RopeNode *node = &rope->nodes[i];
		node->pos_curr = (Vector2){pos.x + GetRandomValue(-200, 200), pos.y + (i*rope->segment_dist)};
		node->pos_prev = node->pos_curr;
	}
}

void RopeIntegrate(Rope *rope, float dt) {
	Vector2 mouse_pos = GetMousePosition();
	Vector2 mouse_delta = GetMouseDelta();

	for(uint8_t i = 0; i < rope->length; i++) {
		RopeNode *node = &rope->nodes[i];
		Vector2 new_prev = node->pos_curr;

		Vector2 vel = Vector2Subtract(node->pos_curr, node->pos_prev);	
		vel = Vector2Scale(vel, 0.995f);
		Vector2 accel = Vector2Scale(GRAVITY, dt2);
		
		//vel = Vector2ClampValue(vel, -240, 240);

		node->pos_curr = Vector2Add(Vector2Add(node->pos_curr, vel), accel);

		if(IsMouseButtonDown(MOUSE_BUTTON_LEFT) && rope->selected_node == -1) {
			float seg_dist = (i == 0) ? rope->segment_dist : rope->segment_dist * 2;
			float r = fmaxf(rope->segment_dist, rope->segment_dist * Vector2Length(mouse_delta));

			if(CheckCollisionPointCircle(mouse_pos, node->pos_curr, r)) 
				rope->selected_node = i;
			else 
				rope->selected_node = -1;
		}

		if(node->flags & RN_PINNED) node->pos_curr = node->pos_prev;

		if(rope->selected_node == i) {
			node->pos_curr = mouse_pos;
			node->pos_prev = mouse_pos;
			new_prev = mouse_pos;
		}

		node->pos_curr.x = Clamp(node->pos_curr.x, 0, 1280);
		node->pos_curr.y = Clamp(node->pos_curr.y, 0, 800);

		node->pos_prev = new_prev;
	}

	if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) rope->selected_node = -1;
}

void RopeSolveConstraints(Rope *rope, float dt) {
	for(uint8_t i = 0; i < rope->length - 1; i++) {
		RopeNode *node_a = &rope->nodes[i];
		RopeNode *node_b = &rope->nodes[i + 1];

		Vector2 delta = Vector2Subtract(node_a->pos_curr, node_b->pos_curr); 	
		float dist = Vector2Length(delta);
		float correction = (dist - rope->segment_dist) / dist; 

		if(dist == 0) continue;

		Vector2 prev_a = node_a->pos_curr;
		Vector2 prev_b = node_b->pos_curr;
		
		if(node_a->flags & RN_PINNED || rope->selected_node == i) 
			node_b->pos_curr = Vector2Add(node_b->pos_curr, Vector2Scale(delta, correction));	
		else if (node_b->flags & RN_PINNED 	|| rope->selected_node == i)
			node_a->pos_curr = Vector2Add(node_b->pos_curr, Vector2Scale(delta, correction));	

		if(!Vector2Equals(prev_a, node_a->pos_curr) || !Vector2Equals(prev_b,  node_b->pos_curr)) continue;

		 node_a->pos_curr = Vector2Subtract(node_a->pos_curr, Vector2Scale(delta, 0.5f * correction));
		 node_b->pos_curr = Vector2Add(node_b->pos_curr, Vector2Scale(delta, 0.5f * correction));
	}
}

void RopeUpdate(Rope *rope, float dt) {
	rope->nodes[0].pos_prev = rope->nodes[0].pos_curr;

	for(uint8_t i = 0; i < rope->iteration_steps; i++) {
		RopeIntegrate(rope, dt);
		RopeSolveConstraints(rope, dt);
	}
}

void RopeDraw(Rope *rope) {
	for(uint8_t i = 0; i < rope->length - 1; i++) {
		Vector2 p0 = rope->nodes[i].pos_curr;
		Vector2 p1 = rope->nodes[i + 1].pos_curr;

		DrawLineEx(p0, p1, 16.0f, RAYWHITE);
		DrawCircleV(p0, 8.0f, RAYWHITE);
	}

	DrawCircleV(rope->nodes[0].pos_curr, 12.0f, DARKPURPLE);
}

void RopeClose(Rope *rope) {
	free(rope->nodes);
}

