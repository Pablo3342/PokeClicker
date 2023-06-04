#pragma once

enum DIALOG_BOX {
	PROFFESOR,
	FIGHT,
	NONE
};

enum PLAYER_ANIMATION {
	WALKING_UP_1,
	WALKING_UP_2,
	WALKING_DOWN_1,
	WALKING_DOWN_2
};

enum MAP {
	SPAWN, FOREST, BEACH
};

template<typename Type>
struct Point {
	Type x;
	Type y;
};

template<typename Type>
struct Size {
	Type w;
	Type h;
};

struct EngineVector {
	float x;
	float y;
};

template<typename Type>
struct Hitbox {
	Type x; // left
	Type y; // top
	Type w;
	Type h;
};