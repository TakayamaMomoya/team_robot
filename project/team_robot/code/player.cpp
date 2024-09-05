//*****************************************************
//
// プレイヤーの処理[player.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "player.h"
#include "cameraState.h"
#include "camera.h"
#include "slow.h"
#include "inputManager.h"
#include "debugproc.h"
#include "effect3D.h"
#include "pause.h"
#include "mobFactory.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const string PATH_PARAM = "data\\TEXT\\playerParam.txt";	// パラメーターデータのパス
}

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
CPlayer *CPlayer::s_pPlayer = nullptr;	// 自身のポインタ

//=====================================================
// 優先順位を決めるコンストラクタ
//=====================================================
CPlayer::CPlayer(int nPriority)
{
	s_pPlayer = this;
	m_pController = nullptr;
	m_bLockOn = false;
}

//=====================================================
// デストラクタ
//=====================================================
CPlayer::~CPlayer()
{

}

//=====================================================
// 生成処理
//=====================================================
CPlayer *CPlayer::Create(void)
{
	if (s_pPlayer == nullptr)
	{
		s_pPlayer = new CPlayer;

		if (s_pPlayer != nullptr)
		{
			s_pPlayer->Init();
		}
	}

	return s_pPlayer;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CPlayer::Init(void)
{
	// ブラーをかけない設定にする
	EnableBlur(false);

	// 継承クラスの初期化
	CMotion::Init();

	SetMotion(E_Motion::MOTION_WALK_FRONT);

	// 読込
	Load();

	// モデルの設定
	CMotion::Load(&m_param.m_stPathBody[0]);

	// 初期トランスフォームの設定
	SetPosition(Player::DEFAULT_POS);
	SetRotation(Player::DEFAULT_ROT);
	MultiplyMtx(false);	// 階層構造のマトリックスをかけ合わせる

	// カメラの行動設定
	Camera::ChangeState(new CFollowPlayer);
	Camera::SkipToDest();	// 目標位置までカメラを飛ばす

	// コントローラーの生成
	m_pController = new CPlayerController;

	if (m_pController != nullptr)
	{
		m_pController->Init();
	}

	return S_OK;
}

//=====================================================
// 読込処理
//=====================================================
void CPlayer::Load(void)
{
	std::ifstream file(PATH_PARAM);

	if (file.is_open())
	{
		std::string temp;

		while (std::getline(file, temp))
		{// 読み込むものがなくなるまで読込
			std::istringstream iss(temp);
			std::string key;
			iss >> key;

			if (key == "BODY_PATH")
			{// 見た目のパス
				iss >> m_param.m_stPathBody;
			}

			if (file.eof())
			{// 読み込み終了
				break;
			}
		}
		
		file.close();
	}
	else
	{
		assert(("ランキングファイルが開けませんでした", false));
	}
}

//=====================================================
// 終了処理
//=====================================================
void CPlayer::Uninit(void)
{
	s_pPlayer = nullptr;

	if (m_pController != nullptr)
	{
		m_pController->Uninit();
		m_pController = nullptr;
	}

	// 継承クラスの終了
	CMotion::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void CPlayer::Update(void)
{
	// ロックオン
	Lockon();

	// 入力
	Input();

	// 前回の位置を保存
	D3DXVECTOR3 pos = GetPosition();
	SetPositionOld(pos);

	// 移動量の減衰
	D3DXVECTOR3 move = GetMove();
	move *= 0.98f;
	SetMove(move);

	// 移動量を位置に反映
	pos += move;
	SetPosition(pos);

	// 継承クラスの更新
	CMotion::Update();

// デバッグ処理
#if _DEBUG
	Debug();
#endif // _DEBUG
}

//=====================================================
// ロックオン
//=====================================================
void CPlayer::Lockon(void)
{
	// 捉えている敵がいたらクォータニオンを補正

	// 敵のリストを取得
	CMobFactory *pMobfactory = CMobFactory::GetInstance();

	if (pMobfactory == nullptr)
		return;

	list<CMob*> aEnemy = pMobfactory->GetEnemy();

	// 近い敵を捕捉
	float fDistMin = FLT_MAX;
	CMob* pEnemyNearest = nullptr;

	for (CMob* pEnemy : aEnemy)
	{
		if (pEnemy == nullptr)
			continue;

		D3DXVECTOR3 posEnemy = pEnemy->GetPosition();
		D3DXVECTOR3 pos = GetPosition();

		float fDiff;
		bool bNear = universal::DistCmp(pos, posEnemy, fDistMin, &fDiff);

		if (bNear)
		{
			pEnemyNearest = pEnemy;
			fDistMin = fDiff;
		}
	}

	if (pEnemyNearest != nullptr && m_bLockOn)
	{
		D3DXQUATERNION quat = GetQuaternion();
		D3DXVECTOR3 pos = GetPosition();
		D3DXVECTOR3 posEnemy = pEnemyNearest->GetPosition();
		D3DXVECTOR3 vecForward = pEnemyNearest->GetForward();
		D3DXVECTOR3 vecUp = GetUp();

		D3DXQUATERNION quatResult = universal::SmoothFaceTowardsTarget(quat, posEnemy,pos, vecForward, vecUp, 0.5f);

		SetQuaternion(quatResult);
	}
}

//=====================================================
// 入力処理
//=====================================================
void CPlayer::Input(void)
{
	CInputManager *pInputManager = CInputManager::GetInstance();

	if (pInputManager == nullptr)
		return;

	if (pInputManager->GetTrigger(CInputManager::BUTTON_PAUSE))
	{// ポーズの発生
		CPause::Create();
	}
}

//=====================================================
// 移動の入力
//=====================================================
void CPlayer::InputMove(void)
{
	CSlow *pSlow = CSlow::GetInstance();
	CInputManager *pInputManager = CInputManager::GetInstance();

	if (pInputManager == nullptr)
	{
		return;
	}
}

//=====================================================
// イベントの管理
//=====================================================
void CPlayer::Event(EVENT_INFO *pEventInfo)
{
	int nMotion = GetMotion();

	D3DXVECTOR3 offset = pEventInfo->offset;
	D3DXMATRIX mtxParent;
	D3DXMATRIX mtxPart = GetParts(pEventInfo->nIdxParent)->pParts->GetMatrix();

	universal::SetOffSet(&mtxParent, mtxPart, offset);

	D3DXVECTOR3 pos = { mtxParent._41,mtxParent._42 ,mtxParent._43 };
}

//=====================================================
// 描画処理
//=====================================================
void CPlayer::Draw(void)
{
	// 継承クラスの描画
	CMotion::Draw();
}

//=====================================================
// ヒット処理
//=====================================================
void CPlayer::Hit(float fDamage)
{

}

//=====================================================
// デバッグ表示
//=====================================================
void CPlayer::Debug(void)
{
#ifndef _DEBUG

	return;

#endif

	CDebugProc *pDebugProc = CDebugProc::GetInstance();

	if (pDebugProc == nullptr)
	{
		return;
	}

#if 1
	pDebugProc->Print("\nプレイヤーの位置[%f,%f,%f]", GetPosition().x, GetPosition().y, GetPosition().z);
	pDebugProc->Print("\nプレイヤーの移動量[%f,%f,%f]", GetMove().x, GetMove().y, GetMove().z);
	pDebugProc->Print("\nプレイヤーの向き[%f,%f,%f]", GetRotation().x, GetRotation().y, GetRotation().z);
#endif
}