//*****************************************************
//
// �J�����̏���[camera.h]
// Author:���R����
//
//*****************************************************

#ifndef _CAMERA_H_
#define _CAMERA_H_

//****************************************************
// �O���錾
//****************************************************
class CCameraState;

//****************************************************
// �C���N���[�h
//****************************************************
#include "main.h"

//****************************************************
// �}�N����`
//****************************************************
#define MIN_DRAW		(10.0f)					//�`����J�n���鋗��
#define MAX_DRAW		(700000.0f)				//�`����I�����鋗��

//****************************************************
// �N���X�̒�`
//****************************************************
class CCamera
{
public:
	//�\���̐錾
	typedef struct
	{
		D3DXVECTOR3 posV;	// ���_
		D3DXVECTOR3 posVOld;	// ���_�̑O��̈ʒu
		D3DXVECTOR3 posR;	// �����_
		D3DXVECTOR3 posVDest;	// �ړI�̎��_
		D3DXVECTOR3 posRDest;	// �ړI�̒����_
		D3DXVECTOR3 vecU;	// ������x�N�g��
		D3DXVECTOR3 rot;	// ����
		float fViewAngle;	// ����p
		float fLength;	// ���_���璍���_�܂ł̋���
		float fRoll;	// ���[����]��
		D3DXMATRIX mtxProjection;	// �v���W�F�N�V�����}�g���b�N�X
		D3DXMATRIX mtxView;	// �r���[�}�g���b�N�X
		int nTimerQuake;	// �U���̃^�C�}�[
		float fQuakeSizeV;	// �U���̃T�C�Y
		float fQuakeSizeH;	// �U���̃T�C�Y
	}Camera;

	// �����o�֐�
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
	void SkipToDest(void);	// �ڕW�ʒu�܂ŃJ�����̎��_�����_�𓮂���

private:
	Camera m_camera;	// �\���̂̏��
	CCameraState *m_pState;	// �X�e�C�g�̃|�C���^
	D3DXVECTOR3 m_posAbove;	// ��󎋓_�ʒu
};

namespace Camera
{
// �s���̐؂�ւ�
void ChangeState(CCameraState *pBehavior);

// ���[���̒���
void ControlRoll(float fDist, float fFact);
}

#endif