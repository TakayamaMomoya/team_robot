//*****************************************************
//
// モブファクトリー[mobFactory.cpp]
// Author:��山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "mobFactory.h"
#include "mob.h"

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
CMobFactory *CMobFactory::s_pMobFactory = nullptr;	// 自身のポインタ

//*****************************************************
// 定数定義
//*****************************************************
namespace
{

}

//=====================================================
// 優先順位を決めるコンストラクタ
//=====================================================
CMobFactory::CMobFactory(int nPriority)
{

}

//=====================================================
// デストラクタ
//=====================================================
CMobFactory::~CMobFactory()
{

}

//=====================================================
// 生成処理
//=====================================================
CMobFactory *CMobFactory::Create(void)
{
	if (s_pMobFactory != nullptr)
		return s_pMobFactory;

	s_pMobFactory = new CMobFactory;

	if (s_pMobFactory == nullptr)
		return nullptr;

	s_pMobFactory->Init();

	return s_pMobFactory;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CMobFactory::Init(void)
{
	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CMobFactory::Uninit(void)
{
	Release();
}

//=====================================================
// 更新処理
//=====================================================
void CMobFactory::Update(void)
{

}

//=====================================================
// 描画処理
//=====================================================
void CMobFactory::Draw(void)
{

}

//=====================================================
// モブの生成
//=====================================================
CMob *CreateMob(void)
{
	CMob *pMob = nullptr;

	pMob = new CMob;

	return pMob;
}

namespace MobFactroy
{
//=====================================================
// モブの生成
//=====================================================
CMob *CreateMob(CMob::E_Faction fanction)
{
	CMobFactory *pMobfactory = CMobFactory::GetInstance();

	if (pMobfactory == nullptr)
		return nullptr;

	 CMob *pMob = nullptr;

	 switch (fanction)
	 {
	 case CMob::E_Faction::FACTION_ALLY:	// 味方の生成
		 break;
	 case CMob::E_Faction::FACTION_ENEMY:	// 敵の生成
		 break;
	 default:
		 throw std::invalid_argument("Unknown MobType");
		 break;
	 }

	 if (pMob != nullptr)
		 pMob->Init();
	
	 return pMob;
}
}