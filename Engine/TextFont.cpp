#include "TextFont.h"
#include "Direct3D.h"

namespace TextFont {
	FontData data;
	DirectWriteCustomFont* Write;

	void TextFont::Initialize(HWND hwnd)
	{
		Write = new DirectWriteCustomFont(&data);

		// 初期化
		Write->Init(Direct3D::GetSwapChain(),hwnd);

		// フォントデータを改変
		data.fontSize = 60;
		data.fontWeight = DWRITE_FONT_WEIGHT_ULTRA_BLACK;
		data.Color = D2D1::ColorF(D2D1::ColorF::Red);
		data.font = Write->GetFontName(FontList::FONT::Kenney);
		data.shadowColor = D2D1::ColorF(D2D1::ColorF::White);
		data.shadowOffset = D2D1::Point2F(5.0f, -5.0f);

		// フォントをセット
		Write->SetFont(data);
	}

	void TextFont::Draw(std::string _text, DirectX::XMFLOAT2 _pos)
	{
		Write->DrawString(_text, _pos, D2D1_DRAW_TEXT_OPTIONS_NONE);
	}

	void Draw(std::string _text, DirectX::XMFLOAT2 _pos, FontData _data)
	{
		Write->SetFont(_data);
		Draw(_text, _pos);
		Write->SetFont(data);
	}

	void TextFont::Release()
	{
		delete Write;
		Write = nullptr;
	}
}