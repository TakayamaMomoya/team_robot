//*****************************************************
//
// �u���[�C�x���g�̏���[blurEvent.h]
// Author:���R����
//
//*****************************************************
#ifndef _BLUREVENT_H_    // ��d�C���N���[�h�̖h�~
#define _BLUREVENT_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "object.h"

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CBlurEvent : public CObject
{
public:
    CBlurEvent();	// �R���X�g���N�^
	~CBlurEvent();	// �f�X�g���N�^

    // �����o�֐�
    HRESULT Init(void); // ������
    void Uninit(void);  // �I��
    void Update(void);  // �X�V
    void Draw(void);  // �`��

    // �ÓI�����o�֐�
    static CBlurEvent *Create(float fTime,float fDensity,float fSize);    // ��������
    static CBlurEvent *GetInstance(void) { return s_pBlurEvent; }    // �C���X�^���X�̎擾

private:
    // �����o�ϐ�
    float m_fTimerBlur; // �u���[��������^�C�}�[
    float m_fTimerInitial; // �����^�C�}�[
    float m_fDensityBlur; // �u���[�̔Z��
    float m_fSizeBlur; // �u���[�̃T�C�Y

    // �ÓI�����o�ϐ�
    static CBlurEvent *s_pBlurEvent;   // ���g�̃|�C���^
};

#endif
