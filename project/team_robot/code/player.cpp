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
#include "manager.h"
#include "cameraState.h"
#include "camera.h"
#include "slow.h"
#include "inputManager.h"
#include "inputjoypad.h"
#include "inputkeyboard.h"
#include "pause.h"
#include "debugproc.h"
#include "blockManager.h"
#include "effect3D.h"
#include "polygon3D.h"
#include "blur.h"
#include "renderer.h"
#include "meshRoad.h"
#include "game.h"
#include "MyEffekseer.h"
#include "sound.h"
#include "guardRail.h"
#include "playerNinja.h"
#include "orbit.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const std::string PATH_PARAM = "data\\TEXT\\playerParam.txt";	// パラメーターデータのパス
const float NOTROTATE = 1.0f;		// 回転しないようにする値
const float DIST_LIMIT = 5000.0f;	// ワイヤー制限距離
const float LINE_CORRECT_DRIFT = 40.0f;	// ドリフト補正のしきい値
const float SIZE_BLUR = -20.0f;	// ブラーのサイズ
const float DENSITY_BLUR = 0.5f;	// ブラーの濃さ
const float SE_CHANGE_SPEED = 10.0f;  // エンジン音とアクセル音が切り替わる速度の値
const float HANDLE_INERTIA = 0.03f;  // カーブ時の角度変更慣性
const float HANDLE_INERTIA_RESET = 0.07f;  // 体勢角度リセット時の角度変更慣性倍率
const float HANDLE_INERTIA_DRIFT = 0.08f;  // ドリフト時の角度変更慣性倍率
const float HANDLE_INERTIA_DEFAULT = 0.1f;  // ドリフト姿勢から通常姿勢に戻る時の角度変更慣性倍率
const float HANDLE_CURVE_MAG = -0.04f;  // 体勢からカーブへの倍率
const float SIZE_SPEEDBLUR = 13.0f;	// スピードブラーのサイズ
const float DENSITY_SPEEDBLUR = 0.3f;	// スピードブラーの濃さ
const float ROT_CURVE_LIMIT = 0.02f;  // ハンドル操作がきく用になる角度の限界
const float ROT_Z_DRIFT = 1.0f;  // ドリフト中のZ軸の角度
const float GRAVITY = -0.2f;  // 重力の倍率
const float GRAVITY_GROUND = -9.0f;  // 接地時の重力
const float HEIGH_FRONT_WHEEL = 55.0f;  // 前輪の高さ
const float HEIGH_REAR_WHEEL = 65.0f;  // 後輪の高さ
const float ROT_BIKE_FRONT_LIMIT = 1.5f;  // 前回りの角度限界
const float ROT_BIKE_REAR_LIMIT = -1.35f;  // 後ろ回りの角度限界
const float ROT_AIRBIKE_MAG = 0.015f;  // 空中での回転倍率
}

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
CPlayer *CPlayer::m_pPlayer = nullptr;	// 自身のポインタ

//=====================================================
// 優先順位を決めるコンストラクタ
//=====================================================
CPlayer::CPlayer(int nPriority)
{
	m_pPlayer = this;

	ZeroMemory(&m_info, sizeof(CPlayer::SInfo));
	ZeroMemory(&m_param, sizeof(CPlayer::SParam));
	ZeroMemory(&m_fragMotion, sizeof(CPlayer::SFragMotion));
	ZeroMemory(&m_fragNinja, sizeof(CPlayer::SFragNinja));
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
	if (m_pPlayer == nullptr)
	{
		m_pPlayer = new CPlayer;

		if (m_pPlayer != nullptr)
		{
			m_pPlayer->Init();
		}
	}

	return m_pPlayer;
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

	SetMotion(MOTION_WALK_FRONT);

	// 読込
	Load();

	// モデルの設定
	CMotion::Load(&m_param.aPathBody[0]);

	// バイクに乗っている忍者の初期化とモデルの設定
	if (m_pPlayerNinja == nullptr)
	{
		m_pPlayerNinja = CPlayerNinja::Create();

		if (m_pPlayerNinja != nullptr)
		{
			m_pPlayerNinja->SetMatrix(GetMatrix());
		}
	}

	m_info.pRoap = CPolygon3D::Create(GetPosition());

	// デフォルト値設定
	m_info.fLengthDrift = 1500.0f;
	m_info.bGrabOld = true;
	m_info.fDesityBlurDrift = DENSITY_BLUR;
	m_info.fSizeBlurDrift = SIZE_BLUR;

	// 初期トランスフォームの設定
	SetPosition(Player::DEFAULT_POS);
	SetRotation(Player::DEFAULT_ROT);

	MultiplyMtx(false);	// 階層構造のマトリックスをかけ合わせる

	// カメラの行動設定
	Camera::ChangeState(new CFollowPlayer);

	CCamera *pCamera = CManager::GetCamera();

	if (pCamera == nullptr)
		return E_FAIL;

	pCamera->SkipToDest();	// 目標位置までカメラを飛ばす

	// テールランプ用軌跡の生成
	m_info.orbitColor = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	m_info.pOrbitLamp = COrbit::Create(GetMatrix(), D3DXVECTOR3(20.0f, 220.0f, -80.0f), D3DXVECTOR3(-20.0f, 220.0f, -80.0f), m_info.orbitColor, 60);

	// サウンドインスタンスの取得
	CSound* pSound = CSound::GetInstance();

	// エンジンのSE再生
	if (pSound != nullptr)
		pSound->Play(pSound->LABEL_SE_ENGIN);

	m_info.bEnableInput = true;

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
				iss >> m_param.aPathBody;
			}

			if (key == "SPEED_MAX")
			{// 最大速度
				iss >> m_param.fSpeedMax;
				m_param.fSpeedMaxInitial = m_param.fSpeedMax;
			}

			if (key == "FACT_ACCELE")
			{// 加速係数
				iss >> m_param.fFactAccele;
			}

			if (key == "FACT_ATTENU")
			{// 減速係数
				iss >> m_param.fFactAttenu;
			}

			if (key == "FACT_BRAKE")
			{// ブレーキ減速係数
				iss >> m_param.fFactBrake;
			}

			if (key == "ANGLE_MAX_CURVE")
			{// カーブ最大角度
				iss >> m_param.fAngleMaxCurve;
			}

			if (key == "COLLIDER_SIZE")
			{// プレイヤー当たり判定サイズ
				iss >> m_param.sizeCollider.x >> m_param.sizeCollider.y >> m_param.sizeCollider.z;
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
	m_pPlayer = nullptr;
	m_info.pOrbitLamp->Uninit();

	// 継承クラスの終了
	CMotion::Uninit();

	// サウンドの停止
	// サウンドインスタンスの取得
	CSound* pSound = CSound::GetInstance();

	if (pSound != nullptr)
		pSound->Stop();
}

