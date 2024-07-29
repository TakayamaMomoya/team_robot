//*****************************************************
//
// カメラの処理[camera.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _CAMERA_H_
#define _CAMERA_H_

//****************************************************
// 前方宣言
//****************************************************
class CCameraState;

//****************************************************
// インクルード
//****************************************************
#include "main.h"

//****************************************************
// マクロ定義
//****************************************************
#define MIN_DRAW		(10.0f)					//描画を開始する距離
#define MAX_DRAW		(700000.0f)				//描画を終了する距離

//****************************************************
// クラスの定義
//****************************************************
class CCamera
{
public:
	//構造体宣言
	typedef struct
	{
		D3DXVECTOR3 posV;	// 視点
		D3DXVECTOR3 posVOld;	// 視点の前回の位置
		D3DXVECTOR3 posR;	// 注視点
		D3DXVECTOR3 posVDest;	// 目的の視点
		D3DXVECTOR3 posRDest;	// 目的の注視点
		D3DXVECTOR3 vecU;	// 上方向ベクトル
		D3DXVECTOR3 rot;	// 向き
		float fViewAngle;	// 視野角
		float fLength;	// 視点から注視点までの距離
		float fRoll;	// ロール回転量
		D3DXMATRIX mtxProjection;	// プロジェクションマトリックス
		D3DXMATRIX mtxView;	// ビューマトリックス
		int nTimerQuake;	// 振動のタイマー
		float fQuakeSizeV;	// 振動のサイズ
		float fQuakeSizeH;	// 振動のサイズ
	}Camera;

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void SetCamera(void);
	void SetDist(float fDist) { m_camera.fLength = fDist; }
	float GetDist(void) { return m_camera.fLength; }
	Camera *GetCamera(void);
	void SetQuake(float fQuakeSizeH, float fQuakeSizeV, int nTime);
	void Quake(void);
	void MoveDist(float fFact);
	void SetPosRDest(D3DXVECTOR3 posRDest) { m_camera.posRDest = posRDest; }
	void SetPosVDest(D3DXVECTOR3 posVDest) { m_camera.posVDest = posVDest; }
	void ChangeState(CCameraState *pBehavior);
	void SetPosV(void);
	void SetPosR(void);
	float GetRoll(void) { return m_camera.fRoll; }
	void SetRoll(float fRoll) { m_camera.fRoll = fRoll; }
	D3DXVECTOR3 GetPosAbove(void) { return m_posAbove; }
	void SetPosAbove(D3DXVECTOR3 posAbove) { m_posAbove = posAbove; }
	void SkipToDest(void);	// 目標位置までカメラの視点注視点を動かす

private:
	Camera m_camera;	// 構造体の情報
	CCameraState *m_pState;	// ステイトのポインタ
	D3DXVECTOR3 m_posAbove;	// 上空視点位置
};

namespace Camera
{
// 行動の切り替え
void ChangeState(CCameraState *pBehavior);

// ロールの調整
void ControlRoll(float fDist, float fFact);

// 目標地点までのスキップ
void SkipToDest(void);
}

#endif