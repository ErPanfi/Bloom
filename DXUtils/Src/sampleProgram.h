//--------------------------------------------------------------------------------------
// Copyright (c) Luca Giona 2013. Do not distribute. (Master Game Development - University of Verona)
//--------------------------------------------------------------------------------------

#pragma once

#include <Windows.h>
#include <d3d11.h>
#include <string>

namespace mcg
{
	class SampleProgram
	{
	public:
		SampleProgram(const HINSTANCE hInstance, const int nCmdShow);
		virtual ~SampleProgram();

		void setWndClassex(const WNDCLASSEX& wcex);
		void setWindowSize(const LONG width, const LONG height);
		void setWindowTitle(LPCWSTR windowTitle);
		void setMultisampleCount(const unsigned int multisampleCount);

		int run();

		static SampleProgram* getWindow(const HWND hWnd);

		// Da reimplementare a piacimento
		virtual HRESULT initializeResources(); // Chiamata una sola volta dopo l'inizializzazione di DX.
		virtual void cleanResouces(); // Chiamata alla fine del programma.
		virtual void preRender(); // Chiamata ad ogni ciclo del loop di rendering prima della funzione render().
		virtual void postRender(); // Chiamata ad ogni ciclo del loop di rendering dopo della funzione render().
		virtual void render(); // Chiamata ad ogni ciclo del loop.

		// KEYBOARD
		virtual void keyPressed(WPARAM key, LPARAM param); // http://msdn.microsoft.com/en-us/library/windows/desktop/ms646280(v=vs.85).aspx
		virtual void keyReleased(WPARAM key, LPARAM param); // http://msdn.microsoft.com/en-us/library/windows/desktop/ms646281(v=vs.85).aspx
		// MOUSE 
		virtual void mouseKeyPressed(int xPos, int yPos, WPARAM buttons); // http://msdn.microsoft.com/en-us/library/windows/desktop/ms645610(v=vs.85).aspx
		virtual void mouseKeyReleased(int xPos, int yPos, WPARAM buttons); // http://msdn.microsoft.com/en-us/library/windows/desktop/ms645611(v=vs.85).aspx
		virtual void mouseKeyMoved(int xPos, int yPos, WPARAM buttons); // http://msdn.microsoft.com/en-us/library/windows/desktop/ms645616(v=vs.85).aspx

		unsigned int getWindowWidth() const;
		unsigned int getWindowHeight() const;
		float getWindowAspectRatio() const;

	protected:
		void renderWindowContent();
		void cleanDevice();

		virtual HRESULT initializeWindow();
		virtual HRESULT initializeDirectX();
		virtual HRESULT initializeDevice();
		virtual HRESULT initializeRenderTarget();
		
		HWND mHWnd;
		HINSTANCE mHInst;
		ID3D11Device* mPd3dDevice;
		IDXGISwapChain* mPSwapChain;
		ID3D11DeviceContext* mPd3dDeviceContext;
		ID3D11RenderTargetView* mPRenderTargetView;
		ID3D11DepthStencilView* mPDepthStencilView;



	private:
		WNDCLASSEX mWcex;
		RECT mWindowRect;
		int mNCmdShow;
		LPCWSTR mWindowTitle;

		D3D_DRIVER_TYPE mDriverType;
		int mMuntisampleCount;

		static SampleProgram* mRunningWindow;
	};
	
	LRESULT CALLBACK defaultWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	int inputEventCallback(SampleProgram* window, UINT message, WPARAM wParam, LPARAM lParam);
}


