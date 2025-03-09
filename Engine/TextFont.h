#pragma once
#include "DirectWriteCustomFont.h"
#include <string>

namespace TextFont {

	void Initialize(HWND hwnd);

	void Draw(std::string _text, DirectX::XMFLOAT2 _pos);

	void Draw(std::string _text, DirectX::XMFLOAT2 _pos, FontData _data);

	void Release();
}