//=====================================================
// 更新処理
//=====================================================
void CPlayer::Update(void)
{
#ifdef _DEBUG
	if (CGame::GetInstance()->GetStop())
		return;
#endif

	if (m_info.bEnableInput)
	{
		// 入力
		Input();

		//当たり判定
		Collision();
	}

	// 前回の位置を保存
	D3DXVECTOR3 pos = GetPosition();
	SetPositionOld(pos);

	// 状態管理
	ManageState();

	// モーション管理
	ManageMotion();

	// 忍者のモーション管理
	ManageMotionNinja();

	// 継承クラスの更新
	CMotion::Update();

	if (m_pPlayerNinja != nullptr)
	{// バイクに乗った忍者の追従
		m_pPlayerNinja->SetPosition(D3DXVECTOR3(0.0f, 50.0f, 0.0f));
		CObject3D::Draw();
		m_pPlayerNinja->SetMatrixParent(GetMatrix());
	}

	// スピードによるブラーの管理
	ManageSpeedBlur();

// デバッグ処理
#if _DEBUG
	Debug();
#endif // _DEBUG
}

//=====================================================
// 入力処理
//=====================================================
void CPlayer::Input(void)
{
	// 移動操作
	InputMove();

	// ワイヤーの操作
	InputWire();

	// 刀の操作
	InputKatana();

	// スピードの管理
	ManageSpeed();

	// 斬撃エフェクトの管理
	ManageSlashEffect();

	CInputManager *pInputManager = CInputManager::GetInstance();

	if (pInputManager != nullptr)
	{// ポーズの発生
		if (pInputManager->GetTrigger(CInputManager::BUTTON_PAUSE))
		{
			CPause::Create();
		}
	}
}

//=====================================================
// 移動の入力
//=====================================================
void CPlayer::InputMove(void)
{
	CSlow *pSlow = CSlow::GetInstance();
	CInputManager *pInputManager = CInputManager::GetInstance();

	// サウンドインスタンスの取得
	CSound* pSound = CSound::GetInstance();

	if (pInputManager == nullptr)
	{
		return;
	}

	// アクセル値の取得
	float fAccele = pInputManager->GetAccele();

	m_info.fSpeedDest = fAccele * m_param.fSpeedMax;

#if 0
	CDebugProc::GetInstance()->Print("\nアクセル値[%f]", fAccele);
#endif

	if (m_info.pBlockGrab == nullptr)
	{
		// ハンドルの操作
		CInputManager::SAxis axis = pInputManager->GetAxis();
		m_info.fAngleHandle = axis.axisMove.x;
	}

	// ブレーキ値の取得
	float fBrake = pInputManager->GetBrake();

	m_info.fSpeed += (0.0f - m_info.fSpeed) * m_param.fFactBrake * fBrake;

	if (m_info.fSpeed < SE_CHANGE_SPEED && m_bMove)
	{
		// アクセルSEからエンジンSEへ変更
		if (pSound != nullptr)
		{
			pSound->Play(pSound->LABEL_SE_ENGIN);
			pSound->Stop(pSound->LABEL_SE_ACCELERATOR);
		}

		m_bMove = false;
	}
	else if (m_info.fSpeed >= SE_CHANGE_SPEED && !m_bMove)
	{
		// エンジンSEからアクセルSEへ変更
		if (pSound != nullptr)
		{
			pSound->Play(pSound->LABEL_SE_ACCELERATOR);
			pSound->Stop(pSound->LABEL_SE_ENGIN);
		}

		m_bMove = true;
	}

#if 0
	CDebugProc::GetInstance()->Print("\nブレーキ値[%f]", fBrake);
#endif
}

