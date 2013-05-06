//--------------------------------------------------------------------------------------
// Copyright (c) Luca Giona 2013. Do not distribute. (Master Game Development - University of Verona)
//--------------------------------------------------------------------------------------

#pragma once

#include <DirectXMath.h>

namespace mcg
{
	class SphericCamera
	{
	public:
		SphericCamera();

		DirectX::XMMATRIX getViewMatrix() const;
		DirectX::XMVECTOR getCameraPosition() const;
		DirectX::XMFLOAT4 getCameraPositionFlaot4() const;

		void setFocusPoint(const DirectX::XMVECTOR focusPoint);

		void xRotation(const float dTheta);
		void yRotation(const float dPhi);
		void translate(const float dRho);
		
	private:
		void adjustTheta();
		void adjustPhi();
		void adjustRho();

		DirectX::XMVECTOR mFocusPoint;
		float mPhi;
		float mTheta;
		float mRho;
	};
}
