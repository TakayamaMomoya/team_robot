//*****************************************************
//
// �p�[�c�̏���[parts.cpp]
// Author:���R����
//
//*****************************************************

#ifndef _PARTS_H_
#define _PARTS_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "objectX.h"

//*****************************************************
// �N���X��`
//*****************************************************
class CParts : public CObjectX
{
public:
	CParts();	// �R���X�g���N�^
	~CParts();	// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(void);	// �X�V
	void Draw(void);	// �`��

	// �ϐ��擾�E�ݒ�֐�
	D3DXVECTOR3 GetPosOrg(void) { return m_posOrg; }	// �����̈ʒu
	void SetPosOrg(D3DXVECTOR3 pos) { m_posOrg = pos; }
	D3DXMATRIX* GetMatrixOld(void) { return &m_mtxWorldOld; }

	// �ÓI�����o�֐�
	static CParts *Create(void);	// ��������

private:
	// �����o�ϐ�
	D3DXVECTOR3 m_posOrg;
	D3DXMATRIX m_mtxWorldOld;	// �O��̃��[���h�}�g���b�N�X
};

#endif