//=====================================================
// ワイヤーの操作
//=====================================================
void CPlayer::InputWire(void)
{
	CInputJoypad* pJoypad = CInputJoypad::GetInstance();

	if (pJoypad == nullptr)
		return;

	D3DXVECTOR2 vecStickR =
	{
		pJoypad->GetJoyStickRX(0),
		pJoypad->GetJoyStickRY(0)
	};

	// スティック入力角度
	float fAngleInput = atan2f(vecStickR.x, vecStickR.y);

	// 入力角度とカメラ角度を足した角度
	D3DXVECTOR3 rotCamera = CManager::GetCamera()->GetCamera()->rot;

	float fAngle = fAngleInput + rotCamera.y + D3DX_PI;
	universal::LimitRot(&fAngle);

	// スティック入力の強さ
	float fLength = sqrtf(vecStickR.x * vecStickR.x + vecStickR.y * vecStickR.y);

	universal::LimitRot(&fAngleInput);

	// ブロックのチェック
	CBlockManager *pBlockManager = CBlockManager::GetInstance();

	CBlock *pBlock = pBlockManager->GetHead();

	if (m_info.pBlockGrab != nullptr)
	{
		m_info.pBlockGrab->EnableCurrent(true);

		// ブラーをかける
		Blur::AddParameter(0.0f, 0.01f, 15.0f, 0.0f, 0.7f);

		// 差分角度の計算
		D3DXVECTOR3 posPlayer = GetPosition();
		D3DXVECTOR3 posBlock = m_info.pBlockGrab->GetPosition();
		D3DXVECTOR3 vecDiff = posBlock - posPlayer;
		float vecLength = D3DXVec3Length(&(posBlock - posPlayer));

		D3DXVECTOR3 vecDiffNormal = vecDiff;
		D3DXVec3Normalize(&vecDiffNormal, &vecDiff);

		D3DXVECTOR3 move = GetMove();
		universal::VecConvertLength(&vecDiff, fabs(D3DXVec3Dot(&move, &vecDiffNormal)));

		float fAngleDiff = atan2f(vecDiff.x, vecDiff.z);

		// ドリフトを変えるかの判定
		JudgeChangeDrift(fAngle, fAngleDiff, fLength);

		// 回転の制御
		ManageRotateGrab(fAngleDiff);

		// ワイヤーを外すかの判定
		JudgeRemoveWire(fLength);

		// ドリフトの補正
		LimitDrift(m_info.fLengthDrift);

		// ロープの制御
		ControlRoap();

		// 後輪の位置取得
		float PosX = GetParts(3)->pParts->GetMatrix()._41;
		float PosY = GetParts(3)->pParts->GetMatrix()._42;
		float PosZ = GetParts(3)->pParts->GetMatrix()._43;

		// エフェクトの再生
		MyEffekseer::CreateEffect(CEffekseer::TYPE_DRIFT, D3DXVECTOR3(PosX, PosY, PosZ), D3DXVECTOR3(0.0f, fAngleDiff, 0.0f));
		
		// サウンドインスタンスの取得
		CSound* pSound = CSound::GetInstance();

		// ドリフトSEの再生
		if (pSound != nullptr && !m_bDrift)
			pSound->Play(pSound->LABEL_SE_DRIFT);

		// ドリフトしている
		m_bDrift = true;
	}
	else
	{
		// カメラのロール値をまっすぐに戻す
		Camera::ControlRoll(0.0f, 0.1f);

		// 掴むブロックの探知
		SarchGrab();

		// サウンドインスタンスの取得
		CSound* pSound = CSound::GetInstance();

		// ドリフトSEの停止
		if (pSound != nullptr && m_bDrift)
			pSound->Stop(pSound->LABEL_SE_DRIFT);

		// ドリフトしていない
		m_bDrift = false;
	}

	if (CInputKeyboard::GetInstance() != nullptr)
	{
		if (CInputKeyboard::GetInstance()->GetTrigger(DIK_F4))
		{// 操作方法変更
			m_info.bManual = m_info.bManual ? false : true;
		}
	}
}

//=====================================================
// ドリフトを変えるかの判定
//=====================================================
void CPlayer::JudgeChangeDrift(float fAngle, float fAngleDiff, float fLength)
{
	CInputJoypad* pJoypad = CInputJoypad::GetInstance();

	if (pJoypad == nullptr)
		return;

	if (m_info.fTimerDriftChange > 0.0f)
	{
		m_info.fTimerDriftChange -= CManager::GetDeltaTime();

		if (pJoypad->GetRStickTrigger(CInputJoypad::DIRECTION::DIRECTION_DOWN, 0) ||
			pJoypad->GetRStickTrigger(CInputJoypad::DIRECTION::DIRECTION_LEFT, 0) ||
			pJoypad->GetRStickTrigger(CInputJoypad::DIRECTION::DIRECTION_UP, 0) ||
			pJoypad->GetRStickTrigger(CInputJoypad::DIRECTION::DIRECTION_RIGHT, 0))
		{// 弾いた瞬間
			m_info.fTimerFlip = 0.8f;

			m_info.nCntFlip++;

			float fDiffInput = fAngle - fAngleDiff;

			m_info.fAngleDrift = 0.29f;

			Blur::AddParameter(5.0f, 0.5f, 15.0f, 0.0f, 0.7f);
		}
		else
		{
			if (m_info.fTimerFlip > 0.0f)
			{// 弾きタイマー減算
				m_info.fTimerFlip -= CManager::GetDeltaTime();
			}
			else
			{// 猶予時間が過ぎると弾きカウンターリセット
				if (fLength <= 0.5f)
				{// 何も掴んでいなければリセット
					m_info.nCntFlip = 0;
				}
			}
		}
	}
}

