#include <DirectXMath.h>
#include <cmath>

/// <summary>
/// ÉIÉyÉåÅ[É^Å[Ç‹Ç∆Çﬂ
/// </summary>

using namespace DirectX;

/// <summary>
/// XMFLOAT3ìØémÇÃâ¡éZ
/// </summary>
inline XMFLOAT3 operator +(const XMFLOAT3& a, const XMFLOAT3& b) {
	return { a.x + b.x,a.y + b.y, a.z + b.z };
}

/// <summary>
/// XMVECTORÇ∆XMFLOAT3ÇÃâ¡éZ
/// </summary>
inline XMFLOAT3 operator +(const XMVECTOR& a, const XMFLOAT3& b) {
	XMVECTOR c = XMLoadFloat3(&b);
	XMFLOAT3 temp;
	XMStoreFloat3(&temp, a + c);
	return temp;
}

/// <summary>
/// XMFLOAT3Ç∆XMVECTORÇÃâ¡éZ
/// </summary>
inline XMFLOAT3 operator +(const XMFLOAT3& a, const XMVECTOR& b) {
	
	XMVECTOR c = XMLoadFloat3(&a);
	XMFLOAT3 temp;
	XMStoreFloat3(&temp, c + b);
	return temp;
}

/// <summary>
/// XMVECTORÇ∆XMFLOAT3ÇÃâ¡éZë„ì¸
/// </summary>
inline void operator +=(const XMVECTOR& a, XMFLOAT3& b) {
	XMVECTOR c = XMLoadFloat3(&b);
	XMFLOAT3 temp;
	c = a + c;
	XMStoreFloat3(&temp, c);
	b = temp;
}

/// <summary>
/// XMFLOAT3Ç∆XMVECTORÇÃâ¡éZë„ì¸
/// </summary>
inline void operator +=(XMFLOAT3& a, const XMVECTOR& b) {

	XMVECTOR c = XMLoadFloat3(&a);
	XMFLOAT3 temp;
	c = c + b;
	XMStoreFloat3(&temp, c);
	a = temp;
}

/// <summary>
/// XMFLOAT3ìØémÇÃå∏éZ
/// </summary>
inline float operator -(const XMFLOAT3& a, const XMFLOAT3& b) {
	return std::sqrtf((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) + (a.z - b.z) * (a.z - b.z));
}

/// <summary>
/// XMVECTORÇ∆XMFLOAT3ÇÃå∏éZ
/// </summary>
inline XMFLOAT3 operator -(const XMVECTOR& a, const XMFLOAT3& b) {
	XMVECTOR c = XMLoadFloat3(&b);
	XMFLOAT3 temp;
	XMStoreFloat3(&temp, a - c);
	return temp;
}

/// <summary>
/// XMFLOAT3Ç∆XMVECTORÇÃå∏éZ
/// </summary>
inline XMFLOAT3 operator -(const XMFLOAT3& a, const XMVECTOR& b) {

	XMVECTOR c = XMLoadFloat3(&a);
	XMFLOAT3 temp;
	XMStoreFloat3(&temp, c - b);
	return temp;
}

/// <summary>
/// XMVECTORÇ∆XMFLOAT3ÇÃå∏éZë„ì¸
/// </summary>
inline void operator -=(const XMVECTOR& a, XMFLOAT3& b) {
	XMVECTOR c = XMLoadFloat3(&b);
	XMFLOAT3 temp;
	c = a - c;
	XMStoreFloat3(&temp, c);
	b = temp;
}

/// <summary>
/// XMFLOAT3Ç∆XMVECTORÇÃå∏éZë„ì¸
/// </summary>
inline void operator -=(XMFLOAT3& a, const XMVECTOR& b) {

	XMVECTOR c = XMLoadFloat3(&a);
	XMFLOAT3 temp;
	c = c - b;
	XMStoreFloat3(&temp, c);
	a = temp;
}

/// <summary>
/// XMVECTORÇ∆XMFLOAT3ÇÃèÊéZ
/// </summary>
inline XMFLOAT3 operator *(const XMVECTOR& a, const XMFLOAT3& b) {
	XMVECTOR c = XMLoadFloat3(&b);
	XMFLOAT3 temp;
	XMStoreFloat3(&temp, a * c);
	return temp;
}

/// <summary>
/// XMFLOAT3Ç∆XMVECTORÇÃèÊéZ
/// </summary>
inline XMFLOAT3 operator *(const XMFLOAT3& a, const XMVECTOR& b) {

	XMVECTOR c = XMLoadFloat3(&a);
	XMFLOAT3 temp;
	XMStoreFloat3(&temp, c * b);
	return temp;
}

/// <summary>
/// XMFLOAT3Ç∆intÇÃèÊéZ
/// </summary>
inline XMFLOAT3 operator *(const XMFLOAT3& a, int& b) {
	return { a.x * b ,a.y * b ,a.z * b };
}

/// <summary>
/// XMFLOAT3Ç∆floatÇÃèÊéZ
/// </summary>
inline XMFLOAT3 operator *(const XMFLOAT3& a, float& b) {
	return { a.x * b,a.y * b,a.z * b };
}