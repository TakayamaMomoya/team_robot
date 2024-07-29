//*****************************************************
//
// ブロックエディターの処理[editBlock.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "editBlock.h"
#include "objectX.h"
#include "inputkeyboard.h"
#include "inputmouse.h"
#include "manager.h"
#include "debugproc.h"
#include "block.h"
#include "blockManager.h"
#include "effect3D.h"
#include "manager.h"
#include "camera.h"
#include "texture.h"
#include "game.h"
#include <assert.h>

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const float SPEED_MOVE = 3.0f;	// 移動速度
const float SPEED_ROTATION = 0.01f;	// 回転速度
const float SIZE_ICON = 200.0f;	// アイコンのサイズ
const char* PATH_ICON = "data\\TEXTURE\\UI\\blockIcon.png";	// ブロックのアイコン
}

//=====================================================
// コンストラクタ
//=====================================================
CEditBlock::CEditBlock()
{
	ZeroMemory(&m_aPath[0], sizeof(m_aPath));
	m_posCurrent = { 0.0f,0.0f,0.0f };
	m_pCurrentBlock = nullptr;
}

//=====================================================
// デストラクタ
//=====================================================
CEditBlock::~CEditBlock()
{

}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CEditBlock::Init(void)
{
	// ステイトの初期設定
	ChangeState(new CStateCreateBlockNormal);

	CEdit::Init();

	// 全ブロックのアイコンをマップコンテナに格納
	CreateBlockIconAll();

	return S_OK;
}

//=====================================================
// 全ブロックのアイコンをマップコンテナに格納
//=====================================================
void CEditBlock::CreateBlockIconAll(void)
{
	CBlockManager *pBlockManager = CBlockManager::GetInstance();

	if (pBlockManager == nullptr)
		return;

	CBlock *pBlock = pBlockManager->GetHead();

	while (pBlock != nullptr)
	{
		CBlock *pBlockNext = pBlock->GetNext();

		AddBlockToMap(pBlock);

		pBlock = pBlockNext;
	}
}

//=====================================================
// マップコンテナにブロックを追加
//=====================================================
void CEditBlock::AddBlockToMap(CBlock *pBlock)
{
	D3DXVECTOR3 pos = pBlock->GetPosition();

	CPolygon3D *pIcon = CPolygon3D::Create(pos);

	if (pIcon == nullptr)
		return;

	pIcon->SetPosition(pos);
	pIcon->SetSize(SIZE_ICON, SIZE_ICON);
	pIcon->SetMode(CPolygon3D::MODE::MODE_BILLBOARD);
	pIcon->SetVtx();
	pIcon->EnableZtest(true);
	pIcon->EnableFog(false);
	pIcon->EnableLighting(false);

	int nIdx = Texture::GetIdx(PATH_ICON);
	pIcon->SetIdxTexture(nIdx);



	m_aIcon[pBlock] = pIcon;
}

//=====================================================
// 終了処理
//=====================================================
void CEditBlock::Uninit(void)
{
	// 全アイコン削除
	DeleteAllIcon();

	if (m_pState != nullptr)
	{
		m_pState->Uninit(this);
		delete m_pState;
		m_pState = nullptr;
	}

	CEdit::Uninit();
}

//=====================================================
// 全アイコンの削除
//=====================================================
void CEditBlock::DeleteAllIcon(void)
{
	for (auto it = m_aIcon.begin(); it != m_aIcon.end();)
	{
		it->second->Uninit();

		auto itNext = std::next(it);

		m_aIcon.erase(it);

		it = itNext;
	}
}

