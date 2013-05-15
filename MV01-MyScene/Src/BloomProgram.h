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

#include "BaseShader.h"
#include "RenderToTexture.h"

namespace mcg
{
	class TextDrawer;
	class TextFont;
}

class BloomProgram : public mcg::SampleProgram
{
public:
	BloomProgram(const HINSTANCE hInstance, const int nCmdShow);

	HRESULT initializeResources();
	void cleanResouces();

	void preRender();
	void render();

	void keyPressed(WPARAM key, LPARAM param);
	void keyReleased(WPARAM key, LPARAM param);

	struct PosTexCoords
	{
		DirectX::XMFLOAT3 posCoord;
		DirectX::XMFLOAT2 texCoord;
	};

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
		BLUR_LEVEL_INC,
		BLUR_LEVEL_DEC,
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
	ID3D11DepthStencilState* mDepthStencilState;

	//clock_t	clockTick;
	bool applyBlur;
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

	/*
	ID3D11VertexShader*			mTextureTrivialVS;
	ID3D11Texture2D*			mBaseTextureRenderTarget;
	ID3D11RenderTargetView*		mBaseTextureRenderTargetView;
	ID3D11Texture2D*			mBrightPassTextureRenderTarget;
	ID3D11RenderTargetView*		mBrightPassTextureRenderTargetView;
	ID3D11PixelShader*			mBrightPassPS;
	ID3D11InputLayout*			mBrightPassPSInputLayout;
	ID3D11PixelShader*			mTexMergePS;
	ID3D11InputLayout*			mTexMergePSInputLayout;
	ID3D11SamplerState*			mTexSamplerState;
	*/

	RenderToTexture*	mpPhongShader;
	RenderToTexture*	mpBrightPassShader;
	RenderToTexture*	mpTextureMergeShader;
	ID3D11Buffer*		mpTextureVertexBuffer;
	ID3D11Buffer*		mpTextureIndexBuffer;

	RenderToTexture*	mpBlurShader;
	struct BlurParamsStruct
	{
		int		blurLevel;
		float	padding[3];
	};

	float				floatBlurLevel;
	int					intBlurLevel;
	ID3D11Buffer*		mBlurParamBuffer;
	void updateBlurLevel(bool inc);
};