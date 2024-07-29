//*****************************************************
//
// ���[�^�[[meter.h]
// Author:�X��x��
//
//*****************************************************

#ifndef _METER_H_
#define _METER_H_

//*****************************************************
// �O���錾
//*****************************************************
class CNumber;
class CUI;

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "object.h"

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CMeter : public CObject
{
public:
	CMeter(int nPriority = 0);	// �R���X�g���N�^
	~CMeter();	//	�f�X�g���N�^

	// �����o�֐�
	HRESULT Init();
	void Uninit();
	void Update();

	// �ÓI�����o�֐�
	static CMeter* Create();	// ����
	static CMeter* GetInstance() { return m_pMeter; }	// �C���X�^���X�擾

private:
	// �����o�֐�
	void Acceleration();
	void Needle();

	//�����o�ϐ�
	CNumber* m_pNumber;	// �i���o�[�̃|�C���^
	CUI* m_pNeedle;	// �jUI�̃|�C���^
	CUI* m_pBackMeter;	// ���[�^�[�w�i�̃|�C���^

	// �ÓI�����o�ϐ�
	static CMeter* m_pMeter;	// ���g�̃|�C���^
};

#endif
