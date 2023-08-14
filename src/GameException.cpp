#include "GameException.h"

#include <sstream>

#include <windows.h>

GameException::GameException(const char* file, int line, HRESULT errorCode):mFile(file), mLine(line), mErrorCode(errorCode) {}

const char* GameException::what() const noexcept {
    std::ostringstream oss;
    oss << GetType() << std::endl << GetExceptionLocation();
    mWhatBuffer = oss.str();
    return mWhatBuffer.c_str();
}

const char* GameException::GetType() const {
    return "Game Exception";
}

const std::string& GameException::GetFile() const {
    return mFile;
}

int GameException::GetLine() const {
    return mLine;
}

std::string GameException::GetExceptionLocation() const {
    std::ostringstream oss;
    oss << "[file]" << mFile << std::endl
        << "[line]" << mLine;
    return oss.str();
}

HRESULT GameException::GetErrorCode() const {
    return mErrorCode;
}

std::string GameException::GetErrorString() const {
    return TranslateErrorCode(mErrorCode);
}

std::string GameException::TranslateErrorCode(HRESULT errorCode) {
    char* pMessageBuffer = nullptr;
    DWORD MessageLength = FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr,
        errorCode,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        reinterpret_cast<char*>(&pMessageBuffer),
        0,
        nullptr
    );
    if(MessageLength == 0) {
        return "Unidentified error code";
    }
    std::string errorString(pMessageBuffer);
    LocalFree(pMessageBuffer);
    return errorString;
}