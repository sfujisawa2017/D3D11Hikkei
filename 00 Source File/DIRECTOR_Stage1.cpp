#include "DIRECTOR.h"

//
//
//
void DIRECTOR::Stage1()
{
	//サンプルメッシュ　表示
	D3DXMATRIX View,Proj;
	// ビュートランスフォーム
	D3DXVECTOR3 Eye( 0.0f, 0.0f,-15.0f ); //視点位置
	D3DXVECTOR3 Lookat( 0.0f, 0.0f, 0.0f );//注視位置
	D3DXVECTOR3 Up( 0.0f, 1.0f, 0.0f );//上方位置
	D3DXMatrixLookAtLH( &View, &Eye, &Lookat, &Up );
	// プロジェクショントランスフォーム
	D3DXMatrixPerspectiveFovLH( &Proj, D3DX_PI/4, (FLOAT)WINDOW_WIDTH/(FLOAT)WINDOW_HEIGHT, 0.1f, 100.0f );

	//Static mesh
	m_pStaticMesh->Render(View,Proj,D3DXVECTOR3(1,1,-1),Eye);
	
	//Hierarchy Mesh
	m_pHierarchyMesh->Render(View,Proj,D3DXVECTOR3(1,1,-1),D3DXVECTOR3(0,0,-1));
	m_pHierarchyMesh->m_pAnimController->AdvanceTime(0.01,NULL);

	//Skinned Mesh
	m_pSkinMesh->m_View=View;
	m_pSkinMesh->m_Eye=Eye;
	m_pSkinMesh->m_Proj=Proj;

	m_pSkinMesh->Render();

	//サンプルサウンド再生
	m_pSound->PlaySound(0,false);
}