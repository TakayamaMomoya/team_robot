//*****************************************************
//
// ブロックエディターの処理[editBlock.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "editMesh.h"
#include "inputkeyboard.h"
#include "inputmouse.h"
#include "effect3D.h"
#include "meshRoad.h"
#include "objectX.h"
#include "debugproc.h"
#include "camera.h"
#include "manager.h"
#include "tunnel.h"
#include "guardRail.h"
#include "game.h"
#include "enemySpawner.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const float SPEED_MOVE = 5.0f;	// 移動速度
const float SPEED_ROLL = 0.05f;	// 回転速度
const float DEFAULT_HEIGHT_GR = 600.0f;	// ガードレールのデフォルト高さ
const char* PATH_SAVE = "data\\MAP\\road00.bin";	// 保存ファイルのパス
const char* PATH_SAVE_ENEMY = "data\\MAP\\enemy00.txt";	// 敵保存ファイルのデフォルトパス
}

//=====================================================
// コンストラクタ
//=====================================================
CEditMesh::CEditMesh()
{
	m_pMesh = nullptr;
	m_pManipulator = nullptr;
	m_pState = nullptr;
	m_bCurrent = false;
}

//=====================================================
// デストラクタ
//=====================================================
CEditMesh::~CEditMesh()
{

}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CEditMesh::Init(void)
{
	strcpy(&m_aPathSave[0], (char*)PATH_SAVE);
	strcpy(&m_aPathSaveEnemy[0], (char*)PATH_SAVE_ENEMY);

	CEdit::Init();

	// マニピュレータの生成
	m_pManipulator = CObjectX::Create();

	// モデルの割り当て
	int nIdx = CModel::Load("data\\MODEL\\block\\manipulator.x");
	m_pManipulator->BindModel(nIdx);

	ChangeState(new CStateEditMeshCreateMesh);

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CEditMesh::Uninit(void)
{
	m_itCurrent = std::vector<CMeshRoad::SInfoRoadPoint>::iterator();

	if (m_pState != nullptr)
	{
		delete m_pState;
		m_pState = nullptr;
	}

	if (m_pManipulator != nullptr)
	{
		m_pManipulator->Uninit();
		m_pManipulator = nullptr;
	}

	CEdit::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void CEditMesh::Update(void)
{
	CEdit::Update();

	// マニピュレーターのトランスフォーム更新
	D3DXVECTOR3 pos = GetPosition();
	D3DXVECTOR3 rot = GetRotation();

	m_pManipulator->SetPosition(pos);
	m_pManipulator->SetRotation(rot);

	if (m_pState != nullptr)	// ステイトの更新
		m_pState->Update(this);

	// ステイトによるモードの切り替え
	ImGui::Text("ModeSelect");

	if (ImGui::Button("CreateEdge", ImVec2(70, 30)))	// メッシュ生成
		ChangeState(new CStateEditMeshCreateMesh);

	if (ImGui::Button("CreateTunnel", ImVec2(70, 30)))	// トンネルの生成
		ChangeState(new CStateEditMeshCreateTunnel);

	if (ImGui::Button("CreateGR", ImVec2(70, 30)))	// ガードレールの生成
		ChangeState(new CStateEditMeshCreateGR);

	if (ImGui::Button("SelectGR", ImVec2(70, 30)))	// ガードレールの選択
		ChangeState(new CStateEditMeshSelectGR);

	if (ImGui::Button("AdjustRoadPoint", ImVec2(70, 30)))	// ロードポイントの調節
		ChangeState(new CStateEditMeshAdjustRoadPoint);

	if (ImGui::Button("DeleteRoadPoint", ImVec2(70, 30)))	// 辺の削除
		ChangeState(new CStateEditMeshDeleteRoadPoint);

	ImGui::InputText("SavePath", &m_aPathSave[0], 256);	// メッシュデータの保存パス
	ImGui::InputText("SavePathEnemy", &m_aPathSaveEnemy[0], 256);	// 敵データの保存パス

	// メッシュロードの選択
	CMeshRoad::SelectMeshRoad();

	// 全ロードポイントの削除
	CMeshRoad *pMesh = CMeshRoad::GetInstance();

	if (pMesh != nullptr)
	{
		pMesh->DeleteAllRoadEvent();
	}

	// イテレイター選択
	SelectIterator();
}

//=====================================================
// ステイトの変更
//=====================================================
void CEditMesh::ChangeState(CStateEditMesh *pState)
{
	if (m_pState != nullptr)
	{
		delete m_pState;
		m_pState = nullptr;
	}

	m_pState = pState;
}

//=====================================================
// イテレイターの選択
//=====================================================
std::vector<CMeshRoad::SInfoRoadPoint>::iterator CEditMesh::SelectIterator(void)
{
	//CMeshRoad *pMesh = CMeshRoad::GetInstance();

	//if (pMesh == nullptr)
	//	return std::vector<CMeshRoad::SInfoRoadPoint>::iterator();

	//std::vector<CMeshRoad::SInfoRoadPoint> *paRP = pMesh->GetArrayRP();

	//D3DXVECTOR3 posHit;
	//D3DXVECTOR3 posNear;
	//D3DXVECTOR3 posFar;
	//D3DXVECTOR3 vecDiff;

	//universal::ConvertScreenPosTo3D(&posNear, &posFar, &vecDiff);

	//if (!m_bCurrent)
	//{// ブロックを探している状態
	//	for (auto it = paRP->begin();it != paRP->end() - 1;)
	//	{
	//		// ロードポイントごとのレイとの判定
	//		CollideRPRay(it, posFar, posNear, vecDiff);

	//		if (m_bCurrent)
	//			break;

	//		it++;
	//	}
	//}
	//else
	//{// ブロックを動かす
	//	MoveCurrentRP();

	//	// 道の調整
	//	pMesh->CreateVtxBuffEdge();
	//}

	return m_itCurrent;
}

//=====================================================
// ロードポイントのレイの判定
//=====================================================
void CEditMesh::CollideRPRay(std::vector<CMeshRoad::SInfoRoadPoint>::iterator it, D3DXVECTOR3 posFar, D3DXVECTOR3 posNear, D3DXVECTOR3 vecDiff)
{
	CInputMouse *pMouse = CInputMouse::GetInstance();
	CInputKeyboard *pKeyboard = CInputKeyboard::GetInstance();

	if (m_bCurrent || pMouse == nullptr || pKeyboard == nullptr)
		return;

	D3DXVECTOR3 posRP = it->pos;

	bool bHit = universal::CalcRaySphere(posNear, vecDiff, posRP, 200.0f);

	if (bHit)
	{
		CEffect3D::Create(posRP, 100.0f, 3, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	}

	if (pMouse->GetTrigger(CInputMouse::BUTTON_LMB))
	{
		if (bHit)
		{
			m_bCurrent = true;

			m_itCurrent = it;

			m_posCurrent = it->pos;

			return;
		}
	}
}

//=====================================================
// 敵の保存
//=====================================================
void CEditMesh::SaveEnemy(const char* pPath)
{
	FILE *pFile = nullptr;

	pFile = fopen(pPath, "w");

	if (pFile != nullptr)
	{
		fprintf(pFile, "#====================================================================\n");
		fprintf(pFile, "#\n");
		fprintf(pFile, "# 敵配置情報[enemy.txt]\n");
		fprintf(pFile, "# Author：Momoya Takayama\n");
		fprintf(pFile, "#\n");
		fprintf(pFile, "#====================================================================\n");
		fprintf(pFile, "SCRIPT\n\n");

		fprintf(pFile, "#====================================================================\n");
		fprintf(pFile, "# 配置情報\n");
		fprintf(pFile, "#====================================================================\n");

		for (auto it : m_listEnemySpawner)
		{
			fprintf(pFile, "SETSPAWNER\n");

			int nIdx = it->GetIdxSpline();

			fprintf(pFile, " IDX = %d\n", nIdx);

			fprintf(pFile, "END_SETSPAWNER\n\n");
		}

		fprintf(pFile, "END_SCRIPT\n");

		fclose(pFile);
	}
}

//=====================================================
// 敵の読込処理
//=====================================================
void CEditMesh::LoadEnemy(const char* pPath)
{
	char cTemp[256];
	int nCntBlock = 0;

	//ファイルから読み込む
	FILE *pFile = fopen(pPath, "r");

	if (pFile != nullptr)
	{//ファイルが開けた場合
		while (true)
		{
			//文字読み込み
			(void)fscanf(pFile, "%s", &cTemp[0]);

			if (strcmp(cTemp, "SETSPAWNER") == 0)
			{
				int nIdx = 0;

				while (true)
				{
					(void)fscanf(pFile, "%s", &cTemp[0]);

					if (strcmp(cTemp, "IDX") == 0)
					{// 番号
						(void)fscanf(pFile, "%s", &cTemp[0]);

						(void)fscanf(pFile, "%d", &nIdx);
					}

					if (strcmp(cTemp, "END_SETSPAWNER") == 0)
					{// 設定終わり
						CEnemySpawner *pSpawner = CEnemySpawner::Create(nIdx);

						break;
					}
				}
			}

			if (strcmp(cTemp, "END_SCRIPT") == 0)
			{
				break;
			}
		}

		fclose(pFile);
	}
}

//=====================================================
// 選択したロードポイントの移動
//=====================================================
void CEditMesh::MoveCurrentRP(void)
{
	CInputMouse *pMouse = CInputMouse::GetInstance();
	CInputKeyboard *pKeyboard = CInputKeyboard::GetInstance();

	if (pMouse == nullptr || pKeyboard == nullptr || !m_bCurrent)
		return;

	D3DXVECTOR3 posHit;
	D3DXVECTOR3 posNear;
	D3DXVECTOR3 posFar;
	D3DXVECTOR3 vecDiff;

	universal::ConvertScreenPosTo3D(&posNear, &posFar, &vecDiff);

	universal::CalcRayFlat(m_posCurrent, D3DXVECTOR3(0.0f, 1.0f, 0.0f), posNear, posFar, &posHit);

	if (pKeyboard->GetRelease(DIK_C))
	{
		m_bCurrent = false;
	}
	else if (pKeyboard->GetRelease(DIK_V))
	{
		m_bCurrent = false;
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

		m_itCurrent->pos = posHit;

		m_posCurrent.x = posHit.x;
		m_posCurrent.z = posHit.z;
	}

	if (pMouse->GetRelease(CInputMouse::BUTTON_LMB))
	{
		m_bCurrent = false;
	}
}

//****************************************************************************************
// メッシュの生成
//****************************************************************************************
void CStateEditMeshCreateMesh::Update(CEditMesh *pEdit)
{
	CInputKeyboard *pKeyboard = CInputKeyboard::GetInstance();
	CInputMouse* pMouse = CInputMouse::GetInstance();

	D3DXVECTOR3 pos = pEdit->GetPosition();
	D3DXVECTOR3 rot = CManager::GetCamera()->GetCamera()->rot;

	D3DXVECTOR3 vecPole = universal::PolarCoordinates(D3DXVECTOR3(D3DX_PI * 0.5f, rot.y - D3DX_PI * 0.5f, 0.0f));

	CEffect3D::Create(pos + vecPole * 200.0f, 50.0f, 5, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	CEffect3D::Create(pos - vecPole * 200.0f, 50.0f, 5, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	D3DXVECTOR3 posHit;
	D3DXVECTOR3 posNear;
	D3DXVECTOR3 posFar;
	D3DXVECTOR3 vecDiff;

	universal::ConvertScreenPosTo3D(&posNear, &posFar, &vecDiff);

	universal::CalcRayFlat(D3DXVECTOR3(0.0f, 100.0f, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), posNear, posFar, &posHit);

	D3DXVECTOR3 posEdit = pEdit->GetPosition();

	// 移動
	if (pKeyboard->GetPress(DIK_V))
	{// 上下移動
		CCamera *pCamera = CManager::GetCamera();

		if (pCamera == nullptr)
			return;

		CCamera::Camera *pInfoCamera = pCamera->GetCamera();

		// カメラとブロックの平面差分ベクトルを法線にする
		D3DXVECTOR3 posCamera = { pInfoCamera->posV.x,0.0f, pInfoCamera->posV.z };
		D3DXVECTOR3 posBlock = { posEdit.x,0.0f,posEdit.z };
		D3DXVECTOR3 vecDiff = posCamera - posBlock;

		D3DXVec3Normalize(&vecDiff, &vecDiff);

		universal::CalcRayFlat(posEdit, vecDiff, posNear, posFar, &posHit);

		// y軸以外は固定する
		posHit.x = posEdit.x;
		posHit.z = posEdit.z;
	}

	posEdit = posHit;

	pEdit->SetPosition(posEdit);

	bool bStop = CGame::GetInstance()->GetStop();

	if (bStop)
	{
		if (pKeyboard->GetTrigger(DIK_SPACE))
		{// ロードポイントの追加
			CMeshRoad *pMesh = CMeshRoad::GetInstance();

			pMesh->AddRoadPoint(pos, true);
		}
	}

	if (ImGui::Button("Save", ImVec2(100, 50)))
	{// 保存
		CMeshRoad *pMesh = CMeshRoad::GetInstance();

		if (pMesh != nullptr)
			pMesh->Save(pEdit->GetSavePath());
	}
}

//****************************************************************************************
// トンネルの生成
//****************************************************************************************
//=====================================================
// コンストラクタ
//=====================================================
CStateEditMeshCreateTunnel::CStateEditMeshCreateTunnel() : m_bEnd(false)
{
	CMeshRoad *pMeshRoad = CMeshRoad::GetInstance();
	pMeshRoad->ResetIterator();
}

//=====================================================
// 更新処理
//=====================================================
void CStateEditMeshCreateTunnel::Update(CEditMesh *pEdit)
{
	CMeshRoad *pMesh = CMeshRoad::GetInstance();

	CInputKeyboard *pKeyboard = CInputKeyboard::GetInstance();

	if (pKeyboard == nullptr || pMesh == nullptr)
		return;

	// ロードポイントの選択
	std::vector<CMeshRoad::SInfoRoadPoint>::iterator it = pMesh->SelectRoadPoint();

	if (pKeyboard->GetTrigger(DIK_SPACE))
	{// ロードポイントの決定
		SetTunnel(it);
	}

	if (pKeyboard->GetTrigger(DIK_R))
	{// 選択の撤回
		m_bEnd = false;
	}

	if (m_bEnd)
	{// 選択したロードポイントにエフェクトを出す
		D3DXVECTOR3 pos = m_itStart->pos;
		pos.y += 100.0f;
		CEffect3D::Create(pos, 50.0f, 3, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	}

	if (ImGui::Button("Save", ImVec2(100, 50)))
	{// 保存
		CMeshRoad *pMesh = CMeshRoad::GetInstance();

		if (pMesh != nullptr)
			pMesh->Save(pEdit->GetSavePath());
	}
}

//=====================================================
// トンネルの設置
//=====================================================
void CStateEditMeshCreateTunnel::SetTunnel(std::vector<CMeshRoad::SInfoRoadPoint>::iterator it)
{// 最初と最後の辺の設定
	if (!m_bEnd)
	{// 最初の辺設定
		m_itStart = it;
		m_bEnd = true;
	}
	else if (m_bEnd)
	{// 最後の辺を設定
		m_itEnd = it;

		// トンネルの生成
		CTunnel *pTunnel = CTunnel::Create(m_itStart, m_itEnd);

		CMeshRoad *pMeshRoad = CMeshRoad::GetInstance();

		if (pMeshRoad != nullptr)
		{
			std::vector<CTunnel*> *paTunnel = pMeshRoad->GetArrayTunnnel();

			if (paTunnel != nullptr)
			{
				paTunnel->push_back(pTunnel);
			}
		}
	}
}

//****************************************************************************************
// ガードレールの生成
//****************************************************************************************
//=====================================================
// コンストラクタ
//=====================================================
CStateEditMeshCreateGR::CStateEditMeshCreateGR() : m_bEnd(false), m_bLeft(false), m_fHeight(0.0f)
{
	CMeshRoad *pMeshRoad = CMeshRoad::GetInstance();
	pMeshRoad->ResetIterator();

	m_fHeight = DEFAULT_HEIGHT_GR;
}

//=====================================================
// 更新処理
//=====================================================
void CStateEditMeshCreateGR::Update(CEditMesh *pEdit)
{
	CMeshRoad *pMesh = CMeshRoad::GetInstance();

	CInputKeyboard *pKeyboard = CInputKeyboard::GetInstance();

	if (pKeyboard == nullptr || pMesh == nullptr)
		return;

	// ロードポイントの選択
	std::vector<CMeshRoad::SInfoRoadPoint>::iterator it = pMesh->SelectRoadPoint();

	if (pKeyboard->GetTrigger(DIK_SPACE))
	{// ロードポイントの決定
		SetGR(it);
	}

	if (pKeyboard->GetTrigger(DIK_R))
	{// 選択の撤回
		m_bEnd = false;
	}

	ImGui::Text("[GuardRail]");

	const char* aText[2] =
	{
		"Right",
		"Left"
	};

	// 左右切り替え
	if (ImGui::Button(aText[(int)m_bLeft], ImVec2(70, 20)))
		m_bLeft = m_bLeft ? false : true;

	// 高さ切り替え
	ImGui::DragFloat("Height", &m_fHeight, 2.0f, -FLT_MAX, FLT_MAX);

	D3DXVECTOR3 posHeight = it->pos;	// 高さの見本エフェクト
	posHeight.y += m_fHeight;
	CEffect3D::Create(posHeight, 50.0f, 3, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

	if (m_bEnd)
	{// 選択したロードポイントにエフェクトを出す
		D3DXVECTOR3 pos = m_itStart->pos;
		pos.y += 100.0f;
		CEffect3D::Create(pos, 50.0f, 3, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	}

	if (ImGui::Button("Save", ImVec2(100, 50)))
	{// 保存
		CMeshRoad *pMesh = CMeshRoad::GetInstance();

		if (pMesh != nullptr)
			pMesh->Save(pEdit->GetSavePath());
	}
}

//=====================================================
// ガードレールの設置
//=====================================================
void CStateEditMeshCreateGR::SetGR(std::vector<CMeshRoad::SInfoRoadPoint>::iterator it)
{// 最初と最後の辺の設定
	if (!m_bEnd)
	{// 最初の辺設定
		m_itStart = it;
		m_bEnd = true;
	}
	else if (m_bEnd)
	{// 最後の辺を設定
		m_itEnd = it;

		// ガードレールの生成
		CGuardRail *pGR = CGuardRail::Create(m_itStart, m_itEnd, m_bLeft, m_fHeight);

		// 配列に保存
		CMeshRoad *pMeshRoad = CMeshRoad::GetInstance();

		if (pMeshRoad != nullptr)
		{
			std::vector<CGuardRail*> *paGR = pMeshRoad->GetArrayGR();

			if (pGR != nullptr)
			{
				paGR->push_back(pGR);
			}
		}
	}
}

//****************************************************************************************
// ガードレールの選択
//****************************************************************************************
CStateEditMeshSelectGR::CStateEditMeshSelectGR()
{// コンストラクタ
	CMeshRoad *pMesh = CMeshRoad::GetInstance();

	if (pMesh == nullptr)
		return;

	std::vector<CGuardRail*> *paGR = pMesh->GetArrayGR();

	m_it = paGR->begin();
}

void CStateEditMeshSelectGR::Update(CEditMesh *pEdit)
{
	CMeshRoad *pMesh = CMeshRoad::GetInstance();

	if (pMesh == nullptr)
		return;	// メッシュのチェック

	std::vector<CGuardRail*> *paGR = pMesh->GetArrayGR();

	if (paGR->empty() || (*m_it) == nullptr)
		return;	// 配列に中身あるかチェック

	ImGui::Text("[SelectGuardRail]");

	int nDist = std::distance(paGR->begin(), m_it);
	int nSize = paGR->size();

	// 選択したガードレールの表示
	(*m_it)->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	if (ImGui::DragInt("Index", &nDist, 1.0f, 0, nSize - 1))
	{
		// イテレータを再初期化して指定番号の要素に移動
		m_it = paGR->begin();
		std::advance(m_it, nDist);
	}

	if (ImGui::Button("NextGR", ImVec2(70, 30)))
	{
		if (m_it != paGR->end() && std::next(m_it) != paGR->end())
			std::advance(m_it, 1);
	}
	if (ImGui::Button("PrevGR", ImVec2(70, 30)))
	{
		if (m_it != paGR->begin())
			std::advance(m_it, -1);
	}

	if (ImGui::Button("DeleteGR", ImVec2(70, 30)))
	{
		if ((*m_it) == nullptr)
			return;

		(*m_it)->Uninit();
		paGR->erase(m_it);

		m_it = paGR->begin();
	}

	// 選択したガードレールの表示
	(*m_it)->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
}

//****************************************************************************************
// ロードポイントの調節
//****************************************************************************************
CStateEditMeshAdjustRoadPoint::CStateEditMeshAdjustRoadPoint()
{// コンストラクタ
	CMeshRoad *pMesh = CMeshRoad::GetInstance();

	pMesh->ResetIterator();
}

void CStateEditMeshAdjustRoadPoint::Update(CEditMesh *pEdit)
{
	CMeshRoad *pMesh = CMeshRoad::GetInstance();

	// ロードポイントの選択
	// 位置の調整
	std::vector<CMeshRoad::SInfoRoadPoint>::iterator it = pMesh->SelectRoadPoint();

	D3DXVECTOR3 pos = it->pos;

	ImGui::DragFloat("posRoadPointPOS.X", &pos.x, 2.0f, -FLT_MAX, FLT_MAX);
	ImGui::DragFloat("posRoadPointPOS.Y", &pos.y, 2.0f, -FLT_MAX, FLT_MAX);
	ImGui::DragFloat("posRoadPointPOS.Z", &pos.z, 2.0f, -FLT_MAX, FLT_MAX);

	it->pos = pos;

	// 幅の調整
	float fWidth = it->fWidth;
	ImGui::DragFloat("Width", &fWidth, 2.0f, -FLT_MAX, FLT_MAX);
	it->fWidth = fWidth;

	pMesh->CreateVtxBuffEdge();

	if (ImGui::Button("Save", ImVec2(100, 50)))
	{// 保存
		CMeshRoad *pMesh = CMeshRoad::GetInstance();

		if (pMesh != nullptr)
			pMesh->Save(pEdit->GetSavePath());
	}

	if (ImGui::Button("AjustRoadEvent", ImVec2(100, 50)))
	{// 道イベントを調整
		pMesh->AjustRoadEvent();
	}

	if (ImGui::TreeNode("Enemy"))
	{
		if (ImGui::Button("CreateEnemy", ImVec2(100, 50)))
		{// 敵生成
			size_t idx = pMesh->GetIdxRoad(it);

			CEnemySpawner *pSpawner = CEnemySpawner::Create(idx + 1);

			if (pSpawner != nullptr)
			{
				pEdit->AddSpawner(pSpawner);
			}
		}

		if (ImGui::Button("SaveEnemy", ImVec2(100, 50)))
		{
			pEdit->SaveEnemy(pEdit->GetSavePathEnemy());
		}

		ImGui::TreePop();
	}
}

//****************************************************************************************
// 辺の削除
//****************************************************************************************
CStateEditMeshDeleteRoadPoint::CStateEditMeshDeleteRoadPoint()
{// コンストラクタ
	CMeshRoad *pMesh = CMeshRoad::GetInstance();

	pMesh->ResetIterator();
}

void CStateEditMeshDeleteRoadPoint::Update(CEditMesh *pEdit)
{
	if (ImGui::Button("Save", ImVec2(100, 50)))
	{// 保存
		CMeshRoad *pMesh = CMeshRoad::GetInstance();

		if (pMesh != nullptr)
			pMesh->Save(pEdit->GetSavePath());
	}

	CMeshRoad *pMesh = CMeshRoad::GetInstance();

	// 辺の選択
	std::vector<CMeshRoad::SInfoRoadPoint>::iterator it = pMesh->SelectRoadPoint();

	if (ImGui::Button("Delete", ImVec2(100, 50)))
	{
		CMeshRoad::GetInstance()->DeleteRoadPoint(it);

		return;
	}
}