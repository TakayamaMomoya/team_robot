//*****************************************************
//
// 2D�A�j���[�V�����̏���[animation2D.cpp]
// Author:���R����
//
//*****************************************************

#ifndef _ANIM2D_H_
#define _ANIM2D_H_

#include "polygon2D.h"

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CAnim2D : public CPolygon2D
{
public:
	CAnim2D(int nPriotity = 5);	// �R���X�g���N�^
	~CAnim2D();	// �f�X�g���N�^

	CAnim2D *Create(int nPtnAnim, int nNumAnim, int nTimeAnim, int nNumV);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetInfo(int nPtnAnim, int nNumAnim, int nTimeAnim, int nNumV);

private:
	int m_nCounterAnim;	// �A�j���[�V�����J�E���^�[
	int m_nPatternAnim;	// �A�j���p�^�[��NO.
	int m_nTimerAnim;	// �A�j���[�V�������鎞��
	int m_nNumAnim;	// �A�j���[�V�����̖���
	int m_nNumV;	// �A�j���[�V�����̏c�̍s�Ő܂�Ԃ��܂ł̖���
};

#endif
