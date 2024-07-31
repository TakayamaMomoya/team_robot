//*****************************************************
//
// カメラステイト[CameraState.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "CameraState.h"
#include "camera.h"
#include "player.h"
#include "effect3D.h"
#include "inputmouse.h"
#include "inputkeyboard.h"
#include "debugproc.h"
#include "block.h"
#include "manager.h"
#include "title.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const float MOVE_SPEED = 21.0f;	//移動スピード
const float ROLL_SPEED = 0.04f;	//回転スピード
const float FACT_CORRECT_POS = 0.4f;	// 位置補正係数
const float FACT_CORRECT_CONTOROLLL = 0.9f;	// 操作時の位置補正係数
const float LENGTH_FOLLOW = 412.0f;	// 追従時のカメラ距離
const float ANGLE_FOLLOW = D3DX_PI * 0.45f;	// 追従時のカメラ角度
const float LENGTHPOSR_FOLLOW = 4126.0f;	// 追従時の先を見る距離
const float FOV_FOLLOW = 93.0f;	// 追従時の視野角
const float SPEED_MOVE_ABOVE = 20.0f;	// 上空視点時の移動速度
const float SPEED_ZOOM_ABOVE = 10.0f;	// ズーム速度
const D3DXVECTOR3 POS_RESULT = { 12726.0f, 2500.7f, -27695.0f };	// リザルト演出を行う位置
}

//***********************************************************************************
// プレイヤーの追従
//***********************************************************************************
//=====================================================
// コンストラクタ
//=====================================================
CFollowPlayer::CFollowPlayer()
{
	CCamera *pCamera = CManager::GetCamera();

	if (pCamera != nullptr)
	{
		CCamera::Camera *pInfoCamera = pCamera->GetCamera();

		pInfoCamera->fLength = LENGTH_FOLLOW;

		pInfoCamera->rot.x = ANGLE_FOLLOW;
	}
}

