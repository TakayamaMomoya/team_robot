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
	void AddAlly(CMob *pMob);	// 味方の追加
	void AddEnemy(CMob *pMob);	// 敵機の追加

	// 変数取得・設定関数
	list<CMob*> GetAlly(void) { return m_aAlly; }	// 味方リスト
	void SetAlly(list<CMob*> list) { m_aAlly = list; }
	list<CMob*> GetEnemy(void) { return m_aEnemy; }	// 敵リスト
	void SetEnemy(list<CMob*> list) { m_aEnemy = list; }

	// 静的メンバ関数
	static CMobFactory *Create(void);	// 生成処理
	static CMobFactory *GetInstance(void) { return s_pMobFactory; }	// インスタンス取得

private:
	// メンバ変数
	list<CMob*> m_aAlly;	// 味方のリスト
	list<CMob*> m_aEnemy;	// 敵のリスト

	// 静的メンバ変数
	static CMobFactory *s_pMobFactory;	// 自身のポインタ
};

namespace MobFactroy
{
CMob *CreateMob(CMob::E_Faction fanction);	// モブの生成(ショートカット関数)
}

#endif