//=====================================================
// ブロックを掴んでるときの回転制御
//=====================================================
void CPlayer::ManageRotateGrab(float fAngleDiff)
{
	float fDiff = m_info.rotDriftStart.y - fAngleDiff;

	universal::LimitRot(&fDiff);

	D3DXVECTOR3 rotDest = m_info.rotDriftStart;

	if (fDiff > 0.0f)
	{
		rotDest.y = fAngleDiff + D3DX_PI * m_info.fAngleDrift;

		// カメラロール
		Camera::ControlRoll(0.3f, 0.04f);
	}
	else
	{
		rotDest.y = fAngleDiff - D3DX_PI * m_info.fAngleDrift;

		// カメラロール
		Camera::ControlRoll(-0.3f, 0.04f);
	}

	universal::LimitRot(&rotDest.y);

	universal::FactingRot(&m_info.rotDriftStart.y, rotDest.y, 0.15f);
}

//=====================================================
// ワイヤーを外すかの判定
//=====================================================
void CPlayer::JudgeRemoveWire(float fLength)
{
	// ブロックのエリア内かの判定
	D3DXVECTOR3 posPlayer = GetPosition();
	bool bGrab = m_info.pBlockGrab->CanGrab(posPlayer);

	if (m_info.bManual)
	{
		if (fLength <= 0.5f)
		{// スティックを離したらワイヤーを外す
			RemoveWire();
		}
	}
	else
	{
		if (m_info.bGrabOld && !bGrab)
		{// ブロック指定のエリアを回り切ったらワイヤーを外す
			RemoveWire();
		}
	}

	// 現在のエリア内かの判定を保存
	m_info.bGrabOld = bGrab;
}

//=====================================================
// ロープの制御
//=====================================================
void CPlayer::ControlRoap(void)
{
	if (m_info.pRoap != nullptr && m_info.pBlockGrab != nullptr)
	{
		D3DXVECTOR3 posPlayer = GetPosition();
		D3DXVECTOR3 posBlock = m_info.pBlockGrab->GetPosition();
		D3DXVECTOR3 vecDiff = posBlock - posPlayer;

		LPDIRECT3DVERTEXBUFFER9 pVtxBuff = m_info.pRoap->GetVtxBuff();

		//頂点情報のポインタ
		VERTEX_3D *pVtx;

		//頂点バッファをロックし、頂点情報へのポインタを取得
		pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		//頂点座標の設定
		float fHeight = 100.0f;

		D3DXVECTOR3 vec = { vecDiff.z, 0.0f, -vecDiff.x };

		D3DXVec3Normalize(&vec, &vec);

		pVtx[0].pos = D3DXVECTOR3(posBlock.x - vec.x * 20.0f, fHeight, posBlock.z);
		pVtx[1].pos = D3DXVECTOR3(posBlock.x + vec.x * 20.0f, fHeight, posBlock.z);
		pVtx[2].pos = D3DXVECTOR3(posPlayer.x - vec.x * 20.0f, fHeight, posPlayer.z);
		pVtx[3].pos = D3DXVECTOR3(posPlayer.x + vec.x * 20.0f, fHeight, posPlayer.z);

		//頂点バッファをアンロック
		pVtxBuff->Unlock();
	}
}

