#pragma once
#include "BASE.h"
#include "WINDOW.h"
#include "DIRECT3D11.h"

//
//
//
class CAMERA : public CELEMENT
{
public:
	//Data
	DIRECT3D11* m_pD3D;

	float m_fX;
	float m_fY;
	float m_fZ;
	float m_fGX;//íçéã
	float m_fGY;//íçéã
	float m_fGZ;//íçéã
	DWORD m_dwWindowWidth;
	DWORD m_dwWindowHeight;
	bool m_boGaze;//=true íçéãÇ†ÇËÉÇÅ[Éh
	D3DXVECTOR3 m_vPos;
	D3DXMATRIX m_mView;
	D3DXMATRIX m_mProj;
	
	//Method
	CAMERA();
	~CAMERA();
	void SetCameraPosition(float x,float y,float z);
	void SetCameraPositionGaze(float x,float y,float z,float gx,float gy,float gz);//íçéãÇ†ÇË
	HRESULT Init(DWORD dwWidth,DWORD dwHeight);
private:
	//Method
	HRESULT SetViewProj();

};