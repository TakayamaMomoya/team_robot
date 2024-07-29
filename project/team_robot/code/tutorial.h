//*****************************************************
//
// �`���[�g���A������[tutorial.h]
// Author:���R����
//
//*****************************************************
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "object.h"

//*****************************************************
// �O���錾
//*****************************************************
class CUI;
class CStateResult;
class CPolygon3D;
class CEffekseerEffect;

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CTutorial : public CObject
{// �`���[�g���A���̃N���X
public:
	CTutorial();	// �R���X�g���N�^
	~CTutorial();	// �f�X�g���N�^

	// �����o�֐�
	virtual HRESULT Init(void);	// ������
	virtual void Uninit(void);	// �I��
	virtual void Update();	// �X�V
	virtual void Draw();	// �`��
	void ChangeState(CStateResult *pState);	// �X�e�C�g�̕ύX
	void AddLimit(int nIdx, float fValue);	// �����l�̒ǉ�

	// �ϐ��擾�E�ݒ�֐�
	void EnableEnd(bool bEnd) { m_bEnd = bEnd; }	// �I���t���O
	bool IsEnd(void) { return m_bEnd; }
	std::map<int, CUI*> *GetMap(void) { return &m_mapUI; }	// UI�̃}�b�v�R���e�i�擾
	std::map<int, float> GetMapCounter(void) { return m_mapCounter; }	// �J�E���^�[�̃}�b�v�R���e�i�擾
	std::map<int, float> GetMapLimit(void) { return m_mapLimit; }	// ���~�b�g�̃}�b�v�R���e�i�擾
	void SetMapCounter(std::map<int, float> map) { m_mapCounter = map; }	// �J�E���^�[�̃}�b�v�R���e�i�擾
	void StartGame(void);	// �Q�[���J�n�̏���

	// �ÓI�����o�֐�
	static CTutorial *Create(void);	// ��������
private:
	// �����o�ϐ�
	std::map<int, CUI*> m_mapUI;	// UI�̃}�b�v�R���e�i
	std::map<int, float> m_mapCounter;	// �J�E���^�[�̃}�b�v�R���e�i
	std::map<int, float> m_mapLimit;	// ���~�b�g�̃}�b�v�R���e�i
	CStateResult *m_pState;	// �X�e�C�g�̃|�C���^
	bool m_bEnd;	// �I���t���O
};

class CStateResult
{// �X�e�C�g�̊��N���X
public:
	CStateResult() {};	// �R���X�g���N�^
	virtual ~CStateResult() {};	 // �f�X�g���N�^

	// �����o�֐�
	virtual void Init(CTutorial *pTutorial) = 0;	// ����������
	virtual void Uninit(CTutorial *pTutorial);	// �I������
	virtual void Update(CTutorial *pTutorial) = 0;	// �X�V����
	virtual bool IsEndInput(int nNum, CTutorial *pTutorial);	// �I�����Ă��邩�ǂ���
private:
};

class CStateTutorialMove : public CStateResult
{// �ړ��̃`���[�g���A��
public:
	CStateTutorialMove();	// �R���X�g���N�^
	virtual ~CStateTutorialMove();	 // �f�X�g���N�^

	// �����o�֐�
	void Init(CTutorial *pTutorial) override;	// ����������
	void Uninit(CTutorial *pTutorial) override;	// �I������
	void Update(CTutorial *pTutorial) override;	// �X�V����

private:
	enum E_MENU
	{// ���j���[����
		MENU_ACCELE = 0,	// �A�N�Z��
		MENU_BRAKE,	// �u���[�L
		MENU_MAX
	};
};

class CStateTutorialDrift : public CStateResult
{// �h���t�g�̃`���[�g���A��
public:
	CStateTutorialDrift();	// �R���X�g���N�^
	virtual ~CStateTutorialDrift();	 // �f�X�g���N�^

	// �����o�֐�
	void Init(CTutorial *pTutorial) override;	// ����������
	void Uninit(CTutorial *pTutorial) override;	// �I������
	void Update(CTutorial *pTutorial) override;	// �X�V����

private:
	enum E_MENU
	{// ���j���[����
		MENU_DRIFT = 0,	// �h���t�g
		MENU_MAX
	};
};

class CStateTutorialParry : public CStateResult
{// �p���B�̃`���[�g���A��
public:
	CStateTutorialParry();	// �R���X�g���N�^
	virtual ~CStateTutorialParry();	 // �f�X�g���N�^

	// �����o�֐�
	void Init(CTutorial *pTutorial) override;	// ����������
	void Uninit(CTutorial *pTutorial) override;	// �I������
	void Update(CTutorial *pTutorial) override;	// �X�V����

private:
	enum E_MENU
	{// ���j���[����
		MENU_PARRY = 0,	// �p���B
		MENU_MAX
	};
};

class CStateTutorialEnd : public CStateResult
{// �`���[�g���A���̏I��
public:
	CStateTutorialEnd();	// �R���X�g���N�^
	virtual ~CStateTutorialEnd();	 // �f�X�g���N�^

	// �����o�֐�
	void Init(CTutorial *pTutorial) override;	// ����������
	void Uninit(CTutorial *pTutorial) override;	// �I������
	void Update(CTutorial *pTutorial) override;	// �X�V����

private:
	// �����o�֐�
	void ScalingGate(void);	// �Q�[�g�̃X�P�[�����O�␳
	void CollidePlayer(CTutorial *pTutorial);	// �v���C���[�ƃQ�[�g�̔���
	void ForwardPlayer(void);	// �v���C���[��i�߂鏈��

	// �����o�ϐ�
	CEffekseerEffect *m_pEffect;	// �G�t�F�N�g�̃|�C���^
};

namespace Tutorial
{
CUI *CreateUIDefault(void);
}

#endif