//=====================================================
// 更新
//=====================================================
void CFollowPlayer::Update(CCamera *pCamera)
{
	if (pCamera == nullptr)
		return;

	CCamera::Camera *pInfoCamera = pCamera->GetCamera();
	CPlayer *pPlayer = CPlayer::GetInstance();

	if (pPlayer == nullptr)
	{
		return;
	}

	D3DXVECTOR3 pos = pPlayer->GetMtxPos(2);
	D3DXMATRIX mtx = pPlayer->GetMatrix();

	pInfoCamera->posRDest = pos;

	D3DXMATRIX mtxOffset;
	universal::SetOffSet(&mtxOffset, mtx, D3DXVECTOR3(0.0f, 400.0f, 400.0f));

	D3DXVECTOR3 posCameraDest = { mtxOffset._41,mtxOffset._42,mtxOffset._43 };

	//目標の視点設定
	pInfoCamera->posVDest = posCameraDest;

	D3DXMATRIX mtxUp;
	universal::SetOffSet(&mtxUp, mtx, D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	D3DXVECTOR3 vecUp = { mtxUp._41 - mtx._41,mtxUp._42 - mtx._42 ,mtxUp._43 - mtx._43 };
	pInfoCamera->vecU = vecUp;

	pCamera->MoveDist(FACT_CORRECT_POS);
}

//=====================================================
// 操作する
//=====================================================
void CMoveControl::Update(CCamera *pCamera)
{
	if (pCamera == nullptr)
		return;

	CCamera::Camera *pInfoCamera = pCamera->GetCamera();

	// 入力取得
	CInputKeyboard *pKeyboard = CInputKeyboard::GetInstance();
	CInputMouse *pMouse = CInputMouse::GetInstance();

	float fMove = MOVE_SPEED;

	if (!m_bAbove)
	{
		//マウス操作======================================================
		if (pMouse->GetPress(CInputMouse::BUTTON_RMB) == true)
		{//右クリック中、視点旋回
			D3DXVECTOR3 rot;

			//マウスの移動量代入
			rot = { (float)pMouse->GetMoveIX() * ROLL_SPEED, (float)pMouse->GetMoveIY() * ROLL_SPEED, 0.0f };

			D3DXVec3Normalize(&rot, &rot);

			//視点の旋回
			pInfoCamera->rot.y += rot.x * ROLL_SPEED;
			pInfoCamera->rot.x -= rot.y * ROLL_SPEED;

			if (pKeyboard->GetPress(DIK_LSHIFT) == true)
			{//加速
				fMove *= 7;
			}

			D3DXVECTOR3 rotMove = pInfoCamera->rot;
			D3DXVECTOR3 vecPole = { 0.0f,0.0f,0.0f };

			// 視点移動===============================================
			if (pKeyboard->GetPress(DIK_A) == true)
			{// 左移動
				pInfoCamera->posVDest.x -= sinf(pInfoCamera->rot.y - D3DX_PI * 0.5f) * fMove;
				pInfoCamera->posVDest.z -= cosf(pInfoCamera->rot.y - D3DX_PI * 0.5f) * fMove;
				pCamera->SetPosR();
			}
			if (pKeyboard->GetPress(DIK_D) == true)
			{// 右移動
				pInfoCamera->posVDest.x -= sinf(pInfoCamera->rot.y - D3DX_PI * -0.5f) * fMove;
				pInfoCamera->posVDest.z -= cosf(pInfoCamera->rot.y - D3DX_PI * -0.5f) * fMove;
				pCamera->SetPosR();
			}
			if (pKeyboard->GetPress(DIK_W) == true)
			{// 前移動
				pInfoCamera->posVDest.x += sinf(-pInfoCamera->rot.x) * sinf(pInfoCamera->rot.y) * fMove;
				pInfoCamera->posVDest.y -= cosf(-pInfoCamera->rot.x) * MOVE_SPEED;
				pInfoCamera->posVDest.z += sinf(-pInfoCamera->rot.x) * cosf(pInfoCamera->rot.y) * fMove;
				pCamera->SetPosR();
			}
			if (pKeyboard->GetPress(DIK_S) == true)
			{// 後移動
				pInfoCamera->posVDest.x += sinf(-pInfoCamera->rot.x + D3DX_PI) * sinf(pInfoCamera->rot.y) * fMove;
				pInfoCamera->posVDest.y -= cosf(-pInfoCamera->rot.x + D3DX_PI) * MOVE_SPEED;
				pInfoCamera->posVDest.z += sinf(-pInfoCamera->rot.x + D3DX_PI) * cosf(pInfoCamera->rot.y) * fMove;
				pCamera->SetPosR();
			}
			if (pKeyboard->GetPress(DIK_E) == true)
			{// 上昇
				pInfoCamera->posVDest.y += fMove;
				pCamera->SetPosR();
			}
			if (pKeyboard->GetPress(DIK_Q) == true)
			{// 下降
				pInfoCamera->posVDest.y -= fMove;
				pCamera->SetPosR();
			}

			pCamera->SetPosR();
		}
	}
	else
	{
		CCamera::Camera *pInfoCamera = pCamera->GetCamera();
		D3DXVECTOR3 posAbove = pCamera->GetPosAbove();

		pInfoCamera->posVDest = posAbove;
		pInfoCamera->posRDest = { posAbove.x,0.0f,posAbove.z + 1.0f };

		if (pMouse->GetPress(CInputMouse::BUTTON_RMB))
		{// 右クリック中、移動可能
			posAbove.x -= pMouse->GetMoveIX() * SPEED_MOVE_ABOVE * posAbove.y * 0.001f;
			posAbove.z += pMouse->GetMoveIY() * SPEED_MOVE_ABOVE * posAbove.y * 0.001f;
		}

		posAbove.y += pMouse->GetMoveIZ() * SPEED_ZOOM_ABOVE;
		pInfoCamera->posVDest.y = posAbove.y;

		universal::LimitValuefloat(&posAbove.y, FLT_MAX, 1000.0f);

		pCamera->SetPosAbove(posAbove);

		if (pKeyboard->GetTrigger(DIK_R))
		{// 位置を上空視点基準にしてみる
			if (m_bAbove)
			{
				CCamera::Camera *pInfoCamera = pCamera->GetCamera();

				pInfoCamera->posVDest = posAbove;
				pInfoCamera->posVDest.y = 5000.0f;
				pInfoCamera->posRDest = pInfoCamera->posVDest;

				pInfoCamera->posRDest.z += 500.0f;

				m_bAbove = false;
			}
		}
	}

	if (pKeyboard->GetTrigger(DIK_G))
	{
		m_bAbove = m_bAbove ? false : true;
	}


	pCamera->MoveDist(FACT_CORRECT_CONTOROLLL);
}

//**************************************************************************
// タイトル
//**************************************************************************
//=====================================================
// 更新
//=====================================================
void CCameraStateTitle::Update(CCamera* pCamera)
{
	CCamera::Camera* pInfoCamera = pCamera->GetCamera();

	pInfoCamera->rot.y += 0.005f;

	universal::LimitRot(&pInfoCamera->rot.y);

	pCamera->SetPosV();

	CDebugProc::GetInstance()->Print("\n視点  [%f, %f, %f]", pInfoCamera->posV.x, pInfoCamera->posV.y, pInfoCamera->posV.z);
	CDebugProc::GetInstance()->Print("\n注視点[%f, %f, %f]", pInfoCamera->posR.x, pInfoCamera->posR.y, pInfoCamera->posR.z);
	CDebugProc::GetInstance()->Print("\nカメラ距離[%f]", pInfoCamera->fLength);
	CDebugProc::GetInstance()->Print("\n角度  [%f, %f, %f]", pInfoCamera->rot.x, pInfoCamera->rot.y, pInfoCamera->rot.z);
}

//**************************************************************************
// リザルト
//**************************************************************************
//=====================================================
// コンストラクタ
//=====================================================
CCameraStateResult::CCameraStateResult()
{
}

//=====================================================
// 更新
//=====================================================
void CCameraStateResult::Update(CCamera* pCamera)
{
	CCamera::Camera *pInfoCamera = pCamera->GetCamera();

	if (pInfoCamera == nullptr)
		return;

	// カメラ位置の設定
	pInfoCamera->posR = POS_RESULT;
	pCamera->SetPosV();
}