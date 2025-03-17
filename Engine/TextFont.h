#pragma once
#include "DirectWriteCustomFont.h"
#include <string>

namespace TextFont {

	//初期化
	void Initialize(HWND hwnd);

	/// <summary>
	/// 指定位置に描画
	/// </summary>
	/// <param name="_text">描画する文字列</param>
	/// <param name="_pos">描画位置</param>
	void Draw(std::string _text, DirectX::XMFLOAT2 _pos);

	/// <summary>
	/// 指定位置に描画
	/// </summary>
	/// <param name="_text">描画する文字列</param>
	/// <param name="_pos">描画位置</param>
	/// <param name="_data">使用するFontData</param>
	void Draw(std::string _text, DirectX::XMFLOAT2 _pos, FontData _data);

	/// <summary>
	/// 領域内に描画
	/// </summary>
	/// <param name="_text">描画する文字列</param>
	/// <param name="_pos1">左上の位置</param>
	/// <param name="_pos2">右下の位置</param>
	void Draw(std::string _text, DirectX::XMFLOAT2 _pos1, DirectX::XMFLOAT2 _pos2);

	/// <summary>
	/// 領域内に描画
	/// </summary>
	/// <param name="_text">描画する文字列</param>
	/// <param name="_pos1">左上の位置</param>
	/// <param name="_pos2">右下の位置</param>
	/// <param name="_data">使用するFontData</param>
	void Draw(std::string _text, DirectX::XMFLOAT2 _pos1, DirectX::XMFLOAT2 _pos2, FontData _data);

	std::wstring GetFontName(FontList::FONT _font=FontList::FONT::Kenney);

	void Release();
}