//*****************************************************
//
// エネミーの処理[enemy.cpp]
// Author:大原怜将
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "enemy.h"
#include "debugproc.h"
#include "effect3D.h"
#include "shuriken.h"
#include "player.h"
#include "orbit.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
	const float SPEED_DEFAULT = 50.0f;
}

//=====================================================
// コンストラクタ
//=====================================================
CEnemy::CEnemy(int nPriority)
{

}

//=====================================================
// デストラクタ
//=====================================================
CEnemy::~CEnemy()
{

}

//=====================================================
// 生成処理
//=====================================================
CEnemy* CEnemy::Create()
{
	CEnemy *pEnemy = nullptr;

	if (pEnemy == nullptr)
	{
		pEnemy = new CEnemy;

		if (pEnemy != nullptr)
		{
			// 初期化
			pEnemy->Init();
		}
	}

	return pEnemy;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CEnemy::Init(void)
{
	m_info.nIdxSpline = 1;

	// 継承クラスの初期化
	CMotion::Init();

	// モデルの設定
	CMotion::Load("data\\MOTION\\motionEnemy.txt");

	CEnemyBehaviour *pBehaviour = new CEnemyBehaviourChasePlayer;

	pBehaviour->Init(this);

	m_listBehaviour.push_back(pBehaviour);

	m_info.pOrbit = COrbit::Create(GetMatrix(), D3DXVECTOR3(20.0f, 220.0f, -80.0f), D3DXVECTOR3(-20.0f, 220.0f, -80.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 60);

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CEnemy::Uninit(void)
{
	m_info.pOrbit->SetEnd(true);

	// 全ビヘイビア解放
	ReleaseAllBehaviour();

	// 継承クラスの終了
	CMotion::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void CEnemy::Update(void)
{
	// 継承クラスの更新
	CMotion::Update();

	// ビヘイビアの更新
	for (auto it : m_listBehaviour)
	{
		it->Update(this);

		if (m_listBehaviour.empty())
			break;
	}

	if (m_info.pOrbit != nullptr)
		m_info.pOrbit->SetOffset(GetMatrix(), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), m_info.pOrbit->GetID());

	// モーションの管理
	ManageMotion();
}

//=====================================================
// モーションの管理
//=====================================================
void CEnemy::ManageMotion(void)
{
	int nMotion = GetMotion();
	bool bFinish = IsFinish();

	if (m_fragMotion.bAttack)
	{// 攻撃モーション
		if (nMotion != MOTION::MOTION_ATTACK)
		{
			SetMotion(MOTION::MOTION_ATTACK);
		}

		if (bFinish)
		{
			m_fragMotion.bAttack = false;

			CShuriken::Create(GetMtxPos(5), GetForward());
		}
	}
	else
	{// ニュートラルモーション
		if (nMotion != MOTION::MOTION_NEUTRAL)
		{
			SetMotion(MOTION::MOTION_NEUTRAL);
		}
	}
}

//=====================================================
// 全ビヘイビア解放
//=====================================================
void CEnemy::ReleaseAllBehaviour(void)
{
	for (auto it : m_listBehaviour)
	{
		it->Uninit(this);

		delete it;
	}

	m_listBehaviour.clear();
}

//=====================================================
// 描画処理
//=====================================================
void CEnemy::Draw(void)
{
	// 継承クラスの描画
	CMotion::Draw();
}