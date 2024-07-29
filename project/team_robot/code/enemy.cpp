//*****************************************************
//
// �G�l�~�[�̏���[enemy.cpp]
// Author:�匴�叫
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "enemy.h"
#include "debugproc.h"
#include "effect3D.h"
#include "shuriken.h"
#include "player.h"
#include "orbit.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
	const float SPEED_DEFAULT = 50.0f;
}

//=====================================================
// �R���X�g���N�^
//=====================================================
CEnemy::CEnemy(int nPriority)
{

}

//=====================================================
// �f�X�g���N�^
//=====================================================
CEnemy::~CEnemy()
{

}

//=====================================================
// ��������
//=====================================================
CEnemy* CEnemy::Create()
{
	CEnemy *pEnemy = nullptr;

	if (pEnemy == nullptr)
	{
		pEnemy = new CEnemy;

		if (pEnemy != nullptr)
		{
			// ������
			pEnemy->Init();
		}
	}

	return pEnemy;
}

//=====================================================
// ����������
//=====================================================
HRESULT CEnemy::Init(void)
{
	m_info.nIdxSpline = 1;

	// �p���N���X�̏�����
	CMotion::Init();

	// ���f���̐ݒ�
	CMotion::Load("data\\MOTION\\motionEnemy.txt");

	CEnemyBehaviour *pBehaviour = new CEnemyBehaviourChasePlayer;

	pBehaviour->Init(this);

	m_listBehaviour.push_back(pBehaviour);

	m_info.pOrbit = COrbit::Create(GetMatrix(), D3DXVECTOR3(20.0f, 220.0f, -80.0f), D3DXVECTOR3(-20.0f, 220.0f, -80.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 60);

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CEnemy::Uninit(void)
{
	m_info.pOrbit->SetEnd(true);

	// �S�r�w�C�r�A���
	ReleaseAllBehaviour();

	// �p���N���X�̏I��
	CMotion::Uninit();
}

//=====================================================
// �X�V����
//=====================================================
void CEnemy::Update(void)
{
	// �p���N���X�̍X�V
	CMotion::Update();

	// �r�w�C�r�A�̍X�V
	for (auto it : m_listBehaviour)
	{
		it->Update(this);

		if (m_listBehaviour.empty())
			break;
	}

	if (m_info.pOrbit != nullptr)
		m_info.pOrbit->SetOffset(GetMatrix(), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), m_info.pOrbit->GetID());

	// ���[�V�����̊Ǘ�
	ManageMotion();
}

//=====================================================
// ���[�V�����̊Ǘ�
//=====================================================
void CEnemy::ManageMotion(void)
{
	int nMotion = GetMotion();
	bool bFinish = IsFinish();

	if (m_fragMotion.bAttack)
	{// �U�����[�V����
		if (nMotion != MOTION::MOTION_ATTACK)
		{
			SetMotion(MOTION::MOTION_ATTACK);
		}

		if (bFinish)
		{
			m_fragMotion.bAttack = false;

			CShuriken::Create(GetMtxPos(5), GetForward());
		}
	}
	else
	{// �j���[�g�������[�V����
		if (nMotion != MOTION::MOTION_NEUTRAL)
		{
			SetMotion(MOTION::MOTION_NEUTRAL);
		}
	}
}

//=====================================================
// �S�r�w�C�r�A���
//=====================================================
void CEnemy::ReleaseAllBehaviour(void)
{
	for (auto it : m_listBehaviour)
	{
		it->Uninit(this);

		delete it;
	}

	m_listBehaviour.clear();
}

//=====================================================
// �`�揈��
//=====================================================
void CEnemy::Draw(void)
{
	// �p���N���X�̕`��
	CMotion::Draw();
}