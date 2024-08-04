//*****************************************************
//
// �J�����X�e�C�g[CameraState.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
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
// �萔��`
//*****************************************************
namespace
{
const float MOVE_SPEED = 21.0f;	//�ړ��X�s�[�h
const float ROLL_SPEED = 0.04f;	//��]�X�s�[�h
const float FACT_CORRECT_POS = 0.4f;	// �ʒu�␳�W��
const float FACT_CORRECT_CONTOROLLL = 0.9f;	// ���쎞�̈ʒu�␳�W��
const float LENGTH_FOLLOW = 412.0f;	// �Ǐ]���̃J��������
const float ANGLE_FOLLOW = D3DX_PI * 0.45f;	// �Ǐ]���̃J�����p�x
const float LENGTHPOSR_FOLLOW = 4126.0f;	// �Ǐ]���̐�����鋗��
const float FOV_FOLLOW = 93.0f;	// �Ǐ]���̎���p
const float SPEED_MOVE_ABOVE = 20.0f;	// ��󎋓_���̈ړ����x
const float SPEED_ZOOM_ABOVE = 10.0f;	// �Y�[�����x
const D3DXVECTOR3 POS_RESULT = { 12726.0f, 2500.7f, -27695.0f };	// ���U���g���o���s���ʒu
}

//***********************************************************************************
// �v���C���[�̒Ǐ]
//***********************************************************************************
//=====================================================
// �R���X�g���N�^
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
// �X�V
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

	//�ڕW�̎��_�ݒ�
	pInfoCamera->posVDest = posCameraDest;

	D3DXMATRIX mtxUp;
	universal::SetOffSet(&mtxUp, mtx, D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	D3DXVECTOR3 vecUp = { mtxUp._41 - mtx._41,mtxUp._42 - mtx._42 ,mtxUp._43 - mtx._43 };
	pInfoCamera->vecU = vecUp;

	pCamera->MoveDist(FACT_CORRECT_POS);
}

