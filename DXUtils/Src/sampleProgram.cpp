//--------------------------------------------------------------------------------------
// Copyright (c) Luca Giona 2013. Do not distribute. (Master Game Development - University of Verona)
//--------------------------------------------------------------------------------------

#include "sampleProgram.h"

#include <Windows.h>
#include <Windowsx.h>

#include "utils.h"
#include "types.h"

mcg::SampleProgram* mcg::SampleProgram::mRunningWindow = nullptr;

namespace mcg
{
	//Default WndProc
	LRESULT CALLBACK defaultWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		PAINTSTRUCT ps;
		HDC hdc;

		SampleProgram* runningWindow = SampleProgram::getWindow(hWnd);

		switch( message )
		{
			case WM_PAINT:
			{
				// Chiamato ogni volta si necessita di ridisegnare qualcosa a schermo
				hdc = BeginPaint( hWnd, &ps );
				EndPaint( hWnd, &ps );
			} break;

			case WM_DESTROY:
			{
				// Chiamato prima di uscire dall'applicazione
				PostQuitMessage( 0 );
			} break;

			default:
			{
				SampleProgram* window = SampleProgram::getWindow(hWnd);
				if(inputEventCallback(window, message, wParam, lParam) != 0)
				{
					// Per tutti gli altri eventi, lascia a Windows gestire le chiamate
					return DefWindowProc( hWnd, message, wParam, lParam );
				}
			}
		}
	
		return 0;
	}


	int inputEventCallback(SampleProgram* window, UINT message, WPARAM wParam, LPARAM lParam)
	{
		if(window == nullptr)
			return 2;

		switch( message )
		{
			// KEYBOARD
			case WM_KEYDOWN: window->keyPressed(wParam, lParam); break;		
			case WM_KEYUP: window->keyReleased(wParam, lParam); break;

			// MOUSE BUTTONS
			case WM_RBUTTONDOWN:
			case WM_LBUTTONDOWN:
			case WM_MBUTTONDOWN: window->mouseKeyPressed(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), wParam); break;

			case WM_LBUTTONUP:
			case WM_RBUTTONUP:
			case WM_MBUTTONUP: window->mouseKeyReleased(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), wParam); break;

			// MOUSE MOVE
			case WM_MOUSEMOVE: window->mouseKeyMoved(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), wParam); break;

			default: return 1;
		}

		return 0;
	}

}


/////////////////////////////////////////////////////////////////////////////////////////////////////
using namespace mcg;


SampleProgram::SampleProgram(const HINSTANCE hInstance, const int nCmdShow)
	: mNCmdShow(nCmdShow)
	, mHInst(hInstance)
	, mHWnd(nullptr)
	, mDriverType(D3D_DRIVER_TYPE_NULL)
	, mPSwapChain(nullptr)
	, mPd3dDevice(nullptr)
	, mPd3dDeviceContext(nullptr)
	, mPRenderTargetView(nullptr)
	, mMuntisampleCount(1)
{
	mWcex.cbSize = sizeof( WNDCLASSEX );
	mWcex.style = CS_HREDRAW | CS_VREDRAW;
	mWcex.lpfnWndProc = defaultWndProc;
	mWcex.cbClsExtra = 0;
	mWcex.cbWndExtra = 0;
	mWcex.hInstance = hInstance;
	mWcex.hIcon = LoadIcon( hInstance, ( LPCTSTR )IDI_APPLICATION );
	mWcex.hCursor = LoadCursor( nullptr, IDC_ARROW );
	mWcex.hbrBackground = ( HBRUSH )( COLOR_WINDOW + 1 );
	mWcex.lpszMenuName = nullptr;
	mWcex.lpszClassName = L"GraphicsProgrammingWindow";
	mWcex.hIconSm = LoadIcon( mWcex.hInstance, ( LPCTSTR )IDI_APPLICATION );

	mWindowRect.left = 0;
	mWindowRect.top = 0;
	mWindowRect.right = 800;
 	mWindowRect.bottom= 600;

	mWindowTitle = L"MCG Sample Program";
}

SampleProgram::~SampleProgram()
{
	cleanDevice();
}

SampleProgram* SampleProgram::getWindow(const HWND hWnd)
{
	if(mRunningWindow!= nullptr && hWnd == mRunningWindow->mHWnd)
		return mRunningWindow;
	else
		return mRunningWindow;
}

void SampleProgram::setWindowSize(const LONG width, const LONG height)
{
	mWindowRect.right = width;
	mWindowRect.bottom = height;
}

void SampleProgram::setWndClassex(const WNDCLASSEX& wcex)
{
	mWcex = wcex;
}

void SampleProgram::setWindowTitle(LPCWSTR windowTitle)
{
	mWindowTitle = windowTitle;
}

