#pragma once

#include "Drawable.h"
#include "../Vertex.h"

class Box : public Drawable {
public:
	Box(Graphics& graphics);
	void Update();
};