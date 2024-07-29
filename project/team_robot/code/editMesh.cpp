//*****************************************************
//
// �u���b�N�G�f�B�^�[�̏���[editBlock.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
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
// �萔��`
//*****************************************************
namespace
{
const float SPEED_MOVE = 5.0f;	// �ړ����x
const float SPEED_ROLL = 0.05f;	// ��]���x
const float DEFAULT_HEIGHT_GR = 600.0f;	// �K�[�h���[���̃f�t�H���g����
const char* PATH_SAVE = "data\\MAP\\road00.bin";	// �ۑ��t�@�C���̃p�X
const char* PATH_SAVE_ENEMY = "data\\MAP\\enemy00.txt";	// �G�ۑ��t�@�C���̃f�t�H���g�p�X
}

//=====================================================
// �R���X�g���N�^
//=====================================================
CEditMesh::CEditMesh()
{
	m_pMesh = nullptr;
	m_pManipulator = nullptr;
	m_pState = nullptr;
	m_bCurrent = false;
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CEditMesh::~CEditMesh()
{

}

//=====================================================
// ����������
//=====================================================
HRESULT CEditMesh::Init(void)
{
	strcpy(&m_aPathSave[0], (char*)PATH_SAVE);
	strcpy(&m_aPathSaveEnemy[0], (char*)PATH_SAVE_ENEMY);

	CEdit::Init();

	// �}�j�s�����[�^�̐���
	m_pManipulator = CObjectX::Create();

	// ���f���̊��蓖��
	int nIdx = CModel::Load("data\\MODEL\\block\\manipulator.x");
	m_pManipulator->BindModel(nIdx);

	ChangeState(new CStateEditMeshCreateMesh);

	return S_OK;
}

//=====================================================
// �I������
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
// �X�V����
//=====================================================
void CEditMesh::Update(void)
{
	CEdit::Update();

	// �}�j�s�����[�^�[�̃g�����X�t�H�[���X�V
	D3DXVECTOR3 pos = GetPosition();
	D3DXVECTOR3 rot = GetRotation();

	m_pManipulator->SetPosition(pos);
	m_pManipulator->SetRotation(rot);

	if (m_pState != nullptr)	// �X�e�C�g�̍X�V
		m_pState->Update(this);

	// �X�e�C�g�ɂ�郂�[�h�̐؂�ւ�
	ImGui::Text("ModeSelect");

	if (ImGui::Button("CreateEdge", ImVec2(70, 30)))	// ���b�V������
		ChangeState(new CStateEditMeshCreateMesh);

	if (ImGui::Button("CreateTunnel", ImVec2(70, 30)))	// �g���l���̐���
		ChangeState(new CStateEditMeshCreateTunnel);

	if (ImGui::Button("CreateGR", ImVec2(70, 30)))	// �K�[�h���[���̐���
		ChangeState(new CStateEditMeshCreateGR);

	if (ImGui::Button("SelectGR", ImVec2(70, 30)))	// �K�[�h���[���̑I��
		ChangeState(new CStateEditMeshSelectGR);

	if (ImGui::Button("AdjustRoadPoint", ImVec2(70, 30)))	// ���[�h�|�C���g�̒���
		ChangeState(new CStateEditMeshAdjustRoadPoint);

	if (ImGui::Button("DeleteRoadPoint", ImVec2(70, 30)))	// �ӂ̍폜
		ChangeState(new CStateEditMeshDeleteRoadPoint);

	ImGui::InputText("SavePath", &m_aPathSave[0], 256);	// ���b�V���f�[�^�̕ۑ��p�X
	ImGui::InputText("SavePathEnemy", &m_aPathSaveEnemy[0], 256);	// �G�f�[�^�̕ۑ��p�X

	// ���b�V�����[�h�̑I��
	CMeshRoad::SelectMeshRoad();

	// �S���[�h�|�C���g�̍폜
	CMeshRoad *pMesh = CMeshRoad::GetInstance();

	if (pMesh != nullptr)
	{
		pMesh->DeleteAllRoadEvent();
	}

	// �C�e���C�^�[�I��
	SelectIterator();
}

//=====================================================
// �X�e�C�g�̕ύX
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
// �C�e���C�^�[�̑I��
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
	//{// �u���b�N��T���Ă�����
	//	for (auto it = paRP->begin();it != paRP->end() - 1;)
	//	{
	//		// ���[�h�|�C���g���Ƃ̃��C�Ƃ̔���
	//		CollideRPRay(it, posFar, posNear, vecDiff);

	//		if (m_bCurrent)
	//			break;

	//		it++;
	//	}
	//}
	//else
	//{// �u���b�N�𓮂���
	//	MoveCurrentRP();

	//	// ���̒���
	//	pMesh->CreateVtxBuffEdge();
	//}

	return m_itCurrent;
}

