//*****************************************************
//
// ブラーイベントの処理[blurEvent.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "blurEvent.h"
#include "blur.h"
#include "manager.h"

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
CBlurEvent *CBlurEvent::s_pBlurEvent = nullptr;	// 自身のポインタ

//=====================================================
// コンストラクタ
//=====================================================
CBlurEvent::CBlurEvent() : m_fDensityBlur(0.0f), m_fSizeBlur(0.0f), m_fTimerBlur(0.0f), m_fTimerInitial(0.0f)
{

}

//=====================================================
// デストラクタ
//=====================================================
CBlurEvent::~CBlurEvent()
{

}

//=====================================================
// 生成処理
//=====================================================
CBlurEvent *CBlurEvent::Create(float fTime, float fDensity, float fSize)
{
	if (s_pBlurEvent == nullptr)
	{
		s_pBlurEvent = new CBlurEvent;

		if (s_pBlurEvent != nullptr)
		{
			s_pBlurEvent->m_fTimerBlur = fTime;
			s_pBlurEvent->m_fDensityBlur = fDensity;
			s_pBlurEvent->m_fSizeBlur = fSize;
			s_pBlurEvent->Init();
		}
	}

	return s_pBlurEvent;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CBlurEvent::Init(void)
{
	m_fTimerInitial = m_fTimerBlur;

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CBlurEvent::Uninit(void)
{
	s_pBlurEvent = nullptr;

	Release();
}

//=====================================================
// 更新処理
//=====================================================
void CBlurEvent::Update(void)
{
	m_fTimerBlur -= CManager::GetDeltaTime();

	if (m_fTimerBlur < 0.0f)
	{// イベントの終了とともに自身を破棄
		// イベントの終了
		Blur::ResetBlur();

		// 自身の終了
		Uninit();

		return;
	}
	else
	{
		// 時間の割合を計算
		float fRate = 1.0f - m_fTimerBlur / m_fTimerInitial;

		// sinカーブでの計算
		fRate = sinf(D3DX_PI * fRate);

		// ブラーの設定
		float fSize = m_fSizeBlur * fRate;
		float fDesnity = m_fDensityBlur * fRate;
		Blur::SetBlur(fSize, fDesnity);
	}
}

//=====================================================
// 描画処理
//=====================================================
void CBlurEvent::Draw(void)
{

}