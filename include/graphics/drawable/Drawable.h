#pragma once

#include <vector>

#include "../Graphics.h"
#include "../bindable/Bindable.h"

class Drawable {
public:
	Drawable() = default;
	void Draw(Graphics& graphics);
	void AddBind(std::unique_ptr<Bindable> bind);
protected:
	unsigned int mIndexCount;
private:
	std::vector<std::unique_ptr<Bindable>> mBinds;
};