#include "GameException.h"

#include <sstream>

#include <windows.h>

GameException::GameException(const wchar_t* const type, const wchar_t* const file, const int line, const HRESULT errorCode):mType(type), mFile(file), mLine(line), mErrorCode(errorCode) {}

const wchar_t* GameException::What() const noexcept {
    std::wostringstream woss;
    woss << GetType() << std::endl << GetExceptionLocation();
    mWhatBuffer = woss.str();
    return mWhatBuffer.c_str();
}

const wchar_t* GameException::GetType() const noexcept {
    return mType.c_str();
}

const std::wstring& GameException::GetFile() const noexcept {
    return mFile;
}

int GameException::GetLine() const noexcept {
    return mLine;
}

std::wstring GameException::GetExceptionLocation() const noexcept {
    std::wostringstream woss;
    woss << L"[file]" << mFile << std::endl << L"[line]" << mLine;
    return woss.str();
}

HRESULT GameException::GetErrorCode() const noexcept {
    return mErrorCode;
}

std::wstring GameException::GetErrorString() const noexcept {
    return TranslateErrorCode(mErrorCode);
}

std::wstring GameException::TranslateErrorCode(HRESULT errorCode) noexcept {
    wchar_t* messageBuffer = nullptr;
    DWORD MessageLength = FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr,
        errorCode,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        reinterpret_cast<wchar_t*>(&messageBuffer),
        0,
        nullptr
    );
    if(MessageLength == 0) {
        return L"Unidentified error code";
    }
    std::wstring errorString(messageBuffer);
    LocalFree(messageBuffer);
    return errorString;
}