//=====================================================
// 更新処理
//=====================================================
void CEditBlock::Update(void)
{
	if (m_pState != nullptr)
	{// ステイトの更新
		m_pState->Update(this);
	}

	// ステイトの選択
	ImGui::Text("[ChangeMode]");

	if (ImGui::Button("CreateNormalBlock", ImVec2(70, 30)))	// 通常ブロックの生成
		ChangeState(new CStateCreateBlockNormal);

	if (ImGui::Button("EditGrabBlock", ImVec2(70, 30)))	// 掴めるブロックの編集
		ChangeState(new CStateEditGrabBlock);

	// レイでのブロック選択
	RaySelectBlock();

	CEdit::Update();

	if (m_pMoveBlock != nullptr)
	{
		CEffect3D::Create(m_pMoveBlock->GetPosition(), 100.0f, 4, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

		if (ImGui::TreeNode("TransformSelectBlock"))
		{
			D3DXVECTOR3 pos = m_pMoveBlock->GetPosition();
			D3DXVECTOR3 rot = m_pMoveBlock->GetRotation();

			if (ImGui::TreeNode("POS"))
			{
				ImGui::DragFloat("POS.X", &pos.x, 5.0f, -FLT_MAX, FLT_MAX);
				ImGui::DragFloat("POS.Y", &pos.y, 5.0f, -FLT_MAX, FLT_MAX);
				ImGui::DragFloat("POS.Z", &pos.z, 5.0f, -FLT_MAX, FLT_MAX);

				ImGui::TreePop();
			}
			if (ImGui::TreeNode("ROT"))
			{
				// ブロック向き
				ImGui::DragFloat("ROT.X", &rot.x, 0.01f, -D3DX_PI, D3DX_PI);
				ImGui::DragFloat("ROT.Y", &rot.y, 0.01f, -D3DX_PI, D3DX_PI);
				ImGui::DragFloat("ROT.Z", &rot.z, 0.01f, -D3DX_PI, D3DX_PI);

				ImGui::TreePop();
			}

			m_pMoveBlock->SetPosition(pos);
			m_pMoveBlock->SetRotation(rot);

			m_aIcon[m_pMoveBlock]->SetPosition(pos);

			ImGui::TreePop();
		}

		if (CInputKeyboard::GetInstance()->GetTrigger(DIK_DELETE))
		{
			m_aIcon[m_pMoveBlock]->Uninit();
			m_aIcon.erase(m_pMoveBlock);
			m_pMoveBlock->Uninit();
			m_pMoveBlock = nullptr;
		}
	}
}

//=====================================================
// レイでのブロックの選択
//=====================================================
void CEditBlock::RaySelectBlock(void)
{
	CBlockManager *pBlockManager = CBlockManager::GetInstance();

	if (pBlockManager == nullptr)
		return;

	D3DXVECTOR3 posNear;
	D3DXVECTOR3 posFar;
	D3DXVECTOR3 vecDiff;

	universal::ConvertScreenPosTo3D(&posNear, &posFar, &vecDiff);

	if (m_pCurrentBlock == nullptr)
	{// ブロックを探している状態
		CBlock *pBlock = pBlockManager->GetHead();

		while (pBlock != nullptr)
		{
			CBlock *pBlockNext = pBlock->GetNext();

			CollideBlockRay(pBlock, posFar, posNear, vecDiff);

			if (m_pCurrentBlock != nullptr)
			{// ブロックが選べたらwhileを抜ける
				break;
			}

			pBlock = pBlockNext;
		}

		if (m_pCurrentBlock == nullptr)
		{
			D3DXVECTOR3 posHit;

			universal::CalcRayFlat(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), posNear, posFar, &posHit);
		}
	}
	else
	{// ブロックを動かす
		MoveCurrentBlock(posFar, posNear);
	}
}

