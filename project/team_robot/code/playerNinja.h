//*****************************************************
//
// プレイヤー忍者[playerNinja.cpp]
// Author:髙山桃也
//
//*****************************************************
#ifndef _PLAYERNINJA_H_
#define _PLAYERNINJA_H_

//*****************************************************
// インクルード
//*****************************************************
#include "motion.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CBlockGrab;
class CPolygon3D;
class CPlayerNinja;
class CEffekseerEffect;

//*****************************************************
// クラスの定義
//*****************************************************
class CPlayerNinja : public CMotion
{
public:
	CPlayerNinja(int nPriority = 4);	// コンストラクタ
	~CPlayerNinja();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(void);	// 更新
	void Draw(void);	// 描画

	// 静的メンバ関数
	static CPlayerNinja *Create(void);	// 生成

private:
	// メンバ関数
	void Event(EVENT_INFO *pEventInfo);	// イベント
	void ManagekatanaCollision(D3DXVECTOR3 pos);	// 刀の判定の管理
};

#endif