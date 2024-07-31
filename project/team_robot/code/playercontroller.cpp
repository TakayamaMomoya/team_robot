//*****************************************************
//
// プレイヤーコントローラー[playercontroller.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "playercontroller.h"
#include "inputManager.h"
#include "player.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const float SPEED_MOVE = 5.0f;	// スピード
const float SPEED_YAW = D3DX_PI * 0.01f;	// ヨー速度
const float SPEED_PITCH = D3DX_PI * 0.01f;	// ピッチ速度
const float SPEED_QUICKBOOST = 300.0f;	// クイックブースト速度
}

//=====================================================
// 優先順位を決めるコンストラクタ
//=====================================================
CPlayerController::CPlayerController()
{

}

//=====================================================
// デストラクタ
//=====================================================
CPlayerController::~CPlayerController()
{

}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CPlayerController::Init(void)
{
	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CPlayerController::Uninit(void)
{
	Release();
}

//=====================================================
// 更新処理
//=====================================================
void CPlayerController::Update(void)
{
	// 移動の入力
	InputMove();
}

//=====================================================
// 移動の入力
//=====================================================
void CPlayerController::InputMove(void)
{
	CPlayer *pPlayer = CPlayer::GetInstance();
	CInputManager *pInputManager = CInputManager::GetInstance();

	if (pPlayer == nullptr || pInputManager == nullptr)
		return;

	CInputManager::SAxis axis = pInputManager->GetAxis();

	// 水平移動の加速量
	D3DXVec3Normalize(&axis.axisMove, &axis.axisMove);

	D3DXVECTOR3 vecForward = pPlayer->GetForward();
	D3DXVECTOR3 vecRight = pPlayer->GetRight();
	D3DXVECTOR3 vecUp = pPlayer->GetUp();

	D3DXVECTOR3 move = pPlayer->GetMove();
	move -= vecForward * axis.axisMove.z * SPEED_MOVE;
	move -= vecRight * axis.axisMove.x *  SPEED_MOVE;

	// 上下移動の操作
	if(pInputManager->GetPress(CInputManager::BUTTON::BUTTON_UP))
		move += vecUp * SPEED_MOVE;

	if (pInputManager->GetPress(CInputManager::BUTTON::BUTTON_DOWN))
		move -= vecUp * SPEED_MOVE;

	// クイックブーストの操作
	if (pInputManager->GetTrigger(CInputManager::BUTTON::BUTTON_QUICKBOOST))
	{
		move -= vecForward * axis.axisMove.z * SPEED_QUICKBOOST;
		move -= vecRight * axis.axisMove.x * SPEED_QUICKBOOST;
	}

	// 移動量の反映
	pPlayer->SetMove(move);

	// ヨー・ピッチの操作
	D3DXVec3Normalize(&axis.axisCamera, &axis.axisCamera);

	D3DXVECTOR3 rot = { 0.0f,0.0f,0.0f };

	rot.x += axis.axisCamera.y * SPEED_PITCH;
	rot.y += axis.axisCamera.x * SPEED_YAW;

	pPlayer->SetRotation(rot);
}