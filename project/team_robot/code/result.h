//*****************************************************
//
// ���U���g����[result.h]
// Author:���R����
//
//*****************************************************
#ifndef _RESULT_H_
#define _RESULT_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "object.h"

//*****************************************************
// �O���錾
//*****************************************************
class CUI;
class CStateResult;
class CNumber;

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CResult : public CObject
{// �`���[�g���A���̃N���X
public:
	CResult();	// �R���X�g���N�^
	~CResult();	// �f�X�g���N�^

	// �����o�֐�
	virtual HRESULT Init(void);	// ������
	virtual void Uninit(void);	// �I��
	virtual void Update();	// �X�V
	virtual void Draw();	// �`��
	void ChangeState(CStateResult *pState);	// �X�e�C�g�̕ύX
	void StartFade(void);	// �t�F�[�h

	// �ϐ��擾�E�ݒ�֐�

	// �ÓI�����o�֐�
	static CResult *Create(void);	// ��������
private:
	// �����o�֐�
	void SetPlayer(void);	// �v���C���[�ݒ�
	void SetCamera(void);	// �J�����ݒ�

	// �����o�ϐ�
	CStateResult *m_pState;
};

class CStateResult
{// �X�e�C�g�̊��N���X
public:
	CStateResult() {};	// �R���X�g���N�^
	virtual ~CStateResult() {};	 // �f�X�g���N�^

	// �����o�֐�
	virtual void Init(CResult *pResult) = 0;	// ����������
	virtual void Uninit(CResult *pResult);	// �I������
	virtual void Update(CResult *pResult) = 0;	// �X�V����

private:
};

class CStateResultDispTime : public CStateResult
{// �^�C���\��
public:
	CStateResultDispTime();	// �R���X�g���N�^
	virtual ~CStateResultDispTime();	 // �f�X�g���N�^

	// �����o�֐�
	void Init(CResult *pResult) override;	// ����������
	void Uninit(CResult *pResult) override;	// �I������
	void Update(CResult *pResult) override;	// �X�V����

private:
	enum E_NUMBER
	{
		NUMBER_MINUTE = 0,	// ���\��
		NUMBER_SECOND,	// �b�\��
		NUMBER_MILLISSEC,	// �~���b�\��
		NUMBER_MAX
	};

	// �����o�֐�
	void SetNumber(void);	// �����̐ݒ�

	// �����o�ϐ�
	CNumber *m_apTime[NUMBER_MAX];	// �����̃|�C���^�z��
};

namespace Tutorial
{
CUI *CreateUIDefault(void);
}

#endif