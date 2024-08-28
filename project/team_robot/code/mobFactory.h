//*****************************************************
//
// ���u�t�@�N�g���[[mobFactory.cpp]
// Author:���R����
//
//*****************************************************
#ifndef _MOBFACTORy_H_
#define _MOBFACTORy_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "object.h"
#include "mob.h"

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CMobFactory : public CObject
{
public:
	CMobFactory(int nPriority = 4);	// �R���X�g���N�^
	~CMobFactory();	// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(void);	// �X�V
	void Draw(void);	// �`��
	CMob *CreateMob(CMob::E_Faction fanction);	// ���u�̐���
	void AddAlly(CMob *pMob);	// �����̒ǉ�
	void AddEnemy(CMob *pMob);	// �G�@�̒ǉ�

	// �ϐ��擾�E�ݒ�֐�
	list<CMob*> GetAlly(void) { return m_aAlly; }	// �������X�g
	void SetAlly(list<CMob*> list) { m_aAlly = list; }
	list<CMob*> GetEnemy(void) { return m_aEnemy; }	// �G���X�g
	void SetEnemy(list<CMob*> list) { m_aEnemy = list; }

	// �ÓI�����o�֐�
	static CMobFactory *Create(void);	// ��������
	static CMobFactory *GetInstance(void) { return s_pMobFactory; }	// �C���X�^���X�擾

private:
	// �����o�ϐ�
	list<CMob*> m_aAlly;	// �����̃��X�g
	list<CMob*> m_aEnemy;	// �G�̃��X�g

	// �ÓI�����o�ϐ�
	static CMobFactory *s_pMobFactory;	// ���g�̃|�C���^
};

namespace MobFactroy
{
CMob *CreateMob(CMob::E_Faction fanction);	// ���u�̐���(�V���[�g�J�b�g�֐�)
}

#endif