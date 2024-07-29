//*****************************************************
//
// �S�[������[goal.cpp]
// Author:�X��x��
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "goal.h"
#include "universal.h"
#include "debugproc.h"
#include "effect3D.h"
#include "fade.h"
#include "game.h"
#include "timer.h"
#include "result.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const float MOVESPEED = 4.0f;	// �ړ����x
const float WIDTH_GOAL = 100.0f;	// �S�[���̕�
}

//*****************************************************
// �ÓI�����o�ϐ�
//*****************************************************
CGoal *CGoal::m_pGoal = nullptr;	// ���g�̃|�C���^

//=====================================================
// �R���X�g���N�^
//=====================================================
CGoal::CGoal(int nPriority)
{
	m_nTransitionTime = 0;		// �J�ڃJ�E���^�[
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ʒu
	m_posStart = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �S�[���̎n�_
	m_posEnd = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �S�[���̏I�_
	m_fRot = 0.0f;		// ����
	m_pObj3D = nullptr;		// �I�u�W�F�N�g3D�̃|�C���^
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CGoal::~CGoal()
{
}

//=====================================================
// ����
//=====================================================
CGoal* CGoal::Create(D3DXVECTOR3 pos,float fRot, float fLength)
{
	if (m_pGoal != nullptr)
		return m_pGoal;

	m_pGoal = new CGoal;

	if (m_pGoal != nullptr)
	{
		m_pGoal->m_pos = pos;
		m_pGoal->m_fRot = fRot;
		m_pGoal->m_fLength = fLength;

		// ������
		m_pGoal->Init();
	}

	return m_pGoal;
}

//=====================================================
// ������
//=====================================================
HRESULT CGoal::Init()
{
	// �S�[���e�[�v�̐���
	m_pObj3D = CPolygon3D::Create(m_pos);

	// �S�[���̐ݒ�
	SetGoal();

	return S_OK;
}

//=====================================================
// �S�[���̐ݒ�
//=====================================================
void CGoal::SetGoal(void)
{
	// �n�_�E�I�_�̌v�Z
	D3DXVECTOR3 vec = { sinf(m_fRot) * m_fLength ,0.0f,cosf(m_fRot) * m_fLength };

	m_posStart = m_pos + vec;
	m_posEnd = m_pos - vec;

	if (m_pObj3D != nullptr)
	{
		// �F�ݒ�
		m_pObj3D->SetColor(D3DXCOLOR(1.0f, 0.5f, 0.0f, 0.5f));

		// �T�C�Y�ݒ�
		m_pObj3D->SetSize(WIDTH_GOAL, m_fLength);

		m_pObj3D->SetRotation(D3DXVECTOR3(0.0f, m_fRot, 0.0f));
		m_pObj3D->SetPosition(m_pos);
	}
}

//=====================================================
// �I��
//=====================================================
void CGoal::Uninit()
{
	m_pGoal = nullptr;

	if (m_pObj3D != nullptr)
	{
		m_pObj3D->Uninit();
		m_pObj3D = nullptr;
	}

	// ���g�̍폜
	Release();
}

//=====================================================
// �X�V
//=====================================================
void CGoal::Update()
{
	// �v���C���[���擾
	CPlayer* pPlayer = CPlayer::GetInstance();

	// �t�F�[�h���擾
	CFade* pFade = CFade::GetInstance();

	// �ʒu�擾
	D3DXVECTOR3 posPlayer = pPlayer->GetPosition();

	// �ړ��ʎ擾
	D3DXVECTOR3 movePlayer = pPlayer->GetMove();

	// �^�C�}�[�̎擾
	CTimer* pTimer = CTimer::GetInstance();

	// ��_�̊���
	float fCross = 0.0f;

	bool bHit = universal::IsCross(posPlayer,		// �v���C���[�̈ʒu
		m_posStart,		// �S�[���̎n�_
		m_posEnd,			// �S�[���̏I�_
		&fCross,		// ��_�̊���
		posPlayer + movePlayer);	// �v���C���[�̈ړ���

	bool bHitNext = universal::IsCross(posPlayer + movePlayer,		// �v���C���[�̎���̈ʒu
		m_posStart,		// �S�[���̎n�_
		m_posEnd,			// �S�[���̏I�_
		nullptr,		// ��_�̊���
		posPlayer + movePlayer);	// �v���C���[�̈ړ���

	// �O�ς̔���
	if (!bHit && bHitNext)
	{
		if (fCross >= 0.0f && fCross <= 1.0f)
		{// �n�_�ƏI�_�̊Ԃ�ʂ�����
			// ���U���g�̐���
			CResult::Create();
		}
	}

#ifdef _DEBUG
	CEffect3D::Create(m_posStart, 400.0f, 3, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
	CEffect3D::Create(m_posEnd, 200.0f, 3, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
#endif

#if 0
	CDebugProc::GetInstance()->Print("\nfCross�̒l[%f]", fCross);
	CDebugProc::GetInstance()->Print("\nbHit[%d]", bHit);
	CDebugProc::GetInstance()->Print("\nbHitOld[%d]", bHitNext);
	//CDebugProc::GetInstance()->Print("\nstartPos�̈ʒu[%f, %f, %f]", m_posStart.x, m_posStart.y, m_posStart.z);
	//CDebugProc::GetInstance()->Print("\nendPos�̈ʒu[%f, %f, %f]", m_posEnd.x, m_posEnd.y, m_posEnd.z);
#endif
}

//=====================================================
// �`��
//=====================================================
void CGoal::Draw()
{

}
