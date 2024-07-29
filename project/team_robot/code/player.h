//*****************************************************
//
// プレイヤーの処理[player.cpp]
// Author:髙山桃也
//
//*****************************************************

#ifndef _PLAYER_H_
#define _PLAYER_H_

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
class CMeshRoad;
class COrbit;
class CEffekseerEffect;

//*****************************************************
// クラスの定義
//*****************************************************
class CPlayer : public CMotion
{
public:
	// 列挙型定義
	enum E_Motion
	{// モーション
		MOTION_NEUTRAL = 0,	// 待機
		MOTION_WALK_FRONT,	// 前歩
		MOTION_MAX
	};

	// 構造体定義
	struct S_Param
	{
		string m_stPathBody = "";	// 見た目のバス
	};

	CPlayer(int nPriority = 4);	// コンストラクタ
	~CPlayer();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(void);	// 更新
	void Draw(void);	// 描画
	void Hit(float fDamage);	// ヒット処理

	// 静的メンバ関数
	static CPlayer *Create(void);	// 生成処理
	static CPlayer *GetInstance(void) { return s_pPlayer; }	// インスタンスの取得

private:
	// 構造体定義
	struct S_FragMotion
	{// モーションのフラグ
		bool bMove = false;	// 移動
	};

	// メンバ関数
	void Load(void);	// パラメーター読込
	void Input(void);	// 入力
	void InputMove(void);	// 移動の入力
	void Event(EVENT_INFO *pEventInfo);	// イベントの処理
	void Debug(void);	// デバッグ処理

	// メンバ変数
	S_FragMotion m_fragMotion;	// モーションのフラグ
	S_Param m_param;	// パラメーター

	// 静的メンバ変数
	static CPlayer *s_pPlayer;	// 自身のポインタ
};

namespace Player
{
const D3DXVECTOR3 DEFAULT_POS = { 0.0f,0.0f,0.0f };	// 初期位置
const D3DXVECTOR3 DEFAULT_ROT = { 0.0f,0.0f,0.0f };	// 初期向き
}

#endif