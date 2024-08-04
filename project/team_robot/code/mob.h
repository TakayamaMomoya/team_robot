//*****************************************************
//
// モブの処理[mob.cpp]
// Author:髙山桃也
//
//*****************************************************
#ifndef _MOB_H_
#define _MOB_H_

//*****************************************************
// インクルード
//*****************************************************
#include "motion.h"

//*****************************************************
// 前方宣言
//*****************************************************

//*****************************************************
// クラスの定義
//*****************************************************
class CMob : public CMotion
{
public:
	// 列挙型定義
	enum E_Faction
	{// 勢力
		FACTION_NONE = 0,	// 何でもない
		FACTION_ALLY,	// 仲間
		FACTION_ENEMY,	// 敵
		FACTION_MAX
	};

	CMob(int nPriority = 4);	// コンストラクタ
	~CMob();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(void);	// 更新
	void Draw(void);	// 描画

	// 変数取得・設定関数
	void SetLife(float fLife) { m_fLife = fLife; }	// ライフ
	float GetLife(void) { return m_fLife; }
	void SetFanction(E_Faction fanction) { m_faction = fanction; }	// 勢力
	E_Faction GetFanction(void) { return m_faction; }

	// 静的メンバ関数
	static CMob *Create(void);	// 生成処理

private:
	// メンバ変数
	float m_fLife = 0.0f;	// 体力
	E_Faction m_faction = FACTION_NONE;	// 勢力
};

#endif