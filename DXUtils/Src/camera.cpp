//--------------------------------------------------------------------------------------
// Copyright (c) Luca Giona 2013. Do not distribute. (Master Game Development - University of Verona)
//--------------------------------------------------------------------------------------

#include "camera.h"
#include <algorithm>

using namespace mcg;
using namespace DirectX;

SphericCamera::SphericCamera()
	: mFocusPoint(XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f))
	, mPhi(0.0f)
	, mTheta(XM_PIDIV2)
	, mRho(10)
{}


XMMATRIX SphericCamera::getViewMatrix() const
{
	XMVECTOR up = {0.0f, 1.0f, 0.0f, 0.0f};
	return XMMatrixLookAtLH(getCameraPosition(), mFocusPoint, up);
}

XMVECTOR SphericCamera::getCameraPosition() const
{
	const float cosPhi = std::cos(mPhi);
	const float sinPhi = std::sin(mPhi);
	const float cosTheta = std::cos(mTheta);
	const float sinTheta = std::sin(mTheta);
	XMVECTOR ret = 
	{
		mRho * sinTheta * sinPhi,
		mRho * cosTheta,
		-mRho * sinTheta * cosPhi,
		0.0f
	};

	return XMVectorAdd(ret, mFocusPoint);
}

XMFLOAT4 SphericCamera::getCameraPositionFlaot4() const
{
	XMFLOAT4 ret;
	XMStoreFloat4(&ret, getCameraPosition());
	return ret;
}

void SphericCamera::xRotation(const float dTheta)
{
	mTheta +=dTheta;
	adjustTheta();
}

void SphericCamera::yRotation(const float dPhi)
{	
	mPhi +=dPhi;
	adjustPhi();
}

void SphericCamera::translate(const float dRho)
{
	mRho += dRho;
	adjustRho();
}

void SphericCamera::setFocusPoint(const DirectX::XMVECTOR focusPoint)
{
	mFocusPoint = focusPoint;
}

void SphericCamera::adjustTheta()
{
	mTheta = std::min(std::max(mTheta, 0.1f), (XM_PI - 0.1f));
}

void SphericCamera::adjustPhi()
{
	float div = std::floor(std::abs(mPhi / XM_2PI)); //std::trunc
	if(mPhi > 0.0f)
		mPhi -= div * XM_2PI;
	else
		mPhi += (div+1.0f) * XM_2PI;
}

void SphericCamera::adjustRho()
{
	if(mRho < 0.01f)
		mRho = 0.01f;
}
