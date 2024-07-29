//*****************************************************
//
// �A�j���[�V�����}�l�[�W���[[animManager.h]
// Author:���R����
//
//*****************************************************
#ifndef _ANIMMANAGER_H_	// ��d�C���N���[�h�h�~
#define _ANIMMANAGER_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "object.h"

//*****************************************************
// �O���錾
//*****************************************************
class CAnim3D;

//*****************************************************
// �}�N����`
//*****************************************************
#define MAX_STRING	(256)	// �ő啶����

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CAnimManager : public CObject
{
public:
	// �񋓌^��`
	enum E_TYPE
	{// ���
		TYPE_EXPLOSION = 0,	// ����
		TYPE_FLASH,	// �Ή�
		TYPE_JUMP,	// �����Ԃ�
		TYPE_SLASH,	// �a��
		TYPE_AIRSLASH,	// �󒆎a��
		TYPE_MAX
	};

	CAnimManager(int nPriority = 4);	// �R���X�g���N�^
	~CAnimManager();	// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);	// ����������
	void Uninit(void);	// �I������
	void Update(void);	// �X�V����
	void Draw(void);	// �`�揈��
	CAnim3D *CreateEffect(D3DXVECTOR3 pos, E_TYPE type);	// �G�t�F�N�g�̐���

	// �ÓI�����o�֐�
	static CAnimManager *Create(void);	// ��������
	static CAnimManager *GetInstance(void) { return s_pAnimEffect3D; }	//	�C���X�^���X�̎擾

private:
	// �\���̒�`
	struct S_InfoAnimEffect
	{// �A�j���[�V�����G�t�F�N�g���
		char acPath[MAX_STRING];	// �p�X
		int nNumAnim;	// �A�j���[�V������
		int nSpeedAnim;	// �A�j���[�V�������x
		float fSize;	// �T�C�Y
		D3DXCOLOR col;	// �F
		bool bAdd;	// ���Z�������邩�ǂ���
		bool bLoop;	// ���[�v���邩�ǂ���
		bool bBillboard;	// �r���{�[�h�ɂ��邩�ǂ���
	};
	
	// �����o�֐�
	void Load(void);
	
	// �����o�ϐ�
	S_InfoAnimEffect *m_apAnimEffect[TYPE_MAX];	// ���̃|�C���^

	// �ÓI�����o�ϐ�
	static CAnimManager *s_pAnimEffect3D;	// ���g�̃|�C���^
};

namespace Anim3D
{
//*****************************************************
// �V���[�g�J�b�g�֐�
//*****************************************************
CAnim3D *CreateAnim(D3DXVECTOR3 pos, CAnimManager::E_TYPE type);	// �A�j���[�V�����̐���
}

#endif