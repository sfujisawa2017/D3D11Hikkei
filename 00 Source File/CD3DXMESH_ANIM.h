#include <stdio.h>
#include <windows.h>
#include<d3dx9.h>
#include <d3d11.h>
#include <d3dx10.h>
#include <d3dx11.h>
#include <d3dCompiler.h>
#include "BASE.h"

#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dx10.lib")
#pragma comment(lib,"d3dx11.lib")
#pragma comment(lib,"d3dCompiler.lib")

//マクロ
#define SAFE_RELEASE(x) if(x){x->Release(); x=0;}
#define SAFE_DELETE(x) if(x){delete x; x=0;}
#define SAFE_DELETE_ARRAY(p){ if(p){ delete[] (p);   (p)=NULL;}}

//構造体定義
struct BUFFER_PER_MESH
{
	D3DXMATRIX mW;//ワールド行列
	D3DXMATRIX mWVP;//ワールドから射影までの変換行列
	D3DXVECTOR4 vLightDir;//ライト方向
	D3DXVECTOR4 vEye;//カメラ位置
};

struct BUFFER_PER_MATERIAL
{
	D3DXVECTOR4 vAmbient;//アンビエント光
	D3DXVECTOR4 vDiffuse;//ディフューズ色
	D3DXVECTOR4 vSpecular;//鏡面反射
};

//オリジナル　マテリアル構造体
struct MY_HMATERIAL
{
	CHAR szName[100];
	D3DXVECTOR4 Ambient;//アンビエント
	D3DXVECTOR4 Diffuse;//ディフューズ
	D3DXVECTOR4 Specular;//スペキュラー
	CHAR szTextureName[256];//テクスチャーファイル名
	ID3D11ShaderResourceView* pTexture;
	DWORD dwNumFace;//そのマテリアルであるポリゴン数
	MY_HMATERIAL()
	{
		ZeroMemory(this,sizeof(MY_HMATERIAL));
	}
	~MY_HMATERIAL()
	{
		SAFE_RELEASE(pTexture);
	}
};
//頂点の構造体
struct MY_VERTEX_TEX
{
	D3DXVECTOR3 vPos;
	D3DXVECTOR3 vNorm;
	D3DXVECTOR2 vTex;
};

struct MY_VERTEX_NOTEX
{
	D3DXVECTOR3 vPos;
	D3DXVECTOR3 vNorm;
};

//パーツメッシュ　アプリ定義メッシュ(スタティックメッシュのサンプルにおけるCD3DXMESHクラスと同等）
struct PARTS_MESH
{
	DWORD dwNumMaterial;
	ID3D11Buffer* pVertexBuffer;
	ID3D11Buffer** ppIndexBuffer;
	char TextureFileName[8][256];//テクスチャーファイル名（８枚まで）
	MY_HMATERIAL* pMaterial;
	bool Tex;
};
//派生フレーム構造体
struct MYHFRAME: public D3DXFRAME
{
	D3DXMATRIXA16 CombinedTransformationMatrix;
	PARTS_MESH* pPartsMesh;
	MYHFRAME()
	{
		ZeroMemory(this,sizeof(MYHFRAME));
	}
};
//メッシュコンテナー構造体 フレーム内で使用
struct MYHMESHCONTAINER: public D3DXMESHCONTAINER
{
	LPDIRECT3DTEXTURE9*  ppTextures;
};
//Xファイル内のアニメーション階層を読み下すクラス。
class MY_H_HIERARCHY: public ID3DXAllocateHierarchy
{
public:
	MY_H_HIERARCHY(){}
	STDMETHOD(CreateFrame)(THIS_ LPCSTR , LPD3DXFRAME *);
	STDMETHOD(CreateMeshContainer)(THIS_ LPCSTR ,CONST D3DXMESHDATA* ,CONST D3DXMATERIAL* , 
		CONST D3DXEFFECTINSTANCE* , DWORD , CONST DWORD *, LPD3DXSKININFO , LPD3DXMESHCONTAINER *);
	STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME );
	STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER );
};

//
//
//
class CD3DXMESH_ANIM
{
public:
	//Data
	HWND m_hWnd;
	DWORD m_dwNumMaterial;
	LPD3DXMESH m_pMesh;
	D3DXVECTOR3 m_vPos;
	float m_fYaw,m_fPitch,m_fRoll;
	float m_fScale;
	D3DXMATRIX m_View;
	D3DXMATRIX m_Proj;
	D3DXVECTOR3 m_LightDir;
	D3DXVECTOR3 m_Eye;
	//アニメ関連
	MY_H_HIERARCHY* m_pHierarchy;
	LPD3DXFRAME m_pFrameRoot;
	LPD3DXANIMATIONCONTROLLER m_pAnimController;
	//Dx9
	LPDIRECT3D9 m_pD3d9;
	LPDIRECT3DDEVICE9 m_pDevice9;
	//Dx11
	ID3D11Device* m_pDevice11;
	ID3D11DeviceContext *m_pDeviceContext11;
	ID3D11InputLayout* m_pVertexLayout;
	ID3D11InputLayout* m_pVertexLayout_NoTex;
	ID3D11VertexShader* m_pVertexShader;
	ID3D11VertexShader* m_pVertexShaderNoTex;
	ID3D11PixelShader* m_pPixelShader;
	ID3D11PixelShader* m_pPixelShaderNoTex;
	ID3D11Buffer* m_pConstantBuffer0;
	ID3D11Buffer* m_pConstantBuffer1;
	ID3D11SamplerState* m_pSampleLinear;//テクスチャーのサンプラー

	//Method
	CD3DXMESH_ANIM();
	~CD3DXMESH_ANIM();
	HRESULT Init(HWND hWnd,ID3D11Device*,ID3D11DeviceContext*,LPSTR);
	HRESULT InitDx9();
	HRESULT InitShader();
	void Render(D3DXMATRIX& mView,D3DXMATRIX& mProj,D3DXVECTOR3& vLight,D3DXVECTOR3& vEye);
	HRESULT LoadXAnimMesh(LPSTR FileName);
	void BuildAllMesh(D3DXFRAME* pFrame);
	HRESULT CreateAppMeshFromD3DXMesh(LPD3DXFRAME pFrame);
	void DrawFrame(LPD3DXFRAME p);
	void DrawPartsMesh(PARTS_MESH* p,D3DXMATRIX World);
	void UpdateHierarchyMatrices(D3DXFRAME* p,LPD3DXMATRIX pParentMatrix);
};