#include "DIRECTOR.h"

//
//
//
DIRECTOR::DIRECTOR()
{
	ZeroMemory(this,sizeof(DIRECTOR));
	m_Scene=PLAY_STAGE1;
}
//
//
//
DIRECTOR::~DIRECTOR()
{
	SAFE_DELETE(m_pCamera);
	SAFE_DELETE(m_pD3d);
	SAFE_DELETE(m_pWindow);
	SAFE_DELETE(m_pSound);
}
//
//
//
void DIRECTOR::Run(HINSTANCE hInstance)
{
	WCHAR dir[512];
	GetCurrentDirectory(sizeof(dir),dir);
	InitDirectory(dir);

	m_hInstance=hInstance;
	if(FAILED(Init()))
	{
		return;
	}
	ShowWindow(m_hWnd,SW_SHOW);
	UpdateWindow(m_hWnd) ;
	// メッセージループ
	MSG msg={0};
	ZeroMemory(&msg,sizeof(msg));
	while(msg.message!=WM_QUIT)
	{
		if( PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			MainLoop();
		}
	}
}
//
//
//
void DIRECTOR::MainLoop()
{
	m_pD3d->Clear();
	switch(m_Scene)
	{
		case OPENING:
		break;
		case PLAY_STAGE1:
			Stage1();
		break;
		case WIN:
		break;
		case GAMEOVER:
		break;
	}
	FixFPS60();
	m_pD3d->Present();
}
//
//
//
HRESULT DIRECTOR::Init()
{
	//window
	m_pWindow=new WINDOW;
	if(!m_pWindow)
	{
		return E_FAIL;
	}
	MFAIL(m_pWindow->InitWindow(m_hInstance,0,0,WINDOW_WIDTH,WINDOW_HEIGHT,APP_NAME),L"ウィンドウ作成失敗");
	m_hWnd=m_pWindow->m_hWnd;
	//direct3D11
	D3D_INIT di;
	m_pD3d=new DIRECT3D11;
	if(m_pD3d==NULL)
	{
		MSG(L"Direct3Dの初期化失敗");
		return E_FAIL;
	}
	di.hWnd=m_hWnd;
	MFAIL(m_pD3d->Init(&di),L"Direct3D初期化失敗");
	
	//メッシュ読み込み
	
	//StaticMesh
	SetVisualDirectory();
	m_pStaticMesh=new CD3DXMESH;	
	m_pStaticMesh->Init(m_hWnd,m_pD3d->m_pDevice,m_pD3d->m_pDeviceContext,"StaticMesh.x");
	m_pStaticMesh->m_vPos=D3DXVECTOR3(0,2,0);

	//HierarchyMesh
	SetVisualDirectory();
	m_pHierarchyMesh=new CD3DXMESH_ANIM;
	m_pHierarchyMesh->Init(m_hWnd,m_pD3d->m_pDevice,m_pD3d->m_pDeviceContext,"HierarchyMesh.x");
	
	//SKINMESH
	SetVisualDirectory();
	CD3DXSKINMESH_INIT si;
	si.hWnd=m_hWnd;
	si.pDevice=m_pD3d->m_pDevice;
	si.pDeviceContext=m_pD3d->m_pDeviceContext;
	m_pSkinMesh=new CD3DXSKINMESH;
	MFAIL(m_pSkinMesh->Init(&si),L"スキンメッシュ初期化失敗");

	SetVisualDirectory();
	m_pSkinMesh->CreateFromX("SkinnedMesh.x");
	m_pSkinMesh->m_Pos=D3DXVECTOR3(0,-2,0);
	//camera
	m_pCamera=new CAMERA;
	m_pCamera->Init(WINDOW_WIDTH,WINDOW_HEIGHT);
	m_pCamera->SetCameraPosition(0,0,-1);

	//Sound(XAuido2)
	m_pSound=new SOUND;
	MFAIL(m_pSound->Init(),L"サウンド初期化失敗");
	//サウンド読み込み
	SetSoundDirectory();
	m_pSound->LoadSound("Chorus.wav");

	return S_OK;
}
//
//
//
void DIRECTOR::FixFPS60()
{
	static INT Frames=0,FPS=0;
	static LARGE_INTEGER Frq={0},PreviousTime={0},CurrentTime={0};
	DOUBLE Time=0;
	char sz[11]={0};

	while(Time<16.6666)//1100ms / 60frame=16.6666 
	{
		QueryPerformanceFrequency(&Frq);
		
		QueryPerformanceCounter(&CurrentTime);
		Time=CurrentTime.QuadPart-PreviousTime.QuadPart;
		Time *=(DOUBLE)1100.0 / (DOUBLE)Frq.QuadPart;		
	}
	PreviousTime=CurrentTime;
}