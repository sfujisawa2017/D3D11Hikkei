#include "SOUND.h"

//
//
//
SOUND::SOUND()
{
	ZeroMemory(this,sizeof(SOUND));
}
//
//
//
SOUND::~SOUND()
{
	for(int i=0;i<MAX_WAV;i++)
	{
		if(m_pSourceVoice[i] ) m_pSourceVoice[i]->DestroyVoice();
	}
	for(int i=0;i<MAX_WAV;i++) SAFE_DELETE(m_pWavBuffer[i]);
	SAFE_RELEASE(m_pXAudio2);

	CoUninitialize();
}
//
//
//
HRESULT SOUND::Init()
{
	CoInitializeEx(NULL,COINIT_MULTITHREADED);	

	if(FAILED(XAudio2Create(&m_pXAudio2,0)))
	{
		CoUninitialize();
		return E_FAIL;
	}
	if( FAILED(m_pXAudio2->CreateMasteringVoice(&m_pMasteringVoice)))
	{
		CoUninitialize();
		return E_FAIL;
	}
	return S_OK;
}
//
//
//
int SOUND::LoadSound(char* szFileName)
{
	static int iIndex=-1;
	iIndex++;
	HMMIO hMmio=NULL;//WindowsマルチメディアAPIのハンドル(WindowsマルチメディアAPIはWAVファイル関係の操作用のAPI)
	DWORD dwWavSize=0;//WAVファイル内　WAVデータのサイズ（WAVファイルはWAVデータで占められているので、ほぼファイルサイズと同一）
	WAVEFORMATEX* pwfex;//WAVのフォーマット 例）16ビット、44110Hz、ステレオなど
	MMCKINFO ckInfo;//　チャンク情報
	MMCKINFO riffckInfo;// 最上部チャンク（RIFFチャンク）保存用
	PCMWAVEFORMAT pcmWaveForm;
	//WAVファイル内のヘッダー情報（音データ以外）の確認と読み込み
	hMmio=mmioOpenA(szFileName, NULL, MMIO_ALLOCBUF | MMIO_READ);
	//ファイルポインタをRIFFチャンクの先頭にセットする
	mmioDescend( hMmio, &riffckInfo, NULL, 0 );
	// ファイルポインタを'f' 'm' 't' ' ' チャンクにセットする
	ckInfo.ckid = mmioFOURCC('f', 'm', 't', ' ');
	mmioDescend( hMmio, &ckInfo, &riffckInfo, MMIO_FINDCHUNK );
	//フォーマットを読み込む
	mmioRead( hMmio, (HPSTR) &pcmWaveForm,sizeof(pcmWaveForm));
	pwfex = (WAVEFORMATEX*)new CHAR[ sizeof(WAVEFORMATEX) ];
	memcpy( pwfex, &pcmWaveForm, sizeof(pcmWaveForm) );
	pwfex->cbSize = 0;	
	mmioAscend( hMmio, &ckInfo, 0 );
	// WAVファイル内の音データの読み込み	
	ckInfo.ckid = mmioFOURCC('d', 'a', 't', 'a');
	mmioDescend( hMmio, &ckInfo, &riffckInfo, MMIO_FINDCHUNK );//データチャンクにセット
	dwWavSize = ckInfo.cksize;
	m_pWavBuffer[iIndex]=new BYTE[ dwWavSize ];
	DWORD dwOffset=ckInfo.dwDataOffset;
	mmioRead( hMmio, (HPSTR)m_pWavBuffer[iIndex],dwWavSize);
	//ソースボイスにデータを詰め込む	
	if(FAILED(m_pXAudio2->CreateSourceVoice(&m_pSourceVoice[iIndex],pwfex)))
	{
		MessageBox(0,L"ソースボイス作成失敗",0,MB_OK);
		return E_FAIL;
	}
	m_dwWavSize[iIndex]=dwWavSize;

	return iIndex;
}
//
//
//
void  SOUND::PlaySound(int iSoundIndex,bool boLoop)
{
	XAUDIO2_VOICE_STATE state;
	m_pSourceVoice[iSoundIndex]->GetState(&state);
	if(state.BuffersQueued>0)
	{
		return;
	}
	
	XAUDIO2_BUFFER buffer={0};
	buffer.pAudioData=m_pWavBuffer[iSoundIndex];
	buffer.Flags=XAUDIO2_END_OF_STREAM;
	buffer.AudioBytes=m_dwWavSize[iSoundIndex];
	if(boLoop)
	{
		buffer.LoopCount = XAUDIO2_LOOP_INFINITE;
	}
	if(FAILED(m_pSourceVoice[iSoundIndex]->SubmitSourceBuffer(&buffer)))
	{
		MessageBox(0,L"ソースボイスにサブミット失敗",0,MB_OK);
		return;
	}
	m_pSourceVoice[iSoundIndex]->Start(0,XAUDIO2_COMMIT_NOW);
}
//
//
//
void SOUND::StopSound(int iSoundIndex)
{
	m_pSourceVoice[iSoundIndex]->Stop(0,XAUDIO2_COMMIT_NOW);
}