//=====================================================
// ���삷��
//=====================================================
void CMoveControl::Update(CCamera *pCamera)
{
	if (pCamera == nullptr)
		return;

	CCamera::Camera *pInfoCamera = pCamera->GetCamera();

	// ���͎擾
	CInputKeyboard *pKeyboard = CInputKeyboard::GetInstance();
	CInputMouse *pMouse = CInputMouse::GetInstance();

	float fMove = MOVE_SPEED;

	if (!m_bAbove)
	{
		//�}�E�X����======================================================
		if (pMouse->GetPress(CInputMouse::BUTTON_RMB) == true)
		{//�E�N���b�N���A���_����
			D3DXVECTOR3 rot;

			//�}�E�X�̈ړ��ʑ��
			rot = { (float)pMouse->GetMoveIX() * ROLL_SPEED, (float)pMouse->GetMoveIY() * ROLL_SPEED, 0.0f };

			D3DXVec3Normalize(&rot, &rot);

			//���_�̐���
			pInfoCamera->rot.y += rot.x * ROLL_SPEED;
			pInfoCamera->rot.x -= rot.y * ROLL_SPEED;

			if (pKeyboard->GetPress(DIK_LSHIFT) == true)
			{//����
				fMove *= 7;
			}

			D3DXVECTOR3 rotMove = pInfoCamera->rot;
			D3DXVECTOR3 vecPole = { 0.0f,0.0f,0.0f };

			// ���_�ړ�===============================================
			if (pKeyboard->GetPress(DIK_A) == true)
			{// ���ړ�
				pInfoCamera->posVDest.x -= sinf(pInfoCamera->rot.y - D3DX_PI * 0.5f) * fMove;
				pInfoCamera->posVDest.z -= cosf(pInfoCamera->rot.y - D3DX_PI * 0.5f) * fMove;
				pCamera->SetPosR();
			}
			if (pKeyboard->GetPress(DIK_D) == true)
			{// �E�ړ�
				pInfoCamera->posVDest.x -= sinf(pInfoCamera->rot.y - D3DX_PI * -0.5f) * fMove;
				pInfoCamera->posVDest.z -= cosf(pInfoCamera->rot.y - D3DX_PI * -0.5f) * fMove;
				pCamera->SetPosR();
			}
			if (pKeyboard->GetPress(DIK_W) == true)
			{// �O�ړ�
				pInfoCamera->posVDest.x += sinf(-pInfoCamera->rot.x) * sinf(pInfoCamera->rot.y) * fMove;
				pInfoCamera->posVDest.y -= cosf(-pInfoCamera->rot.x) * MOVE_SPEED;
				pInfoCamera->posVDest.z += sinf(-pInfoCamera->rot.x) * cosf(pInfoCamera->rot.y) * fMove;
				pCamera->SetPosR();
			}
			if (pKeyboard->GetPress(DIK_S) == true)
			{// ��ړ�
				pInfoCamera->posVDest.x += sinf(-pInfoCamera->rot.x + D3DX_PI) * sinf(pInfoCamera->rot.y) * fMove;
				pInfoCamera->posVDest.y -= cosf(-pInfoCamera->rot.x + D3DX_PI) * MOVE_SPEED;
				pInfoCamera->posVDest.z += sinf(-pInfoCamera->rot.x + D3DX_PI) * cosf(pInfoCamera->rot.y) * fMove;
				pCamera->SetPosR();
			}
			if (pKeyboard->GetPress(DIK_E) == true)
			{// �㏸
				pInfoCamera->posVDest.y += fMove;
				pCamera->SetPosR();
			}
			if (pKeyboard->GetPress(DIK_Q) == true)
			{// ���~
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
		{// �E�N���b�N���A�ړ��\
			posAbove.x -= pMouse->GetMoveIX() * SPEED_MOVE_ABOVE * posAbove.y * 0.001f;
			posAbove.z += pMouse->GetMoveIY() * SPEED_MOVE_ABOVE * posAbove.y * 0.001f;
		}

		posAbove.y += pMouse->GetMoveIZ() * SPEED_ZOOM_ABOVE;
		pInfoCamera->posVDest.y = posAbove.y;

		universal::LimitValuefloat(&posAbove.y, FLT_MAX, 1000.0f);

		pCamera->SetPosAbove(posAbove);

		if (pKeyboard->GetTrigger(DIK_R))
		{// �ʒu����󎋓_��ɂ��Ă݂�
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
// �^�C�g��
//**************************************************************************
//=====================================================
// �X�V
//=====================================================
void CCameraStateTitle::Update(CCamera* pCamera)
{
	CCamera::Camera* pInfoCamera = pCamera->GetCamera();

	pInfoCamera->rot.y += 0.005f;

	universal::LimitRot(&pInfoCamera->rot.y);

	pCamera->SetPosV();

	CDebugProc::GetInstance()->Print("\n���_  [%f, %f, %f]", pInfoCamera->posV.x, pInfoCamera->posV.y, pInfoCamera->posV.z);
	CDebugProc::GetInstance()->Print("\n�����_[%f, %f, %f]", pInfoCamera->posR.x, pInfoCamera->posR.y, pInfoCamera->posR.z);
	CDebugProc::GetInstance()->Print("\n�J��������[%f]", pInfoCamera->fLength);
	CDebugProc::GetInstance()->Print("\n�p�x  [%f, %f, %f]", pInfoCamera->rot.x, pInfoCamera->rot.y, pInfoCamera->rot.z);
}

//**************************************************************************
// ���U���g
//**************************************************************************
//=====================================================
// �R���X�g���N�^
//=====================================================
CCameraStateResult::CCameraStateResult()
{
}

//=====================================================
// �X�V
//=====================================================
void CCameraStateResult::Update(CCamera* pCamera)
{
	CCamera::Camera *pInfoCamera = pCamera->GetCamera();

	if (pInfoCamera == nullptr)
		return;

	// �J�����ʒu�̐ݒ�
	pInfoCamera->posR = POS_RESULT;
	pCamera->SetPosV();
}