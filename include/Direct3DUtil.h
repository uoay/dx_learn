#pragma once

#include "directx/d3dx12.h"

#include "GameException.h"

#define THROW_IF_FAILED(hr) {\
	if (FAILED(hr)) {\
		throw Direct3DUtil::Exception(__FILE__, __LINE__, hr);\
	}\
}
#define THROW_LAST_EXCEPTION() throw Direct3DUtil::Exception(__FILE__, __LINE__, GetLastError());

class Direct3DUtil{
	friend class Graphics;
private:
	class Exception : public GameException {
	public:
		Exception(const char* file, int line, HRESULT errorCode);
		virtual const char* what() const noexcept;
		virtual const char* GetType() const;
	};
};