//*****************************************************
//
// ����̊Ǘ�[weaponManager.h]
// Author:���R����
//
//*****************************************************
#ifndef _WEAPONMANAGER_H_
#define _WEAPONMANAGER_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "object.h"

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CWeaponManager : public CObject
{
public:
	CWeaponManager();	// �R���X�g���N�^
	~CWeaponManager();	// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(void);	// �X�V
	void Draw(void);	// �`��

	// �ÓI�����o�֐�
	static CWeaponManager *Create(void);

private:
	// �����o�ϐ�
	//vector<CMob*> m_aAlly;	// �����̃��X�g

	// �ÓI�����o�ϐ�
	static CWeaponManager *s_pMobFactory;	// ���g�̃|�C���^
};

#endif