//=====================================================
// ���[�h�|�C���g�̃��C�̔���
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
// �G�̕ۑ�
//=====================================================
void CEditMesh::SaveEnemy(const char* pPath)
{
	FILE *pFile = nullptr;

	pFile = fopen(pPath, "w");

	if (pFile != nullptr)
	{
		fprintf(pFile, "#====================================================================\n");
		fprintf(pFile, "#\n");
		fprintf(pFile, "# �G�z�u���[enemy.txt]\n");
		fprintf(pFile, "# Author�FMomoya Takayama\n");
		fprintf(pFile, "#\n");
		fprintf(pFile, "#====================================================================\n");
		fprintf(pFile, "SCRIPT\n\n");

		fprintf(pFile, "#====================================================================\n");
		fprintf(pFile, "# �z�u���\n");
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
// �G�̓Ǎ�����
//=====================================================
void CEditMesh::LoadEnemy(const char* pPath)
{
	char cTemp[256];
	int nCntBlock = 0;

	//�t�@�C������ǂݍ���
	FILE *pFile = fopen(pPath, "r");

	if (pFile != nullptr)
	{//�t�@�C�����J�����ꍇ
		while (true)
		{
			//�����ǂݍ���
			(void)fscanf(pFile, "%s", &cTemp[0]);

			if (strcmp(cTemp, "SETSPAWNER") == 0)
			{
				int nIdx = 0;

				while (true)
				{
					(void)fscanf(pFile, "%s", &cTemp[0]);

					if (strcmp(cTemp, "IDX") == 0)
					{// �ԍ�
						(void)fscanf(pFile, "%s", &cTemp[0]);

						(void)fscanf(pFile, "%d", &nIdx);
					}

					if (strcmp(cTemp, "END_SETSPAWNER") == 0)
					{// �ݒ�I���
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
// �I���������[�h�|�C���g�̈ړ�
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
	{// �ړ�
		if (pKeyboard->GetPress(DIK_V))
		{// �㉺�ړ�
			CCamera *pCamera = CManager::GetCamera();

			if (pCamera == nullptr)
				return;

			CCamera::Camera *pInfoCamera = pCamera->GetCamera();

			// �J�����ƃu���b�N�̕��ʍ����x�N�g����@���ɂ���
			D3DXVECTOR3 posCamera = { pInfoCamera->posV.x,0.0f, pInfoCamera->posV.z };
			D3DXVECTOR3 posBlock = { m_posCurrent.x,0.0f,m_posCurrent.z };
			D3DXVECTOR3 vecDiff = posCamera - posBlock;

			D3DXVec3Normalize(&vecDiff, &vecDiff);

			universal::CalcRayFlat(m_posCurrent, vecDiff, posNear, posFar, &posHit);

			// y���ȊO�͌Œ肷��
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
// ���b�V���̐���
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

	// �ړ�
	if (pKeyboard->GetPress(DIK_V))
	{// �㉺�ړ�
		CCamera *pCamera = CManager::GetCamera();

		if (pCamera == nullptr)
			return;

		CCamera::Camera *pInfoCamera = pCamera->GetCamera();

		// �J�����ƃu���b�N�̕��ʍ����x�N�g����@���ɂ���
		D3DXVECTOR3 posCamera = { pInfoCamera->posV.x,0.0f, pInfoCamera->posV.z };
		D3DXVECTOR3 posBlock = { posEdit.x,0.0f,posEdit.z };
		D3DXVECTOR3 vecDiff = posCamera - posBlock;

		D3DXVec3Normalize(&vecDiff, &vecDiff);

		universal::CalcRayFlat(posEdit, vecDiff, posNear, posFar, &posHit);

		// y���ȊO�͌Œ肷��
		posHit.x = posEdit.x;
		posHit.z = posEdit.z;
	}

	posEdit = posHit;

	pEdit->SetPosition(posEdit);

	bool bStop = CGame::GetInstance()->GetStop();

	if (bStop)
	{
		if (pKeyboard->GetTrigger(DIK_SPACE))
		{// ���[�h�|�C���g�̒ǉ�
			CMeshRoad *pMesh = CMeshRoad::GetInstance();

			pMesh->AddRoadPoint(pos, true);
		}
	}

	if (ImGui::Button("Save", ImVec2(100, 50)))
	{// �ۑ�
		CMeshRoad *pMesh = CMeshRoad::GetInstance();

		if (pMesh != nullptr)
			pMesh->Save(pEdit->GetSavePath());
	}
}

//****************************************************************************************
// �g���l���̐���
//****************************************************************************************
//=====================================================
// �R���X�g���N�^
//=====================================================
CStateEditMeshCreateTunnel::CStateEditMeshCreateTunnel() : m_bEnd(false)
{
	CMeshRoad *pMeshRoad = CMeshRoad::GetInstance();
	pMeshRoad->ResetIterator();
}

//=====================================================
// �X�V����
//=====================================================
void CStateEditMeshCreateTunnel::Update(CEditMesh *pEdit)
{
	CMeshRoad *pMesh = CMeshRoad::GetInstance();

	CInputKeyboard *pKeyboard = CInputKeyboard::GetInstance();

	if (pKeyboard == nullptr || pMesh == nullptr)
		return;

	// ���[�h�|�C���g�̑I��
	std::vector<CMeshRoad::SInfoRoadPoint>::iterator it = pMesh->SelectRoadPoint();

	if (pKeyboard->GetTrigger(DIK_SPACE))
	{// ���[�h�|�C���g�̌���
		SetTunnel(it);
	}

	if (pKeyboard->GetTrigger(DIK_R))
	{// �I���̓P��
		m_bEnd = false;
	}

	if (m_bEnd)
	{// �I���������[�h�|�C���g�ɃG�t�F�N�g���o��
		D3DXVECTOR3 pos = m_itStart->pos;
		pos.y += 100.0f;
		CEffect3D::Create(pos, 50.0f, 3, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	}

	if (ImGui::Button("Save", ImVec2(100, 50)))
	{// �ۑ�
		CMeshRoad *pMesh = CMeshRoad::GetInstance();

		if (pMesh != nullptr)
			pMesh->Save(pEdit->GetSavePath());
	}
}

//=====================================================
// �g���l���̐ݒu
//=====================================================
void CStateEditMeshCreateTunnel::SetTunnel(std::vector<CMeshRoad::SInfoRoadPoint>::iterator it)
{// �ŏ��ƍŌ�̕ӂ̐ݒ�
	if (!m_bEnd)
	{// �ŏ��̕Ӑݒ�
		m_itStart = it;
		m_bEnd = true;
	}
	else if (m_bEnd)
	{// �Ō�̕ӂ�ݒ�
		m_itEnd = it;

		// �g���l���̐���
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
// �K�[�h���[���̐���
//****************************************************************************************
//=====================================================
// �R���X�g���N�^
//=====================================================
CStateEditMeshCreateGR::CStateEditMeshCreateGR() : m_bEnd(false), m_bLeft(false), m_fHeight(0.0f)
{
	CMeshRoad *pMeshRoad = CMeshRoad::GetInstance();
	pMeshRoad->ResetIterator();

	m_fHeight = DEFAULT_HEIGHT_GR;
}

//=====================================================
// �X�V����
//=====================================================
void CStateEditMeshCreateGR::Update(CEditMesh *pEdit)
{
	CMeshRoad *pMesh = CMeshRoad::GetInstance();

	CInputKeyboard *pKeyboard = CInputKeyboard::GetInstance();

	if (pKeyboard == nullptr || pMesh == nullptr)
		return;

	// ���[�h�|�C���g�̑I��
	std::vector<CMeshRoad::SInfoRoadPoint>::iterator it = pMesh->SelectRoadPoint();

	if (pKeyboard->GetTrigger(DIK_SPACE))
	{// ���[�h�|�C���g�̌���
		SetGR(it);
	}

	if (pKeyboard->GetTrigger(DIK_R))
	{// �I���̓P��
		m_bEnd = false;
	}

	ImGui::Text("[GuardRail]");

	const char* aText[2] =
	{
		"Right",
		"Left"
	};

	// ���E�؂�ւ�
	if (ImGui::Button(aText[(int)m_bLeft], ImVec2(70, 20)))
		m_bLeft = m_bLeft ? false : true;

	// �����؂�ւ�
	ImGui::DragFloat("Height", &m_fHeight, 2.0f, -FLT_MAX, FLT_MAX);

	D3DXVECTOR3 posHeight = it->pos;	// �����̌��{�G�t�F�N�g
	posHeight.y += m_fHeight;
	CEffect3D::Create(posHeight, 50.0f, 3, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

	if (m_bEnd)
	{// �I���������[�h�|�C���g�ɃG�t�F�N�g���o��
		D3DXVECTOR3 pos = m_itStart->pos;
		pos.y += 100.0f;
		CEffect3D::Create(pos, 50.0f, 3, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	}

	if (ImGui::Button("Save", ImVec2(100, 50)))
	{// �ۑ�
		CMeshRoad *pMesh = CMeshRoad::GetInstance();

		if (pMesh != nullptr)
			pMesh->Save(pEdit->GetSavePath());
	}
}

//=====================================================
// �K�[�h���[���̐ݒu
//=====================================================
void CStateEditMeshCreateGR::SetGR(std::vector<CMeshRoad::SInfoRoadPoint>::iterator it)
{// �ŏ��ƍŌ�̕ӂ̐ݒ�
	if (!m_bEnd)
	{// �ŏ��̕Ӑݒ�
		m_itStart = it;
		m_bEnd = true;
	}
	else if (m_bEnd)
	{// �Ō�̕ӂ�ݒ�
		m_itEnd = it;

		// �K�[�h���[���̐���
		CGuardRail *pGR = CGuardRail::Create(m_itStart, m_itEnd, m_bLeft, m_fHeight);

		// �z��ɕۑ�
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
// �K�[�h���[���̑I��
//****************************************************************************************
CStateEditMeshSelectGR::CStateEditMeshSelectGR()
{// �R���X�g���N�^
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
		return;	// ���b�V���̃`�F�b�N

	std::vector<CGuardRail*> *paGR = pMesh->GetArrayGR();

	if (paGR->empty() || (*m_it) == nullptr)
		return;	// �z��ɒ��g���邩�`�F�b�N

	ImGui::Text("[SelectGuardRail]");

	int nDist = std::distance(paGR->begin(), m_it);
	int nSize = paGR->size();

	// �I�������K�[�h���[���̕\��
	(*m_it)->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	if (ImGui::DragInt("Index", &nDist, 1.0f, 0, nSize - 1))
	{
		// �C�e���[�^���ď��������Ďw��ԍ��̗v�f�Ɉړ�
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

	// �I�������K�[�h���[���̕\��
	(*m_it)->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
}

//****************************************************************************************
// ���[�h�|�C���g�̒���
//****************************************************************************************
CStateEditMeshAdjustRoadPoint::CStateEditMeshAdjustRoadPoint()
{// �R���X�g���N�^
	CMeshRoad *pMesh = CMeshRoad::GetInstance();

	pMesh->ResetIterator();
}

void CStateEditMeshAdjustRoadPoint::Update(CEditMesh *pEdit)
{
	CMeshRoad *pMesh = CMeshRoad::GetInstance();

	// ���[�h�|�C���g�̑I��
	// �ʒu�̒���
	std::vector<CMeshRoad::SInfoRoadPoint>::iterator it = pMesh->SelectRoadPoint();

	D3DXVECTOR3 pos = it->pos;

	ImGui::DragFloat("posRoadPointPOS.X", &pos.x, 2.0f, -FLT_MAX, FLT_MAX);
	ImGui::DragFloat("posRoadPointPOS.Y", &pos.y, 2.0f, -FLT_MAX, FLT_MAX);
	ImGui::DragFloat("posRoadPointPOS.Z", &pos.z, 2.0f, -FLT_MAX, FLT_MAX);

	it->pos = pos;

	// ���̒���
	float fWidth = it->fWidth;
	ImGui::DragFloat("Width", &fWidth, 2.0f, -FLT_MAX, FLT_MAX);
	it->fWidth = fWidth;

	pMesh->CreateVtxBuffEdge();

	if (ImGui::Button("Save", ImVec2(100, 50)))
	{// �ۑ�
		CMeshRoad *pMesh = CMeshRoad::GetInstance();

		if (pMesh != nullptr)
			pMesh->Save(pEdit->GetSavePath());
	}

	if (ImGui::Button("AjustRoadEvent", ImVec2(100, 50)))
	{// ���C�x���g�𒲐�
		pMesh->AjustRoadEvent();
	}

	if (ImGui::TreeNode("Enemy"))
	{
		if (ImGui::Button("CreateEnemy", ImVec2(100, 50)))
		{// �G����
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
// �ӂ̍폜
//****************************************************************************************
CStateEditMeshDeleteRoadPoint::CStateEditMeshDeleteRoadPoint()
{// �R���X�g���N�^
	CMeshRoad *pMesh = CMeshRoad::GetInstance();

	pMesh->ResetIterator();
}

void CStateEditMeshDeleteRoadPoint::Update(CEditMesh *pEdit)
{
	if (ImGui::Button("Save", ImVec2(100, 50)))
	{// �ۑ�
		CMeshRoad *pMesh = CMeshRoad::GetInstance();

		if (pMesh != nullptr)
			pMesh->Save(pEdit->GetSavePath());
	}

	CMeshRoad *pMesh = CMeshRoad::GetInstance();

	// �ӂ̑I��
	std::vector<CMeshRoad::SInfoRoadPoint>::iterator it = pMesh->SelectRoadPoint();

	if (ImGui::Button("Delete", ImVec2(100, 50)))
	{
		CMeshRoad::GetInstance()->DeleteRoadPoint(it);

		return;
	}
}