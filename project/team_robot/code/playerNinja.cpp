//*****************************************************
//
// プレイヤー忍者[playerNinja.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "playerNinja.h"
#include "player.h"
#include "shuriken.h"
#include "debugproc.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const char* PATH_BODY = "data\\MOTION\\motionPlayer.txt";	// 自身のモーションデータ
const float RANGE_GUARD = 500.0f;	// ガード範囲
}

//=====================================================
// 優先順位を決めるコンストラクタ
//=====================================================
CPlayerNinja::CPlayerNinja(int nPriority)
{

}

//=====================================================
// デストラクタ
//=====================================================
CPlayerNinja::~CPlayerNinja()
{

}

//=====================================================
// 生成処理
//=====================================================
CPlayerNinja *CPlayerNinja::Create(void)
{
	CPlayerNinja *pNinja = nullptr;

	pNinja = new CPlayerNinja;

	if (pNinja != nullptr)
	{
		pNinja->Init();
	}

	return pNinja;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CPlayerNinja::Init(void)
{
	// ブラーをかけない設定にする
	EnableBlur(false);

	// 継承クラスの初期化
	CMotion::Init();

	// モデルの設定
	CMotion::Load((char*)PATH_BODY);

	SetMotion(CPlayer::MOTION::MOTION_NEUTRAL);

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CPlayerNinja::Uninit(void)
{
	// 継承クラスの終了
	CMotion::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void CPlayerNinja::Update(void)
{
	CDebugProc::GetInstance()->Print("\n忍者のモーション[%d]", GetMotion());

	CMotion::Update();
}

//=====================================================
// イベントの管理
//=====================================================
void CPlayerNinja::Event(EVENT_INFO *pEventInfo)
{
	int nMotion = GetMotion();

	D3DXVECTOR3 offset = pEventInfo->offset;
	D3DXMATRIX mtxParent;
	D3DXMATRIX mtxPart = GetParts(pEventInfo->nIdxParent)->pParts->GetMatrix();

	universal::SetOffSet(&mtxParent, mtxPart, offset);

	D3DXVECTOR3 pos = { mtxParent._41,mtxParent._42 ,mtxParent._43 };

	if (nMotion == CPlayer::MOTION_NINJA::MOTION_NINJA_SLASHDOWN ||
		nMotion == CPlayer::MOTION_NINJA::MOTION_NINJA_SLASHUP)
	{// 斬撃時
		ManagekatanaCollision(pos);
	}
}

//=====================================================
// 刀の判定の管理
//=====================================================
void CPlayerNinja::ManagekatanaCollision(D3DXVECTOR3 pos)
{
	// 手裏剣リストの取得
	std::list<CShuriken*> listShuriken = CShuriken::GetList();

	for (auto it : listShuriken)
	{
		// 手裏剣との距離を測る
		D3DXVECTOR3 pos = GetMtxPos(2);
		D3DXVECTOR3 posShuriken = it->GetPosition();

		bool bHit = universal::DistCmp(pos, posShuriken, RANGE_GUARD, nullptr);

		if (bHit)
		{// 範囲内なら手裏剣のヒット処理を呼ぶ
			it->Hit(0.0f);	// ダメージ値は使わない
		}
	}
}

//=====================================================
// 描画処理
//=====================================================
void CPlayerNinja::Draw(void)
{
	// 継承クラスの描画
	CMotion::Draw();
}