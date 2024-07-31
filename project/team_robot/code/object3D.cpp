//*****************************************************
//
// 3Dオブジェクト[object3D.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "object3D.h"
#include "renderer.h"

//=====================================================
// 優先順位を決めるコンストラクタ
//=====================================================
CObject3D::CObject3D(int nPriority) : CGameObject(nPriority), m_mtxWorld(), m_mtxParent()
{

}

//=====================================================
// デストラクタ
//=====================================================
CObject3D::~CObject3D()
{

}

//=====================================================
// 初期化
//=====================================================
HRESULT CObject3D::Init(void)
{
	// 親マトリックスの初期化
	ResetMtxParent();

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CObject3D::Uninit(void)
{
	CGameObject::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void CObject3D::Update(void)
{

}

//=====================================================
// 描画
//=====================================================
void CObject3D::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = Renderer::GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;

	// ワールドマトリックス初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 向きを反映
	D3DXVECTOR3 rot = GetRotation();
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXVECTOR3 pos = GetPosition();
	D3DXMatrixTranslation(&mtxTrans,
		pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// 親マトリックスをかけ合わせる
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &m_mtxParent);

	// ワールドマトリックス設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);
}

//=====================================================
// 親マトリックスのリセット
//=====================================================
void CObject3D::ResetMtxParent(void)
{
	D3DXMatrixIdentity(&m_mtxParent);
}

//=====================================================
// 前方ベクトルの取得
//=====================================================
D3DXVECTOR3 CObject3D::GetForward(void)
{
	return D3DXVECTOR3( m_mtxWorld._31, m_mtxWorld._32, m_mtxWorld._33 );
}

//=====================================================
// 上方ベクトルの取得
//=====================================================
D3DXVECTOR3 CObject3D::GetUp(void)
{
	return D3DXVECTOR3( m_mtxWorld._21, m_mtxWorld._22, m_mtxWorld._23 );
}

//=====================================================
// 右方ベクトルの取得
//=====================================================
D3DXVECTOR3 CObject3D::GetRight(void)
{
	return D3DXVECTOR3( m_mtxWorld._11, m_mtxWorld._12, m_mtxWorld._13 );
}