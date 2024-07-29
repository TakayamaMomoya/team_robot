//*****************************************************
//
// �藠���̏���[shuriken.h]
// Author:�匴�叫
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "shuriken.h"
#include "MyEffekseer.h"
#include "model.h"
#include "player.h"
#include "manager.h"
#include "effect3D.h"
#include "MyEffekseer.h"
#include "debugproc.h"

//*****************************************************
// �ÓI�����o�ϐ�
//*****************************************************
std::list<CShuriken*> CShuriken::m_aShuriken;

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
	const float MOVE_SPEED = 120.0f;  // �X�s�[�h
	const float LIFE_DEFAULT = 5.0f;	// �f�t�H���g�̎���
}

//=====================================================
// �R���X�g���N�^
//=====================================================
CShuriken::CShuriken() : m_fLife(0.0f), m_pWindEffect(nullptr)
{
	
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CShuriken::~CShuriken()
{

}

//=====================================================
// ��������
//=====================================================
CShuriken* CShuriken::Create(D3DXVECTOR3 pos, D3DXVECTOR3 vecForward)
{
	CShuriken *pShuriken = nullptr;
	pShuriken = new CShuriken;

	if (pShuriken != nullptr)
	{
		pShuriken->SetPosition(pos);
		pShuriken->CalcMove(vecForward);
		pShuriken->Init();
	}

	return pShuriken;
}

//=====================================================
// ����������
//=====================================================
HRESULT CShuriken::Init(void)
{
	CObjectX::Init();

	// ���f���Ǎ�
	int nIdx = CModel::Load("data\\MODEL\\Player\\shuriken.x");
	BindModel(nIdx);

	m_aShuriken.push_back(this);

	m_fLife = LIFE_DEFAULT;

	m_pWindEffect = MyEffekseer::CreateEffect(CEffekseer::TYPE::TYPE_THROWSHURIKEN, GetPosition());

	return S_OK;
}

//=====================================================
// �ړ��ʂ̌v�Z
//=====================================================
void CShuriken::CalcMove(D3DXVECTOR3 vecForward)
{
	// �v���C���[�擾
	CPlayer* pPlayer = CPlayer::GetInstance();

	if (pPlayer == nullptr)
		return;

	// �v���C���[�̈ړ��x�N�g���擾
	float fSpeedPlayer = pPlayer->GetSpeed();
	D3DXVECTOR3 moveEnemy = vecForward * fSpeedPlayer;

	// �v���C���[�̓��̃}�g���b�N�X�擾
	D3DXVECTOR3 posHeadPlayer = pPlayer->GetNInjaBody()->GetMtxPos(2);

	D3DXVECTOR3 pos = GetPosition();
	D3DXVECTOR3 vecDiff = posHeadPlayer - pos;

	universal::VecConvertLength(&vecDiff, MOVE_SPEED);

	D3DXVECTOR3 move = moveEnemy + vecDiff;

	SetMove(move);
}

//=====================================================
// �I������
//=====================================================
void CShuriken::Uninit(void)
{
	if (m_pWindEffect != nullptr)
	{
		m_pWindEffect->Uninit();
		m_pWindEffect = nullptr;
	}

	CObjectX::Uninit();

	m_aShuriken.remove(this);
}

//=====================================================
// �X�V����
//=====================================================
void CShuriken::Update(void)
{
	CObjectX::Update();

	// �ړ��ʂ̔��f
	D3DXVECTOR3 pos = GetPosition();
	D3DXVECTOR3 move = GetMove();

	pos += move;

	SetPosition(pos);

	CEffect3D::Create(pos, 100.0f, 10, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

	// �����̌���
	m_fLife -= CManager::GetDeltaTime();

	if (m_fLife <= 0.0f)
	{
		m_fLife = 0.0f;

		Uninit();

		return;
	}

	if (m_pWindEffect != nullptr)
	{
		m_pWindEffect = m_pWindEffect->FollowPosition(GetPosition());
	}

#ifdef _DEBUG
	// �f�o�b�O����
	Debug();
#endif
}

//=====================================================
// �`�揈��
//=====================================================
void CShuriken::Draw(void)
{
	// �p���N���X�̏�����
	CObjectX::Draw();
}

//=====================================================
// �q�b�g����
//=====================================================
void CShuriken::Hit(float fDamage)
{
	// �e�������̃G�t�F�N�g���o��
	CEffekseer* pEffekseer = CManager::GetMyEffekseer();

	MyEffekseer::CreateEffect(CEffekseer::TYPE_PARRY, GetPosition(), GetRotation());

	// �I������
	Uninit();
}

//=====================================================
// �f�o�b�O����
//=====================================================
void CShuriken::Debug(void)
{
	// �f�o�b�O�v���b�N�擾
	CDebugProc* pDebugProc = CDebugProc::GetInstance();

	if (pDebugProc == nullptr)
		return;

	pDebugProc->Print("\n�藠���̈ړ�[%f,%f,%f]", GetMove().x, GetMove().y, GetMove().z);
	pDebugProc->Print("\n�藠���̈ʒu[%f,%f,%f]", GetPosition().x, GetPosition().y, GetPosition().z);
}