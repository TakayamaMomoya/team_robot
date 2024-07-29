//*****************************************************
//
// チュートリアル処理[tutorial.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "tutorial.h"
#include "meshRoad.h"
#include "UI.h"
#include "timer.h"
#include "inputManager.h"
#include "player.h"
#include "debugproc.h"
#include "manager.h"
#include "texture.h"
#include "effect3D.h"
#include "camera.h"
#include "fade.h"
#include "blurEvent.h"
#include "MyEffekseer.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const char* PATH_ROAD = "data\\MAP\\road01.bin";	// チュートリアルメッシュロードのパス
const D3DXVECTOR3 POS_DEFAULT_UI = { SCREEN_WIDTH * 0.7f, SCREEN_HEIGHT * 0.5f, 0.0f };	// UIのデフォルト位置
const D3DXVECTOR2 SIZE_DEFAULT_UI = { SCREEN_WIDTH * 0.1f, SCREEN_HEIGHT * 0.1f};	// UIのデフォルトサイズ
const float LINE_INPUT = 0.3f;	// 入力と判定するスピード
const float TIME_ACCELE = 5.0f;	// アクセルに必要な時間
const float TIME_BRAKE = 3.0f;	// ブレーキに必要な時間
const D3DXVECTOR2 SIZE_GATE = { 3000.0f,3000.0f };	// ゲートのサイズ
const float DIST_GATE = 3000.0f;	// ゲートの距離
const D3DXVECTOR3 ROT_GATE = { D3DX_PI * 0.5f, D3DX_PI * 0.5f, 0.0f };	// ゲートの向き
const float SPEED_RATE_PLAYER = 0.8f;	// プレイヤーがゲートに入るときのスピードの割合
const float SPEED_EXPAND_GATE = 0.2f;	// ゲートの拡張速度
const float SIZE_GATE_EFFECT = 1000.0f;	// ゲートエフェクトの目標サイズ
const float HEIGTH_GATE = 500.0f;	// ゲートの高さ
const D3DXVECTOR3 POS_PLAYER_INITIAL = { -11409.0f,0.0f,26395.0f };	// チュートリアルのプレイヤー初期位置
const D3DXVECTOR3 ROT_PLAYER_INITIAL = { 0.0f,1.77f,0.0f };	// チュートリアルのプレイヤー初期向き
}

//=====================================================
// コンストラクタ
//=====================================================
CTutorial::CTutorial() : CObject(1), m_pState(nullptr)
{

}

//=====================================================
// デストラクタ
//=====================================================
CTutorial::~CTutorial()
{

}