//=====================================================
// 掴むブロックの探知
//=====================================================
void CPlayer::SarchGrab(void)
{
	CInputJoypad* pJoypad = CInputJoypad::GetInstance();

	if (pJoypad == nullptr)
		return;

	// 入力角度の計算
	D3DXVECTOR2 vecStickR =
	{
		pJoypad->GetJoyStickRX(0),
		pJoypad->GetJoyStickRY(0)
	};

	float fAngleInput = atan2f(vecStickR.x, vecStickR.y);

	// ブロックマネージャーの取得
	CBlockManager *pBlockManager = CBlockManager::GetInstance();

	// 計算用変数
	CBlockGrab *pBlockGrab = nullptr;	
	CBlockGrab* pBlockMin = nullptr;
	float fLengthMin = 0.0f;
	float fAngleMin = D3DX_PI;

	// ブロックリスト取得
	std::list<CBlockGrab*> *pListGrab = pBlockManager->GetListGrab();

	for (CBlockGrab *pBlock : *pListGrab)
	{
		D3DXVECTOR3 posPlayer = GetPosition();
		D3DXVECTOR3 rotPlayer = GetRotation();
		D3DXVECTOR3 posBlock = pBlock->GetPosition();
		D3DXVECTOR3 vecBlockDiff = posBlock - posPlayer;
		float fAngleDiff = atan2f(vecBlockDiff.x, vecBlockDiff.z) - rotPlayer.y;
		float fDiff = fabs(fAngleInput - fAngleDiff);

		universal::LimitRot(&fDiff);
		pBlock->EnableCurrent(false);

		if (fDiff < D3DX_PI * 0.5f && fDiff > -D3DX_PI * 0.5f)
		{
			float fLengthDiff = sqrtf(vecBlockDiff.x * vecBlockDiff.x + vecBlockDiff.z * vecBlockDiff.z);

			if (fLengthDiff <= DIST_LIMIT && (pBlockMin == nullptr || fLengthMin > fLengthDiff))
			{
				pBlockMin = pBlock;

				fAngleMin = fDiff;

				fLengthMin = fLengthDiff;
			}
		}
	}

	if (pBlockMin != nullptr)
	{
		pBlockGrab = pBlockMin;

		m_info.fLengthDrift = fLengthMin;

		m_info.fAngleDrift = 0.4f;
	}

	if (pBlockGrab != nullptr)
	{
		if (pJoypad->GetRStickTrigger(CInputJoypad::DIRECTION::DIRECTION_DOWN, 0) ||
			pJoypad->GetRStickTrigger(CInputJoypad::DIRECTION::DIRECTION_LEFT, 0) ||
			pJoypad->GetRStickTrigger(CInputJoypad::DIRECTION::DIRECTION_UP, 0) ||
			pJoypad->GetRStickTrigger(CInputJoypad::DIRECTION::DIRECTION_RIGHT, 0))
		{// 弾いた瞬間
			m_info.pBlockGrab = pBlockGrab;

			m_info.fTimerDriftChange = 0.7f;

			m_info.rotDriftStart = GetRotation();
			m_info.rotDriftStart.x += D3DX_PI * 0.5f;
		}
	}
}

//=====================================================
// ワイヤーを外す
//=====================================================
void CPlayer::RemoveWire(void)
{
	m_info.nCntFlip = 0;
	m_info.fCntAngle = 0.0f;

	m_info.pBlockGrab = nullptr;

	m_info.fLengthDrift = 0.0f;

	m_info.rotDriftStart.x -= D3DX_PI * 0.5f;
	universal::LimitRot(&m_info.rotDriftStart.x);

	// ブラーを戻す
	Blur::ResetBlur();
}

//=====================================================
// 刀の操作
//=====================================================
void CPlayer::InputKatana(void)
{
	CInputManager *pInputManager = CInputManager::GetInstance();

	if (pInputManager == nullptr)
		return;

	if (pInputManager->GetTrigger(CInputManager::BUTTON_KATANA))
	{
		// 刀モーションフラグ管理
		ManageKanataAtttack();
	}
}

//=====================================================
// 刀の攻撃の管理
//=====================================================
void CPlayer::ManageKanataAtttack(void)
{
	if (m_fragNinja.bSlashDown)
	{
		m_fragNinja.bSlashUp = true;
	}
	else
	{
		m_fragNinja.bSlashDown = true;
	}
}

//=====================================================
// ドリフトの補正
//=====================================================
void CPlayer::LimitDrift(float fLength)
{
	if (m_info.pBlockGrab == nullptr)
		return;

	D3DXVECTOR3 posPlayer = GetPosition();
	D3DXVECTOR3 posBlock = m_info.pBlockGrab->GetPosition();
	D3DXVECTOR3 vecDiff = posPlayer - posBlock;

	vecDiff.y = 0.0f;

	float fLengthDiff = sqrtf(vecDiff.x * vecDiff.x + vecDiff.z * vecDiff.z);

	//if (fLengthDiff < DIST_LIMIT - LINE_CORRECT_DRIFT)
	{
		//m_info.fAngleHandle = 0.2f;

		/*universal::VecConvertLength(&vecDiff, fLength);

		D3DXVECTOR3 posDest = posBlock + vecDiff;

		universal::MoveToDest(&posPlayer, posDest, 0.1f);

		SetPosition(posPlayer);*/
	}
	/*else if(fLengthDiff >= fLength + LINE_CORRECT_DRIFT)
	{
		m_info.fAngleHandle = 0.2f;
	}
	else
	{
		m_info.fAngleHandle = 0.0f;
	}*/
}

