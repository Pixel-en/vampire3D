#include <DirectXMath.h>
#include <cmath>

using namespace DirectX;

inline XMFLOAT3 operator +(const XMVECTOR& a, const XMFLOAT3& b) {
	XMVECTOR c = XMLoadFloat3(&b);
	XMFLOAT3 temp;
	XMStoreFloat3(&temp, a + c);
	return temp;
}

inline XMFLOAT3 operator +(const XMFLOAT3& a, const XMVECTOR& b) {
	
	XMVECTOR c = XMLoadFloat3(&a);
	XMFLOAT3 temp;
	XMStoreFloat3(&temp, c + b);
	return temp;
}

inline void operator +=(const XMVECTOR& a, XMFLOAT3& b) {
	XMVECTOR c = XMLoadFloat3(&b);
	XMFLOAT3 temp;
	c = a + c;
	XMStoreFloat3(&temp, c);
	b = temp;
}

inline void operator +=(XMFLOAT3& a, const XMVECTOR& b) {

	XMVECTOR c = XMLoadFloat3(&a);
	XMFLOAT3 temp;
	c = c + b;
	XMStoreFloat3(&temp, c);
	a = temp;
}

inline float operator -(const XMFLOAT3& a, const XMFLOAT3& b) {
	return std::sqrtf((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) + (a.z - b.z) * (a.z - b.z));
}

inline XMFLOAT3 operator -(const XMVECTOR& a, const XMFLOAT3& b) {
	XMVECTOR c = XMLoadFloat3(&b);
	XMFLOAT3 temp;
	XMStoreFloat3(&temp, a - c);
	return temp;
}

inline XMFLOAT3 operator -(const XMFLOAT3& a, const XMVECTOR& b) {

	XMVECTOR c = XMLoadFloat3(&a);
	XMFLOAT3 temp;
	XMStoreFloat3(&temp, c - b);
	return temp;
}

inline void operator -=(const XMVECTOR& a, XMFLOAT3& b) {
	XMVECTOR c = XMLoadFloat3(&b);
	XMFLOAT3 temp;
	c = a - c;
	XMStoreFloat3(&temp, c);
	b = temp;
}

inline void operator -=(XMFLOAT3& a, const XMVECTOR& b) {

	XMVECTOR c = XMLoadFloat3(&a);
	XMFLOAT3 temp;
	c = c - b;
	XMStoreFloat3(&temp, c);
	a = temp;
}

inline XMFLOAT3 operator *(const XMVECTOR& a, const XMFLOAT3& b) {
	XMVECTOR c = XMLoadFloat3(&b);
	XMFLOAT3 temp;
	XMStoreFloat3(&temp, a * c);
	return temp;
}

inline XMFLOAT3 operator *(const XMFLOAT3& a, const XMVECTOR& b) {

	XMVECTOR c = XMLoadFloat3(&a);
	XMFLOAT3 temp;
	XMStoreFloat3(&temp, c * b);
	return temp;
}