int SampleProgram::run()
{
	if(FAILED(initializeWindow()))
		return 1;

	if(FAILED(initializeDirectX()))
		return 1;

	if(FAILED(initializeResources()))
		return 1;

	mRunningWindow = this;

	// Main message loop
    MSG msg = {0};
    while( WM_QUIT != msg.message )
    {
        if( PeekMessage( &msg, nullptr, 0, 0, PM_REMOVE ) )
        {
            TranslateMessage( &msg );
            DispatchMessage( &msg );
        }
        else
        {
			renderWindowContent();
        }
    }

	mRunningWindow = nullptr;
	return ( int )msg.wParam;
}

void SampleProgram::renderWindowContent()
{
	preRender();
	render();
	postRender();
}

void SampleProgram::preRender()
{

}

void SampleProgram::postRender()
{

}

void SampleProgram::render()
{
	FLOAT clearColor[] = { 0.39f, 0.58f, 0.93f, 1.0f };
    mPd3dDeviceContext->ClearRenderTargetView( mPRenderTargetView, clearColor );
	mPSwapChain->Present( 0, 0 );
}

HRESULT SampleProgram::initializeResources()
{
	return S_OK;
}

void SampleProgram::cleanResouces()
{

}

HRESULT SampleProgram::initializeWindow()
{
	// Registrazione della classe
	if( !RegisterClassEx( &mWcex ) )
	{
		showErrorMessageBox(L"Register class failed!");
		return E_FAIL;
	}

	// Creazione della finestra
	AdjustWindowRect( &mWindowRect, WS_OVERLAPPEDWINDOW, FALSE );
    mHWnd = CreateWindow( mWcex.lpszClassName, mWindowTitle, WS_OVERLAPPEDWINDOW,
                           CW_USEDEFAULT, CW_USEDEFAULT, mWindowRect.right - mWindowRect.left, mWindowRect.bottom - mWindowRect.top, nullptr, nullptr, mWcex.hInstance,
                           nullptr );

	if( !mHWnd )
	{
		showErrorMessageBox(L"Window creation failed!");
		return E_FAIL;
	}

	// Mostra la finestra
    ShowWindow( mHWnd, mNCmdShow );

    return S_OK;
}

HRESULT SampleProgram::initializeDirectX()
{
	HRESULT hr = S_OK;

	// Registra l'area in cui compare la finestra
	RECT rc;
    GetClientRect( mHWnd, &rc );
    UINT width = rc.right - rc.left;
    UINT height = rc.bottom - rc.top;

	hr = initializeDevice();
	if(FAILED(hr))
		return E_FAIL;

	hr = initializeRenderTarget();
	if(FAILED(hr))
		return E_FAIL;

    // Setta il viewport
    D3D11_VIEWPORT vp;
    vp.Width = static_cast<float>(width);
    vp.Height = static_cast<float>(height);
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    mPd3dDeviceContext->RSSetViewports( 1, &vp );

	return S_OK;
}

HRESULT SampleProgram::initializeDevice()
{
	HRESULT hr = S_OK;

	// Registra l'area in cui compare la finestra
	RECT rc;
    GetClientRect( mHWnd, &rc );
    UINT width = rc.right - rc.left;
    UINT height = rc.bottom - rc.top;

    UINT createDeviceFlags = 0;
#ifdef _DEBUG
	// Crea un device in modalità debug
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	// Lo swap chain è responsabile del rendering a schermo effettivo. 
	// Solitamente è inizializzato in modalità double buffered, ovvero contiene solitamente due buffer: front e back
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory( &sd, sizeof( sd ) );
	// Numero di buffer
    sd.BufferCount = 1; // Numero di Back_buffer
    sd.BufferDesc.Width = width;
    sd.BufferDesc.Height = height;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // Formato RGBA a 32bit
    sd.BufferDesc.RefreshRate.Numerator = 60; // Refresh rate schermo
    sd.BufferDesc.RefreshRate.Denominator = 1; // Fattore di divisione refresh rate (intero)
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // Utilizzo del backbuffer: il rendering viene scritto qui
    sd.OutputWindow = mHWnd; // handler finistra
	sd.SampleDesc.Count = mMuntisampleCount; // Serve per il multisampling - 1 viene disabilitato
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE; // Modalità fullscree/windows

	// Definisce quali tipi di driver compatibili cercare
	// Hardware: utilizza schede video. Reference: se il driver "emula" in modo software alcune funzioni. (_Null : driver incapace di effettuare il rendering)
    D3D_DRIVER_TYPE driverTypes[] =
    {
        D3D_DRIVER_TYPE_HARDWARE,
        D3D_DRIVER_TYPE_REFERENCE,
		D3D_DRIVER_TYPE_SOFTWARE,
    };
    UINT numDriverTypes = sizeof( driverTypes ) / sizeof( driverTypes[0] );

    for( UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; ++driverTypeIndex)
    {
		// Prova ad inizializzare per i tipi di driver definiti
        mDriverType = driverTypes[driverTypeIndex];
        hr = D3D11CreateDeviceAndSwapChain(nullptr, mDriverType, nullptr, createDeviceFlags, nullptr, 0,
                                            D3D11_SDK_VERSION, &sd, &mPSwapChain, &mPd3dDevice, nullptr, &mPd3dDeviceContext );
		// Appena funziona esci
		if( SUCCEEDED( hr ) )
		{
			if(mDriverType != D3D_DRIVER_TYPE_HARDWARE)
				showWarningMessageBox(L"No hardware Direct3D here.", L"Warning", mHWnd);

            break;
		}
    }

	// Errore: niente di buono qui.
	if( FAILED( hr ) )
	{
		showErrorMessageBox(L"Device and swap chain creation failed!", L"Error", mHWnd);
		return hr;
	}

	return S_OK;
}