//=====================================================
// ブロックに対するレイの判定
//=====================================================
void CEditBlock::CollideBlockRay(CBlock *pBlock, D3DXVECTOR3 posFar, D3DXVECTOR3 posNear, D3DXVECTOR3 vecDiff)
{
	CInputMouse *pMouse = CInputMouse::GetInstance();
	CInputKeyboard *pKeyboard = CInputKeyboard::GetInstance();

	if (pBlock == nullptr || pMouse == nullptr || pKeyboard == nullptr)
		return;

	D3DXVECTOR3 posBlock = pBlock->GetPosition();

	bool bHit = universal::CalcRaySphere(posNear, vecDiff, posBlock, SIZE_ICON);

	if (bHit)
	{
		CEffect3D::Create(posBlock, 100.0f, 3, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

		if (pMouse->GetTrigger(CInputMouse::BUTTON_LMB))
		{
			m_pCurrentBlock = pBlock;

			m_posCurrent = pBlock->GetPosition();

			m_pMoveBlock = pBlock;

			return;
		}
	}
}

//=====================================================
// 選択したブロックの移動
//=====================================================
void CEditBlock::MoveCurrentBlock(D3DXVECTOR3 posFar, D3DXVECTOR3 posNear)
{
	CInputMouse *pMouse = CInputMouse::GetInstance();
	CInputKeyboard *pKeyboard = CInputKeyboard::GetInstance();

	if (pMouse == nullptr || pKeyboard == nullptr || m_pCurrentBlock == nullptr)
		return;

	D3DXVECTOR3 posHit;

	universal::CalcRayFlat(m_posCurrent, D3DXVECTOR3(0.0f, 1.0f, 0.0f), posNear, posFar, &posHit);

	if (pKeyboard->GetRelease(DIK_C))
	{
		m_pCurrentBlock = nullptr;
	}
	else if(pKeyboard->GetRelease(DIK_V))
	{
		m_pCurrentBlock = nullptr;
	}
	else if (pKeyboard->GetPress(DIK_C))
	{// 回転
		D3DXVECTOR3 rot = m_pCurrentBlock->GetRotation();

		D3DXVECTOR3 vecDiff = posHit - m_posCurrent;

		rot.y = atan2f(vecDiff.x, vecDiff.z);

		m_pCurrentBlock->SetRotation(rot);
	}
	else
	{// 移動
		if (pKeyboard->GetPress(DIK_V))
		{// 上下移動
			CCamera *pCamera = CManager::GetCamera();

			if (pCamera == nullptr)
				return;

			CCamera::Camera *pInfoCamera = pCamera->GetCamera();

			// カメラとブロックの平面差分ベクトルを法線にする
			D3DXVECTOR3 posCamera = { pInfoCamera->posV.x,0.0f, pInfoCamera->posV.z };
			D3DXVECTOR3 posBlock = { m_posCurrent.x,0.0f,m_posCurrent.z };
			D3DXVECTOR3 vecDiff = posCamera - posBlock;

			D3DXVec3Normalize(&vecDiff, &vecDiff);

			universal::CalcRayFlat(m_posCurrent, vecDiff, posNear, posFar, &posHit);

			// y軸以外は固定する
			posHit.x = m_posCurrent.x;
			posHit.z = m_posCurrent.z;
		}
		
		m_pCurrentBlock->SetPosition(posHit);
		m_aIcon[m_pCurrentBlock]->SetPosition(posHit);

		m_posCurrent.x = posHit.x;
		m_posCurrent.z = posHit.z;
	}

	if (pMouse->GetRelease(CInputMouse::BUTTON_LMB))
	{
		m_pCurrentBlock = nullptr;
	}
}

//=====================================================
// 保存を行う処理
//=====================================================
void CEditBlock::Save(void)
{
	ImGui::Text("[Save]");

	ImGui::InputText("SavePath", &m_aPath[0], 256);

	if (ImGui::Button("Save", ImVec2(50.0f, 20.0f)))
	{// 保存
		CBlockManager *pBlockManager = CBlockManager::GetInstance();

		if (pBlockManager != nullptr)
		{
			pBlockManager->Save(&m_aPath[0]);
		}
	}
}

//=====================================================
// ステイトの変更
//=====================================================
void CEditBlock::ChangeState(CStateEditBlock *pState)
{
	if (m_pState != nullptr)
	{
		m_pState->Uninit(this);
		delete m_pState;
		m_pState = nullptr;
	}

	m_pState = pState;

	if (m_pState != nullptr)
	{
		m_pState->Init(this);
	}
}

//===================================================================================
// 通常ブロックの生成
//===================================================================================
//=====================================================
// 初期化処理
//=====================================================
void CStateCreateBlockNormal::Init(CEditBlock *pEdit)
{
	m_nIdxObject = 0;
	m_pBlockCurrent = nullptr;

	CBlockManager *pBlockManager = CBlockManager::GetInstance();

	if (pBlockManager != nullptr)
	{
		CBlockManager::SInfoBlock *pInfoBlock = pBlockManager->GetInfoBlock();

		if (pInfoBlock == nullptr)
		{
			nullptr;
		}

		m_pObjectCursor = nullptr;

		m_pObjectCursor = CObjectX::Create();

		// タイプの初期設定
		m_type = CBlock::TYPE_WALL;

		// モデル番号の設定
		m_pObjectCursor->BindModel(pInfoBlock[0].nIdxModel);

		m_pObjectCursor->SetEmissiveCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
	}
}

//=====================================================
// 終了処理
//=====================================================
void CStateCreateBlockNormal::Uninit(CEditBlock *pEdit)
{
	if (m_pObjectCursor != nullptr)
	{
		m_pObjectCursor->Uninit();
		m_pObjectCursor = nullptr;
	}
}

//=====================================================
// 更新処理
//=====================================================
void CStateCreateBlockNormal::Update(CEditBlock *pEdit)
{
	// 情報取得
	CInputKeyboard *pKeyboard = CInputKeyboard::GetInstance();
	CInputMouse* pMouse = CInputMouse::GetInstance();
	CBlockManager *pBlockManager = CBlockManager::GetInstance();

	D3DXVECTOR3 rot = pEdit->GetRotation();
	D3DXVECTOR3 rotCamera = CManager::GetCamera()->GetCamera()->rot;
	D3DXVECTOR3 pos = pEdit->GetPosition();
	float fSpeed = SPEED_MOVE;

	if (pKeyboard == nullptr || pMouse == nullptr || pBlockManager == nullptr )
		return;

	if (!pMouse->GetPress(CInputMouse::BUTTON_RMB))
	{// 右クリックしてないときに、移動可能

		float fMove = SPEED_MOVE;

		if (pKeyboard->GetPress(DIK_LSHIFT) == true)
		{// 加速
			fMove *= 7;
		}

		// ブロック移動
		if (pKeyboard->GetPress(DIK_A) == true)
		{// 左移動
			pos.x -= sinf(rotCamera.y - D3DX_PI * 0.5f) * fMove;
			pos.z -= cosf(rotCamera.y - D3DX_PI * 0.5f) * fMove;
		}
		if (pKeyboard->GetPress(DIK_D) == true)
		{// 右移動
			pos.x -= sinf(rotCamera.y - D3DX_PI * -0.5f) * fMove;
			pos.z -= cosf(rotCamera.y - D3DX_PI * -0.5f) * fMove;
		}
		if (pKeyboard->GetPress(DIK_W) == true)
		{// 前移動
			pos.x -= sinf(D3DX_PI * 0.5f) * sinf(rotCamera.y) * fMove;
			pos.z -= sinf(D3DX_PI * 0.5f) * cosf(rotCamera.y) * fMove;
		}
		if (pKeyboard->GetPress(DIK_S) == true)
		{// 後移動
			pos.x -= sinf(-D3DX_PI * 0.5f) * sinf(rotCamera.y) * fMove;
			pos.z -= sinf(-D3DX_PI * 0.5f) * cosf(rotCamera.y) * fMove;
		}
		if (pKeyboard->GetPress(DIK_E) == true)
		{// 上昇
			pos.y += fMove;
		}
		if (pKeyboard->GetPress(DIK_Q) == true)
		{// 下降
			pos.y -= fMove;
		}
	}

	D3DXVECTOR3 posNear;
	D3DXVECTOR3 posFar;
	D3DXVECTOR3 vecDiff;

	universal::ConvertScreenPosTo3D(&posNear, &posFar, &vecDiff);

	D3DXVECTOR3 posHit;
	universal::CalcRayFlat(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), posNear, posFar, &posHit);

	pos = posHit;
	
	int nNumBlock = pBlockManager->GetNumBlock();
	CBlockManager::SInfoBlock *pInfoBlock = pBlockManager->GetInfoBlock();

	if (ImGui::TreeNode("SelectBlockType"))
	{
		for (int i = 0; i < nNumBlock; i++)
		{// ブロックの種類選択
			if (ImGui::Button(&pInfoBlock[i].aTag[0], ImVec2(50.0f, 20.0f)))
			{
				m_nIdxObject = i;

				m_pObjectCursor->BindModel(pInfoBlock[i].nIdxModel);
			}
		}

		ImGui::TreePop();
	}

	if (ImGui::TreeNode("SelectBlock"))
	{
		// ブロック選択
		SelectBlock();

		ImGui::TreePop();
	}

	// ブロックのビヘイビアの変更
	ChangeBlockBehaviour();

	ImGui::Text("[Action]");

	bool bStop = CGame::GetInstance()->GetStop();

	if (bStop)
	{
		if (pKeyboard->GetTrigger(DIK_SPACE))
		{// ブロック生成
			CreateBlock(m_pObjectCursor->GetPosition(), pEdit);
		}
	}

	if (ImGui::Button("DeleteAll", ImVec2(80.0f, 20.0f)))
	{// 全削除
		CBlockManager *pBlockManager = CBlockManager::GetInstance();

		if (pBlockManager != nullptr)
		{
			pBlockManager->DeleteAll();
		}
	}

	// セーブ
	pEdit->Save();

	// エディットにトランスフォームを適用
	pEdit->SetPosition(pos);
	pEdit->SetRotation(rot);

	if (m_pObjectCursor != nullptr)
	{// カーソルのトランスフォーム
		m_pObjectCursor->SetPosition(pos);
		m_pObjectCursor->SetRotation(rot);
	}
}

