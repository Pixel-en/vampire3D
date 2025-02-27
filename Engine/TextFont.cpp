#include "TextFont.h"
#include "Direct3D.h"
//#include "TextFont.h"
//#include "Direct3D.h"
//
//namespace TextFont
//{
//
//	DirectWriteCustomFont* Write;
//
//	std::vector<FontData*> datas;
//
//	void TextFont::Initialize(HWND hwnd)
//	{
//		FontData data;
//
//		Write = new DirectWriteCustomFont(&data);
//
//		Write->Init(Direct3D::GetSwapChain(),hwnd);
//
//	}
//
//	int Load(std::string filename)
//	{
//		FontData* pData = new FontData;
//
//		bool isExist = false;
//		//開いたファイルから同じファイル名がないか探す
//		for (int i = 0; i < datas.size(); i++) {
//			//既に開いているなら
//			if (datas[i] != nullptr && datas[i]->font == Write->StringToWString(filename)) {
//				pData->font = datas[i]->font;
//				isExist = true;
//				break;
//			}
//		}
//
//		if (isExist == false) {
//			if (FAILED(Write->AddPath(filename))) {
//				delete pData;
//				pData = nullptr;
//				return -1;
//			}
//
//			pData->font = Write->GetFontName(Write->GetFontNameNum() - 1);
//		}
//
//		for (int i = 0; i < datas.size(); i++) {
//			if (datas[i] == nullptr) {
//				datas[i] = pData;
//				return i;
//			}
//		}
//
//		datas.push_back(pData);
//		int handle = (int)datas.size() - 1;
//
//		return handle;
//
//
//	}
//
//	void Draw(int handle, std::string text, XMFLOAT2 pos)
//	{
//		Write->SetFont(*datas[handle]);
//		Write->DrawString(text, pos, D2D1_DRAW_TEXT_OPTIONS_NONE, false);
//	}
//
//	void Draw(int handle, std::string text, XMFLOAT2 point1, XMFLOAT2 point2)
//	{
//		Write->SetFont(*datas[handle]);
//		Write->DrawString(text, { point1.x,point1.y,point2.x,point2.y }, D2D1_DRAW_TEXT_OPTIONS_NONE);
//	}
//
//	void Release()
//	{
//		delete Write;
//		Write = nullptr;
//	}
//
//	void SetData(int handle, DWRITE_FONT_WEIGHT weight, DWRITE_FONT_STYLE style, DWRITE_FONT_STRETCH stretch, float size, XMFLOAT3 color)
//	{
//		datas[handle]->fontWeight = weight;
//		datas[handle]->fontStyle = style;
//		datas[handle]->fontStretch = stretch;
//		datas[handle]->fontSize = size;
//		datas[handle]->Color = { color.x,color.y,color.z };
//	}
//}


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
		data.font = Write->GetFontName(2);
		data.shadowColor = D2D1::ColorF(D2D1::ColorF::White);
		data.shadowOffset = D2D1::Point2F(5.0f, -5.0f);

		// フォントをセット
		Write->SetFont(data);
	}

	void TextFont::Draw(std::string text, DirectX::XMFLOAT2 pos)
	{
		Write->DrawString(text, pos, D2D1_DRAW_TEXT_OPTIONS_NONE);
	}

	void TextFont::Release()
	{
		delete Write;
		Write = nullptr;
	}
}