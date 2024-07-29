//*****************************************************
//
// タイムの処理[timer.cpp]
// Author:森川駿弥
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "timer.h"
#include "manager.h"
#include "debugproc.h"
#include "goal.h"
#include "player.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const int MINUTES_LIMIT = 9;		// 分の上限値
const int SECOND_LIMIT = 59;		// 秒の上限値
const int MILLI_LIMIT = 99;			// ミリ秒の上限
const int MINUTES_DIGIT = 1;		// 分表示の桁数
const int TIME_DIGIT = 2;			// それぞれの桁数
const float MAGNIFICATION = 100.0f;	// 掛ける倍率
const float MINUTES_WIDTH = SCREEN_WIDTH * 0.44f;	// 分のX座標
const float SECOND_WIDTH = SCREEN_WIDTH * 0.5f;		// 秒のX座標
const float MILLI_WIDTH = SCREEN_WIDTH * 0.6f;		// ミリ秒のX座標
}

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
CTimer* CTimer::s_pTimer = nullptr;

//=====================================================
// 優先順位を決めるコンストラクタ
//=====================================================
CTimer::CTimer(int nPriority) : CObject(nPriority)
{
	m_fSecond = 0.0f;			// 現在の時間(秒)
	m_fMilli = 0.0f;			// 現在の時間(ミリ秒)
	m_IsStop = false;
	m_pMinutes = nullptr;		// 分表示のポインタ
	m_pSecond = nullptr;		// 秒表示用のポインタ
	m_pMilli = nullptr;			// ミリセカンド表示用のポインタ
}

//=====================================================
// デストラクタ
//=====================================================
CTimer::~CTimer()
{
}

//=====================================================
// 生成処理
//=====================================================
CTimer* CTimer::Create(void)
{
	if (s_pTimer == nullptr)
	{// インスタンス生成
		s_pTimer = new CTimer;
	}

	if (s_pTimer != nullptr)
	{// 初期化
		s_pTimer->Init();

		if (s_pTimer->m_pMinutes == nullptr)
		{// 分表示
			// 生成・位置、サイズ設定
			s_pTimer->m_pMinutes = CNumber::Create(MINUTES_DIGIT, MINUTES_LIMIT);
			s_pTimer->m_pMinutes->SetPosition(D3DXVECTOR3(MINUTES_WIDTH, 75.0f, 0.0f));
			s_pTimer->m_pMinutes->SetSizeAll(35.0f, 35.0f);
		}

		if (s_pTimer->m_pSecond == nullptr)
		{// 秒表示
			// 生成・位置、サイズ設定
			s_pTimer->m_pSecond = CNumber::Create(TIME_DIGIT, SECOND_LIMIT);
			s_pTimer->m_pSecond->SetPosition(D3DXVECTOR3(SECOND_WIDTH, 75.0f, 0.0f));
			s_pTimer->m_pSecond->SetSizeAll(35.0f, 35.0f);
		}

		if (s_pTimer->m_pMilli == nullptr)
		{// ミリ秒表示
			// 生成・位置、サイズ設定
			s_pTimer->m_pMilli = CNumber::Create(TIME_DIGIT, MILLI_LIMIT);
			s_pTimer->m_pMilli->SetPosition(D3DXVECTOR3(MILLI_WIDTH, 85.0f, 0.0f));
			s_pTimer->m_pMilli->SetSizeAll(25.0f,15.0f);
		}
	}

	return s_pTimer;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CTimer::Init(void)
{
	// 分・秒・ミリ秒の初期化
	m_fSecond = 0.0f;
	m_fMilli = 0.0f;

	m_IsStop = false;	// タイマー停止のフラグ

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CTimer::Uninit(void)
{
	// 静的メンバ変数の終了
	if (s_pTimer != nullptr)
		s_pTimer = nullptr;

	if (m_pMinutes != nullptr)
	{// 分の終了
		m_pMinutes->Uninit();
		m_pMinutes = nullptr;
	}

	if (m_pSecond != nullptr)
	{// 秒の終了
		m_pSecond->Uninit();
		m_pSecond = nullptr;
	}

	if (m_pMilli != nullptr)
	{// ミリ秒の終了
		m_pMilli->Uninit();
		m_pMilli = nullptr;
	}

	// 自身の破棄
	Release();
}

//=====================================================
// 更新処理
//=====================================================
void CTimer::Update(void)
{
	// 分の更新
	if (!m_IsStop)
	{
		Minutes();
	}
}

//=====================================================
// 分の更新
//=====================================================
void CTimer::Minutes()
{
	// 秒の更新
	Second();

	// 分の計算
	int nMinutes = (int)m_fSecond / SECOND_LIMIT;

	// 分の上限値超えないように
	if (nMinutes >= MINUTES_LIMIT)
		nMinutes = MINUTES_LIMIT;

	// 分表示
	if (m_pMinutes != nullptr)
		m_pMinutes->SetValue(nMinutes, MINUTES_DIGIT);
}

//=====================================================
// 秒の更新
//=====================================================
void CTimer::Second()
{
	// ミリ秒の更新
	MilliSecond();

	// デルタタイム取得
	float fDeltaTime = CManager::GetDeltaTime();

	// デルタタイム(リアル時間)加算
	m_fSecond += fDeltaTime;

	// 秒の計算
	int nSecond = (int)m_fSecond % SECOND_LIMIT;

	// 秒表示
	if (m_pSecond != nullptr)
		m_pSecond->SetValue(nSecond, TIME_DIGIT);
}

//=====================================================
// ミリ秒更新
//=====================================================
void CTimer::MilliSecond()
{
	// デルタタイム取得
	float fDeltaTime = CManager::GetDeltaTime();

	// デルタタイム(リアル時間)加算
	m_fMilli += fDeltaTime;

	// 秒を100倍
	m_fMilli = m_fSecond * MAGNIFICATION;

	// ミリ秒の計算
	int nMsecond = (int)m_fMilli % MILLI_LIMIT;

	// ミリ秒表示
	if (m_pMilli != nullptr)
		m_pMilli->SetValue(nMsecond, TIME_DIGIT);
}
