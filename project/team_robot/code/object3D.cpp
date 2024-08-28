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
#include "debugproc.h"

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
	D3DXMatrixIdentity(&m_mtxWorld);

	// 親マトリックスの初期化
	ResetMtxParent();

	D3DXQuaternionIdentity(&m_quat);

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
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	// 変数宣言
	D3DXMATRIX mtxRot, mtxTrans;

	// クォータニオン用のオフセット
	D3DXMATRIX mtxUp;
	universal::SetOffSet(&mtxUp, m_mtxWorld, D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	D3DXVECTOR3 vecUp = { mtxUp._41 - m_mtxWorld._41,mtxUp._42 - m_mtxWorld._42 ,mtxUp._43 - m_mtxWorld._43 };

	// ワールドマトリックス初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	D3DXVECTOR3 rot = GetRotation();

	// Y回転
	D3DXQUATERNION quatUp;
	D3DXQuaternionRotationAxis(
		&quatUp, &vecUp, rot.y);
	D3DXQuaternionNormalize(&quatUp, &quatUp); // クォータニオンの正規化
	D3DXQuaternionMultiply(&m_quat, &m_quat, &quatUp);	// クォータニオンの累積

	// クォータニオンを反映させる
	D3DXMatrixRotationQuaternion(
		&m_mtxWorld, &m_quat);	// クォータニオンでの回転

	// X回転
	D3DXMATRIX mtxRight;
	universal::SetOffSet(&mtxRight, m_mtxWorld, D3DXVECTOR3(1.0f, 0.0f, 0.0f));
	D3DXVECTOR3 vecRight = { mtxRight._41 - m_mtxWorld._41,mtxRight._42 - m_mtxWorld._42 ,mtxRight._43 - m_mtxWorld._43 };

	D3DXQUATERNION quatRight;
	D3DXQuaternionRotationAxis(
		&quatRight, &vecRight, rot.x);
	D3DXQuaternionNormalize(&quatRight, &quatRight); // クォータニオンの正規化
	D3DXQuaternionMultiply(&m_quat, &m_quat, &quatRight);	// クォータニオンの累積

	// クォータニオンを反映させる
	D3DXMatrixRotationQuaternion(
		&m_mtxWorld, &m_quat);	// クォータニオンでの回転

	// Z軸回転
	D3DXVECTOR3 vecFront = { m_mtxWorld._31, m_mtxWorld._32, m_mtxWorld._33 };
	D3DXQUATERNION quatFront;
	D3DXQuaternionRotationAxis(
		&quatFront, &vecFront, rot.z);	// クォータニオンを作成
	D3DXQuaternionNormalize(&quatFront, &quatFront); // クォータニオンの正規化
	D3DXQuaternionMultiply(&m_quat, &m_quat, &quatFront);	// クォータニオンの累積

	// クォータニオンを反映させる
	D3DXMatrixRotationQuaternion(
		&m_mtxWorld, &m_quat);	// クォータニオンでの回転

	// 位置を反映
	D3DXVECTOR3 pos = GetPosition();
	D3DXMatrixTranslation(&mtxTrans,
		pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// 親のマトリックスとかけ合わせる
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &m_mtxParent);

	// ワールドマトリックス設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	SetRotation(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
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