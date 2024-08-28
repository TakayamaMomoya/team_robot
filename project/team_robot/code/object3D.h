//*****************************************************
//
// 3D�I�u�W�F�N�g[object3D.h]
// Author:���R����
//
//*****************************************************
#ifndef _OBJECT3D_H_
#define _OBJECT3D_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "gameObject.h"

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CObject3D	: public CGameObject
{
public:
	CObject3D(int nPriority = 4);	// �R���X�g���N�^
	~CObject3D();	//	�f�X�g���N�^

	// �����o�֐�
	virtual HRESULT Init(void);	// ������
	virtual void Uninit(void);	// �I��
	virtual void Update(void);	// �X�V
	virtual void Draw(void);	// �`��
	void ResetMtxParent(void);	// �e�}�g���b�N�X�̃��Z�b�g
	D3DXVECTOR3 GetForward(void);	// �}�g���b�N�X����O���x�N�g�����擾
	D3DXVECTOR3 GetUp(void);	// �}�g���b�N�X�������x�N�g�����擾
	D3DXVECTOR3 GetRight(void);	// �}�g���b�N�X����E���x�N�g�����擾

	// �ϐ��擾�E�ݒ�֐�
	D3DXMATRIX GetMatrix(void) { return m_mtxWorld; }	// ���[���h�}�g���b�N�X
	void SetMatrix(D3DXMATRIX mtx) { m_mtxWorld = mtx; }
	D3DXMATRIX GetMatrixParent(void) { return m_mtxParent; }	// �e�}�g���b�N�X
	void SetMatrixParent(D3DXMATRIX mtx) { m_mtxParent = mtx; }
	D3DXQUATERNION GetQuaternion(void) { return m_quat; }	// �ݐσN�H�[�^�j�I��
	void SetQuaternion(D3DXQUATERNION quat) { m_quat = quat; }

private:
	// �����o�ϐ�
	D3DXMATRIX m_mtxWorld;	// ���[���h�}�g���b�N�X
	D3DXMATRIX m_mtxParent;	// �e�}�g���b�N�X
	D3DXQUATERNION m_quat;	// �ݐσN�H�[�^�j�I��
};

#endif