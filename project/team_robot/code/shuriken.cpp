//*****************************************************
//
// 手裏剣の処理[shuriken.h]
// Author:大原怜将
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "shuriken.h"
#include "MyEffekseer.h"
#include "model.h"
#include "player.h"
#include "manager.h"
#include "effect3D.h"
#include "MyEffekseer.h"
#include "debugproc.h"

//*****************************************************
// 静的メンバ変数
//*****************************************************
std::list<CShuriken*> CShuriken::m_aShuriken;

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
	const float MOVE_SPEED = 120.0f;  // スピード
	const float LIFE_DEFAULT = 5.0f;	// デフォルトの寿命
}

//=====================================================
// コンストラクタ
//=====================================================
CShuriken::CShuriken() : m_fLife(0.0f), m_pWindEffect(nullptr)
{
	
}

//=====================================================
// デストラクタ
//=====================================================
CShuriken::~CShuriken()
{

}

//=====================================================
// 生成処理
//=====================================================
CShuriken* CShuriken::Create(D3DXVECTOR3 pos, D3DXVECTOR3 vecForward)
{
	CShuriken *pShuriken = nullptr;
	pShuriken = new CShuriken;

	if (pShuriken != nullptr)
	{
		pShuriken->SetPosition(pos);
		pShuriken->CalcMove(vecForward);
		pShuriken->Init();
	}

	return pShuriken;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CShuriken::Init(void)
{
	CObjectX::Init();

	// モデル読込
	int nIdx = CModel::Load("data\\MODEL\\Player\\shuriken.x");
	BindModel(nIdx);

	m_aShuriken.push_back(this);

	m_fLife = LIFE_DEFAULT;

	m_pWindEffect = MyEffekseer::CreateEffect(CEffekseer::TYPE::TYPE_THROWSHURIKEN, GetPosition());

	return S_OK;
}

//=====================================================
// 移動量の計算
//=====================================================
void CShuriken::CalcMove(D3DXVECTOR3 vecForward)
{
	// プレイヤー取得
	CPlayer* pPlayer = CPlayer::GetInstance();

	if (pPlayer == nullptr)
		return;

	// プレイヤーの移動ベクトル取得
	float fSpeedPlayer = pPlayer->GetSpeed();
	D3DXVECTOR3 moveEnemy = vecForward * fSpeedPlayer;

	// プレイヤーの頭のマトリックス取得
	D3DXVECTOR3 posHeadPlayer = pPlayer->GetNInjaBody()->GetMtxPos(2);

	D3DXVECTOR3 pos = GetPosition();
	D3DXVECTOR3 vecDiff = posHeadPlayer - pos;

	universal::VecConvertLength(&vecDiff, MOVE_SPEED);

	D3DXVECTOR3 move = moveEnemy + vecDiff;

	SetMove(move);
}

//=====================================================
// 終了処理
//=====================================================
void CShuriken::Uninit(void)
{
	if (m_pWindEffect != nullptr)
	{
		m_pWindEffect->Uninit();
		m_pWindEffect = nullptr;
	}

	CObjectX::Uninit();

	m_aShuriken.remove(this);
}

//=====================================================
// 更新処理
//=====================================================
void CShuriken::Update(void)
{
	CObjectX::Update();

	// 移動量の反映
	D3DXVECTOR3 pos = GetPosition();
	D3DXVECTOR3 move = GetMove();

	pos += move;

	SetPosition(pos);

	CEffect3D::Create(pos, 100.0f, 10, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

	// 寿命の減少
	m_fLife -= CManager::GetDeltaTime();

	if (m_fLife <= 0.0f)
	{
		m_fLife = 0.0f;

		Uninit();

		return;
	}

	if (m_pWindEffect != nullptr)
	{
		m_pWindEffect = m_pWindEffect->FollowPosition(GetPosition());
	}

#ifdef _DEBUG
	// デバッグ処理
	Debug();
#endif
}

//=====================================================
// 描画処理
//=====================================================
void CShuriken::Draw(void)
{
	// 継承クラスの初期化
	CObjectX::Draw();
}

//=====================================================
// ヒット処理
//=====================================================
void CShuriken::Hit(float fDamage)
{
	// 弾いた時のエフェクトを出す
	CEffekseer* pEffekseer = CManager::GetMyEffekseer();

	MyEffekseer::CreateEffect(CEffekseer::TYPE_PARRY, GetPosition(), GetRotation());

	// 終了する
	Uninit();
}

//=====================================================
// デバッグ処理
//=====================================================
void CShuriken::Debug(void)
{
	// デバッグプロック取得
	CDebugProc* pDebugProc = CDebugProc::GetInstance();

	if (pDebugProc == nullptr)
		return;

	pDebugProc->Print("\n手裏剣の移動[%f,%f,%f]", GetMove().x, GetMove().y, GetMove().z);
	pDebugProc->Print("\n手裏剣の位置[%f,%f,%f]", GetPosition().x, GetPosition().y, GetPosition().z);
}