//*****************************************************
//
// ゴールエディターの処理[editGoal.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "editGoal.h"
#include "goal.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{

}

//=====================================================
// コンストラクタ
//=====================================================
CEditGoal::CEditGoal()
{

}

//=====================================================
// デストラクタ
//=====================================================
CEditGoal::~CEditGoal()
{

}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CEditGoal::Init(void)
{
	CEdit::Init();

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CEditGoal::Uninit(void)
{
	CEdit::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void CEditGoal::Update(void)
{
	CEdit::Update();

	// ゴールのパラメーター調整
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