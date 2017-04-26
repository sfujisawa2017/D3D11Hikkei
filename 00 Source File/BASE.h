#pragma once
#include <stdio.h>
#include <windows.h>
#include <d3d11.h>
#include <d3dx10.h>
#include <d3dx11.h>
#include <d3dCompiler.h>
//必要なライブラリファイルのロード
#pragma comment(lib,"d3dx10.lib")
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dx11.lib")
#pragma comment(lib,"d3dCompiler.lib")
#pragma comment(lib,"winmm.lib")
//警告非表示
#pragma warning(disable : 4305)
#pragma warning(disable : 4996)
#pragma warning(disable : 4018)
#pragma warning(disable : 4111)
//
//
#define APP_NAME L"空ゲームアプリ　メッシュ、サウンド機能付き"
#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480
//
//
#define SAFE_DELETE(p) { if(p) { delete (p); (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p); (p)=NULL; } }
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }
#define MFAIL(code,string) if(FAILED( code ) ) { MessageBox(0,string,L"error",MB_OK); return E_FAIL; }
#define MFALSE(code,string) if(!( code ) ) { MessageBox(0,string,L"error",MB_OK); return E_FAIL; }
#define MSG(t) MessageBox(0,t,0,MB_OK);
//
//
class CELEMENT
{
};

//
//プロトタイプ
void InitDirectory(WCHAR* root);
void SetRootDirectory();
void SetDataDirectory();
void SetVisualDirectory();
void SetShaderDirectory();
void SetSoundDirectory();