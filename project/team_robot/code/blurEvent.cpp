//*****************************************************
//
// �u���[�C�x���g�̏���[blurEvent.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "blurEvent.h"
#include "blur.h"
#include "manager.h"

//*****************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************
CBlurEvent *CBlurEvent::s_pBlurEvent = nullptr;	// ���g�̃|�C���^

//=====================================================
// �R���X�g���N�^
//=====================================================
CBlurEvent::CBlurEvent() : m_fDensityBlur(0.0f), m_fSizeBlur(0.0f), m_fTimerBlur(0.0f), m_fTimerInitial(0.0f)
{

}

//=====================================================
// �f�X�g���N�^
//=====================================================
CBlurEvent::~CBlurEvent()
{

}

//=====================================================
// ��������
//=====================================================
CBlurEvent *CBlurEvent::Create(float fTime, float fDensity, float fSize)
{
	if (s_pBlurEvent == nullptr)
	{
		s_pBlurEvent = new CBlurEvent;

		if (s_pBlurEvent != nullptr)
		{
			s_pBlurEvent->m_fTimerBlur = fTime;
			s_pBlurEvent->m_fDensityBlur = fDensity;
			s_pBlurEvent->m_fSizeBlur = fSize;
			s_pBlurEvent->Init();
		}
	}

	return s_pBlurEvent;
}

//=====================================================
// ����������
//=====================================================
HRESULT CBlurEvent::Init(void)
{
	m_fTimerInitial = m_fTimerBlur;

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CBlurEvent::Uninit(void)
{
	s_pBlurEvent = nullptr;

	Release();
}

//=====================================================
// �X�V����
//=====================================================
void CBlurEvent::Update(void)
{
	m_fTimerBlur -= CManager::GetDeltaTime();

	if (m_fTimerBlur < 0.0f)
	{// �C�x���g�̏I���ƂƂ��Ɏ��g��j��
		// �C�x���g�̏I��
		Blur::ResetBlur();

		// ���g�̏I��
		Uninit();

		return;
	}
	else
	{
		// ���Ԃ̊������v�Z
		float fRate = 1.0f - m_fTimerBlur / m_fTimerInitial;

		// sin�J�[�u�ł̌v�Z
		fRate = sinf(D3DX_PI * fRate);

		// �u���[�̐ݒ�
		float fSize = m_fSizeBlur * fRate;
		float fDesnity = m_fDensityBlur * fRate;
		Blur::SetBlur(fSize, fDesnity);
	}
}

//=====================================================
// �`�揈��
//=====================================================
void CBlurEvent::Draw(void)
{

}