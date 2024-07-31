//*****************************************************
//
// �v���C���[�R���g���[���[[playercontroller.cpp]
// Author:���R����
//
//*****************************************************
#ifndef _PLAYERCONTROLLER_H_
#define _PLAYERCONTROLLER_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "object.h"

//*****************************************************
// �O���錾
//*****************************************************
class CPlayer;

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CPlayerController : public CObject
{
public:
	CPlayerController();	// �R���X�g���N�^
	~CPlayerController();	// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(void);	// �X�V

private:
	// �����o�֐�
	void InputMove(void);	// �ړ��̓���
};

#endif