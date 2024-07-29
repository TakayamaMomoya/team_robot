//*****************************************************
//
// �G�l�~�[�r�w�C�r�A[enemyBehaviour.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "enemyBehaviour.h"
#include "enemy.h"
#include "effect3D.h"
#include "debugproc.h"
#include "player.h"
#include "myLib.h"
#include "meshRoad.h"
#include "manager.h"
#include "MyEffekseer.h"
#include "shuriken.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const float SPEED_DEFAULT_CHASE = 50.0f;	// �ǐՂ̕W�����x
const float TIME_THROW_SHURIKEN = 2.0f;	    // �藠���𓊂���p�x
const float LENGTH_PLAYER_FRONT = 3000.0f;	// �v���C���[�̑O�Ƃ݂Ȃ�����
const float SPEED_ESCAPE = 5.0f;            // ������Ƃ��ɉ��Z���鑬�x
const float TIME_ESCAPE = 2.0f;             // �藠�������ׂē����Ă��瓦����܂ł̎���
const int MAX_SHURIKEN = 5;                 // ������藠���̐�
}

//********************************************************************************
// �v���C���[�̒ǐ�
//********************************************************************************
//=====================================================
// �R���X�g���N�^
//=====================================================
CEnemyBehaviourChasePlayer::CEnemyBehaviourChasePlayer()
{
	m_state = STATE::STATE_NONE;
	m_fTimerAttack = 0.0f;
	m_nNumShuriken = 0;
	m_pBigShuriken = nullptr;
	m_pFlashEffect = nullptr;
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CEnemyBehaviourChasePlayer::~CEnemyBehaviourChasePlayer()
{

}

//=====================================================
// ������
//=====================================================
void CEnemyBehaviourChasePlayer::Init(CEnemy *pEnemy)
{
	m_state = STATE::STATE_CHASE;

	// �X�v���C���̏����擾
	m_pSpline = CMeshRoad::GetInstance()->GetCenterSpline();

	// ���[�h�|�C���g�̏����擾
	std::vector<CMeshRoad::SInfoRoadPoint> pRoadPoint = *CMeshRoad::GetInstance()->GetArrayRP();

	// �x�N�^�[��K�v�ȃT�C�Y�ɒ���
	int nSize = pRoadPoint.size();
	m_vPos.resize(nSize);

	for (int i = 0; i < nSize; i++)
	{
		m_vPos[i] = pRoadPoint[i].pos;
	}

	// ���S�X�v���C���̏�����
	if (m_pSpline != nullptr)
		m_pSpline->Init(m_vPos);

	// �����X�s�[�h�̌v�Z
	m_fSpeedDefault = SPEED_DEFAULT_CHASE;
	CalcSpeed(pEnemy);

	m_nNumShuriken = MAX_SHURIKEN;

	pEnemy->SetPosition(D3DXVECTOR3(pRoadPoint[0].pos.x, pRoadPoint[0].pos.y, pRoadPoint[0].pos.z));

	// �ł����藠���̐���
	CreateBigShuriken();
}

//=====================================================
// �I��
//=====================================================
void CEnemyBehaviourChasePlayer::Uninit(CEnemy *pEnemy)
{
	if (m_pSpline != nullptr)
	{
		m_pSpline = nullptr;
	}

	m_pFlashEffect = nullptr;
}

//=====================================================
// �X�V
//=====================================================
void CEnemyBehaviourChasePlayer::Update(CEnemy *pEnemy)
{
	
	// ��Ԃ̊Ǘ�
	ManageState(pEnemy);

	// ���S���Ă��邩�擾
	bool bDeath = pEnemy->IsDeath();

	// ���S���Ă����ꍇ�����𔲂���
	if (bDeath)
		return;

	// �ʒu�̕��
	InterpolatePosition(pEnemy);

	// �藠���̒Ǐ]
	FollowBigShuriken(pEnemy);

	// �G�t�F�N�g�̒Ǐ]
	ThrowEffect(pEnemy);
}

//=====================================================
// ��Ԃ̊Ǘ�
//=====================================================
void CEnemyBehaviourChasePlayer::ManageState(CEnemy *pEnemy)
{
	switch (m_state)
	{
	case CEnemyBehaviourChasePlayer::STATE_CHASE:	// �ǐՏ��
	{
		// ���x��ǐ՗p�ɉ���
		CPlayer *pPlayer = CPlayer::GetInstance();

		m_fSpeedDefault += (pPlayer->GetSpeed() * 4.0f - m_fSpeedDefault) * 0.1f;

		CalcSpeed(pEnemy);

		// �v���C���[�̑O�ɏo�����̔���
		bool bFront = CollidePlayerFront(pEnemy);

		if (!bFront)
		{// �v���C���[�̑O�ɏo����U�����J�n
			m_state = STATE_ATTACK;
		}

		CDebugProc::GetInstance()->Print("\n�G[�ǐՏ��]");
	}
		break;
	case CEnemyBehaviourChasePlayer::STATE_ATTACK:	// �U�����
	{
		// ���x���v���C���[�ɍ��킹��
		CPlayer *pPlayer = CPlayer::GetInstance();

		if (CollidePlayerFront(pEnemy))
		{
			m_fSpeedDefault += (pPlayer->GetSpeed() * 0.5f - m_fSpeedDefault) * 0.1f;

			CDebugProc::GetInstance()->Print("\n�G������");

			// ��莞�Ԃ��ƂɎ藠���𓊂���
			float fDeltaTime = CManager::GetDeltaTime();
			m_fTimerAttack += fDeltaTime;

			if (TIME_THROW_SHURIKEN <= m_fTimerAttack)
			{
				// �^�C�}�[���Z�b�g
				m_fTimerAttack = 0.0f;

				// �藠�������ׂē������瓦����
				if (m_nNumShuriken <= 0)
				{
					m_state = STATE_ESCAPE;
					D3DXVECTOR3 pos = pEnemy->GetMtxPos(1);
					MyEffekseer::CreateEffect(CEffekseer::TYPE::TYPE_ENGINE, pos);
					break;
				}
					
				// �藠���𓊂���
				ThrowShuriken(pEnemy);

				m_nNumShuriken--;
			}
		}
		else
		{
			m_fSpeedDefault += (pPlayer->GetSpeed() * 1.5f - m_fSpeedDefault) * 0.1f;

			CDebugProc::GetInstance()->Print("\n�G������");
		}

		// �X�s�[�h�̌v�Z
		CalcSpeed(pEnemy);

		CDebugProc::GetInstance()->Print("\n�G[�U�����]");
	}
		break;
	case CEnemyBehaviourChasePlayer::STATE_ESCAPE:	// �������
	{
		m_fSpeedDefault += SPEED_ESCAPE;

		// �X�s�[�h�̌v�Z
		CalcSpeed(pEnemy);

		// ��莞�Ԍo�Ɠ�����
		float fDeltaTime = CManager::GetDeltaTime();
		m_fTimerAttack += fDeltaTime;

		if (TIME_ESCAPE <= m_fTimerAttack)
		{
			pEnemy->Uninit();
			D3DXVECTOR3 pos = pEnemy->GetMtxPos(1);
			MyEffekseer::CreateEffect(CEffekseer::TYPE::TYPE_SMOKE, pos);
			return;
		}

		CDebugProc::GetInstance()->Print("\n�G�������");
	}
		break;
	default:
		break;
	}
}

//=====================================================
// �v���C���[�̑O�ɏo������
//=====================================================
bool CEnemyBehaviourChasePlayer::CollidePlayerFront(CEnemy *pEnemy)
{
	CPlayer *pPlayer = CPlayer::GetInstance();

	if (pPlayer == nullptr)
		return false;

	D3DXVECTOR3 posPlayer = pPlayer->GetPosition();
	D3DXVECTOR3 posEnemy = pEnemy->GetPosition();

	// �O���x�N�g���̐����ɐ����𐶐�
	D3DXVECTOR3 vecForward = posEnemy - pEnemy->GetForward() * LENGTH_PLAYER_FRONT;

	CEffect3D::Create(vecForward, 100.0f, 5, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

	D3DXVECTOR3 vec1 = universal::Vec3Cross(pEnemy->GetForward(), D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	D3DXVECTOR3 vec2 = -universal::Vec3Cross(pEnemy->GetForward(), D3DXVECTOR3(0.0f, 1.0f, 0.0f));

	D3DXVec3Normalize(&vec1, &vec1);
	D3DXVec3Normalize(&vec2, &vec2);

	vec1 = vec1 * 1000.0f + vecForward;
	vec2 = vec2 * 1000.0f + vecForward;

	CEffect3D::Create(vec1, 100.0f, 5, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	CEffect3D::Create(vec2, 100.0f, 5, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

	float fRate;

	bool bHit = universal::IsCross(posPlayer, vec1, vec2, &fRate);

	CDebugProc::GetInstance()->Print("\n�ǂ��z������[%d][%d]", bHit);

	if (!bHit)
		return false;

	//if (fRate > 1.0f || fRate < 0.0f)
		//return false;

	return true;
}

//=====================================================
// �ʒu�̕��
//=====================================================
void CEnemyBehaviourChasePlayer::InterpolatePosition(CEnemy *pEnemy)
{
	D3DXVECTOR3 pos = pEnemy->GetPosition();
	int nIdxSpline = pEnemy->GetIdxSpline();

	if (m_fRate >= 1.0f)
	{
		nIdxSpline++;

		// �T�C�Y�𒴂�����߂�
		if (nIdxSpline >= (int)m_vPos.size())
			nIdxSpline = 1;

		// �X�s�[�h�̌v�Z
		CalcSpeed(pEnemy);

		m_fRate -= 1.0f;
	}

	m_fRate += m_fSpeed;

	// �ʒu���
	if (m_pSpline != nullptr)
	{
		if (m_pSpline->IsEmpty())
		{// �X�v���C�����󂾂������̍Ď擾
			CMeshRoad *pMesh = CMeshRoad::GetInstance();

			if (pMesh == nullptr)
				return;

			m_pSpline = pMesh->GetCenterSpline();

			if (m_pSpline->IsEmpty())
				return;	// �Ď擾���Ă��󂾂����珈����ʂ��Ȃ�
		}

		pos = m_pSpline->Interpolate(m_fRate, nIdxSpline);
	}

	pEnemy->SetIdxSpline(nIdxSpline);

	// �ʒu�ƌ����X�V
	pEnemy->SetPosition(pos);

	// �����̐���
	ControllRot(pEnemy);

	// �f�o�b�O�\��
	Debug(pEnemy);
}

//=====================================================
// �X�s�[�h�̌v�Z
//=====================================================
void CEnemyBehaviourChasePlayer::CalcSpeed(CEnemy *pEnemy)
{
	int nIdxSpline = pEnemy->GetIdxSpline();

	if (m_pSpline == nullptr)
		return;

	if (m_pSpline->IsEmpty())
		return;

	float fLength = m_pSpline->GetLength(nIdxSpline, 50);

	if (fLength == 0)
		return;	// 0����h�~

	m_fSpeed = m_fSpeedDefault / fLength;
}

//=====================================================
// �����̐���
//=====================================================
void CEnemyBehaviourChasePlayer::ControllRot(CEnemy *pEnemy)
{
	int nIdxSpline = pEnemy->GetIdxSpline();

	D3DXVECTOR3 pos = pEnemy->GetPosition();
	D3DXVECTOR3 rot = pEnemy->GetRotation();

	// ���̃f�[�^�_�̕�������
	universal::FactingRotTarget(&rot, pos, m_vPos[nIdxSpline], 0.05f);

	pEnemy->SetRotation(D3DXVECTOR3(0.0f, rot.y, 0.0f));
}

//=====================================================
// �ł����藠���̐���
//=====================================================
void CEnemyBehaviourChasePlayer::CreateBigShuriken(void)
{
	if (m_pBigShuriken != nullptr)
	{
		m_pBigShuriken->Uninit();
		m_pBigShuriken = nullptr;
	}

	if(m_nNumShuriken > 0)
	m_pBigShuriken = MyEffekseer::CreateEffect(CEffekseer::TYPE::TYPE_WINDSHURIKEN, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
}

//=====================================================
// �ł����藠���̒Ǐ]
//=====================================================
void CEnemyBehaviourChasePlayer::FollowBigShuriken(CEnemy *pEnemy)
{
	if (m_pBigShuriken == nullptr)
		return;

	D3DXVECTOR3 posHand = pEnemy->GetMtxPos(5);

	m_pBigShuriken = m_pBigShuriken->FollowPosition(posHand);

	if (m_pBigShuriken == nullptr)
	{
		CreateBigShuriken();
	}
}

//=====================================================
// �G�t�F�N�g�̒Ǐ]
//=====================================================
void CEnemyBehaviourChasePlayer::ThrowEffect(CEnemy *pEnemy)
{
	if (m_pFlashEffect == nullptr)
		return;

	m_pFlashEffect = m_pFlashEffect->FollowPosition(pEnemy->GetMtxPos(5));
}

//=====================================================
// �藠���������[�V�������n�܂�
//=====================================================
void CEnemyBehaviourChasePlayer::ThrowShuriken(CEnemy *pEnemy)
{
	D3DXVECTOR3 pos = pEnemy->GetMtxPos(5);

	CEnemy::SFragMotion *pFragMotion = pEnemy->GetFragMotion();

	pFragMotion->bAttack = true;
	
	m_pFlashEffect = MyEffekseer::CreateEffect(CEffekseer::TYPE_FLASH00,pos);
}

//=====================================================
// �f�o�b�O�\��
//=====================================================
void CEnemyBehaviourChasePlayer::Debug(CEnemy *pEnemy)
{
	// �f�o�b�O�v���b�N�擾
	CDebugProc* pDebugProc = CDebugProc::GetInstance();

	if (pDebugProc == nullptr)
		return;

	CEffect3D::Create(pEnemy->GetPosition(), 50.0f, 5, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	pDebugProc->Print("\n�G�̃o�C�N�̈ʒu[%f,%f,%f]", pEnemy->GetPosition().x, pEnemy->GetPosition().y, pEnemy->GetPosition().z);
	pDebugProc->Print("\n�G�̑��x[%f]", m_fSpeedDefault);
	pDebugProc->Print("\n�c��̎藠���̐�[%d]", m_nNumShuriken);
}