//=====================================================
// 生成処理
//=====================================================
CTutorial *CTutorial::Create(void)
{
	CTutorial *pTutorial = nullptr;

	pTutorial = new CTutorial;

	if (pTutorial != nullptr)
	{
		pTutorial->Init();
	}

	return pTutorial;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CTutorial::Init(void)
{
	CMeshRoad::Create(PATH_ROAD);

	// プレイヤーをチュートリアルマップにテレポート
	CPlayer *pPlayer = CPlayer::GetInstance();

	if (pPlayer != nullptr)
	{
		pPlayer->SetPosition(POS_PLAYER_INITIAL);
		pPlayer->SetRotation(ROT_PLAYER_INITIAL);
		pPlayer->MultiplyMtx(false);

		CCamera *pCamera = CManager::GetCamera();

		if (pCamera != nullptr)
		{
			pCamera->SkipToDest();
		}
	}

	// 初期ステイトに設定
	ChangeState(new CStateTutorialMove);

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CTutorial::Uninit(void)
{
	if (m_pState != nullptr)
	{
		m_pState->Uninit(this);
		m_pState = nullptr;
	}

	Release();
}

//=====================================================
// 更新処理
//=====================================================
void CTutorial::Update(void)
{
	if (m_pState != nullptr)
	{
		m_pState->Update(this);
	}
}

//=====================================================
// 描画処理
//=====================================================
void CTutorial::Draw(void)
{

}

//=====================================================
// ゲームの開始
//=====================================================
void CTutorial::StartGame(void)
{
	// タイマーの生成
	CTimer::Create();

	// プレイヤーのトランスフォームを設定
	CPlayer *pPlayer = CPlayer::GetInstance();

	if (pPlayer == nullptr)
		return;

	pPlayer->SetPosition(Player::DEFAULT_POS);
	pPlayer->SetRotation(Player::DEFAULT_ROT);

	pPlayer->MultiplyMtx(false);

	// カメラの位置を設定
	CCamera *pCamera = CManager::GetCamera();

	if (pCamera == nullptr)
		return;

	pCamera->SkipToDest();	// 目標位置までカメラを飛ばす
}

//=====================================================
// ステイトの変更
//=====================================================
void CTutorial::ChangeState(CStateResult *pState)
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
// 制限値の追加
//=====================================================
void CTutorial::AddLimit(int nIdx, float fValue)
{
	m_mapLimit[nIdx] = fValue;
}

//=====================================================
// ステイトの終了
//=====================================================
void CStateResult::Uninit(CTutorial *pTutorial)
{
	// UIのマップコンテナクリア
	std::map<int, CUI*> *pMapUI = pTutorial->GetMap();

	for (auto it : *pMapUI)
	{
		it.second->Uninit();
	}

	pMapUI->clear();

	// カウンターのマップコンテナクリア
	std::map<int, float> mapFloat;
	mapFloat.clear();
	pTutorial->SetMapCounter(mapFloat);
}

//=====================================================
// 終了判定
//=====================================================
bool CStateResult::IsEndInput(int nNum, CTutorial *pTutorial)
{
	std::map<int, float> mapCounter = pTutorial->GetMapCounter();
	std::map<int, float> mapLimit = pTutorial->GetMapLimit();

	for (int i = 0; i < nNum; i++)
	{
		if (mapLimit[i] > mapCounter[i])
		{
			return false;
		}
	}

	return true;
}

//********************************************************************************
// 移動チュートリアル
//********************************************************************************
//=====================================================
// コンストラクタ
//=====================================================
CStateTutorialMove::CStateTutorialMove()
{

}

//=====================================================
// デストラクタ
//=====================================================
CStateTutorialMove::~CStateTutorialMove()
{

}

//=====================================================
// 初期化
//=====================================================
void CStateTutorialMove::Init(CTutorial *pTutorial)
{
	std::map<int, CUI*> *pMapUI = pTutorial->GetMap();
	std::map<int, CUI*> mapUI = *pMapUI;

	// パスの一覧
	const char* apPath[MENU_MAX] =
	{
		"data\\TEXTURE\\UI\\tutorial00.png",
		"data\\TEXTURE\\UI\\tutorial01.png"
	};

	// 制限値の一覧
	float aTime[MENU_MAX] =
	{
		TIME_ACCELE,
		TIME_BRAKE
	};

	for (int i = 0; i < MENU_MAX; i++)
	{
		// 各UIの設定
		CUI *pUI = Tutorial::CreateUIDefault();

		if (pUI == nullptr)
			continue;

		int nIdx = Texture::GetIdx(apPath[i]);
		pUI->SetIdxTexture(nIdx);
		mapUI[i] = pUI;
		
		D3DXVECTOR3 posUI = pUI->GetPosition();
		posUI.y = POS_DEFAULT_UI.y + SIZE_DEFAULT_UI.y * i * 2;
		pUI->SetPosition(posUI);
		pUI->SetVtx();

		// 制限値の設定
		pTutorial->AddLimit(i, aTime[i]);
	}

	*pMapUI = mapUI;
}

//=====================================================
// 終了
//=====================================================
void CStateTutorialMove::Uninit(CTutorial *pTutorial)
{
	CStateResult::Uninit(pTutorial);
}

//=====================================================
// 更新
//=====================================================
void CStateTutorialMove::Update(CTutorial *pTutorial)
{
	CInputManager *pInputManager = CInputManager::GetInstance();
	CPlayer *pPlayer = CPlayer::GetInstance();

	if (pInputManager == nullptr || pPlayer == nullptr)
		return;

	std::map<int, float> mapCounter = pTutorial->GetMapCounter();
	float fDeltaTime = CManager::GetDeltaTime();

	// アクセルの判定
	float fAccele = pInputManager->GetAccele();
	
	if (fAccele > LINE_INPUT)
	{// アクセルのカウンターを加算
		mapCounter[MENU_ACCELE] += fDeltaTime;
	}

	// ブレーキの判定
	float fBrake = pInputManager->GetBrake();

	if (fBrake > LINE_INPUT)
	{// ブレーキのカウンターを加算
		mapCounter[MENU_BRAKE] += fDeltaTime;
	}

	pTutorial->SetMapCounter(mapCounter);

	CDebugProc::GetInstance()->Print("\nアクセルカウンター[%f]ブレーキカウンター[%f]", mapCounter[MENU_ACCELE], mapCounter[MENU_BRAKE]);
	
	if (IsEndInput(MENU_MAX,pTutorial))
	{// 移動チュートリアルを終了
		pTutorial->ChangeState(new CStateTutorialDrift);
	}
}

//********************************************************************************
// ドリフトチュートリアル
//********************************************************************************
//=====================================================
// コンストラクタ
//=====================================================
CStateTutorialDrift::CStateTutorialDrift()
{

}

//=====================================================
// デストラクタ
//=====================================================
CStateTutorialDrift::~CStateTutorialDrift()
{

}

//=====================================================
// 初期化
//=====================================================
void CStateTutorialDrift::Init(CTutorial *pTutorial)
{
	std::map<int, CUI*> *pMapUI = pTutorial->GetMap();
	std::map<int, CUI*> mapUI = *pMapUI;

	// パスの一覧
	const char* apPath[MENU_MAX] =
	{
		"data\\TEXTURE\\UI\\tutorial02.png",
	};

	// 制限値の一覧
	float aTime[MENU_MAX] =
	{
		TIME_ACCELE,
	};

	for (int i = 0; i < MENU_MAX; i++)
	{
		// 各UIの設定
		CUI *pUI = Tutorial::CreateUIDefault();

		if (pUI == nullptr)
			continue;

		int nIdx = Texture::GetIdx(apPath[i]);
		pUI->SetIdxTexture(nIdx);
		mapUI[i] = pUI;

		D3DXVECTOR3 posUI = pUI->GetPosition();
		posUI.y = POS_DEFAULT_UI.y + SIZE_DEFAULT_UI.y * i * 2;
		pUI->SetPosition(posUI);
		pUI->SetVtx();

		// 制限値の設定
		pTutorial->AddLimit(i, aTime[i]);
	}

	*pMapUI = mapUI;
}

//=====================================================
// 終了
//=====================================================
void CStateTutorialDrift::Uninit(CTutorial *pTutorial)
{
	CStateResult::Uninit(pTutorial);
}

//=====================================================
// 更新
//=====================================================
void CStateTutorialDrift::Update(CTutorial *pTutorial)
{
	CPlayer *pPlayer = CPlayer::GetInstance();

	if (pPlayer == nullptr)
		return;

	std::map<int, float> mapCounter = pTutorial->GetMapCounter();
	float fDeltaTime = CManager::GetDeltaTime();

	// ドリフトの判定
	bool bDrift = pPlayer->IsDrift();

	if (bDrift)
	{// ドリフトのカウンターを加算
		mapCounter[MENU_DRIFT] += fDeltaTime;
	}

	pTutorial->SetMapCounter(mapCounter);

	CDebugProc::GetInstance()->Print("\nドリフトカウンター[%f]", mapCounter[MENU_DRIFT]);

	if (IsEndInput(MENU_MAX, pTutorial))
	{// ドリフトチュートリアルを終了
		pTutorial->ChangeState(new CStateTutorialParry);
	}
}

//********************************************************************************
// パリィチュートリアル
//********************************************************************************
//=====================================================
// コンストラクタ
//=====================================================
CStateTutorialParry::CStateTutorialParry()
{

}

//=====================================================
// デストラクタ
//=====================================================
CStateTutorialParry::~CStateTutorialParry()
{

}

//=====================================================
// 初期化
//=====================================================
void CStateTutorialParry::Init(CTutorial *pTutorial)
{
	std::map<int, CUI*> *pMapUI = pTutorial->GetMap();
	std::map<int, CUI*> mapUI = *pMapUI;

	// パスの一覧
	const char* apPath[MENU_MAX] =
	{
		"data\\TEXTURE\\UI\\tutorial03.png",
	};

	// 制限値の一覧
	float aTime[MENU_MAX] =
	{
		TIME_ACCELE,
	};

	for (int i = 0; i < MENU_MAX; i++)
	{
		// 各UIの設定
		CUI *pUI = Tutorial::CreateUIDefault();

		if (pUI == nullptr)
			continue;

		int nIdx = Texture::GetIdx(apPath[i]);
		pUI->SetIdxTexture(nIdx);
		mapUI[i] = pUI;

		D3DXVECTOR3 posUI = pUI->GetPosition();
		posUI.y = POS_DEFAULT_UI.y + SIZE_DEFAULT_UI.y * i * 2;
		pUI->SetPosition(posUI);
		pUI->SetVtx();

		// 制限値の設定
		pTutorial->AddLimit(i, aTime[i]);
	}

	*pMapUI = mapUI;
}

//=====================================================
// 終了
//=====================================================
void CStateTutorialParry::Uninit(CTutorial *pTutorial)
{
	CStateResult::Uninit(pTutorial);
}

//=====================================================
// 更新
//=====================================================
void CStateTutorialParry::Update(CTutorial *pTutorial)
{
	CInputManager *pInputManager = CInputManager::GetInstance();
	CPlayer *pPlayer = CPlayer::GetInstance();

	if (pInputManager == nullptr || pPlayer == nullptr)
		return;

	std::map<int, float> mapCounter = pTutorial->GetMapCounter();
	float fDeltaTime = CManager::GetDeltaTime();

	// パリィの判定
	int nMotion = pPlayer->GetMotion();

	if (pInputManager->GetTrigger(CInputManager::BUTTON_KATANA) && (nMotion != CPlayer::MOTION_NINJA::MOTION_NINJA_SLASHDOWN || nMotion != CPlayer::MOTION_NINJA::MOTION_NINJA_SLASHUP))
	{// パリィのカウンターを加算
		mapCounter[MENU_PARRY] += 1.0f;
	}

	pTutorial->SetMapCounter(mapCounter);

	CDebugProc::GetInstance()->Print("\nパリィカウンター[%f]", mapCounter[MENU_PARRY]);

	if (IsEndInput(MENU_MAX, pTutorial))
	{// パリィチュートリアルを終了
		pTutorial->ChangeState(new CStateTutorialEnd);
	}
}

//********************************************************************************
// フリーチュートリアル
//********************************************************************************
//=====================================================
// コンストラクタ
//=====================================================
CStateTutorialEnd::CStateTutorialEnd() : m_pEffect(nullptr)
{

}

//=====================================================
// デストラクタ
//=====================================================
CStateTutorialEnd::~CStateTutorialEnd()
{

}

//=====================================================
// 初期化
//=====================================================
void CStateTutorialEnd::Init(CTutorial *pTutorial)
{
	CPlayer *pPlayer = CPlayer::GetInstance();

	if (pPlayer == nullptr)
		return;

	D3DXVECTOR3 posPlayer = pPlayer->GetPosition();
	D3DXVECTOR3 posGate = posPlayer + pPlayer->GetForward() * DIST_GATE;
	posGate.y += HEIGTH_GATE;

	float fAngle = atan2f(pPlayer->GetForward().x, pPlayer->GetForward().z);	// プレイヤーの方を向かせる

	m_pEffect = MyEffekseer::CreateEffect(CEffekseer::TYPE::TYPE_GATE00, posGate, D3DXVECTOR3(0.0f, fAngle, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	// プレイヤーを操作不可にする
	pPlayer->SetEnableInput(false);
}

//=====================================================
// 終了
//=====================================================
void CStateTutorialEnd::Uninit(CTutorial *pTutorial)
{
	if (m_pEffect != nullptr)
	{
		m_pEffect->Uninit();
		m_pEffect = nullptr;
	}

	CStateResult::Uninit(pTutorial);
}

//=====================================================
// 更新
//=====================================================
void CStateTutorialEnd::Update(CTutorial *pTutorial)
{
	CPlayer *pPlayer = CPlayer::GetInstance();

	if (pPlayer == nullptr)
		return;

	// ゲートのスケーリングの補正
	ScalingGate();

	// プレイヤーを直進させる処理
	ForwardPlayer();

	// ゲートとプレイヤーの判定
	CollidePlayer(pTutorial);
}

//=====================================================
// ゲートのスケーリング補正
//=====================================================
void CStateTutorialEnd::ScalingGate(void)
{
	if (m_pEffect == nullptr)
		return;

	// ゲートエフェクトの補正
	Effekseer::Vector3D scale = m_pEffect->GetScale();

	scale.X += (SIZE_GATE_EFFECT - scale.X) * SPEED_EXPAND_GATE;
	scale.Y += (SIZE_GATE_EFFECT - scale.X) * SPEED_EXPAND_GATE;
	scale.Z += (SIZE_GATE_EFFECT - scale.X) * SPEED_EXPAND_GATE;

	m_pEffect->SetScale(scale);
}

//=====================================================
// ゲートとプレイヤーとの判定
//=====================================================
void CStateTutorialEnd::CollidePlayer(CTutorial *pTutorial)
{
	CPlayer *pPlayer = CPlayer::GetInstance();

	if (pPlayer == nullptr)
		return;

	D3DXVECTOR3 posPlayer = pPlayer->GetPosition();
	D3DXVECTOR3 movePlayer = pPlayer->GetMove();
	D3DXVECTOR3 pos = m_pEffect->GetPositionD3D();
	D3DXVECTOR3 rot = m_pEffect->GetRotationD3D();
	float fWidth = SIZE_GATE_EFFECT;

	D3DXVECTOR3 posStart = { pos.x + sinf(rot.y + D3DX_PI * 0.5f) * fWidth, pos.y, pos.z + cosf(rot.y + D3DX_PI * 0.5f) * fWidth };
	D3DXVECTOR3 posEnd = { pos.x - sinf(rot.y + D3DX_PI * 0.5f) * fWidth, pos.y, pos.z - cosf(rot.y + D3DX_PI * 0.5f) * fWidth };

	float fCross = 0.0f;

	bool bHit = universal::IsCross(posPlayer,		// プレイヤーの位置
		posStart,		// ゴールの始点
		posEnd,			// ゴールの終点
		&fCross,		// 交点の割合
		posPlayer + movePlayer);	// プレイヤーの移動量

	bool bHitNext = universal::IsCross(posPlayer + movePlayer,		// プレイヤーの次回の位置
		posStart,		// ゴールの始点
		posEnd,			// ゴールの終点
		nullptr,		// 交点の割合
		posPlayer + movePlayer);	// プレイヤーの移動量

	// 白フェードをかける
	CFade *pFade = CFade::GetInstance();

	if (pFade == nullptr)
		return;

	if (bHit)
	{
		pFade->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		pFade->SetFade(CScene::MODE_GAME, false);

		// ブラーをかける
		CBlurEvent::Create(1.0f, 0.8f, 10.0f);
	}

	if (pFade->GetState() == CFade::FADE::FADE_OUT)
	{// フェードが消え始める瞬間でゲームをスタートする
		pTutorial->StartGame();

		pTutorial->Uninit();

		// プレイヤーを操作可能にする
		pPlayer->SetEnableInput(true);

		return;
	}

#ifdef _DEBUG
	CEffect3D::Create(posStart, 400.0f, 3, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
	CEffect3D::Create(posEnd, 200.0f, 3, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
#endif
}

//=====================================================
// プレイヤーを直進させる処理
//=====================================================
void CStateTutorialEnd::ForwardPlayer(void)
{
	CPlayer *pPlayer = CPlayer::GetInstance();

	if (pPlayer == nullptr)
		return;
	
	// プレイヤーを直進させる
	D3DXVECTOR3 posPlayer = pPlayer->GetPosition();
	D3DXVECTOR3 movePlayer = pPlayer->GetMove();

	float fSpeed = pPlayer->GetSpeed();

	posPlayer += pPlayer->GetForward() * fSpeed;

	pPlayer->SetPosition(posPlayer);

	// 最大速度まで加速させる
	float fSpeedMax = pPlayer->GetParam().fSpeedMaxInitial;
	float fFactAccele = pPlayer->GetParam().fFactAccele;
	fSpeed += (fSpeedMax - fSpeed) * fFactAccele;

	pPlayer->SetSpeed(fSpeed);
}

namespace Tutorial
{
CUI *CreateUIDefault(void)
{
	CUI *pUI = CUI::Create();
	pUI->SetPosition(POS_DEFAULT_UI);
	pUI->SetSize(SIZE_DEFAULT_UI.x, SIZE_DEFAULT_UI.y);
	pUI->SetVtx();

	return pUI;
}
}