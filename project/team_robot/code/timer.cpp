//*****************************************************
//
// �^�C���̏���[timer.cpp]
// Author:�X��x��
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "timer.h"
#include "manager.h"
#include "debugproc.h"
#include "goal.h"
#include "player.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const int MINUTES_LIMIT = 9;		// ���̏���l
const int SECOND_LIMIT = 59;		// �b�̏���l
const int MILLI_LIMIT = 99;			// �~���b�̏��
const int MINUTES_DIGIT = 1;		// ���\���̌���
const int TIME_DIGIT = 2;			// ���ꂼ��̌���
const float MAGNIFICATION = 100.0f;	// �|����{��
const float MINUTES_WIDTH = SCREEN_WIDTH * 0.44f;	// ����X���W
const float SECOND_WIDTH = SCREEN_WIDTH * 0.5f;		// �b��X���W
const float MILLI_WIDTH = SCREEN_WIDTH * 0.6f;		// �~���b��X���W
}

//*****************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************
CTimer* CTimer::s_pTimer = nullptr;

//=====================================================
// �D�揇�ʂ����߂�R���X�g���N�^
//=====================================================
CTimer::CTimer(int nPriority) : CObject(nPriority)
{
	m_fSecond = 0.0f;			// ���݂̎���(�b)
	m_fMilli = 0.0f;			// ���݂̎���(�~���b)
	m_IsStop = false;
	m_pMinutes = nullptr;		// ���\���̃|�C���^
	m_pSecond = nullptr;		// �b�\���p�̃|�C���^
	m_pMilli = nullptr;			// �~���Z�J���h�\���p�̃|�C���^
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CTimer::~CTimer()
{
}

//=====================================================
// ��������
//=====================================================
CTimer* CTimer::Create(void)
{
	if (s_pTimer == nullptr)
	{// �C���X�^���X����
		s_pTimer = new CTimer;
	}

	if (s_pTimer != nullptr)
	{// ������
		s_pTimer->Init();

		if (s_pTimer->m_pMinutes == nullptr)
		{// ���\��
			// �����E�ʒu�A�T�C�Y�ݒ�
			s_pTimer->m_pMinutes = CNumber::Create(MINUTES_DIGIT, MINUTES_LIMIT);
			s_pTimer->m_pMinutes->SetPosition(D3DXVECTOR3(MINUTES_WIDTH, 75.0f, 0.0f));
			s_pTimer->m_pMinutes->SetSizeAll(35.0f, 35.0f);
		}

		if (s_pTimer->m_pSecond == nullptr)
		{// �b�\��
			// �����E�ʒu�A�T�C�Y�ݒ�
			s_pTimer->m_pSecond = CNumber::Create(TIME_DIGIT, SECOND_LIMIT);
			s_pTimer->m_pSecond->SetPosition(D3DXVECTOR3(SECOND_WIDTH, 75.0f, 0.0f));
			s_pTimer->m_pSecond->SetSizeAll(35.0f, 35.0f);
		}

		if (s_pTimer->m_pMilli == nullptr)
		{// �~���b�\��
			// �����E�ʒu�A�T�C�Y�ݒ�
			s_pTimer->m_pMilli = CNumber::Create(TIME_DIGIT, MILLI_LIMIT);
			s_pTimer->m_pMilli->SetPosition(D3DXVECTOR3(MILLI_WIDTH, 85.0f, 0.0f));
			s_pTimer->m_pMilli->SetSizeAll(25.0f,15.0f);
		}
	}

	return s_pTimer;
}

//=====================================================
// ����������
//=====================================================
HRESULT CTimer::Init(void)
{
	// ���E�b�E�~���b�̏�����
	m_fSecond = 0.0f;
	m_fMilli = 0.0f;

	m_IsStop = false;	// �^�C�}�[��~�̃t���O

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CTimer::Uninit(void)
{
	// �ÓI�����o�ϐ��̏I��
	if (s_pTimer != nullptr)
		s_pTimer = nullptr;

	if (m_pMinutes != nullptr)
	{// ���̏I��
		m_pMinutes->Uninit();
		m_pMinutes = nullptr;
	}

	if (m_pSecond != nullptr)
	{// �b�̏I��
		m_pSecond->Uninit();
		m_pSecond = nullptr;
	}

	if (m_pMilli != nullptr)
	{// �~���b�̏I��
		m_pMilli->Uninit();
		m_pMilli = nullptr;
	}

	// ���g�̔j��
	Release();
}

//=====================================================
// �X�V����
//=====================================================
void CTimer::Update(void)
{
	// ���̍X�V
	if (!m_IsStop)
	{
		Minutes();
	}
}

//=====================================================
// ���̍X�V
//=====================================================
void CTimer::Minutes()
{
	// �b�̍X�V
	Second();

	// ���̌v�Z
	int nMinutes = (int)m_fSecond / SECOND_LIMIT;

	// ���̏���l�����Ȃ��悤��
	if (nMinutes >= MINUTES_LIMIT)
		nMinutes = MINUTES_LIMIT;

	// ���\��
	if (m_pMinutes != nullptr)
		m_pMinutes->SetValue(nMinutes, MINUTES_DIGIT);
}

//=====================================================
// �b�̍X�V
//=====================================================
void CTimer::Second()
{
	// �~���b�̍X�V
	MilliSecond();

	// �f���^�^�C���擾
	float fDeltaTime = CManager::GetDeltaTime();

	// �f���^�^�C��(���A������)���Z
	m_fSecond += fDeltaTime;

	// �b�̌v�Z
	int nSecond = (int)m_fSecond % SECOND_LIMIT;

	// �b�\��
	if (m_pSecond != nullptr)
		m_pSecond->SetValue(nSecond, TIME_DIGIT);
}

//=====================================================
// �~���b�X�V
//=====================================================
void CTimer::MilliSecond()
{
	// �f���^�^�C���擾
	float fDeltaTime = CManager::GetDeltaTime();

	// �f���^�^�C��(���A������)���Z
	m_fMilli += fDeltaTime;

	// �b��100�{
	m_fMilli = m_fSecond * MAGNIFICATION;

	// �~���b�̌v�Z
	int nMsecond = (int)m_fMilli % MILLI_LIMIT;

	// �~���b�\��
	if (m_pMilli != nullptr)
		m_pMilli->SetValue(nMsecond, TIME_DIGIT);
}
