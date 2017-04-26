#pragma once
#include "BASE.h"
#include "WINDOW.h"
#include "DIRECT3D11.h"
#include "CAMERA.h"
#include "CD3DXMESH.h"
#include "CD3DXMESH_ANIM.h"
#include "CD3DXSKINMESH.h"
#include "SOUND.h"

//
//
enum SCENE
{
	OPENING,
	PLAY_STAGE1,
	PLAY_STAGE2,
	PLAY_STAGE3,
	WIN,
	GAMEOVER,
};

//
//
class DIRECTOR : public CELEMENT
{
public:
	//Data
	HINSTANCE m_hInstance;
	WINDOW* m_pWindow;
	HWND m_hWnd;
	SCENE m_Scene;
	DIRECT3D11* m_pD3d;
	CAMERA* m_pCamera;
	SOUND* m_pSound;
	CD3DXMESH* m_pStaticMesh;
	CD3DXMESH_ANIM* m_pHierarchyMesh;
	CD3DXSKINMESH* m_pSkinMesh;
	//Method
	DIRECTOR();
	~DIRECTOR();
	HRESULT Init();
	void Run(HINSTANCE);
	void MainLoop();
	void FixFPS60();
	void Stage1();
};