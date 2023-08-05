#pragma once

#include <exception>
#include <string>

#include <windows.h>

#define GAME_LAST_EXCPETION() GameException(__FILE__, __LINE__, GetLastError()); 

class GameException : public std::exception {
public:
    GameException(const char* file, int line, HRESULT errorCode);
    virtual const char* what() const noexcept;
    virtual const char* GetType() const;
    const std::string& GetFile() const;
    int GetLine() const;
    std::string GetExceptionLocation() const;
    HRESULT GetErrorCode() const;
    std::string GetErrorString() const;
    static std::string TranslateErrorCode(HRESULT errorCode);
protected:
    mutable std::string whatBuffer;
private:
    std::string file;
    int line;
    HRESULT errorCode;
};