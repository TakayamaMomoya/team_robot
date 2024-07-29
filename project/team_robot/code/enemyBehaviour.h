//*****************************************************
//
// エネミービヘイビア[enemyBehaviour.h]
// Author:髙山桃也
//
//*****************************************************
#ifndef _ENEMYBEHAVIOUR_H_
#define _ENEMYBEHAVIOUR_H_

//*****************************************************
// インクルード
//*****************************************************

//*****************************************************
// 前方定義
//*****************************************************
class CEnemy;
class CCutMullSpline;
class CObjectX;
class CEffekseerEffect;

//*****************************************************
// クラスの定義
//*****************************************************
class CEnemyBehaviour
{// ビヘイビアの基底クラス
public:
	CEnemyBehaviour() {};	// コンストラクタ
	virtual ~CEnemyBehaviour() {};	 // デストラクタ

	// メンバ関数
	virtual void Init(CEnemy *pEnemy) = 0;	// 初期化処理
	virtual void Uninit(CEnemy *pEnemy) = 0;	// 終了処理
	virtual void Update(CEnemy *pEnemy) = 0;	// 更新処理

private:
};

class CEnemyBehaviourChasePlayer : public CEnemyBehaviour
{// プレイヤーの追跡
public:
	CEnemyBehaviourChasePlayer();	// コンストラクタ
	virtual ~CEnemyBehaviourChasePlayer();	 // デストラクタ

	// メンバ関数
	void Init(CEnemy *pEnemy) override;	// 初期化処理
	void Uninit(CEnemy *pEnemy) override;	// 終了処理
	void Update(CEnemy *pEnemy) override;	// 更新処理

private:
	// 列挙型定義
	enum STATE
	{
		STATE_NONE = 0,	// 何もしていない状態
		STATE_CHASE,	// 追跡状態
		STATE_ATTACK,	// 攻撃状態
		STATE_ESCAPE,   // 逃走状態
		STATE_MAX
	};

	// メンバ関数
	void ManageState(CEnemy *pEnemy);	// 状態の管理
	bool CollidePlayerFront(CEnemy *pEnemy);	// プレイヤーの前に出た判定
	void CalcSpeed(CEnemy *pEnemy);	// スピードの計算
	void InterpolatePosition(CEnemy *pEnemy);	// 位置の補間
	void ControllRot(CEnemy *pEnemy);	// 向きの制御
	void CreateBigShuriken(void);	// でかい手裏剣の生成
	void FollowBigShuriken(CEnemy *pEnemy);	// でかい手裏剣の追従
	void ThrowShuriken(CEnemy *pEnemy);	// 手裏剣を投げる
	void ThrowEffect(CEnemy *pEnemy);	// エフェクトの追従
	void Debug(CEnemy *pEnemy);	// デバッグ表示

	// メンバ変数
	STATE m_state;	// 状態
	CCutMullSpline *m_pSpline;	// スプライン
	std::vector<D3DXVECTOR3> m_vPos;	// データ点のベクター
	float m_fSpeed;	// 速度
	float m_fSpeedDefault;	// 基準にする速度
	float m_fRate;
	float m_fRateOld;	// 前回のスプラインの割合
	float m_fTimerAttack;	// 攻撃タイマー
	int m_nNumShuriken;  // 投げてくる手裏剣の数
	CEffekseerEffect *m_pBigShuriken;	// 投げる前に手元に出す大きな手裏剣
	CEffekseerEffect *m_pFlashEffect;	// 投げる前の閃光のエフェクト
};

#endif