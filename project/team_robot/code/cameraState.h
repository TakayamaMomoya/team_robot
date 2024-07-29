//*****************************************************
//
// カメラステイト[cameraState.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _CAMERASTATE_H_
#define _CAMERASTATE_H_

//****************************************************
// 前方宣言
//****************************************************
class CCamera;

//****************************************************
// クラスの定義
//****************************************************
// 基底クラス
class CCameraState
{
public:
	CCameraState() {};
	virtual void Update(CCamera *pCamera) = 0;

private:
};

// プレイヤー追従
class CFollowPlayer : public CCameraState
{
public:
	CFollowPlayer();
	void Update(CCamera *pCamera) override;

private:
	float m_fTimerPosR;
	float m_fLengthPosR;
	D3DXVECTOR3 m_rotROld;
	bool m_bDebug;
};

// 操作
class CMoveControl : public CCameraState
{
public:
	CMoveControl() {};
	void Update(CCamera *pCamera) override;

private:
	bool m_bAbove = false;	// 上空視点かどうか
};

// タイトル
class CCameraStateTitle : public CCameraState
{
public:
	CCameraStateTitle() {};
	void Update(CCamera* pCamera) override;

private:

};

// タイトルでプレイヤーを追従
class CCameraStateFollowPlayerTitle : public CCameraState
{
public:
	CCameraStateFollowPlayerTitle() {};
	void Update(CCamera* pCamera) override;

private:

};

// リザルト
class CCameraStateResult : public CCameraState
{
public:
	CCameraStateResult();
	void Update(CCamera* pCamera) override;

private:

};

#endif