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
	enum MOTION
	{
		MOTION_NEUTRAL = 0,	// 待機
		MOTION_WALK_FRONT,	// 前歩
		MOTION_MAX
	};
	enum MOTION_NINJA
	{
		MOTION_NINJA_NEUTRAL = 0,	// 待機
		MOTION_NINJA_SLASHDOWN,	// 切り下ろし
		MOTION_NINJA_SLASHUP,	// 切り上げ
		MOTION_NINJA_MAX
	};
	enum STATE
	{
		STATE_NONE = 0,	// 何でもない状態
		STATE_NORMAL,	// 通常状態
		STATE_MAX
	};
	struct SParam
	{
		char aPathBody[MAX_STRING];	// 見た目のパス
		float fSpeedMax;	// 最大速度
		float fSpeedMaxInitial;	// 初期最大速度
		float fFactAccele;	// 加速係数
		float fFactAttenu;	// 減速係数
		float fFactBrake;	// ブレーキ減速係数
		float fAngleMaxCurve;	// カーブの最大角度
		D3DXVECTOR3 sizeCollider;	// コライダーのサイズ
	};

	CPlayer(int nPriority = 4);	// コンストラクタ
	~CPlayer();	// デストラクタ

	static CPlayer *Create(void);
	static CPlayer *GetInstance(void) { return m_pPlayer; }
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Hit(float fDamage);
	SParam GetParam(void) { return m_param; }
	void SetParam(SParam param) { m_param = param; }
	CBlockGrab *GetBlock(void) { return m_info.pBlockGrab; }
	CMotion *GetNInjaBody(void) { return m_pPlayerNinja; }	// 上に乗っている忍者の取得
	bool IsDrift(void) { return m_info.pBlockGrab != nullptr; }	// ドリフトしているかどうか

	// 変数取得・設定関数
	bool IsEnableInput(void) { return m_info.bEnableInput; }	// 入力有効フラグ
	void SetEnableInput(bool bEnable) { m_info.bEnableInput = bEnable; }
	float GetSpeed(void) { return m_info.fSpeed; }	// スピード
	void SetSpeed(float fSpeed) { m_info.fSpeed = fSpeed; }

private:
	struct SFragMotion
	{
		bool bMove;	// 移動
	};
	struct SFragNinja
	{
		bool bNeutral;	// 待機モーション
		bool bSlashDown;	// 切り下ろし
		bool bSlashUp;	// 切り上げ
	};
	struct SInfo
	{
		STATE state;	// 状態
		float fSpeed;	// 現在の速度
		float fSpeedDest;	// 目標速度
		float fAngleHandle;	// ハンドルをきっている値
		float fTimerFlip;	// ドリフトカウンター
		float fAngleDrift;	// ドリフト時の角度倍率の値
		int nCntFlip;	// 弾きカウンター
		CBlockGrab *pBlockGrab;	// 掴んでいるブロック
		CPolygon3D *pRoap;
		float fCntAngle;	// アングルのカウンター
		bool bGrabOld;
		bool bManual;		// マニュアル操作
		bool bAir;			// 空中にいるかどうか
		bool bEnableInput;	// 入力有効フラグ
		float fLengthDrift;
		float fTimerDriftChange;
		float fSizeBlurDrift;	// ドリフト時のブラーの強さ
		float fDesityBlurDrift;	// ドリフト時のブラーの濃さ
		COrbit* pOrbitLamp;	// テールランプの軌跡
		COrbit* pOrbitRope;	// テールランプの軌跡
		D3DXCOLOR orbitColor;	// テールランプの軌跡
		D3DXVECTOR3 rotDriftStart;	// ドリフトスタート時の角度
		float rotDriftDest;	// ドリフト終了時の角度補正値
	};

	void Load(void);
	void Input(void);
	void Collision(void);
	void InputMove(void);
	void InputWire(void);
	void InputKatana(void);
	void ManageKanataAtttack(void);	// 刀の攻撃管理
	void JudgeChangeDrift(float fAngle, float fAngleDiff, float fLength);	// ドリフト変化の判定
	void ControlRoap(void);	// ロープの制御
	void SarchGrab(void);	// 掴むブロックの探知
	void ManageRotateGrab(float fAngleDiff);	// 掴んでいるときの回転制御
	void JudgeRemoveWire(float fLength);	// ワイヤーを外すかの判定
	void RemoveWire(void);	// ワイヤーを外す処理
	void LimitDrift(float fLength);
	void ManageSpeed(void);
	void ManageSpeedBlur(void);	// スピードによるブラーの管理
	void ManageState(void);
	void ManageMotion(void);
	void ManageMotionNinja(void);	// 忍者のモーション管理
	void ManageSlashEffect(void);	// 斬撃エフェクトの管理
	void Event(EVENT_INFO *pEventInfo);
	void Debug(void);

	// メンバ変数
	SInfo m_info;	// 自身の情報
	SParam m_param;	// パラメータ情報
	SFragMotion m_fragMotion;	// モーションのフラグ
	SFragNinja m_fragNinja;	// 忍者のモーションフラグ
	CMotion* m_pPlayerNinja;  // バイクに乗っている忍者
	bool m_bMove = false;     // 移動しているかどうか
	bool m_bDrift = false;    // ドリフトしているかどうか
	std::list<CEffekseerEffect*> m_listSlashEffect;	// 斬撃エフェクトのリスト

	// 静的メンバ変数
	static CPlayer *m_pPlayer;	// 自身のポインタ
};

namespace Player
{
const D3DXVECTOR3 DEFAULT_POS = { 8500.1f,800.0f,-28621.6f };	// 初期位置
const D3DXVECTOR3 DEFAULT_ROT = { 0.0f,2.0f,0.0f };	// 初期向き
}

#endif