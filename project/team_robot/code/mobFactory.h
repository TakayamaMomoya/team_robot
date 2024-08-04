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

//*****************************************************
// 前方宣言
//*****************************************************
class CMob;

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
	CMob *CreateMob(void);	// モブの生成

	// 静的メンバ関数
	static CMobFactory *Create(void);	// 生成処理
	static CMobFactory *GetInstance(void) { return s_pMobFactory; }	// インスタンス取得

private:
	// メンバ関数

	// メンバ変数
	static CMobFactory *s_pMobFactory;	// 自身のポインタ
};

namespace MobFactroy
{
CMob *CreateMob(void);	// モブの生成(ショートカット関数)
}

#endif