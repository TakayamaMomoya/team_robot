//*****************************************************
//
// 2D�~�̏���[fan2D.h]
// Author:���R����
//
//*****************************************************

#ifndef _FAN2D_H_
#define _FAN2D_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "fan.h"

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CFan2D	: public CFan
{
public:
	CFan2D(int nPriority = 7);
	~CFan2D();	//	�f�X�g���N�^

	static CFan2D *Create(int nPriority = 7,int nNumVtx = 16);	// ��������
	HRESULT Init(void) override;	// ����������
	void Uninit(void) override;	// �I������
	void Update(void) override;	// �X�V����
	void Draw(void) override;	// �`�揈��
	void SetVtx(void) override;	// ���_�ݒ�

private:
};

#endif