#pragma once
#define _WIN32_DCOM
#include <stdio.h>
#include <windows.h>
#include <mmsystem.h>
#include <xaudio2.h>
#include "BASE.h"
#include "WINDOW.h"

#pragma comment(lib,"winmm.lib")

#define MAX_WAV 110 //WAVサウンド最大数

//
//
//
class SOUND : public CELEMENT
{
public:
	HWND m_hWnd;
	IXAudio2* m_pXAudio2;
	IXAudio2MasteringVoice* m_pMasteringVoice;
	IXAudio2SourceVoice* m_pSourceVoice[MAX_WAV];
	BYTE* m_pWavBuffer[MAX_WAV];//波形データ（フォーマット等を含まない、純粋に波形データのみ）
	DWORD m_dwWavSize[MAX_WAV];//波形データのサイズ
	int m_iSoundIndex[11];//今回読み込む音の番号
	int m_iSoundCurrentIndex;//音の現在の再生番号

	SOUND();
	~SOUND();
	HRESULT Init();
	int LoadSound(char* szFileName);
	void  PlaySound(int iSoundIndex,bool boLoop);
	void StopSound(int iSoundIndex);
};