//=====================================================
// 当たり判定処理
//=====================================================
void CPlayer::Collision(void)
{
	// 前回の位置を保存
	D3DXVECTOR3 pos = GetPosition();
	D3DXVECTOR3 posOld = GetPositionOld();
	D3DXVECTOR3 move = GetMove();
	D3DXVECTOR3 rot = GetRotation();
	D3DXVECTOR3 posParts[2];
	D3DXVECTOR3 posOldParts[2];
	D3DXVECTOR3 posDef, posDefOld;
	bool bRoad[2];
	CInputManager* pInputManager = CInputManager::GetInstance();

	// 計算用マトリックスを宣言
	D3DXMATRIX* mtx = &GetMatrix();
	D3DXMATRIX mtxTrans, mtxRot;

	// プレイヤー側の当たり判定サイズから判定用OBBを設定
	D3DXVECTOR3 paramSize = m_param.sizeCollider;
	D3DXVECTOR3 sizeX = universal::PosRelativeMtx(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, rot.y, 0.0f), paramSize);
	D3DXVECTOR3 sizeZ = universal::PosRelativeMtx(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, -rot.y, 0.0f), D3DXVECTOR3(paramSize.x, 0.0f, paramSize.z));

	// タイヤの位置保存
	posParts[0] = universal::GetMtxPos(GetParts(2)->pParts->GetMatrix()) + (pos - posOld);
	posParts[0].y -= HEIGH_FRONT_WHEEL;
	posParts[1] = universal::GetMtxPos(GetParts(3)->pParts->GetMatrix()) + (pos - posOld);
	posParts[1].y -= HEIGH_REAR_WHEEL;

	// タイヤの過去位置保存
	posOldParts[0] = universal::GetMtxPos(*GetParts(2)->pParts->GetMatrixOld()) + (pos - posOld);
	posOldParts[0].y -= HEIGH_FRONT_WHEEL;
	posOldParts[1] = universal::GetMtxPos(*GetParts(3)->pParts->GetMatrixOld()) + (pos - posOld);
	posOldParts[1].y -= HEIGH_REAR_WHEEL;

	// タイヤの中点を計算
	posDef = (posParts[0] + posParts[1]) * 0.5f;

	// メッシュロードのリスト取得
	std::list<CMeshRoad*> listRoad = CMeshRoad::GetArray();
	for (auto it : listRoad)
	{
		// ガードレールとの当たり判定
		// ガードレールのvectorを取得
		std::vector<CGuardRail*>* aGuardRail = it->GetArrayGR();
		for (auto itGuardRail : *aGuardRail)
		{
			// ガードレールと当たっているか判定する
			if (!itGuardRail->CollideGuardRail(&pos, &move, sizeX, &m_info.fSpeed))
				continue;

			// 一か所判定したら抜ける
			rot.y = atan2f(move.x, move.z);

			// ドリフト中の時ドリフト状態を解除する
			m_info.rotDriftDest = 0.0f;
			RemoveWire();

			break;
		}

		// ガードレールの判定分タイヤの位置を計算する
		posParts[0] += pos - posOld;
		posParts[1] += pos - posOld;
		posOldParts[0] += pos - posOld;
		posOldParts[1] += pos - posOld;

		// タイヤそれぞれでmeshRoadと当たり判定をとる
		bRoad[0] = it->CollideRoad(&posParts[0], posOldParts[0]);
		bRoad[1] = it->CollideRoad(&posParts[1], posOldParts[1]);

		// どちらかのタイヤがmeshRoadと当たっていたらループを抜ける
		if (bRoad[0] || bRoad[1])
			break;
	}

	// タイヤそれぞれでblockと当たり判定をとる
	// 先頭オブジェクトを代入
	CBlock* pBlock = CBlockManager::GetInstance()->GetHead();
	while (pBlock)
	{
		// 次のアドレスを保存
		CBlock* pBlockNext = pBlock->GetNext();

		if (D3DXVec3Length(&(pBlock->GetPosition() - pos)) < 10000.0f)
		{
			// タイヤとblockで当たり判定をとる
			if (pBlock->Collide(&posParts[0], posOldParts[0]))
				bRoad[0] = true;

			if (pBlock->Collide(&posParts[1], posOldParts[1]))
				bRoad[1] = true;
		}

		// 両方が判定を通っていたら抜ける
		if (bRoad[0] && bRoad[1])
			break;

		// 次のアドレスを代入
		pBlock = pBlockNext;
	}

	// プレイヤーの高さを調整
	pos.y += ((posParts[0] + posParts[1]) * 0.5f).y - posDef.y;

	// タイヤの位置関係から角度を計算
	if ((posParts[1].y - posParts[0].y) < D3DXVec3Length(&(posParts[0] - posParts[1])) && (bRoad[0] || bRoad[1]))
		rot.x = asinf((posParts[1].y - posParts[0].y) / D3DXVec3Length(&(posParts[0] - posParts[1])));

	// タイヤの状態を判定する
	if (bRoad[0] && bRoad[1])
	{// タイヤが両方道に触れているとき
		move.y = GRAVITY_GROUND;

		m_info.bAir = false;
	}
	else if (bRoad[0] || bRoad[1])
	{// タイヤが片方だけ道に触れているとき
		move.y = GRAVITY_GROUND;

		m_info.bAir = true;
	}
	else
	{// タイヤがどちらも道に触れていないとき

		if (pInputManager != nullptr)
		{
			// ハンドルの操作
			CInputManager::SAxis axis = pInputManager->GetAxis();

			// 空中での前後の回転処理
			if (axis.axisMove.z > 0.0f)
				rot.x += ROT_AIRBIKE_MAG;
			else if (axis.axisMove.z < 0.0f)
				rot.x -= ROT_AIRBIKE_MAG;
			else
			{
				rot.x += ROT_AIRBIKE_MAG;
			}
		}

		m_info.bAir = true;
	}

	// 角度制限
	if (rot.x > ROT_BIKE_FRONT_LIMIT)
		rot.x = ROT_BIKE_FRONT_LIMIT;

	if (rot.x < ROT_BIKE_REAR_LIMIT)
		rot.x = ROT_BIKE_REAR_LIMIT;

#ifdef _DEBUG
	if (CInputJoypad::GetInstance() != nullptr)
	{
		if (CInputJoypad::GetInstance()->GetPress(CInputJoypad::PADBUTTONS_A, 0) || 
			CInputKeyboard::GetInstance()->GetPress(DIK_SPACE))
		{// 操作方法変更
			pos.y += 30.0f;
			move.y = 0.0f;
			rot.x = 0.0f;
		}
	}
