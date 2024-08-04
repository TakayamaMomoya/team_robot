//*****************************************************
//
// ���u�̏���[mob.cpp]
// Author:���R����
//
//*****************************************************
#ifndef _MOB_H_
#define _MOB_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "motion.h"

//*****************************************************
// �O���錾
//*****************************************************

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CMob : public CMotion
{
public:
	// �񋓌^��`
	enum E_Faction
	{// ����
		FACTION_NONE = 0,	// ���ł��Ȃ�
		FACTION_ALLY,	// ����
		FACTION_ENEMY,	// �G
		FACTION_MAX
	};

	CMob(int nPriority = 4);	// �R���X�g���N�^
	~CMob();	// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(void);	// �X�V
	void Draw(void);	// �`��

	// �ϐ��擾�E�ݒ�֐�
	void SetLife(float fLife) { m_fLife = fLife; }	// ���C�t
	float GetLife(void) { return m_fLife; }
	void SetFanction(E_Faction fanction) { m_faction = fanction; }	// ����
	E_Faction GetFanction(void) { return m_faction; }

	// �ÓI�����o�֐�
	static CMob *Create(void);	// ��������

private:
	// �����o�ϐ�
	float m_fLife = 0.0f;	// �̗�
	E_Faction m_faction = FACTION_NONE;	// ����
};

#endif