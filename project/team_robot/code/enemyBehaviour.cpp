//*****************************************************
//
// エネミービヘイビア[enemyBehaviour.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "enemyBehaviour.h"
#include "enemy.h"
#include "effect3D.h"
#include "debugproc.h"
#include "player.h"
#include "myLib.h"
#include "meshRoad.h"
#include "manager.h"
#include "MyEffekseer.h"
#include "shuriken.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const float SPEED_DEFAULT_CHASE = 50.0f;	// 追跡の標準速度
const float TIME_THROW_SHURIKEN = 2.0f;	    // 手裏剣を投げる頻度
const float LENGTH_PLAYER_FRONT = 3000.0f;	// プレイヤーの前とみなす距離
const float SPEED_ESCAPE = 5.0f;            // 逃げるときに加算する速度
const float TIME_ESCAPE = 2.0f;             // 手裏剣をすべて投げてから逃げるまでの時間
const int MAX_SHURIKEN = 5;                 // 投げる手裏剣の数
}

//********************************************************************************
// プレイヤーの追跡
//********************************************************************************
//=====================================================
// コンストラクタ
//=====================================================
CEnemyBehaviourChasePlayer::CEnemyBehaviourChasePlayer()
{
	m_state = STATE::STATE_NONE;
	m_fTimerAttack = 0.0f;
	m_nNumShuriken = 0;
	m_pBigShuriken = nullptr;
	m_pFlashEffect = nullptr;
}

//=====================================================
// デストラクタ
//=====================================================
CEnemyBehaviourChasePlayer::~CEnemyBehaviourChasePlayer()
{

}

//=====================================================
// 初期化
//=====================================================
void CEnemyBehaviourChasePlayer::Init(CEnemy *pEnemy)
{
	m_state = STATE::STATE_CHASE;

	// スプラインの情報を取得
	m_pSpline = CMeshRoad::GetInstance()->GetCenterSpline();

	// ロードポイントの情報を取得
	std::vector<CMeshRoad::SInfoRoadPoint> pRoadPoint = *CMeshRoad::GetInstance()->GetArrayRP();

	// ベクターを必要なサイズに調整
	int nSize = pRoadPoint.size();
	m_vPos.resize(nSize);

	for (int i = 0; i < nSize; i++)
	{
		m_vPos[i] = pRoadPoint[i].pos;
	}

	// 中心スプラインの初期化
	if (m_pSpline != nullptr)
		m_pSpline->Init(m_vPos);

	// 初期スピードの計算
	m_fSpeedDefault = SPEED_DEFAULT_CHASE;
	CalcSpeed(pEnemy);

	m_nNumShuriken = MAX_SHURIKEN;

	pEnemy->SetPosition(D3DXVECTOR3(pRoadPoint[0].pos.x, pRoadPoint[0].pos.y, pRoadPoint[0].pos.z));

	// でかい手裏剣の生成
	CreateBigShuriken();
}

//=====================================================
// 終了
//=====================================================
void CEnemyBehaviourChasePlayer::Uninit(CEnemy *pEnemy)
{
	if (m_pSpline != nullptr)
	{
		m_pSpline = nullptr;
	}

	m_pFlashEffect = nullptr;
}

//=====================================================
// 更新
//=====================================================
void CEnemyBehaviourChasePlayer::Update(CEnemy *pEnemy)
{
	
	// 状態の管理
	ManageState(pEnemy);

	// 死亡しているか取得
	bool bDeath = pEnemy->IsDeath();

	// 死亡していた場合処理を抜ける
	if (bDeath)
		return;

	// 位置の補間
	InterpolatePosition(pEnemy);

	// 手裏剣の追従
	FollowBigShuriken(pEnemy);

	// エフェクトの追従
	ThrowEffect(pEnemy);
}

