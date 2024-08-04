//*****************************************************
//
// モブファクトリー[mobFactory.cpp]
// Author:髙山桃也
//
//*****************************************************
#ifndef _MOBFACTORy_H_
#define _MOBFACTORy_H_

//*****************************************************
// インクルード
//*****************************************************
#include "object.h"
#include "mob.h"

//*****************************************************
// クラスの定義
//*****************************************************
class CMobFactory : public CObject
{
public:
	CMobFactory(int nPriority = 4);	// コンストラクタ
	~CMobFactory();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(void);	// 更新
	void Draw(void);	// 描画
	CMob *CreateMob(CMob::E_Faction fanction);	// モブの生成

	// 静的メンバ関数
	static CMobFactory *Create(void);	// 生成処理
	static CMobFactory *GetInstance(void) { return s_pMobFactory; }	// インスタンス取得

private:
	// メンバ変数
	vector<CMob*> m_aAlly;	// 味方のリスト
	vector<CMob*> m_aEnemy;	// 敵のリスト

	// 静的メンバ変数
	static CMobFactory *s_pMobFactory;	// 自身のポインタ
};

namespace MobFactroy
{
CMob *CreateMob(CMob::E_Faction fanction);	// モブの生成(ショートカット関数)
}

#endif