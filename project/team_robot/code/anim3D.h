//*****************************************************
//
// �RD�A�j���[�V��������[anim3D.h]
// Author:���R����
//
//*****************************************************

#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "polygon3D.h"

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CAnim3D : public CPolygon3D
{
public:
	CAnim3D(int nPriority = 6);	// �R���X�g���N�^
	~CAnim3D();	// �f�X�g���N�^

	static CAnim3D *Create(D3DXVECTOR3 pos, int nNumAnim, int nTimeAnim, bool bLoop = false);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void RandStart(void);	// �A�j���[�V�����̃X�^�[�g�������_���ɂ��鏈��

private:
	void SetAnim(int nAnim, int nNumAnim);

	int m_nCounterAnim;	// �A�j���[�V�����J�E���^�[
	int m_nPatternAnim;	// �A�j���p�^�[��NO.
	int m_nSpeedAnim;	// �A�j���[�V�������x
	int m_nNumAnim;	// �A�j���[�V��������
	bool m_bLoop;	// ���[�v���邩�ǂ���
	bool m_bFinish;	// �I���������ǂ���
};

#endif