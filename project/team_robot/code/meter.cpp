//*****************************************************
//
// タイムの処理[timer.cpp]
// Author:森川駿弥
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "meter.h"
#include "debugproc.h"
#include "texture.h"
#include "UI.h"
#include "number.h"
#include "player.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const char* PATH_TEX_BACKMETER = "data\\TEXTURE\\UI\\meter00.png";	// メーター背景のテクスチャパス
const char* PATH_TEX_NEEDLE = "data\\TEXTURE\\UI\\Needle.png";	// 針のテクスチャパス
const int PLACE = 3;		// 桁数
const D3DXVECTOR2 SIZE_METER = { SCREEN_WIDTH * 0.13f, SCREEN_WIDTH * 0.13f };	// メーター背景のサイズ
const D3DXVECTOR3 POS_METER = { SCREEN_WIDTH - SIZE_METER.x - 10.0f, SCREEN_HEIGHT - SIZE_METER.y - 10.0f, 0.0f };	// メーターの位置
const D3DXVECTOR2 SIZE_NUMBER = { SCREEN_WIDTH * 0.015f, SCREEN_WIDTH * 0.02f };	// メーターの数字のサイズ
const D3DXVECTOR2 SIZE_NEEDLE = { SIZE_METER.x * 0.2f, SIZE_METER.y * 0.9f };	// 針のサイズ
const float MIN_ANGLE_NEEDLE = D3DX_PI * 0.7f;	// 針の最低速度時の角度
const float MAX_ANGLE_NEEDLE = -D3DX_PI * 0.7f;	// 針の最高速度時の角度
}

//=====================================================
// 静的メンバ変数
//=====================================================
CMeter* CMeter::m_pMeter = nullptr;

//=====================================================
// 優先順位を決めるコンストラクタ
//=====================================================
CMeter::CMeter(int nPriority)
{
	m_pNumber = nullptr;	// ナンバーのポインタ
	m_pNeedle = nullptr;		// 針のポインタ
	m_pBackMeter = nullptr;	// メーター背景のポインタ
}

//=====================================================
// デストラクタ
//=====================================================
CMeter::~CMeter()
{

}

//=====================================================
// 生成処理
//=====================================================
CMeter* CMeter::Create(void)
{
	if (m_pMeter == nullptr)
	{// インスタンス生成
		m_pMeter = new CMeter;

		if (m_pMeter != nullptr)
		{// 初期化
			m_pMeter->Init();
		}
	}

	return m_pMeter;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CMeter::Init()
{
	if (m_pBackMeter == nullptr)
	{
		m_pBackMeter = CUI::Create();

		// テクスチャ設定
		int nIdx = Texture::GetIdx(PATH_TEX_BACKMETER);
		m_pBackMeter->SetIdxTexture(nIdx);

		// トランスフォーム設定
		m_pBackMeter->SetPosition(POS_METER);
		m_pBackMeter->SetSize(SIZE_METER.x, SIZE_METER.y);
		m_pBackMeter->SetVtx();
	}

	if (m_pNeedle == nullptr)
	{// 針の生成
		// 生成
		m_pNeedle = CUI::Create();

		// テクスチャ設定
		int nIdx = Texture::GetIdx(PATH_TEX_NEEDLE);
		m_pNeedle->SetIdxTexture(nIdx);

		// トランスフォーム設定
		m_pNeedle->SetPosition(POS_METER);
		m_pNeedle->SetSize(SIZE_NEEDLE.x, SIZE_NEEDLE.y);
		m_pNeedle->SetVtx();
	}

	if (m_pNumber == nullptr)
	{// メーター値の生成
		// 生成
		m_pNumber = CNumber::Create(PLACE, 0);

		// トランスフォーム設定
		m_pNumber->SetPosition(D3DXVECTOR3(POS_METER.x - SIZE_NUMBER.x * 2, POS_METER.y - SIZE_NUMBER.y * 1.8f, 0.0f));
		m_pNumber->SetSizeAll(SIZE_NUMBER.x, SIZE_NUMBER.y);
	}

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CMeter::Uninit(void)
{
	if (m_pNumber != nullptr)
	{
		m_pNumber->Uninit();
		m_pNumber = nullptr;
	}

	if (m_pNeedle != nullptr)
	{
		m_pNeedle->Uninit();
		m_pNeedle = nullptr;
	}

	if (m_pBackMeter != nullptr)
	{
		m_pBackMeter->Uninit();
		m_pBackMeter = nullptr;
	}

	m_pMeter = nullptr;

	// 自身の破棄
	Release();
}

//=====================================================
// 更新処理
//=====================================================
void CMeter::Update(void)
{
	// メーター値
	Acceleration();

	// 針を動かす処理
	Needle();
}

//=====================================================
// メーター値処理
//=====================================================
void CMeter::Acceleration()
{
	CPlayer *pPlayer = CPlayer::GetInstance();

	if (pPlayer == nullptr)
		return;

	// 速度取得
	float fPlayerSpeed = pPlayer->GetSpeed();

	if (m_pNumber != nullptr)
	{// 値表示の制御
		m_pNumber->SetValue((int)fPlayerSpeed, PLACE);
	}
}

//=====================================================
// メーター針処理
//=====================================================
void CMeter::Needle()
{
	CPlayer *pPlayer = CPlayer::GetInstance();

	if (m_pNeedle == nullptr || pPlayer == nullptr)
		return;

	float fSpeed = pPlayer->GetSpeed();
	float fSpeedMax = pPlayer->GetParam().fSpeedMaxInitial;

	float fRate = fSpeed / fSpeedMax;
	universal::LimitValuefloat(&fRate, 1.0f, 0.0f);

	float fDiffAngle = MAX_ANGLE_NEEDLE - MIN_ANGLE_NEEDLE;

	float fRot = MIN_ANGLE_NEEDLE + fDiffAngle * fRate;

	m_pNeedle->SetRotation(fRot);
	m_pNeedle->SetVtx();
}