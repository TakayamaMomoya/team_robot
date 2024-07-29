//*****************************************************
//
// 敵スポナー[enemySpawner.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "enemySpawner.h"
#include "player.h"
#include "effect3D.h"
#include "enemy.h"
#include "meshRoad.h"
#include "myLib.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const float MOVESPEED = 4.0f;	// 移動速度
const float WIDTH_GOAL = 100.0f;	// ゴールの幅
const float NEXT_INTERPOLATE_ROAD = 0.1f;	// 次の位置とみなす補正の値
}

//=====================================================
// コンストラクタ
//=====================================================
CEnemySpawner::CEnemySpawner(int nPriority)
{
	m_nTransitionTime = 0;		// 遷移カウンター
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置
	m_posStart = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ゴールの始点
	m_posEnd = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ゴールの終点
	m_fRot = 0.0f;		// 向き
	m_nIdxSpline = 0;	// スプラインの番号
	m_pObj3D = nullptr;		// オブジェクト3Dのポインタ
}

//=====================================================
// デストラクタ
//=====================================================
CEnemySpawner::~CEnemySpawner()
{

}

//=====================================================
// 生成
//=====================================================
CEnemySpawner* CEnemySpawner::Create(int nIdxSpline,float fRot, float fLength)
{
	CEnemySpawner *pSpawner = nullptr;

	pSpawner = new CEnemySpawner;

	if (pSpawner != nullptr)
	{
		pSpawner->m_nIdxSpline = nIdxSpline;
		pSpawner->m_fRot = fRot;
		pSpawner->m_fLength = fLength;

		pSpawner->SetPositionSpline();

		// 初期化
		pSpawner->Init();
	}

	return pSpawner;
}

//=====================================================
// 初期化
//=====================================================
HRESULT CEnemySpawner::Init()
{
#ifdef _DEBUG
	// テープの生成
	m_pObj3D = CPolygon3D::Create(m_pos);
#endif

	// ポリゴンの設定
	SetPolygon();

	return S_OK;
}

//=====================================================
// スプラインに対応した位置の設定
//=====================================================
void CEnemySpawner::SetPositionSpline(void)
{
	// メッシュロードの取得
	CMeshRoad *pMesh = CMeshRoad::GetInstance();

	if (pMesh == nullptr)
		return;

	// ロードポイントの位置の取得
	CCutMullSpline spline = *pMesh->GetCenterSpline();

	D3DXVECTOR3 pos;

	pos = spline.Interpolate(0.0f, m_nIdxSpline);

	SetPosition(pos);

	// 次回のベクトルとの差分で向きを決める
	D3DXVECTOR3 posNext = spline.Interpolate(NEXT_INTERPOLATE_ROAD, m_nIdxSpline);

	D3DXVECTOR3 vecDiff = posNext - pos;

	float fRot = atan2f(vecDiff.x, vecDiff.z);

	SetRotation(fRot - D3DX_PI * 0.5f);
}

//=====================================================
// ポリゴンの設定
//=====================================================
void CEnemySpawner::SetPolygon(void)
{
	// 始点・終点の計算
	D3DXVECTOR3 vec = { sinf(m_fRot) * m_fLength ,0.0f,cosf(m_fRot) * m_fLength };

	m_posStart = m_pos + vec;
	m_posEnd = m_pos - vec;

	if (m_pObj3D != nullptr)
	{
		// 色設定
		m_pObj3D->SetColor(D3DXCOLOR(1.0f, 0.5f, 0.0f, 0.5f));

		// サイズ設定
		m_pObj3D->SetSize(WIDTH_GOAL, m_fLength);

		m_pObj3D->SetRotation(D3DXVECTOR3(0.0f, m_fRot, 0.0f));
		m_pObj3D->SetPosition(m_pos);
	}
}

//=====================================================
// 終了
//=====================================================
void CEnemySpawner::Uninit()
{
	if (m_pObj3D != nullptr)
	{
		m_pObj3D->Uninit();
		m_pObj3D = nullptr;
	}

	// 自身の削除
	Release();
}

//=====================================================
// 更新
//=====================================================
void CEnemySpawner::Update()
{
	// プレイヤー情報取得
	CPlayer* pPlayer = CPlayer::GetInstance();

	// 位置取得
	D3DXVECTOR3 posPlayer = pPlayer->GetPosition();

	// 移動量取得
	D3DXVECTOR3 movePlayer = pPlayer->GetMove();

	// 交点の割合
	float fCross = 0.0f;

	// 外積の判定
	if (universal::IsCross(posPlayer,		// プレイヤーの位置
		m_posStart,		// ゴールの始点
		m_posEnd,			// ゴールの終点
		&fCross,		// 交点の割合
		posPlayer + movePlayer))	// プレイヤーの移動量
	{
		if (fCross >= 0.0f && fCross <= 1.0f)
		{// 始点と終点の間を通った時
			// 敵の生成
			CreateEnemy();

			// 自身の破棄
			Uninit();
		}
	}

#ifdef _DEBUG
	CEffect3D::Create(m_posStart, 200.0f, 3, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
	CEffect3D::Create(m_posEnd, 200.0f, 3, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
#endif

#if 0
	CDebugProc::GetInstance()->Print("\nfCrossの値[%f]", fCross);
	CDebugProc::GetInstance()->Print("\nstartPosの位置[%f, %f, %f]", m_posStart.x, m_posStart.y, m_posStart.z);
	CDebugProc::GetInstance()->Print("\nendPosの位置[%f, %f, %f]", m_posEnd.x, m_posEnd.y, m_posEnd.z);
#endif
}

//=====================================================
// 敵の生成
//=====================================================
void CEnemySpawner::CreateEnemy(void)
{
	CEnemy *pEnemy = CEnemy::Create();

	if (pEnemy == nullptr)
		return;

	pEnemy->SetIdxSpline(m_nIdxSpline - 1);
}

//=====================================================
// 描画
//=====================================================
void CEnemySpawner::Draw()
{

}