#endif // _DEBUG

	// 位置と移動量と角度を代入
	SetPosition(pos);
	SetMove(move);
	SetRotation(rot);
}

//=====================================================
// スピードの管理
//=====================================================
void CPlayer::ManageSpeed(void)
{
	CSlow* pSlow = CSlow::GetInstance();

	// スピードの増減
	if (m_info.fSpeedDest >= m_info.fSpeed)
	{// 加速しているとき
		m_info.fSpeed += (m_info.fSpeedDest - m_info.fSpeed) * m_param.fFactAccele;

#if 0
		CDebugProc::GetInstance()->Print("\n加速中");
#endif

		//// サウンドインスタンスの取得
		//CSound* pSound = CSound::GetInstance();

		//// アクセルのSE再生
		//if (pSound != nullptr)
		//	pSound->Play(pSound->LABEL_SE_ACCELERATOR);
	}
	else
	{// 減速しているとき
		m_info.fSpeed += (m_info.fSpeedDest - m_info.fSpeed) * m_param.fFactAttenu;

#if 0
		CDebugProc::GetInstance()->Print("\n減速中");
#endif
	}

	D3DXVECTOR3 pos = GetPosition();
	D3DXVECTOR3 move = GetMove();
	D3DXVECTOR3 rot = GetRotation();
	float rotBlock;

	// スロー状態を考慮した移動量の調整
	if (pSlow != nullptr)
	{
		float fScale = pSlow->GetScale();

		pos += move * fScale;
		SetPosition(pos);
	}
	else
	{
		pos += move;
		SetPosition(pos);
	}

	m_info.orbitColor = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.0f);

	if (m_info.pBlockGrab != nullptr)
	{
		// 差分角度の計算
		D3DXVECTOR3 posPlayer = GetPosition();
		D3DXVECTOR3 posBlock = m_info.pBlockGrab->GetPosition();
		rotBlock = atan2f((posBlock.x - posPlayer.x), (posBlock.z - posPlayer.z));
		universal::LimitRot(&rotBlock);

		if (D3DXVec3Cross(&posPlayer, &GetMove(), &(posBlock - posPlayer))->y > 0.0f)
		{
			rot.z += (-ROT_Z_DRIFT - rot.z) * HANDLE_INERTIA_DRIFT;
			m_info.rotDriftDest = D3DX_PI * 0.5f;
		}
		else
		{
			rot.z += (ROT_Z_DRIFT - rot.z) * HANDLE_INERTIA_DRIFT;
			m_info.rotDriftDest = D3DX_PI * -0.5f;
		}
		
		universal::LimitRot(&rot.z);

		float rotDef = rotBlock - rot.y;
		universal::LimitRot(&rotDef);
		rot.y += rotDef;
		universal::LimitRot(&rot.y);

		m_info.orbitColor = D3DXCOLOR(1.0f, 0.15f, 0.0f, 1.0f);
	}
	else if (m_info.fSpeed >= NOTROTATE)
	{// ハンドルの回転を追加
		if (m_info.fAngleHandle == 0.0f)
			rot.z += (m_info.fAngleHandle * m_param.fAngleMaxCurve - rot.z) * HANDLE_INERTIA_RESET;
		else
			rot.z += (m_info.fAngleHandle * m_param.fAngleMaxCurve - rot.z) * HANDLE_INERTIA;
		
		universal::LimitRot(&rot.z);

		if (fabsf(m_info.rotDriftDest) < ROT_CURVE_LIMIT)
		{
			rot.y += rot.z * HANDLE_CURVE_MAG;
			universal::LimitRot(&rot.y);
		}
	}
	else
	{
		rot.z += (0.0f - rot.z) * HANDLE_INERTIA_RESET;
		universal::LimitRot(&rot.z);
	}

	if (m_info.pBlockGrab == nullptr)
	{
		rot.y += (0.0f - m_info.rotDriftDest) * HANDLE_INERTIA_DEFAULT;
		m_info.rotDriftDest += (0.0f - m_info.rotDriftDest) * HANDLE_INERTIA_DEFAULT;
	}

	SetRotation(rot);

	// 極座標が上を向いている分を補正
	rot.x += D3DX_PI * 0.5f;

	if (m_info.pBlockGrab == nullptr)
	{
		m_info.rotDriftStart = rot;
	}

	CDebugProc::GetInstance()->Print("\n調整角度 %f", m_info.rotDriftDest);

	D3DXVECTOR3 vecForward = universal::PolarCoordinates(m_info.rotDriftStart);

	// 現在のスピードと前方ベクトルをかけて移動量に適用
	move.x = vecForward.x * m_info.fSpeed;
	if(!m_info.bAir)
		move.y = vecForward.y * m_info.fSpeed;
	move.z = vecForward.z * m_info.fSpeed;

	// 移動量に重力を適用
	move.y += GRAVITY;

	SetMove(move);

	if (m_info.pOrbitLamp != nullptr)
		m_info.pOrbitLamp->SetOffset(GetMatrix(), m_info.orbitColor, m_info.pOrbitLamp->GetID());
}

