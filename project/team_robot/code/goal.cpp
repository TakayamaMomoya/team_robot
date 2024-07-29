//*****************************************************
//
// ゴール処理[goal.cpp]
// Author:森川駿弥
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "goal.h"
#include "universal.h"
#include "debugproc.h"
#include "effect3D.h"
#include "fade.h"
#include "game.h"
#include "timer.h"
#include "result.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const float MOVESPEED = 4.0f;	// 移動速度
const float WIDTH_GOAL = 100.0f;	// ゴールの幅
}

//*****************************************************
// 静的メンバ変数
//*****************************************************
CGoal *CGoal::m_pGoal = nullptr;	// 自身のポインタ

//=====================================================
// コンストラクタ
//=====================================================
CGoal::CGoal(int nPriority)
{
	m_nTransitionTime = 0;		// 遷移カウンター
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置
	m_posStart = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ゴールの始点
	m_posEnd = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ゴールの終点
	m_fRot = 0.0f;		// 向き
	m_pObj3D = nullptr;		// オブジェクト3Dのポインタ
}

//=====================================================
// デストラクタ
//=====================================================
CGoal::~CGoal()
{
}

//=====================================================
// 生成
//=====================================================
CGoal* CGoal::Create(D3DXVECTOR3 pos,float fRot, float fLength)
{
	if (m_pGoal != nullptr)
		return m_pGoal;

	m_pGoal = new CGoal;

	if (m_pGoal != nullptr)
	{
		m_pGoal->m_pos = pos;
		m_pGoal->m_fRot = fRot;
		m_pGoal->m_fLength = fLength;

		// 初期化
		m_pGoal->Init();
	}

	return m_pGoal;
}

//=====================================================
// 初期化
//=====================================================
HRESULT CGoal::Init()
{
	// ゴールテープの生成
	m_pObj3D = CPolygon3D::Create(m_pos);

	// ゴールの設定
	SetGoal();

	return S_OK;
}

//=====================================================
// ゴールの設定
//=====================================================
void CGoal::SetGoal(void)
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
void CGoal::Uninit()
{
	m_pGoal = nullptr;

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
void CGoal::Update()
{
	// プレイヤー情報取得
	CPlayer* pPlayer = CPlayer::GetInstance();

	// フェード情報取得
	CFade* pFade = CFade::GetInstance();

	// 位置取得
	D3DXVECTOR3 posPlayer = pPlayer->GetPosition();

	// 移動量取得
	D3DXVECTOR3 movePlayer = pPlayer->GetMove();

	// タイマーの取得
	CTimer* pTimer = CTimer::GetInstance();

	// 交点の割合
	float fCross = 0.0f;

	bool bHit = universal::IsCross(posPlayer,		// プレイヤーの位置
		m_posStart,		// ゴールの始点
		m_posEnd,			// ゴールの終点
		&fCross,		// 交点の割合
		posPlayer + movePlayer);	// プレイヤーの移動量

	bool bHitNext = universal::IsCross(posPlayer + movePlayer,		// プレイヤーの次回の位置
		m_posStart,		// ゴールの始点
		m_posEnd,			// ゴールの終点
		nullptr,		// 交点の割合
		posPlayer + movePlayer);	// プレイヤーの移動量

	// 外積の判定
	if (!bHit && bHitNext)
	{
		if (fCross >= 0.0f && fCross <= 1.0f)
		{// 始点と終点の間を通った時
			// リザルトの生成
			CResult::Create();
		}
	}

#ifdef _DEBUG
	CEffect3D::Create(m_posStart, 400.0f, 3, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
	CEffect3D::Create(m_posEnd, 200.0f, 3, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
#endif

#if 0
	CDebugProc::GetInstance()->Print("\nfCrossの値[%f]", fCross);
	CDebugProc::GetInstance()->Print("\nbHit[%d]", bHit);
	CDebugProc::GetInstance()->Print("\nbHitOld[%d]", bHitNext);
	//CDebugProc::GetInstance()->Print("\nstartPosの位置[%f, %f, %f]", m_posStart.x, m_posStart.y, m_posStart.z);
	//CDebugProc::GetInstance()->Print("\nendPosの位置[%f, %f, %f]", m_posEnd.x, m_posEnd.y, m_posEnd.z);
#endif
}

//=====================================================
// 描画
//=====================================================
void CGoal::Draw()
{

}
