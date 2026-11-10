#include "MyMath.h"
#include <math.h>

VECTOR VecCreate(VECTOR start, VECTOR end)
{
	VECTOR result = { 0 };

	result.x = end.x - start.x;
	result.y = end.y - start.y;
	result.z = end.z - start.z;

	return result; 
}

VECTOR VecAdd(VECTOR vecA, VECTOR vecB)
{
	VECTOR result = { 0 };

	result.x = vecA.x + vecB.x;
	result.y = vecA.y + vecB.y;
	result.z = vecA.z + vecB.z;

	return result;
}

VECTOR VecScale(VECTOR vec, float scale)
{
	VECTOR result = { 0 };

	result.x = vec.x * scale;
	result.y = vec.y * scale;
	result.z = vec.z * scale;

	return result;
}

float VecLong(VECTOR vec)
{
	return sqrtf((vec.x * vec.x) + (vec.y * vec.y) + (vec.z * vec.z));
}

VECTOR VecNormalize(VECTOR vec)
{
	// ベクトルの長さを計算
	float vecLong = VecLong(vec);

	// もしも長さが0だと0割り算が起きるのでここで終了
	if (vecLong == 0.0f)
	{
		return VGet(0.0f, 0.0f, 0.0f);
	}

	// ベクトルを自身の長さで割り算すると正規化できる
	// Longで割るということは 1 / Long で掛け算するのと等しい
	VECTOR result = VecScale(vec, 1.0f / vecLong);

	return result;
}

float VecDot(VECTOR vecA, VECTOR vecB)
{
	return vecA.x * vecB.x + vecA.y * vecB.y + vecA.z * vecB.z;
}

float VecCross2D(VECTOR vecA, VECTOR vecB)
{
	return vecA.x * vecB.y - vecA.y * vecB.x;
}

VECTOR VecCross3D(VECTOR vecA, VECTOR vecB)
{
	VECTOR result = { 0 };

	result.x = vecA.y * vecB.z - vecA.z * vecB.y;
	result.y = vecA.z * vecB.x - vecA.x * vecB.z;
	result.z = vecA.x * vecB.y - vecA.y * vecB.x;

	return result;
}

