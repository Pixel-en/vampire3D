//#pragma once
//#include "DirectWriteCustomFont.h"
//#include <string>
//
//namespace TextFont
//{
//
//	void Initialize(HWND hwnd);
//
//	int Load(std::string filename);
//
//	void Draw(int handle, std::string text, XMFLOAT2 pos);
//	void Draw(int handle, std::string text, XMFLOAT2 point1, XMFLOAT2 point2);
//
//	void Release();
//
//	void SetData(int handle,DWRITE_FONT_WEIGHT weight, DWRITE_FONT_STYLE style, DWRITE_FONT_STRETCH stretch, float size,XMFLOAT3 color);
//}

#pragma once
#include "DirectWriteCustomFont.h"
#include <string>

namespace TextFont {

	void Initialize(HWND hwnd);

	void Draw(std::string text, DirectX::XMFLOAT2 pos);

	void Release();
}