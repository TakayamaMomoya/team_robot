//*****************************************************
//
// �G�X�|�i�[[enemySpawner.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "enemySpawner.h"
#include "player.h"
#include "effect3D.h"
#include "enemy.h"
#include "meshRoad.h"
#include "myLib.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const float MOVESPEED = 4.0f;	// �ړ����x
const float WIDTH_GOAL = 100.0f;	// �S�[���̕�
const float NEXT_INTERPOLATE_ROAD = 0.1f;	// ���̈ʒu�Ƃ݂Ȃ��␳�̒l
}

//=====================================================
// �R���X�g���N�^
//=====================================================
CEnemySpawner::CEnemySpawner(int nPriority)
{
	m_nTransitionTime = 0;		// �J�ڃJ�E���^�[
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ʒu
	m_posStart = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �S�[���̎n�_
	m_posEnd = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �S�[���̏I�_
	m_fRot = 0.0f;		// ����
	m_nIdxSpline = 0;	// �X�v���C���̔ԍ�
	m_pObj3D = nullptr;		// �I�u�W�F�N�g3D�̃|�C���^
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CEnemySpawner::~CEnemySpawner()
{

}

//=====================================================
// ����
//=====================================================
CEnemySpawner* CEnemySpawner::Create(int nIdxSpline,float fRot, float fLength)
{
	CEnemySpawner *pSpawner = nullptr;

	pSpawner = new CEnemySpawner;

	if (pSpawner != nullptr)
	{
		pSpawner->m_nIdxSpline = nIdxSpline;
		pSpawner->m_fRot = fRot;
		pSpawner->m_fLength = fLength;

		pSpawner->SetPositionSpline();

		// ������
		pSpawner->Init();
	}

	return pSpawner;
}

//=====================================================
// ������
//=====================================================
HRESULT CEnemySpawner::Init()
{
#ifdef _DEBUG
	// �e�[�v�̐���
	m_pObj3D = CPolygon3D::Create(m_pos);
#endif

	// �|���S���̐ݒ�
	SetPolygon();

	return S_OK;
}

//=====================================================
// �X�v���C���ɑΉ������ʒu�̐ݒ�
//=====================================================
void CEnemySpawner::SetPositionSpline(void)
{
	// ���b�V�����[�h�̎擾
	CMeshRoad *pMesh = CMeshRoad::GetInstance();

	if (pMesh == nullptr)
		return;

	// ���[�h�|�C���g�̈ʒu�̎擾
	CCutMullSpline spline = *pMesh->GetCenterSpline();

	D3DXVECTOR3 pos;

	pos = spline.Interpolate(0.0f, m_nIdxSpline);

	SetPosition(pos);

	// ����̃x�N�g���Ƃ̍����Ō��������߂�
	D3DXVECTOR3 posNext = spline.Interpolate(NEXT_INTERPOLATE_ROAD, m_nIdxSpline);

	D3DXVECTOR3 vecDiff = posNext - pos;

	float fRot = atan2f(vecDiff.x, vecDiff.z);

	SetRotation(fRot - D3DX_PI * 0.5f);
}

//=====================================================
// �|���S���̐ݒ�
//=====================================================
void CEnemySpawner::SetPolygon(void)
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
void CEnemySpawner::Uninit()
{
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
void CEnemySpawner::Update()
{
	// �v���C���[���擾
	CPlayer* pPlayer = CPlayer::GetInstance();

	// �ʒu�擾
	D3DXVECTOR3 posPlayer = pPlayer->GetPosition();

	// �ړ��ʎ擾
	D3DXVECTOR3 movePlayer = pPlayer->GetMove();

	// ��_�̊���
	float fCross = 0.0f;

	// �O�ς̔���
	if (universal::IsCross(posPlayer,		// �v���C���[�̈ʒu
		m_posStart,		// �S�[���̎n�_
		m_posEnd,			// �S�[���̏I�_
		&fCross,		// ��_�̊���
		posPlayer + movePlayer))	// �v���C���[�̈ړ���
	{
		if (fCross >= 0.0f && fCross <= 1.0f)
		{// �n�_�ƏI�_�̊Ԃ�ʂ�����
			// �G�̐���
			CreateEnemy();

			// ���g�̔j��
			Uninit();
		}
	}

#ifdef _DEBUG
	CEffect3D::Create(m_posStart, 200.0f, 3, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
	CEffect3D::Create(m_posEnd, 200.0f, 3, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
#endif

#if 0
	CDebugProc::GetInstance()->Print("\nfCross�̒l[%f]", fCross);
	CDebugProc::GetInstance()->Print("\nstartPos�̈ʒu[%f, %f, %f]", m_posStart.x, m_posStart.y, m_posStart.z);
	CDebugProc::GetInstance()->Print("\nendPos�̈ʒu[%f, %f, %f]", m_posEnd.x, m_posEnd.y, m_posEnd.z);
#endif
}

//=====================================================
// �G�̐���
//=====================================================
void CEnemySpawner::CreateEnemy(void)
{
	CEnemy *pEnemy = CEnemy::Create();

	if (pEnemy == nullptr)
		return;

	pEnemy->SetIdxSpline(m_nIdxSpline - 1);
}

//=====================================================
// �`��
//=====================================================
void CEnemySpawner::Draw()
{

}