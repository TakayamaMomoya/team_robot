//*****************************************************
//
// �X���[�̏���[slow.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "slow.h"
#include "debugproc.h"
#include "manager.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{

}

//*****************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************
CSlow *CSlow::m_pSlow = nullptr;	// ���g�̃|�C���^

//=====================================================
// �R���X�g���N�^
//=====================================================
CSlow::CSlow()
{
	m_pSlow = this;

	m_fScale = 0.0f;
	m_fTimeSlow = 0.0f;
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CSlow::~CSlow()
{

}

//=====================================================
// ��������
//=====================================================
CSlow *CSlow::Create(void)
{
	if (m_pSlow == nullptr)
	{
		m_pSlow = new CSlow;

		if (m_pSlow != nullptr)
		{
			m_pSlow->Init();
		}
	}

	return m_pSlow;
}

//=====================================================
// ����������
//=====================================================
HRESULT CSlow::Init(void)
{
	m_fScale = 1.0f;

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CSlow::Uninit(void)
{
	m_pSlow = nullptr;

	Release();
}

//=====================================================
// �X�V����
//=====================================================
void CSlow::Update(void)
{
	if (m_fTimeSlow > 0.0f)
	{
		float fDeltaTime = CManager::GetDeltaTime();

		m_fTimeSlow -= fDeltaTime;

		if (m_fTimeSlow <= 0.0f)
		{
			SetScale(1.0f);
		}
	}
}

//=====================================================
// �`�揈��
//=====================================================
void CSlow::Draw(void)
{
#ifdef _DEBUG
	CDebugProc *pDebugProc = CDebugProc::GetInstance();

	if (pDebugProc != nullptr)
	{
		pDebugProc->Print("\n�X���[�{��[%f]",m_fScale);
	}
#endif
}

//=====================================================
// �X�P�[���̐ݒ�
//=====================================================
void CSlow::SetScale(float fScale)
{
	if (fScale >= 0.0f)
	{
		m_fScale = fScale;
	}
}

//=====================================================
// �X���[�^�C���̐ݒ�
//=====================================================
void CSlow::SetSlowTime(float fTime, float fScale)
{
	m_fTimeSlow = fTime;

	universal::LimitValuefloat(&m_fTimeSlow, FLT_MAX, 0.0f);
	
	SetScale(fScale);
}

namespace Slow
{
float GetScale(void)
{
	float fScale = 1.0f;

	CSlow *pSlow = CSlow::GetInstance();

	if (pSlow != nullptr)
	{
		fScale = pSlow->GetScale();
	}

	return fScale;
}
}