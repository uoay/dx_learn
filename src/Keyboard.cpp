#include "Keyboard.h"

Keyboard::Event::Event(Type type, unsigned char code) :type(type), code(code) {}

bool Keyboard::Event::IsPress() const {
	return type == Type::Press;
}

bool Keyboard::Event::IsRelease() const {
	return type == Type::Release;
}

unsigned char Keyboard::Event::GetCode() const {
	return code;
}

bool Keyboard::IsKeyPressed(unsigned char code) const {
	return keyStates[code];
}

std::optional<Keyboard::Event> Keyboard::ReadKey() {
	if (keyBuffer.size() > 0u) {
		Keyboard::Event e = keyBuffer.front();
		keyBuffer.pop();
		return e;
	}
	return {};
}

bool Keyboard::IsKeyEmpty() const {
	return keyBuffer.empty();
}

void Keyboard::FlushKey() {
	keyBuffer = std::queue<Event>();
}

std::optional<char> Keyboard::ReadChar() {
	if (charBuffer.size() > 0u) {
		unsigned char code = charBuffer.front();
		charBuffer.pop();
		return code;
	}
	return {};
}

bool Keyboard::IsCharEmpty() const {
	return charBuffer.empty();
}

void Keyboard::FlushChar() {
	charBuffer = std::queue<char>();
}

void Keyboard::Flush() {
	FlushKey();
	FlushChar();
}

void Keyboard::EnableAutoRepeat() {
	autoRepeatEnabled = true;
}

void Keyboard::DisableAutoRepeat() {
	autoRepeatEnabled = false;
}

bool Keyboard::IsAutoRepeatEnabled() const {
	return autoRepeatEnabled;
}

void Keyboard::OnKeyPressed(unsigned char code) {
	keyStates[code] = true;
	keyBuffer.push(Keyboard::Event(Keyboard::Event::Type::Press, code));
	TrimBuffer(keyBuffer);
}

void Keyboard::OnKeyReleased(unsigned char code) {
	keyStates[code] = false;
	keyBuffer.push(Keyboard::Event(Keyboard::Event::Type::Release, code));
	TrimBuffer(keyBuffer);
}

void Keyboard::OnChar(char character) {
	charBuffer.push(character);
	TrimBuffer(charBuffer);
}

void Keyboard::ClearKeyState() {
	keyStates.reset();
}

template<typename T> void Keyboard::TrimBuffer(std::queue<T>& buffer) {
	while (buffer.size() > bufferSize) {
		buffer.pop();
	}
}