HRESULT SampleProgram::initializeRenderTarget()
{
	HRESULT hr = S_OK;

	RECT rc;
    GetClientRect( mHWnd, &rc );
    UINT width = rc.right - rc.left;
    UINT height = rc.bottom - rc.top;

    // Ottieni il backbuffer. Il back buffer è semplicemente una texture 2d. Una texture 2d è semplicemente una matrice bidimensionale di valori.
	ID3D11Texture2D* pBackBuffer;
    hr = mPSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( LPVOID* )&pBackBuffer );
    if( FAILED( hr ) )
	{
		showErrorMessageBox(L"No backbuffer texture!", L"Error", mHWnd);
        return hr;
	}

	// Crea il render target e collega l'output alla texture 2d
    hr = mPd3dDevice->CreateRenderTargetView( pBackBuffer, nullptr, &mPRenderTargetView );
    pBackBuffer->Release();
    if( FAILED( hr ) )
	{
		showErrorMessageBox(L"Render target creation failed!", L"Error", mHWnd);
        return hr;
	}

	// Creazione del depth e stencil buffer.
	ID3D11Texture2D* pDepthStencil = nullptr;
	D3D11_TEXTURE2D_DESC descDepth;
	descDepth.Width = width;
	descDepth.Height = height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;;
	descDepth.SampleDesc.Count = mMuntisampleCount;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;

	hr = mPd3dDevice->CreateTexture2D( &descDepth, nullptr, &pDepthStencil );
	if(FAILED(hr))
	{
		showErrorMessageBox(L"Depth-Stencil buffer creation failed!", L"Error", mHWnd);
        return hr;
	}

	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	descDSV.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDSV.ViewDimension = (mMuntisampleCount > 1)? D3D11_DSV_DIMENSION_TEXTURE2DMS : D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	descDSV.Flags = 0;

	// Create the depth stencil view
	hr = mPd3dDevice->CreateDepthStencilView( pDepthStencil, &descDSV, &mPDepthStencilView );
    if( FAILED( hr ) )
	{
		showErrorMessageBox(L"Depth-Stencil view creation failed!", L"Error", mHWnd);
        return hr;
	}

	// Collega il buffer al merger stage in modo che l'output sia effettuato qui
	mPd3dDeviceContext->OMSetRenderTargets(1, &mPRenderTargetView, mPDepthStencilView );

	return S_OK;
}

void SampleProgram::cleanDevice()
{
	cleanResouces();

	if( mPd3dDeviceContext )
	{
		mPd3dDeviceContext->ClearState();
		mPd3dDeviceContext = nullptr;
	}

	if( mPRenderTargetView )
	{
		mPRenderTargetView->Release();
		mPRenderTargetView = nullptr;
	}

	if( mPSwapChain )
	{
		mPSwapChain->Release();
		mPSwapChain = nullptr;
	}

	if( mPd3dDevice )
	{
		mPd3dDevice->Release();
		mPd3dDevice = nullptr;
	}
}

void SampleProgram::setMultisampleCount(const unsigned int multisampleCount)
{
	if(multisampleCount <= 0)
		mMuntisampleCount = 1;
	else
	{
		unsigned int c = 1;
		while(c <= multisampleCount)
			c = c << 1;

		mMuntisampleCount = c >> 1;
	}
}

unsigned int SampleProgram::getWindowWidth() const
{
	return mWindowRect.right;
}

unsigned int SampleProgram::getWindowHeight() const
{
	return mWindowRect.bottom;
}

float SampleProgram::getWindowAspectRatio() const
{
	return static_cast<float>(getWindowWidth()) / getWindowHeight();
}

void SampleProgram::keyPressed(WPARAM key, LPARAM param)
{
	if(key == VK_ESCAPE)
		PostQuitMessage(0);
}

void SampleProgram::keyReleased(WPARAM key, LPARAM param)
{

}

void SampleProgram::mouseKeyPressed(int xPos, int yPos, WPARAM buttons)
{

}

void SampleProgram::mouseKeyReleased(int xPos, int yPos, WPARAM buttons)
{

}

void SampleProgram::mouseKeyMoved(int xPos, int yPos, WPARAM buttons)
{

}

