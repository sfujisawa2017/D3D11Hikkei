#pragma once
#include "BASE.h"
#include "WINDOW.h"

//
//
struct D3D_INIT
{
	HWND hWnd;
};
//
//
class DIRECT3D11 : public CELEMENT
{
public:
	//Data
	HWND m_hWnd;
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;
	IDXGISwapChain* m_pSwapChain;
	ID3D11RenderTargetView* m_pBackBuffer_TexRTV;
	ID3D11DepthStencilView* m_pBuckBuffer_DSTexDSV;
	ID3D11Texture2D* m_pBuckBuffer_DSTex;
	ID3D11DepthStencilState* m_pBuckBuffer_DSTexState;
	ID3D11BlendState* m_pBlendState;
	
	ID3D11RasterizerState* m_pRasterizerState;
	
	//Method
	DIRECT3D11();
	~DIRECT3D11();

	HRESULT Init(D3D_INIT*);
	void ShowFPS();
	void Clear();
	HRESULT Present();
	HRESULT InitShader();
};
