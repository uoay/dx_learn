#include "Keyboard.h"

Keyboard::Event::Event(Type type, unsigned char code) :mType(type), mCode(code) {}

bool Keyboard::Event::IsPress() const {
	return mType == Type::Press;
}

bool Keyboard::Event::IsRelease() const {
	return mType == Type::Release;
}

unsigned char Keyboard::Event::GetCode() const {
	return mCode;
}

bool Keyboard::IsKeyPressed(unsigned char code) const {
	return mKeyStates[code];
}

std::optional<Keyboard::Event> Keyboard::ReadKey() {
	if (mKeyBuffer.size() > 0) {
		Keyboard::Event e = mKeyBuffer.front();
		mKeyBuffer.pop();
		return e;
	}
	return {};
}

bool Keyboard::IsKeyEmpty() const {
	return mKeyBuffer.empty();
}

void Keyboard::FlushKey() {
	mKeyBuffer = std::queue<Event>();
}

std::optional<char> Keyboard::ReadChar() {
	if (mCharBuffer.size() > 0) {
		unsigned char code = mCharBuffer.front();
		mCharBuffer.pop();
		return code;
	}
	return {};
}

bool Keyboard::IsCharEmpty() const {
	return mCharBuffer.empty();
}

void Keyboard::FlushChar() {
	mCharBuffer = std::queue<char>();
}

void Keyboard::Flush() {
	FlushKey();
	FlushChar();
}

void Keyboard::EnableAutoRepeat() {
	mAutoRepeatEnabled = true;
}

void Keyboard::DisableAutoRepeat() {
	mAutoRepeatEnabled = false;
}

bool Keyboard::IsAutoRepeatEnabled() const {
	return mAutoRepeatEnabled;
}

void Keyboard::OnKeyPressed(unsigned char code) {
	mKeyStates[code] = true;
	mKeyBuffer.push(Keyboard::Event(Keyboard::Event::Type::Press, code));
	TrimBuffer(mKeyBuffer);
}

void Keyboard::OnKeyReleased(unsigned char code) {
	mKeyStates[code] = false;
	mKeyBuffer.push(Keyboard::Event(Keyboard::Event::Type::Release, code));
	TrimBuffer(mKeyBuffer);
}

void Keyboard::OnChar(char character) {
	mCharBuffer.push(character);
	TrimBuffer(mCharBuffer);
}

void Keyboard::ClearKeyState() {
	mKeyStates.reset();
}

template<typename T> void Keyboard::TrimBuffer(std::queue<T>& buffer) {
	while (buffer.size() > mBufferSize) {
		buffer.pop();
	}
}