//=====================================================
// スピードによるブラーの管理
//=====================================================
void CPlayer::ManageSpeedBlur(void)
{
	float fRate = m_info.fSpeed / m_param.fSpeedMaxInitial;

	Blur::SetBlur(SIZE_SPEEDBLUR * fRate, fRate * DENSITY_SPEEDBLUR);
}

//=====================================================
// 状態管理
//=====================================================
void CPlayer::ManageState(void)
{
	switch (m_info.state)
	{
	case STATE_NORMAL:
	{// 通常状態

	}
	default:
		break;
	}
}

//=====================================================
// モーション管理
//=====================================================
void CPlayer::ManageMotion(void)
{
	int nMotion = GetMotion();
	bool bFinish = IsFinish();

	if (m_fragMotion.bMove)
	{// 歩きモーション
		if (nMotion != MOTION_WALK_FRONT)
		{
			SetMotion(MOTION_WALK_FRONT);
		}
	}
	else
	{// 待機モーション
		if (nMotion != MOTION_NEUTRAL)
		{
			SetMotion(MOTION_NEUTRAL);
		}
	}
}

//=====================================================
// 忍者のモーション管理
//=====================================================
void CPlayer::ManageMotionNinja(void)
{
	if (m_pPlayerNinja == nullptr)
		return;
	
	int nMotion = m_pPlayerNinja->GetMotion();
	bool bFinish = m_pPlayerNinja->IsFinish();

	if (m_fragNinja.bSlashUp)
	{
		if (nMotion == MOTION_NINJA::MOTION_NINJA_SLASHDOWN)
		{// 切り下ろしが終わってからモーション再生
			if(bFinish)
				m_pPlayerNinja->SetMotion(MOTION_NINJA::MOTION_NINJA_SLASHUP);
		}
		else
		{// 切り下ろしからの派生でない場合
			if (nMotion != MOTION_NINJA::MOTION_NINJA_SLASHUP)
				m_pPlayerNinja->SetMotion(MOTION_NINJA::MOTION_NINJA_SLASHUP);

			if (bFinish)
				m_fragNinja.bSlashUp = false;
		}
	}
	else if (m_fragNinja.bSlashDown)
	{
		if (nMotion != MOTION_NINJA::MOTION_NINJA_SLASHDOWN)
		{
			// エフェクシア取得
			CEffekseer* pEffekseer = CManager::GetMyEffekseer();

			float PosX = m_pPlayerNinja->GetParts(1)->pParts->GetMatrix()._41;
			float PosY = m_pPlayerNinja->GetParts(1)->pParts->GetMatrix()._42;
			float PosZ = m_pPlayerNinja->GetParts(1)->pParts->GetMatrix()._43;

			// エフェクトの再生
			CEffekseerEffect *pEffect = MyEffekseer::CreateEffect(CEffekseer::TYPE_SLASH, D3DXVECTOR3(PosX, PosY, PosZ));

			m_listSlashEffect.push_back(pEffect);

			m_pPlayerNinja->SetMotion(MOTION_NINJA::MOTION_NINJA_SLASHDOWN);
		}

		if (bFinish)
			m_fragNinja.bSlashDown = false;
	}
	else
	{
		if (nMotion != MOTION_NINJA::MOTION_NINJA_NEUTRAL)
		{
			m_pPlayerNinja->SetMotion(MOTION_NINJA::MOTION_NINJA_NEUTRAL);
		}
	}
}

//=====================================================
// 斬撃エフェクトの管理
//=====================================================
void CPlayer::ManageSlashEffect(void)
{
	if (m_pPlayerNinja == nullptr)
		return;

	D3DXVECTOR3 posEfect = m_pPlayerNinja->GetMtxPos(0);

	for (auto it : m_listSlashEffect)
	{
		if(it != nullptr)
			it = it->FollowPosition(posEfect);
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
	pDebugProc->Print("\nプレイヤーの向き[%f,%f,%f]", GetRotation().x, GetRotation().y, GetRotation().z);/*
	pDebugProc->Print("\n目標速度[%f]", m_info.fSpeedDest);
	pDebugProc->Print("\n現在の速度[%f]", m_info.fSpeed);
	pDebugProc->Print("\n弾きカウンター[%d]", m_info.nCntFlip);
	pDebugProc->Print("\n角度カウンター[%f]", m_info.fCntAngle);
	pDebugProc->Print("\nブラーのサイズ[%f]", m_info.fSizeBlurDrift);
	pDebugProc->Print("\nブラーの濃さ[%f]", m_info.fDesityBlurDrift);*/
#endif

	// ブラーのサイズ調整
	if (CInputKeyboard::GetInstance()->GetPress(DIK_UP))
	{
		m_info.fSizeBlurDrift += 0.3f;
	}
	else if (CInputKeyboard::GetInstance()->GetPress(DIK_DOWN))
	{
		m_info.fSizeBlurDrift -= 0.3f;
	}

	// ブラーの濃さ調整
	if (CInputKeyboard::GetInstance()->GetPress(DIK_LEFT))
	{
		m_info.fDesityBlurDrift += 0.03f;
	}
	else if (CInputKeyboard::GetInstance()->GetPress(DIK_RIGHT))
	{
		m_info.fDesityBlurDrift -= 0.03f;
	}
}