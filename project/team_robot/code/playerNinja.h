//*****************************************************
//
// �v���C���[�E��[playerNinja.cpp]
// Author:���R����
//
//*****************************************************
#ifndef _PLAYERNINJA_H_
#define _PLAYERNINJA_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "motion.h"

//*****************************************************
// �O���錾
//*****************************************************
class CBlockGrab;
class CPolygon3D;
class CPlayerNinja;
class CEffekseerEffect;

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CPlayerNinja : public CMotion
{
public:
	CPlayerNinja(int nPriority = 4);	// �R���X�g���N�^
	~CPlayerNinja();	// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(void);	// �X�V
	void Draw(void);	// �`��

	// �ÓI�����o�֐�
	static CPlayerNinja *Create(void);	// ����

private:
	// �����o�֐�
	void Event(EVENT_INFO *pEventInfo);	// �C�x���g
	void ManagekatanaCollision(D3DXVECTOR3 pos);	// ���̔���̊Ǘ�
};

#endif