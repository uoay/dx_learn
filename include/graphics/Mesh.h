#pragma once

#include "Graphics.h"
#include "Vertex.h"

class Mesh {
	Mesh(
		const Graphics& graphics,
		const std::vector<unsigned int>& indices,
		const DXGI_FORMAT indexFormat,
		const std::vector<Vertex>& vertices,
		const unsigned int startSlot,
		const unsigned int numViews
	);
};