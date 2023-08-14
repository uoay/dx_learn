#include "Direct3DUtil.h"

#include <sstream>

Direct3DUtil::Exception::Exception(const char* file, int line, HRESULT errorCode) :GameException(file, line, errorCode) {}

const char* Direct3DUtil::Exception::what() const noexcept {
	std::ostringstream oss;
	oss << "[Error Code]" << GetErrorCode() << std::endl
		<< "[Description]" << GetErrorString() << std::endl
		<< GetExceptionLocation();
	mWhatBuffer = oss.str();
	return mWhatBuffer.c_str();
}

const char* Direct3DUtil::Exception::GetType() const {
	return "Direct3D Exception";
}