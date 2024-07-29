//*****************************************************
//
// �J�����̏���[camera.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
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
// �萔��`
//*****************************************************
namespace
{
const float MOVE_SPEED = 3.0f;	// �ړ��X�s�[�h
const D3DXVECTOR3 INITIAL_ROT = { D3DX_PI * 0.4f,0.0f,0.0f };	// �����p�x
const float INITIAL_DIST = 300.0f;	// �����̋����i���_���璍���_�j
const float ABOVE_DEFAULT = 10000.0f;	// �f�t�H���g�̏�󎋓_�̍���
}
#define INITIAL_ANGLE	(93.0f)	// �����̎���p
#define HEIGHT_CAMERA (20.0f)	// �J�����̍���

//====================================================
// ����������
//====================================================
HRESULT CCamera::Init(void)
{
	ZeroMemory(&m_camera,sizeof(Camera));

	// �l�̏�����
	m_camera.rot = INITIAL_ROT;
	m_camera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_camera.fViewAngle = INITIAL_ANGLE;
	m_camera.fLength = INITIAL_DIST;

	// ���g�̃|�C���^���k���ɂ���
	m_pState = nullptr;

	m_posAbove = { 0.0f,ABOVE_DEFAULT,0.0f };

	return S_OK;
}

//====================================================
// �I������
//====================================================
void CCamera::Uninit(void)
{

}

//====================================================
// �X�V����
//====================================================
void CCamera::Update(void)
{
	if (m_pState != nullptr)
	{
		m_pState->Update(this);
	}
}

//====================================================
// �ڕW�Ɍ���������
//====================================================
void CCamera::MoveDist(float fFact)
{
	// �ڕW�ʒu�ɕ␳
	m_camera.posV += (m_camera.posVDest - m_camera.posV) * fFact;
	m_camera.posR += (m_camera.posRDest - m_camera.posR) * fFact;
}

//====================================================
// �U������
//====================================================
void CCamera::Quake(void)
{
	//��ʐU��
	if (m_camera.nTimerQuake > 0)
	{
		//���Ԃ����炷
		m_camera.nTimerQuake--;

		//���_�����_�𓮂����ăJ������h�炷
		m_camera.posV.x += (float)(rand() % 201 - 100) * m_camera.fQuakeSizeH * sinf(m_camera.rot.y);
		m_camera.posV.y += (float)(rand() % 101 - 50) * m_camera.fQuakeSizeV;
		m_camera.posV.z += (float)(rand() % 201 - 100) * m_camera.fQuakeSizeH * cosf(m_camera.rot.y);

		m_camera.posR.x += (float)(rand() % 201 - 100) * m_camera.fQuakeSizeH * sinf(m_camera.rot.y);
		m_camera.posR.y += (float)(rand() % 101 - 50) * m_camera.fQuakeSizeV;
		m_camera.posR.z += (float)(rand() % 201 - 100) * m_camera.fQuakeSizeH * cosf(m_camera.rot.y);

		//���X�ɗh��̃T�C�Y������������
		m_camera.fQuakeSizeH += (0.0f - m_camera.fQuakeSizeH) * 0.03f;
		m_camera.fQuakeSizeV += (0.0f - m_camera.fQuakeSizeV) * 0.03f;
	}
}

//====================================================
// �U���ݒ�
//====================================================
void CCamera::SetQuake(float fQuakeSizeH, float fQuakeSizeV, int nTime)
{
	if (fQuakeSizeH > 0.0f && fQuakeSizeV > 0.0f)
	{
		//�h��鎞�ԑ��
		m_camera.nTimerQuake = nTime;

		//�h��̑傫�����
		m_camera.fQuakeSizeH = fQuakeSizeH;
		m_camera.fQuakeSizeV = fQuakeSizeV;
	}
}

//====================================================
// ���_�ݒ�
//====================================================
void CCamera::SetPosV(void)
{
	D3DXVECTOR3 vecPole = universal::PolarCoordinates(m_camera.rot);

	m_camera.posV = m_camera.posR + vecPole * m_camera.fLength;
}

