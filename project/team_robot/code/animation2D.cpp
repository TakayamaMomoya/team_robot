//*****************************************************
//
// �v���C���[�̏���[player.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "main.h"
#include "animation2D.h"
#include "manager.h"

//*****************************************************
// �}�N����`
//*****************************************************
#define ANIM_TIME				(2)											//�A�j���[�V�������s���p�x
#define PLAYER_ANIM				(10)										//�v���C���[�̃A�j���[�V��������

//=====================================================
// �R���X�g���N�^
//=====================================================
CAnim2D::CAnim2D(int nPriority) : CPolygon2D(nPriority)
{
	m_nCounterAnim = 0;
	m_nPatternAnim = 0;
	m_nNumAnim = 0;
	m_nTimerAnim = 0;
	m_nNumV = 0;
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CAnim2D::~CAnim2D()
{

}

//=====================================================
// ��������
//=====================================================
CAnim2D *CAnim2D::Create(int nPtnAnim, int nNumAnim, int nTimeAnim, int nNumV)
{
	CAnim2D *pAnim2D = nullptr;

	if (pAnim2D != nullptr)
	{
		// ����������
		pAnim2D->Init();

		// �A�j���[�V�������ݒ�
		SetInfo(nPtnAnim, nNumAnim, nTimeAnim, nNumV);
	}

	return pAnim2D;
}

//=====================================================
// ����������
//=====================================================
HRESULT CAnim2D::Init(void)
{
	// �p���N���X�̏�����
	CPolygon2D::Init();

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CAnim2D::Uninit(void)
{
	// �p���N���X�̏I��
	CPolygon2D::Uninit();
}

//=====================================================
// �X�V����
//=====================================================
void CAnim2D::Update(void)
{
	// �J�E���^�[�����Z
	m_nCounterAnim++;

	if (m_nCounterAnim % m_nTimerAnim == 0)
	{// ��莞�Ԃ��ƂɃA�j���[�V�������s��
		m_nCounterAnim = 0;
		m_nPatternAnim++;

		// �A�j���[�V�������𒴂�����0���ɖ߂�
		m_nPatternAnim = m_nPatternAnim % m_nNumAnim;
	}

	// �p���N���X�̍X�V
	CPolygon2D::Update();

	SetAnim(m_nPatternAnim,m_nNumAnim,m_nNumV);

	SetVtx();
}

//=====================================================
// �`�揈��
//=====================================================
void CAnim2D::Draw(void)
{
	// �p���N���X�̕`��
	CPolygon2D::Draw();
}

//=====================================================
// �ݒ菈��
//=====================================================
void CAnim2D::SetInfo(int nPtnAnim, int nNumAnim, int nTimeAnim, int nNumV)
{
	m_nPatternAnim = nPtnAnim;
	m_nNumAnim = nNumAnim;
	m_nTimerAnim = nTimeAnim;
	m_nNumV = nNumV;
}