//=====================================================
// ブロックの選択
//=====================================================
void CStateCreateBlockNormal::SelectBlock(void)
{
	CBlockManager *pBlockManager = CBlockManager::GetInstance();

	if (pBlockManager == nullptr)
		return;

	if (m_pBlockCurrent != nullptr)
		m_pBlockCurrent->ResetColor();

	if (ImGui::Button("NextBlock", ImVec2(70, 30)))
	{
		if (m_pBlockCurrent == nullptr)
			m_pBlockCurrent = pBlockManager->GetHead();

		if (m_pBlockCurrent == nullptr)
			return;

		CBlock *pNext = m_pBlockCurrent->GetNext();

		if (pNext != nullptr)
			m_pBlockCurrent = pNext;
	}
	if (ImGui::Button("PrevBlock", ImVec2(70, 30)))
	{
		if (m_pBlockCurrent == nullptr)
			m_pBlockCurrent = pBlockManager->GetHead();

		if (m_pBlockCurrent == nullptr)
			return;

		CBlock *pPrev = m_pBlockCurrent->GetPrev();

		if (pPrev != nullptr)
			m_pBlockCurrent = pPrev;
	}

	if (m_pBlockCurrent != nullptr)
	{
		m_pBlockCurrent->SetEmissiveCol(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

		D3DXVECTOR3 pos = m_pBlockCurrent->GetPosition();
		D3DXVECTOR3 rot = m_pBlockCurrent->GetRotation();

		ImGui::DragFloat("PosSelectBlock.X", &pos.x, 10.0f, -FLT_MAX, FLT_MAX);
		ImGui::DragFloat("PosSelectBlock.Y", &pos.y, 10.0f, -FLT_MAX, FLT_MAX);
		ImGui::DragFloat("PosSelectBlock.Z", &pos.z, 10.0f, -FLT_MAX, FLT_MAX);

		ImGui::DragFloat("RotSelectBlock.X", &rot.x, 0.03f, -D3DX_PI, D3DX_PI);
		ImGui::DragFloat("RotSelectBlock.Y", &rot.y, 0.03f, -D3DX_PI, D3DX_PI);
		ImGui::DragFloat("RotSelectBlock.Z", &rot.z, 0.03f, -D3DX_PI, D3DX_PI);

		m_pBlockCurrent->SetPosition(pos);
		m_pBlockCurrent->SetRotation(rot);
	}

	if (ImGui::Button("DeleteSelectBlock", ImVec2(70, 30)))
	{
		if (m_pBlockCurrent != nullptr)
		{
			m_pBlockCurrent->Uninit();

			m_pBlockCurrent = pBlockManager->GetHead();
		}
	}
}

//=====================================================
// ブロックのビヘイビア変更
//=====================================================
void CStateCreateBlockNormal::ChangeBlockBehaviour(void)
{
	ImGui::Text("[BlockBehaviour]");

	const char* apText[CBlock::BEHAVIOUR::BEHAVIOUR_MAX] =
	{
		"NORMAL",
		"GRAB"
	};

	ImGui::Text("Now[%s]",apText[m_behaviour]);

	for (int i = 0; i < CBlock::BEHAVIOUR::BEHAVIOUR_MAX; i++)
	{
		if (ImGui::Button(apText[i], ImVec2(80.0f, 20.0f)))
		{// ビヘイビア設定
			m_behaviour = (CBlock::BEHAVIOUR)i;
		}
	}
}

//=====================================================
// ブロックの生成
//=====================================================
void CStateCreateBlockNormal::CreateBlock(D3DXVECTOR3 pos,CEditBlock *pEdit)
{
	CBlockManager *pBlockManager = CBlockManager::GetInstance();

	if (pBlockManager == nullptr)
	{
		return;
	}

	CBlockManager::SInfoBlock *pInfoBlock = pBlockManager->GetInfoBlock();

	CBlock *pBlock = CBlock::Create(m_behaviour);

	if (pBlock != nullptr)
	{
		pBlock->BindModel(pInfoBlock[m_nIdxObject].nIdxModel);
		pBlock->SetPosition(pos);
		pBlock->SetRotation(m_pObjectCursor->GetRotation());
		pBlock->SetIdx(m_nIdxObject);

		pEdit->AddBlockToMap(pBlock);
	}
}

//=====================================================
// 削除ブロックのチェック
//=====================================================
CBlock *CStateCreateBlockNormal::CheckDelete(void)
{
	CBlockManager *pBlockManager = CBlockManager::GetInstance();

	if (pBlockManager == nullptr)
	{
		return nullptr;
	}

	CBlock *pBlock = pBlockManager->GetHead();

	while (pBlock != nullptr)
	{
		CBlock *pBlockNext = pBlock->GetNext();

		if (pBlock != nullptr)
		{
			pBlock->ResetColor();

			D3DXVECTOR3 posBlock = pBlock->GetPosition();
			D3DXVECTOR3 pos = m_pObjectCursor->GetPosition();
			D3DXVECTOR3 vtxMax = pBlock->GetVtxMax();
			D3DXVECTOR3 vtxMin = pBlock->GetVtxMin();

			if (pos.x >= posBlock.x + vtxMin.x &&
				pos.x <= posBlock.x + vtxMax.x &&
				pos.z >= posBlock.z + vtxMin.z &&
				pos.z <= posBlock.z + vtxMax.z &&
				pos.y >= posBlock.y + vtxMin.y &&
				pos.y <= posBlock.y + vtxMax.y)
			{// ブロック内にいるとき
				pBlock->SetEmissiveCol(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

				return pBlock;
			}
		}

		pBlock = pBlockNext;
	}

	return nullptr;
}

//===================================================================================
// 掴むブロックの編集
//===================================================================================
//=====================================================
// 初期化処理
//=====================================================
void CStateEditGrabBlock::Init(CEditBlock *pEdit)
{
	// イテレーターの初期化
	CBlockManager *pBlockManager = CBlockManager::GetInstance();

	if (pBlockManager == nullptr)
		return;

	std::list<CBlockGrab*> *pList = pBlockManager->GetListGrab();

	m_it = pList->begin();
}

//=====================================================
// 終了処理
//=====================================================
void CStateEditGrabBlock::Uninit(CEditBlock *pEdit)
{

}

//=====================================================
// 更新処理
//=====================================================
void CStateEditGrabBlock::Update(CEditBlock *pEdit)
{
	// ブロックの選択
	SelectGrabBlock();

	// ブロックの編集
	EditGrabBlock();
}

//=====================================================
// ブロックの選択
//=====================================================
void CStateEditGrabBlock::SelectGrabBlock(void)
{
	CBlockManager *pBlockManager = CBlockManager::GetInstance();

	if (pBlockManager == nullptr)
		return;

	// ブロックの選択
	std::list<CBlockGrab*> *pList = pBlockManager->GetListGrab();

	if (ImGui::Button("NextBlock", ImVec2(70, 30)))
	{
		if (m_it != pList->end() && std::next(m_it) != pList->end())
			std::advance(m_it, 1);
	}
	if (ImGui::Button("PrevBlock", ImVec2(70, 30)))
	{
		if (m_it != pList->begin())
			std::advance(m_it, -1);
	}

	CEffect3D::Create((*m_it)->GetPosition(), 100.0f, 3, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));

}

//=====================================================
// ブロックの編集
//=====================================================
void CStateEditGrabBlock::EditGrabBlock(void)
{
	ImGui::Text("[EditAngle]");

	float fAngle1 = (*m_it)->GetAngleOffset(0);
	float fAngle2 = (*m_it)->GetAngleOffset(1);

	ImGui::DragFloat("Angle1", &fAngle1, 0.05f, fAngle2, FLT_MAX);
	ImGui::DragFloat("Angle2", &fAngle2, 0.05f, -FLT_MAX, fAngle1);

	(*m_it)->SetAngleOffset(fAngle1, 0);
	(*m_it)->SetAngleOffset(fAngle2, 1);

	(*m_it)->SetFan();
}