//====================================================
// �����_�ݒ�
//====================================================
void CCamera::SetPosR(void)
{
	D3DXVECTOR3 vecPole = universal::PolarCoordinates(m_camera.rot);

	m_camera.posRDest = m_camera.posV - vecPole * m_camera.fLength;
}

//====================================================
// �ڕW�n�_�܂ňʒu�����X�L�b�v����
//====================================================
void CCamera::SkipToDest(void)
{
	if (m_pState != nullptr)
		m_pState->Update(this);	// �J�����̈ʒu�Ȃǂ����߂邽�߃X�e�C�g�̍X�V������

	m_camera.posV = m_camera.posVDest;
	m_camera.posR = m_camera.posRDest;
}

//====================================================
// �ݒ菈��
//====================================================
void CCamera::SetCamera(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	//�v���W�F�N�V�����}�g���b�N�X==================================================
	//�v���W�F�N�V�����}�g���b�N�X������
	D3DXMatrixIdentity(&m_camera.mtxProjection);

	//�v���W�F�N�V�����}�g���b�N�X�쐬
	D3DXMatrixPerspectiveFovLH(&m_camera.mtxProjection,
		D3DXToRadian(m_camera.fViewAngle),
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
		MIN_DRAW,
		MAX_DRAW);

	//�}�g���b�N�X�ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION,&m_camera.mtxProjection);

	//�r���[�}�g���b�N�X============================================================
	//�r���[�}�g���b�N�X������
	D3DXMatrixIdentity(&m_camera.mtxView);

	//�r���[�}�g���b�N�X�쐬
	D3DXMatrixLookAtLH(&m_camera.mtxView,
		&m_camera.posV,
		&m_camera.posR,
		&m_camera.vecU);

	//�J�����̃��[���p
	// Z���𒆐S�ɉ�]����s����v�Z
	D3DXVECTOR3 rotationAxis(0.0f, 0.0f, 1.0f);
	D3DXMATRIX rotationMatrix;
	D3DXMatrixRotationAxis(&rotationMatrix, &rotationAxis, m_camera.fRoll);

	m_camera.mtxView *= rotationMatrix;

	//�r���[�}�g���b�N�X�ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &m_camera.mtxView);

#ifdef _DEBUG
#if 0
	CDebugProc::GetInstance()->Print("\n���_�̈ʒu�F[%f,%f,%f]", m_camera.posV.x, m_camera.posV.y, m_camera.posV.z);
	CDebugProc::GetInstance()->Print("\n�����_�̈ʒu�F[%f,%f,%f]", m_camera.posR.x, m_camera.posR.y, m_camera.posR.z);
	CDebugProc::GetInstance()->Print("\n�J�����̌����F[%f,%f,%f]", m_camera.rot.x, m_camera.rot.y, m_camera.rot.z);
	CDebugProc::GetInstance()->Print("\nVecU�F[%f,%f,%f]", m_camera.vecU.x, m_camera.vecU.y, m_camera.vecU.z);
	CDebugProc::GetInstance()->Print("\n�J���������F[%f]", m_camera.fLength);
#endif
#endif
}

//====================================================
// ���擾
//====================================================
CCamera::Camera *CCamera::GetCamera(void)
{
	return &m_camera;
}

//====================================================
// �s���̐؂�ւ�
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
// �s���̐؂�ւ�
void ChangeState(CCameraState *pBehavior)
{
	CCamera *pCamera = CManager::GetCamera();

	if (pCamera != nullptr)
	{
		pCamera->ChangeState(pBehavior);
	}
}

// ���[���l�̒���
void ControlRoll(float fDist, float fFact)
{
	CCamera *pCamera = CManager::GetCamera();

	if (pCamera == nullptr)
		return;

	CCamera::Camera *pInfoCamera = pCamera->GetCamera();

	pInfoCamera->fRoll += (fDist - pInfoCamera->fRoll) * fFact;
}
}