#pragma once
#include <DirectXMath.h>
#include <string>

namespace Debug
{
	void Log(int value, bool isLineFeed = false);
	void Log(float value, bool isLineFeed = false);
	void Log(std::string value, bool isLineFeed = false);
	void Log(DirectX::XMFLOAT3 value, bool isLineFeed = false);
};

