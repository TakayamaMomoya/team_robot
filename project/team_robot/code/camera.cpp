//*****************************************************
//
// カメラの処理[camera.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "manager.h"
#include "renderer.h"
#include "main.h"
#include "camera.h"
#include "CameraState.h"
#include "inputkeyboard.h"
#include "inputmouse.h"
#include "debugproc.h"
#include "game.h"
#include "player.h"
#include "effect3D.h"
#include "meshfield.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const float MOVE_SPEED = 3.0f;	// 移動スピード
const D3DXVECTOR3 INITIAL_ROT = { D3DX_PI * 0.4f,0.0f,0.0f };	// 初期角度
const float INITIAL_DIST = 300.0f;	// 初期の距離（視点から注視点）
const float ABOVE_DEFAULT = 10000.0f;	// デフォルトの上空視点の高さ
}
#define INITIAL_ANGLE	(93.0f)	// 初期の視野角
#define HEIGHT_CAMERA (20.0f)	// カメラの高さ

//====================================================
// 初期化処理
//====================================================
HRESULT CCamera::Init(void)
{
	ZeroMemory(&m_camera,sizeof(Camera));

	// 値の初期化
	m_camera.rot = INITIAL_ROT;
	m_camera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_camera.fViewAngle = INITIAL_ANGLE;
	m_camera.fLength = INITIAL_DIST;

	// 自身のポインタをヌルにする
	m_pState = nullptr;

	m_posAbove = { 0.0f,ABOVE_DEFAULT,0.0f };

	return S_OK;
}

//====================================================
// 終了処理
//====================================================
void CCamera::Uninit(void)
{

}

//====================================================
// 更新処理
//====================================================
void CCamera::Update(void)
{
	if (m_pState != nullptr)
	{
		m_pState->Update(this);
	}
}

//====================================================
// 目標に向かう処理
//====================================================
void CCamera::MoveDist(float fFact)
{
	// 目標位置に補正
	m_camera.posV += (m_camera.posVDest - m_camera.posV) * fFact;
	m_camera.posR += (m_camera.posRDest - m_camera.posR) * fFact;
}

//====================================================
// 振動処理
//====================================================
void CCamera::Quake(void)
{
	//画面振動
	if (m_camera.nTimerQuake > 0)
	{
		//時間を減らす
		m_camera.nTimerQuake--;

		//視点注視点を動かしてカメラを揺らす
		m_camera.posV.x += (float)(rand() % 201 - 100) * m_camera.fQuakeSizeH * sinf(m_camera.rot.y);
		m_camera.posV.y += (float)(rand() % 101 - 50) * m_camera.fQuakeSizeV;
		m_camera.posV.z += (float)(rand() % 201 - 100) * m_camera.fQuakeSizeH * cosf(m_camera.rot.y);

		m_camera.posR.x += (float)(rand() % 201 - 100) * m_camera.fQuakeSizeH * sinf(m_camera.rot.y);
		m_camera.posR.y += (float)(rand() % 101 - 50) * m_camera.fQuakeSizeV;
		m_camera.posR.z += (float)(rand() % 201 - 100) * m_camera.fQuakeSizeH * cosf(m_camera.rot.y);

		//徐々に揺れのサイズを小さくする
		m_camera.fQuakeSizeH += (0.0f - m_camera.fQuakeSizeH) * 0.03f;
		m_camera.fQuakeSizeV += (0.0f - m_camera.fQuakeSizeV) * 0.03f;
	}
}

//====================================================
// 振動設定
//====================================================
void CCamera::SetQuake(float fQuakeSizeH, float fQuakeSizeV, int nTime)
{
	if (fQuakeSizeH > 0.0f && fQuakeSizeV > 0.0f)
	{
		//揺れる時間代入
		m_camera.nTimerQuake = nTime;

		//揺れの大きさ代入
		m_camera.fQuakeSizeH = fQuakeSizeH;
		m_camera.fQuakeSizeV = fQuakeSizeV;
	}
}

