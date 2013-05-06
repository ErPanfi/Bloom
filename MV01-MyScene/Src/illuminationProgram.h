//--------------------------------------------------------------------------------------
// Copyright (c) Luca Giona 2013. Do not distribute. (Master Game Development - University of Verona)
//--------------------------------------------------------------------------------------

#pragma once

#include "sampleProgram.h"
#include "camera.h"
#include "types.h"

#include "GeometricPrimitive.h"

#include <memory>
#include <time.h>

namespace mcg
{
	class TextDrawer;
	class TextFont;
}

class IlluminationProgram : public mcg::SampleProgram
{
public:
	IlluminationProgram(const HINSTANCE hInstance, const int nCmdShow);

	HRESULT initializeResources();
	void cleanResouces();

	void preRender();
	void render();

	void keyPressed(WPARAM key, LPARAM param);
	void keyReleased(WPARAM key, LPARAM param);
private:
	enum KEY_INDICES
	{
		ROTATE_CAMERA_LEFT = 0,
		ROTATE_CAMERA_RIGHT,
		ROTATE_CAMERA_UP,
		ROTATE_CAMERA_DOWN,
		MOVE_CAMERA_FOREWARD,
		MOVE_CAMERA_BACKWARD,
		CPOS_INC,
		CPOS_DEC,
		BTHRESH_INC,
		BTHRESH_DEC,
		MAX_KEY
	};

	mcg::TextDrawer* mTextDrawer;
	mcg::TextFont* mArialFont;

	mcg::SphericCamera mCamera;

	mcg::Transformations mTransforms;
	mcg::Material mMaterial;
	mcg::Material mPlaneMaterial;
	mcg::Lights mLights;

	ID3D11Buffer* mVertexBuffer;
	ID3D11Buffer* mIndexBuffer;
	ID3D11Buffer* mPlaneVertexBuffer;
	ID3D11Buffer* mPlaneIndexBuffer;
	ID3D11Buffer* mTranfCBuffer;
	ID3D11Buffer* mMaterialCBuffer;
	ID3D11Buffer* mPlaneMaterialCBuffer;
	ID3D11Buffer* mLightsCBuffer;
	ID3D11VertexShader* mPhongVS;
	ID3D11PixelShader* mPhongPS;
	ID3D11InputLayout* mPhongInputLayout;
	ID3D11DepthStencilState* mDepthStencilState;

	//clock_t	clockTick;
	bool brightPass;
	bool pauseLightsRotation;
	bool keys[MAX_KEY];

	std::unique_ptr<DirectX::GeometricPrimitive> mLightSphere;

	float cPos;
	float shiftPos;

	float cThresh;
	struct BrightThresh
	{
		float	threshold;
		float	padding[3];
	};

	ID3D11Buffer*	mBrightThreshBuffer;	//apply bright pass
	//ID3D11Buffer*	mNoBrightThreshBuffer;	//no brigth pass

	ID3D11Texture2D*			mBaseTextureRenderTarget;
	ID3D11RenderTargetView*		mBaseTextureRenderTargetView;
	ID3D11Texture2D*			mBrightPassTextureRenderTarget;
	ID3D11RenderTargetView*		mBrightPassTextureRenderTargetView;
	ID3D11PixelShader*			mBrightPassPS;
	ID3D11InputLayout*			mBrightPassPSInputLayout;
	ID3D11PixelShader*			mTexMergePS;
	ID3D11InputLayout*			mTexMergePSInputLayout;
	ID3D11SamplerState*			mTexSamplerState;
};
