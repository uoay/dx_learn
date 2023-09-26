#pragma once

#include <exception>
#include <string>

#include <windows.h>

#define WND_EXCEPTION(errorCode) GameException(L"Window Exception", TEXT(__FILE__), __LINE__, errorCode)
#define WND_THROW_LAST_EXCEPTION() GameException(L"Window Exception" ,TEXT(__FILE__), __LINE__, GetLastError())
#define GFX_THROW_IF_FAILED(hr) {\
	if (FAILED(hr)) {\
		throw GameException(L"Graphics Exception", TEXT(__FILE__), __LINE__, hr);\
	}\
}
#define GFX_THROW_LAST_EXCEPTION() throw GameException(L"Graphics Exception", TEXT(__FILE__), __LINE__, GetLastError());

class GameException : public std::exception {
public:
    GameException(const wchar_t* const type, const wchar_t* const file, const int line, const HRESULT errorCode);
    const wchar_t* What() const noexcept;
    const wchar_t* GetType() const noexcept;
    const std::wstring& GetFile() const noexcept;
    int GetLine() const noexcept;
    std::wstring GetExceptionLocation() const noexcept;
    HRESULT GetErrorCode() const noexcept;
    std::wstring GetErrorString() const noexcept;
    static std::wstring TranslateErrorCode(HRESULT errorCode) noexcept;
protected:
    mutable std::wstring mWhatBuffer;
private:
    std::wstring mType;
    std::wstring mFile;
    int mLine;
    HRESULT mErrorCode;
};