//=====================================================
// 状態の管理
//=====================================================
void CEnemyBehaviourChasePlayer::ManageState(CEnemy *pEnemy)
{
	switch (m_state)
	{
	case CEnemyBehaviourChasePlayer::STATE_CHASE:	// 追跡状態
	{
		// 速度を追跡用に加速
		CPlayer *pPlayer = CPlayer::GetInstance();

		m_fSpeedDefault += (pPlayer->GetSpeed() * 4.0f - m_fSpeedDefault) * 0.1f;

		CalcSpeed(pEnemy);

		// プレイヤーの前に出たかの判定
		bool bFront = CollidePlayerFront(pEnemy);

		if (!bFront)
		{// プレイヤーの前に出たら攻撃を開始
			m_state = STATE_ATTACK;
		}

		CDebugProc::GetInstance()->Print("\n敵[追跡状態]");
	}
		break;
	case CEnemyBehaviourChasePlayer::STATE_ATTACK:	// 攻撃状態
	{
		// 速度をプレイヤーに合わせる
		CPlayer *pPlayer = CPlayer::GetInstance();

		if (CollidePlayerFront(pEnemy))
		{
			m_fSpeedDefault += (pPlayer->GetSpeed() * 0.5f - m_fSpeedDefault) * 0.1f;

			CDebugProc::GetInstance()->Print("\n敵減速中");

			// 一定時間ごとに手裏剣を投げる
			float fDeltaTime = CManager::GetDeltaTime();
			m_fTimerAttack += fDeltaTime;

			if (TIME_THROW_SHURIKEN <= m_fTimerAttack)
			{
				// タイマーリセット
				m_fTimerAttack = 0.0f;

				// 手裏剣をすべて投げたら逃げる
				if (m_nNumShuriken <= 0)
				{
					m_state = STATE_ESCAPE;
					D3DXVECTOR3 pos = pEnemy->GetMtxPos(1);
					MyEffekseer::CreateEffect(CEffekseer::TYPE::TYPE_ENGINE, pos);
					break;
				}
					
				// 手裏剣を投げる
				ThrowShuriken(pEnemy);

				m_nNumShuriken--;
			}
		}
		else
		{
			m_fSpeedDefault += (pPlayer->GetSpeed() * 1.5f - m_fSpeedDefault) * 0.1f;

			CDebugProc::GetInstance()->Print("\n敵加速中");
		}

		// スピードの計算
		CalcSpeed(pEnemy);

		CDebugProc::GetInstance()->Print("\n敵[攻撃状態]");
	}
		break;
	case CEnemyBehaviourChasePlayer::STATE_ESCAPE:	// 逃走状態
	{
		m_fSpeedDefault += SPEED_ESCAPE;

		// スピードの計算
		CalcSpeed(pEnemy);

		// 一定時間経つと逃げる
		float fDeltaTime = CManager::GetDeltaTime();
		m_fTimerAttack += fDeltaTime;

		if (TIME_ESCAPE <= m_fTimerAttack)
		{
			pEnemy->Uninit();
			D3DXVECTOR3 pos = pEnemy->GetMtxPos(1);
			MyEffekseer::CreateEffect(CEffekseer::TYPE::TYPE_SMOKE, pos);
			return;
		}

		CDebugProc::GetInstance()->Print("\n敵逃走状態");
	}
		break;
	default:
		break;
	}
}

