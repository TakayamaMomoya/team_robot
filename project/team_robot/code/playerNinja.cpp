//*****************************************************
//
// �v���C���[�E��[playerNinja.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "playerNinja.h"
#include "player.h"
#include "shuriken.h"
#include "debugproc.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const char* PATH_BODY = "data\\MOTION\\motionPlayer.txt";	// ���g�̃��[�V�����f�[�^
const float RANGE_GUARD = 500.0f;	// �K�[�h�͈�
}

//=====================================================
// �D�揇�ʂ����߂�R���X�g���N�^
//=====================================================
CPlayerNinja::CPlayerNinja(int nPriority)
{

}

//=====================================================
// �f�X�g���N�^
//=====================================================
CPlayerNinja::~CPlayerNinja()
{

}

//=====================================================
// ��������
//=====================================================
CPlayerNinja *CPlayerNinja::Create(void)
{
	CPlayerNinja *pNinja = nullptr;

	pNinja = new CPlayerNinja;

	if (pNinja != nullptr)
	{
		pNinja->Init();
	}

	return pNinja;
}

//=====================================================
// ����������
//=====================================================
HRESULT CPlayerNinja::Init(void)
{
	// �u���[�������Ȃ��ݒ�ɂ���
	EnableBlur(false);

	// �p���N���X�̏�����
	CMotion::Init();

	// ���f���̐ݒ�
	CMotion::Load((char*)PATH_BODY);

	SetMotion(CPlayer::MOTION::MOTION_NEUTRAL);

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CPlayerNinja::Uninit(void)
{
	// �p���N���X�̏I��
	CMotion::Uninit();
}

//=====================================================
// �X�V����
//=====================================================
void CPlayerNinja::Update(void)
{
	CDebugProc::GetInstance()->Print("\n�E�҂̃��[�V����[%d]", GetMotion());

	CMotion::Update();
}

//=====================================================
// �C�x���g�̊Ǘ�
//=====================================================
void CPlayerNinja::Event(EVENT_INFO *pEventInfo)
{
	int nMotion = GetMotion();

	D3DXVECTOR3 offset = pEventInfo->offset;
	D3DXMATRIX mtxParent;
	D3DXMATRIX mtxPart = GetParts(pEventInfo->nIdxParent)->pParts->GetMatrix();

	universal::SetOffSet(&mtxParent, mtxPart, offset);

	D3DXVECTOR3 pos = { mtxParent._41,mtxParent._42 ,mtxParent._43 };

	if (nMotion == CPlayer::MOTION_NINJA::MOTION_NINJA_SLASHDOWN ||
		nMotion == CPlayer::MOTION_NINJA::MOTION_NINJA_SLASHUP)
	{// �a����
		ManagekatanaCollision(pos);
	}
}

//=====================================================
// ���̔���̊Ǘ�
//=====================================================
void CPlayerNinja::ManagekatanaCollision(D3DXVECTOR3 pos)
{
	// �藠�����X�g�̎擾
	std::list<CShuriken*> listShuriken = CShuriken::GetList();

	for (auto it : listShuriken)
	{
		// �藠���Ƃ̋����𑪂�
		D3DXVECTOR3 pos = GetMtxPos(2);
		D3DXVECTOR3 posShuriken = it->GetPosition();

		bool bHit = universal::DistCmp(pos, posShuriken, RANGE_GUARD, nullptr);

		if (bHit)
		{// �͈͓��Ȃ�藠���̃q�b�g�������Ă�
			it->Hit(0.0f);	// �_���[�W�l�͎g��Ȃ�
		}
	}
}

//=====================================================
// �`�揈��
//=====================================================
void CPlayerNinja::Draw(void)
{
	// �p���N���X�̕`��
	CMotion::Draw();
}