//====================================================
// 視点設定
//====================================================
void CCamera::SetPosV(void)
{
	D3DXVECTOR3 vecPole = universal::PolarCoordinates(m_camera.rot);

	m_camera.posV = m_camera.posR + vecPole * m_camera.fLength;
}

//====================================================
// 注視点設定
//====================================================
void CCamera::SetPosR(void)
{
	D3DXVECTOR3 vecPole = universal::PolarCoordinates(m_camera.rot);

	m_camera.posRDest = m_camera.posV - vecPole * m_camera.fLength;
}

//====================================================
// 目標地点まで位置情報をスキップする
//====================================================
void CCamera::SkipToDest(void)
{
	if (m_pState != nullptr)
		m_pState->Update(this);	// カメラの位置などを決めるためステイトの更新を入れる

	m_camera.posV = m_camera.posVDest;
	m_camera.posR = m_camera.posRDest;
}

//====================================================
// 設定処理
//====================================================
void CCamera::SetCamera(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	//プロジェクションマトリックス==================================================
	//プロジェクションマトリックス初期化
	D3DXMatrixIdentity(&m_camera.mtxProjection);

	//プロジェクションマトリックス作成
	D3DXMatrixPerspectiveFovLH(&m_camera.mtxProjection,
		D3DXToRadian(m_camera.fViewAngle),
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
		MIN_DRAW,
		MAX_DRAW);

	//マトリックス設定
	pDevice->SetTransform(D3DTS_PROJECTION,&m_camera.mtxProjection);

	//ビューマトリックス============================================================
	//ビューマトリックス初期化
	D3DXMatrixIdentity(&m_camera.mtxView);

	//ビューマトリックス作成
	D3DXMatrixLookAtLH(&m_camera.mtxView,
		&m_camera.posV,
		&m_camera.posR,
		&m_camera.vecU);

	//カメラのロール用
	// Z軸を中心に回転する行列を計算
	D3DXVECTOR3 rotationAxis(0.0f, 0.0f, 1.0f);
	D3DXMATRIX rotationMatrix;
	D3DXMatrixRotationAxis(&rotationMatrix, &rotationAxis, m_camera.fRoll);

	m_camera.mtxView *= rotationMatrix;

	//ビューマトリックス設定
	pDevice->SetTransform(D3DTS_VIEW, &m_camera.mtxView);

#ifdef _DEBUG
#if 0
	CDebugProc::GetInstance()->Print("\n視点の位置：[%f,%f,%f]", m_camera.posV.x, m_camera.posV.y, m_camera.posV.z);
	CDebugProc::GetInstance()->Print("\n注視点の位置：[%f,%f,%f]", m_camera.posR.x, m_camera.posR.y, m_camera.posR.z);
	CDebugProc::GetInstance()->Print("\nカメラの向き：[%f,%f,%f]", m_camera.rot.x, m_camera.rot.y, m_camera.rot.z);
	CDebugProc::GetInstance()->Print("\nVecU：[%f,%f,%f]", m_camera.vecU.x, m_camera.vecU.y, m_camera.vecU.z);
	CDebugProc::GetInstance()->Print("\nカメラ距離：[%f]", m_camera.fLength);
#endif
#endif
}

//====================================================
// 情報取得
//====================================================
CCamera::Camera *CCamera::GetCamera(void)
{
	return &m_camera;
}

//====================================================
// 行動の切り替え
//====================================================
void CCamera::ChangeState(CCameraState *pBehavior)
{
	if (m_pState != nullptr)
	{
		delete m_pState;
		m_pState = nullptr;
	}

	m_pState = pBehavior;
}

namespace Camera
{
// 行動の切り替え
void ChangeState(CCameraState *pBehavior)
{
	CCamera *pCamera = CManager::GetCamera();

	if (pCamera != nullptr)
	{
		pCamera->ChangeState(pBehavior);
	}
}

// ロール値の調整
void ControlRoll(float fDist, float fFact)
{
	CCamera *pCamera = CManager::GetCamera();

	if (pCamera == nullptr)
		return;

	CCamera::Camera *pInfoCamera = pCamera->GetCamera();

	pInfoCamera->fRoll += (fDist - pInfoCamera->fRoll) * fFact;
}
}