//=====================================================
// プレイヤーの前に出た判定
//=====================================================
bool CEnemyBehaviourChasePlayer::CollidePlayerFront(CEnemy *pEnemy)
{
	CPlayer *pPlayer = CPlayer::GetInstance();

	if (pPlayer == nullptr)
		return false;

	D3DXVECTOR3 posPlayer = pPlayer->GetPosition();
	D3DXVECTOR3 posEnemy = pEnemy->GetPosition();

	// 前方ベクトルの垂直に線分を生成
	D3DXVECTOR3 vecForward = posEnemy - pEnemy->GetForward() * LENGTH_PLAYER_FRONT;

	CEffect3D::Create(vecForward, 100.0f, 5, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

	D3DXVECTOR3 vec1 = universal::Vec3Cross(pEnemy->GetForward(), D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	D3DXVECTOR3 vec2 = -universal::Vec3Cross(pEnemy->GetForward(), D3DXVECTOR3(0.0f, 1.0f, 0.0f));

	D3DXVec3Normalize(&vec1, &vec1);
	D3DXVec3Normalize(&vec2, &vec2);

	vec1 = vec1 * 1000.0f + vecForward;
	vec2 = vec2 * 1000.0f + vecForward;

	CEffect3D::Create(vec1, 100.0f, 5, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	CEffect3D::Create(vec2, 100.0f, 5, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

	float fRate;

	bool bHit = universal::IsCross(posPlayer, vec1, vec2, &fRate);

	CDebugProc::GetInstance()->Print("\n追い越し判定[%d][%d]", bHit);

	if (!bHit)
		return false;

	//if (fRate > 1.0f || fRate < 0.0f)
		//return false;

	return true;
}

//=====================================================
// 位置の補間
//=====================================================
void CEnemyBehaviourChasePlayer::InterpolatePosition(CEnemy *pEnemy)
{
	D3DXVECTOR3 pos = pEnemy->GetPosition();
	int nIdxSpline = pEnemy->GetIdxSpline();

	if (m_fRate >= 1.0f)
	{
		nIdxSpline++;

		// サイズを超えたら戻す
		if (nIdxSpline >= (int)m_vPos.size())
			nIdxSpline = 1;

		// スピードの計算
		CalcSpeed(pEnemy);

		m_fRate -= 1.0f;
	}

	m_fRate += m_fSpeed;

	// 位置補間
	if (m_pSpline != nullptr)
	{
		if (m_pSpline->IsEmpty())
		{// スプラインが空だった時の再取得
			CMeshRoad *pMesh = CMeshRoad::GetInstance();

			if (pMesh == nullptr)
				return;

			m_pSpline = pMesh->GetCenterSpline();

			if (m_pSpline->IsEmpty())
				return;	// 再取得しても空だったら処理を通さない
		}

		pos = m_pSpline->Interpolate(m_fRate, nIdxSpline);
	}

	pEnemy->SetIdxSpline(nIdxSpline);

	// 位置と向き更新
	pEnemy->SetPosition(pos);

	// 向きの制御
	ControllRot(pEnemy);

	// デバッグ表示
	Debug(pEnemy);
}

//=====================================================
// スピードの計算
//=====================================================
void CEnemyBehaviourChasePlayer::CalcSpeed(CEnemy *pEnemy)
{
	int nIdxSpline = pEnemy->GetIdxSpline();

	if (m_pSpline == nullptr)
		return;

	if (m_pSpline->IsEmpty())
		return;

	float fLength = m_pSpline->GetLength(nIdxSpline, 50);

	if (fLength == 0)
		return;	// 0割り防止

	m_fSpeed = m_fSpeedDefault / fLength;
}

//=====================================================
// 向きの制御
//=====================================================
void CEnemyBehaviourChasePlayer::ControllRot(CEnemy *pEnemy)
{
	int nIdxSpline = pEnemy->GetIdxSpline();

	D3DXVECTOR3 pos = pEnemy->GetPosition();
	D3DXVECTOR3 rot = pEnemy->GetRotation();

	// 次のデータ点の方を向く
	universal::FactingRotTarget(&rot, pos, m_vPos[nIdxSpline], 0.05f);

	pEnemy->SetRotation(D3DXVECTOR3(0.0f, rot.y, 0.0f));
}

//=====================================================
// でかい手裏剣の生成
//=====================================================
void CEnemyBehaviourChasePlayer::CreateBigShuriken(void)
{
	if (m_pBigShuriken != nullptr)
	{
		m_pBigShuriken->Uninit();
		m_pBigShuriken = nullptr;
	}

	if(m_nNumShuriken > 0)
	m_pBigShuriken = MyEffekseer::CreateEffect(CEffekseer::TYPE::TYPE_WINDSHURIKEN, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
}

//=====================================================
// でかい手裏剣の追従
//=====================================================
void CEnemyBehaviourChasePlayer::FollowBigShuriken(CEnemy *pEnemy)
{
	if (m_pBigShuriken == nullptr)
		return;

	D3DXVECTOR3 posHand = pEnemy->GetMtxPos(5);

	m_pBigShuriken = m_pBigShuriken->FollowPosition(posHand);

	if (m_pBigShuriken == nullptr)
	{
		CreateBigShuriken();
	}
}

//=====================================================
// エフェクトの追従
//=====================================================
void CEnemyBehaviourChasePlayer::ThrowEffect(CEnemy *pEnemy)
{
	if (m_pFlashEffect == nullptr)
		return;

	m_pFlashEffect = m_pFlashEffect->FollowPosition(pEnemy->GetMtxPos(5));
}

//=====================================================
// 手裏剣投げモーションが始まる
//=====================================================
void CEnemyBehaviourChasePlayer::ThrowShuriken(CEnemy *pEnemy)
{
	D3DXVECTOR3 pos = pEnemy->GetMtxPos(5);

	CEnemy::SFragMotion *pFragMotion = pEnemy->GetFragMotion();

	pFragMotion->bAttack = true;
	
	m_pFlashEffect = MyEffekseer::CreateEffect(CEffekseer::TYPE_FLASH00,pos);
}

//=====================================================
// デバッグ表示
//=====================================================
void CEnemyBehaviourChasePlayer::Debug(CEnemy *pEnemy)
{
	// デバッグプロック取得
	CDebugProc* pDebugProc = CDebugProc::GetInstance();

	if (pDebugProc == nullptr)
		return;

	CEffect3D::Create(pEnemy->GetPosition(), 50.0f, 5, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	pDebugProc->Print("\n敵のバイクの位置[%f,%f,%f]", pEnemy->GetPosition().x, pEnemy->GetPosition().y, pEnemy->GetPosition().z);
	pDebugProc->Print("\n敵の速度[%f]", m_fSpeedDefault);
	pDebugProc->Print("\n残りの手裏剣の数[%d]", m_nNumShuriken);
}