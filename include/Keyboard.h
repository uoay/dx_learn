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
        Type type;
        unsigned char code;
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

    static constexpr unsigned int nKeys = 256u;
    static constexpr unsigned int bufferSize = 16u;
    bool autoRepeatEnabled = false;
    std::bitset<nKeys> keyStates;
    std::queue<Event> keyBuffer;
    std::queue<char> charBuffer;
private:
    void OnKeyPressed(unsigned char keycode);
    void OnKeyReleased(unsigned char keycode);
    void OnChar(char character);
    void ClearKeyState();
    template<typename T> static void TrimBuffer(std::queue<T>& buffer);
};