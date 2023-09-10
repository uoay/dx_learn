#pragma once

#include <bitset>
#include <optional>
#include <queue>

class Keyboard {
    friend class Window;
public:
    class Event {
    public:
        enum class Type {
            Press,
            Release
        };
    private:
        Type mType;
        unsigned char mCode;
    public:
        Event(Type type, unsigned char code);
        bool IsPress() const;
        bool IsRelease() const;
        unsigned char GetCode() const;
    };
public:
    Keyboard() = default;
    Keyboard(const Keyboard&) = delete;
    Keyboard& operator=(const Keyboard&) = delete;
    //按下键盘
    bool IsKeyPressed(unsigned char code) const;
    std::optional<Event> ReadKey();
    bool IsKeyEmpty() const;
    void FlushKey();
    //输入文字
    std::optional<char> ReadChar();
    bool IsCharEmpty() const;
    void FlushChar();
    //Key && Char
    void Flush();

    void EnableAutoRepeat();
    void DisableAutoRepeat();
    bool IsAutoRepeatEnabled() const;

    static constexpr size_t mKeySatesSize = 256;
    static constexpr size_t mBufferSize = 16u;
    bool mAutoRepeatEnabled = false;
    std::bitset<mKeySatesSize> mKeyStates;
    std::queue<Event> mKeyBuffer;
    std::queue<char> mCharBuffer;
private:
    void OnKeyPressed(unsigned char keycode);
    void OnKeyReleased(unsigned char keycode);
    void OnChar(char character);
    void ClearKeyState();
    template<typename T> static void TrimBuffer(std::queue<T>& buffer);
};