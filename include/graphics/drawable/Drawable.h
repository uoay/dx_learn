#pragma once

#include <vector>

#include "../Graphics.h"
#include "../bindable/Bindable.h"

class Drawable {
public:
	Drawable() = default;
	virtual void Draw(Graphics& graphics);
	void AddBind(std::unique_ptr<Bindable> bind);
protected:
	unsigned int mIndexCount;
	unsigned int mInstanceCount = 0;
private:
	std::vector<std::unique_ptr<Bindable>> mBinds;
};