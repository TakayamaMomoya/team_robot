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

	// �ϐ��擾�E�ݒ�֐�
	D3DXMATRIX GetMatrix(void) { return m_mtxWorld; }	// ���[���h�}�g���b�N�X
	void SetMatrix(D3DXMATRIX mtx) { m_mtxWorld = mtx; }
	D3DXMATRIX GetMatrixParent(void) { return m_mtxParent; }	// �e�}�g���b�N�X
	void SetMatrixParent(D3DXMATRIX mtx) { m_mtxParent = mtx; }

private:
	// �����o�ϐ�
	D3DXMATRIX m_mtxWorld;	// ���[���h�}�g���b�N�X
	D3DXMATRIX m_mtxParent;	// �e�}�g���b�N�X
};

#endif