//*****************************************************
//
// リザルト処理[result.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "result.h"
#include "player.h"
#include "camera.h"
#include "manager.h"
#include "cameraState.h"
#include "inputManager.h"
#include "game.h"
#include "number.h"
#include "timer.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const D3DXVECTOR3 POS_DISP_TIME = { SCREEN_WIDTH * 0.5f,SCREEN_HEIGHT * 0.4f, 0.0f };	// タイムの表示位置
}

//=====================================================
// コンストラクタ
//=====================================================
CResult::CResult() : CObject(1), m_pState(nullptr)
{

}

//=====================================================
// デストラクタ
//=====================================================
CResult::~CResult()
{

}

//=====================================================
// 生成処理
//=====================================================
CResult *CResult::Create(void)
{
	CResult *pResult = nullptr;

	pResult = new CResult;

	if (pResult != nullptr)
	{
		pResult->Init();
	}

	return pResult;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CResult::Init(void)
{
	// プレイヤーの設定
	SetPlayer();

	// カメラ位置の設定
	SetCamera();

	// ステイトの変更
	ChangeState(new CStateResultDispTime);

	return S_OK;
}

//=====================================================
// プレイヤー設定
//=====================================================
void CResult::SetPlayer(void)
{
	CPlayer *pPlayer = CPlayer::GetInstance();

	if (pPlayer == nullptr)
		return;
	
	// プレイヤーを操作できないようにする
	pPlayer->SetEnableInput(false);
}

//=====================================================
// カメラ設定
//=====================================================
void CResult::SetCamera(void)
{
	CCamera *pCamera = CManager::GetCamera();

	if (pCamera == nullptr)
		return;
	
	pCamera->ChangeState(new CCameraStateResult);
}

//=====================================================
// 終了処理
//=====================================================
void CResult::Uninit(void)
{
	Release();
}

//=====================================================
// 更新処理
//=====================================================
void CResult::Update(void)
{
	if (m_pState != nullptr)
	{
		m_pState->Update(this);
	}
}

//=====================================================
// 描画処理
//=====================================================
void CResult::Draw(void)
{

}

//=====================================================
// ステイトの変更
//=====================================================
void CResult::ChangeState(CStateResult *pState)
{
	if (m_pState != nullptr)
	{
		m_pState->Uninit(this);
		delete m_pState;
		m_pState = nullptr;
	}

	m_pState = pState;

	if (m_pState != nullptr)
	{
		m_pState->Init(this);
	}
}

//=====================================================
// フェードを始める処理
//=====================================================
void CResult::StartFade(void)
{
	CGame *pGame = CGame::GetInstance();

	if (pGame == nullptr)
		return;

	pGame->SetState(CGame::STATE::STATE_END);
}

//=====================================================
// ステイトの終了
//=====================================================
void CStateResult::Uninit(CResult *pResult)
{

}

//********************************************************************************
// タイム表示
//********************************************************************************
//=====================================================
// コンストラクタ
//=====================================================
CStateResultDispTime::CStateResultDispTime() : m_apTime()
{

}

//=====================================================
// デストラクタ
//=====================================================
CStateResultDispTime::~CStateResultDispTime()
{

}

//=====================================================
// 初期化
//=====================================================
void CStateResultDispTime::Init(CResult *pResult)
{
	// 数字の設定
	SetNumber();
}

//=====================================================
// 数字の設定
//=====================================================
void CStateResultDispTime::SetNumber(void)
{
	CTimer *pTime = CTimer::GetInstance();

	if (pTime == nullptr)
		return;

	int aTime[NUMBER_MAX] =
	{
		pTime->GetMinutes(),
		(int)pTime->GetSecond(),
		(int)pTime->GetMilli()
	};

	for (int i = 0; i < NUMBER_MAX; i++)
	{
		m_apTime[i] = CNumber::Create(2, aTime[i]);


	}
}

//=====================================================
// 終了
//=====================================================
void CStateResultDispTime::Uninit(CResult *pResult)
{
	CStateResult::Uninit(pResult);
}

//=====================================================
// 更新
//=====================================================
void CStateResultDispTime::Update(CResult *pResult)
{
	CInputManager *pInputManager = CInputManager::GetInstance();

	if (pInputManager == nullptr)
		return;

	// フェードを始める
	if (pInputManager->GetTrigger(CInputManager::BUTTON_ENTER))
		pResult->StartFade();
}