//*****************************************************
//
// エネミーの処理[enemy.h]
// Author:大原怜将
//
//*****************************************************
#ifndef _ENEMY_H_
#define _ENEMY_H_

//*****************************************************
// インクルード
//*****************************************************
#include "motion.h"
#include "enemyBehaviour.h"

//*****************************************************
// 前方定義
//*****************************************************
class CBehaviourEnemy;
class COrbit;

//*****************************************************
// クラスの定義
//*****************************************************
class CEnemy : public CMotion
{
public:
	// 列挙型定義
	enum MOTION
	{// モーション
		MOTION_NEUTRAL = 0,	// 待機状態
		MOTION_ATTACK,	// 投げモーション
		MOTION_MAX
	};

	// 構造体定義
	struct SFragMotion
	{// モーションフラグ
		bool bAttack;	// 攻撃
	};

	CEnemy(int nPriority = 4);	// コンストラクタ
	~CEnemy();	                // デストラクタ

	HRESULT Init(void);         // 初期化処理
	void Uninit(void);          // 終了処理
	void Update(void);          // 更新処理
	void Draw(void);            // 描画処理
	SFragMotion *GetFragMotion(void) { return &m_fragMotion; }	// モーションフラグの取得

	// 変数取得・設定関数
	int GetIdxSpline(void) { return m_info.nIdxSpline; }	// スプライン番号
	void SetIdxSpline(int nIdx) { m_info.nIdxSpline = nIdx; }

	// 静的メンバ関数
	static CEnemy* Create();    // 生成処理

private:
	enum STATE
	{// 状態
		STATE_NONE = 0,	         // 何でもない状態
		STATE_NORMAL,	         // 通常状態
		STATE_MAX
	};
	struct SInfo
	{// 情報
		STATE state;             // 状態[
		int nIdxSpline;	// スプラインの番号
		COrbit* pOrbit;	// テールランプの軌跡
	};

	// メンバ関数
	void ManageMotion(void);	// モーションの管理
	void ReleaseAllBehaviour(void);	// 全てのビヘイビアの解放

	// メンバ変数
	SInfo m_info;	// 情報
	SFragMotion m_fragMotion;	// モーションフラグ
	std::list<CEnemyBehaviour*> m_listBehaviour;	// ビヘイビアのリスト
};

#endif