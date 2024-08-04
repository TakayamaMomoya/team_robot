//*****************************************************
//
// �v���C���[�R���g���[���[[playercontroller.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "playercontroller.h"
#include "inputManager.h"
#include "player.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const float SPEED_MOVE = 5.0f;	// �X�s�[�h
const float SPEED_YAW = D3DX_PI * 0.01f;	// ���[���x
const float SPEED_PITCH = D3DX_PI * 0.01f;	// �s�b�`���x
const float SPEED_QUICKBOOST = 300.0f;	// �N�C�b�N�u�[�X�g���x
}

//=====================================================
// �D�揇�ʂ����߂�R���X�g���N�^
//=====================================================
CPlayerController::CPlayerController()
{

}

//=====================================================
// �f�X�g���N�^
//=====================================================
CPlayerController::~CPlayerController()
{

}

//=====================================================
// ����������
//=====================================================
HRESULT CPlayerController::Init(void)
{
	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CPlayerController::Uninit(void)
{
	Release();
}

//=====================================================
// �X�V����
//=====================================================
void CPlayerController::Update(void)
{
	// �ړ��̓���
	InputMove();
}

//=====================================================
// �ړ��̓���
//=====================================================
void CPlayerController::InputMove(void)
{
	CPlayer *pPlayer = CPlayer::GetInstance();
	CInputManager *pInputManager = CInputManager::GetInstance();

	if (pPlayer == nullptr || pInputManager == nullptr)
		return;

	CInputManager::SAxis axis = pInputManager->GetAxis();

	// �����ړ��̉�����
	D3DXVec3Normalize(&axis.axisMove, &axis.axisMove);

	D3DXVECTOR3 vecForward = pPlayer->GetForward();
	D3DXVECTOR3 vecRight = pPlayer->GetRight();
	D3DXVECTOR3 vecUp = pPlayer->GetUp();

	D3DXVECTOR3 move = pPlayer->GetMove();
	move -= vecForward * axis.axisMove.z * SPEED_MOVE;
	move -= vecRight * axis.axisMove.x *  SPEED_MOVE;

	// �㉺�ړ��̑���
	if(pInputManager->GetPress(CInputManager::BUTTON::BUTTON_UP))
		move += vecUp * SPEED_MOVE;

	if (pInputManager->GetPress(CInputManager::BUTTON::BUTTON_DOWN))
		move -= vecUp * SPEED_MOVE;

	// �N�C�b�N�u�[�X�g�̑���
	if (pInputManager->GetTrigger(CInputManager::BUTTON::BUTTON_QUICKBOOST))
	{
		move -= vecForward * axis.axisMove.z * SPEED_QUICKBOOST;
		move -= vecRight * axis.axisMove.x * SPEED_QUICKBOOST;
	}

	// �ړ��ʂ̔��f
	pPlayer->SetMove(move);

	// ���[�E�s�b�`�̑���
	D3DXVec3Normalize(&axis.axisCamera, &axis.axisCamera);

	D3DXVECTOR3 rot = { 0.0f,0.0f,0.0f };

	rot.x += axis.axisCamera.y * SPEED_PITCH;
	rot.y += axis.axisCamera.x * SPEED_YAW;

	pPlayer->SetRotation(rot);
}