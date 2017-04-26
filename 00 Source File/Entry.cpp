#include <windows.h>
#include "DIRECTOR.h"

INT WINAPI WinMain(HINSTANCE hInstance,HINSTANCE,LPSTR,INT)
{
	DIRECTOR* pDirector=new DIRECTOR;
	if(pDirector==NULL)
	{
		MessageBox(0,L"クラス生成失敗　アプリを終了します",NULL,MB_OK);
		return 0;
	}
	//進行はディレクターに
	pDirector->Run(hInstance);

	//アプリ終了
	delete pDirector;
	
	return 0;
}