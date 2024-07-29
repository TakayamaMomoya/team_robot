//*****************************************************
//
// �RD�A�j���[�V�����G�t�F�N�g����[animEffect3D.h]
// Author:���R����
//
//*****************************************************

#ifndef _ANIMEFFECT3D_H_
#define _ANIMEFFECT3D_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "Object.h"

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
class CAnimEffect3D : public CObject
{
public:
	enum TYPE
	{
		TYPE_MUZZLEFLUSH = 0,	// �}�Y���t���b�V��
		TYPE_BLOOD,	// �����Ԃ�
		TYPE_BLOOD1,	// �����Ԃ�
		TYPE_BEAM,	// �r�[��
		TYPE_EXPLOSION,	// ����
		TYPE_HIT00,	// ����
		TYPE_BOOST,	// �u�[�X�g
		TYPE_PLASMA,	// �v���Y�}
		TYPE_BEAMFLASH,	// �r�[���M��
		TYPE_BIGEXPLOSION,	// �ł�������
		TYPE_MAX
	};

	CAnimEffect3D(int nPriority = 4);	// �R���X�g���N�^
	~CAnimEffect3D();	// �f�X�g���N�^

	static CAnimEffect3D *Create(void);
	CAnim3D *CreateEffect(D3DXVECTOR3 pos, TYPE type);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CAnimEffect3D *GetInstance(void) { return m_pAnimEffect3D; }

private:
	struct SInfoAnimEffect
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
	
	void Load(void);
	
	static CAnimEffect3D *m_pAnimEffect3D;	// ���g�̃|�C���^
	SInfoAnimEffect *m_apAnimEffect[TYPE_MAX];	// ���̃|�C���^
};

namespace Anim3D
{
CAnim3D *CreateAnim(D3DXVECTOR3 pos, CAnimEffect3D::TYPE type);
}

#endif