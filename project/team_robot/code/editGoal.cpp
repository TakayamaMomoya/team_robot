//*****************************************************
//
// �S�[���G�f�B�^�[�̏���[editGoal.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "editGoal.h"
#include "goal.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{

}

//=====================================================
// �R���X�g���N�^
//=====================================================
CEditGoal::CEditGoal()
{

}

//=====================================================
// �f�X�g���N�^
//=====================================================
CEditGoal::~CEditGoal()
{

}

//=====================================================
// ����������
//=====================================================
HRESULT CEditGoal::Init(void)
{
	CEdit::Init();

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CEditGoal::Uninit(void)
{
	CEdit::Uninit();
}

//=====================================================
// �X�V����
//=====================================================
void CEditGoal::Update(void)
{
	CEdit::Update();

	// �S�[���̃p�����[�^�[����
	CGoal *pGoal = CGoal::GetInstance();

	if (pGoal != nullptr)
	{
		D3DXVECTOR3 pos = pGoal->GetPosition();
		float fRot = pGoal->GetRotation();

		ImGui::Text("[GoalPos]");
		ImGui::DragFloat("GoalPos.X", &pos.x, 10.0f, -FLT_MAX, FLT_MAX);
		ImGui::DragFloat("GoalPos.Y", &pos.y, 10.0f, -FLT_MAX, FLT_MAX);
		ImGui::DragFloat("GoalPos.Z", &pos.z, 10.0f, -FLT_MAX, FLT_MAX);

		ImGui::Text("[GoalRot]");
		ImGui::DragFloat("GoalRot.Z", &fRot, 0.01f, -D3DX_PI, D3DX_PI);

		pGoal->SetPosition(pos);
		pGoal->SetRotation(fRot);

		pGoal->SetGoal();
	}
}