//*****************************************************
//
// 3D�~�̏���[fan3D.h]
// Author:���R����
//
//*****************************************************

#ifndef _FAN3D_H_
#define _FAN3D_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "fan.h"

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CFan3D	: public CFan
{
public:
	CFan3D(int nPriority = 6);
	~CFan3D();	//	�f�X�g���N�^

	static CFan3D *Create(int nPriority = 3,int nNumVtx = 16);	// ��������
	HRESULT Init(void) override;	// ����������
	void Uninit(void) override;	// �I������
	void Update(void) override;	// �X�V����
	void Draw(void) override;	// �`�揈��
	void DrawNormal(void);	// �ʏ�`��
	void DrawBillboard(void);	// �r���{�[�h�`��
	void SetVtx(void) override;	// ���_�ݒ�
	void SetTex(float fScrollX, float fScrollY);      // �e�N�X�`�����W�ݒ菈��

private:
	D3DXMATRIX m_mtxWorld;	// ���[���h�}�g���b�N�X
	bool m_bBillboard;	// �r���{�[�h���ǂ���
	int m_nStartOffsetCount;  // �g�p����g�`�̊J�n�ʒu�����炷�J�E���g
};

#endif