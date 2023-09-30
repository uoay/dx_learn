#include "drawable/Drawable.h"

#include "GraphicsAccessor.h"

void Drawable::Draw(Graphics& graphics) {
	for (auto& b : mBinds) {
		b->Bind(graphics);
	}
	
}

void Drawable::AddBind(std::unique_ptr<Bindable> bind) {
	mBinds.push_back(std::move(bind));
}