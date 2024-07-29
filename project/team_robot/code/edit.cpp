//*****************************************************
//
// �G�f�B�^�[�̏���[edit.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "edit.h"
#include "debugproc.h"
#include "player.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const float SPEED_MOVE = 10.0f;	// �ړ����x
const float ABOVE_DEFAULT = 10000.0f;	// �f�t�H���g�̏�󎋓_�̍���
const float SPEED_MOVE_PLAYER = 10.0f;	// �v���C���[�̈ړ����x
}

//*****************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************

//=====================================================
// �R���X�g���N�^
//=====================================================
CEdit::CEdit()
{
	m_pos = { 0.0f,0.0f,0.0f };
	m_rot = { 0.0f,0.0f,0.0f };
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CEdit::~CEdit()
{

}

//=====================================================
// ����������
//=====================================================
HRESULT CEdit::Init(void)
{
	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CEdit::Uninit(void)
{
	
}

//=====================================================
// �X�V����
//=====================================================
void CEdit::Update(void)
{
	// �v���C���[�̃e���|�[�g
	TeleportPlayer();
}

//=====================================================
// �v���C���[�̃e���|�[�g
//=====================================================
void CEdit::TeleportPlayer(void)
{
	if (ImGui::TreeNode("TeleportPlayer"))
	{
		CPlayer *pPlayer = CPlayer::GetInstance();

		if (pPlayer == nullptr)
			return;

		D3DXVECTOR3 posPlayer = pPlayer->GetPosition();

		// ImGui�ɂ��ʒu�̕ύX
		ImGui::DragFloat("PosPlayer.X", &posPlayer.x, SPEED_MOVE_PLAYER, -FLT_MAX, FLT_MAX);
		ImGui::DragFloat("PosPlayer.Y", &posPlayer.y, SPEED_MOVE_PLAYER, -FLT_MAX, FLT_MAX);
		ImGui::DragFloat("PosPlayer.Z", &posPlayer.z, SPEED_MOVE_PLAYER, -FLT_MAX, FLT_MAX);

		// �}�E�X�ł̈ʒu�ύX


		pPlayer->SetPosition(posPlayer);

		ImGui::TreePop();
	}
}