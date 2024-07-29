//*****************************************************
//
// �����L���O�̏���[ranking.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "ranking.h"
#include "resultBehavior.h"
#include "fade.h"
#include "inputManager.h"
#include "object.h"
#include "polygon2D.h"
#include "meshcylinder.h"
#include "texture.h"
#include "camera.h"
#include "manager.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const float SPEED_ROTATION = 0.003f;	// ���X�s�[�h
const float HEIGHT_CYLINDER = 800.0f;	// �V�����_�[�̍���
const std::string PATH_RANKING = "data\\TEXT\\ranking.txt";	// �����L���O�̃p�X
const D3DXVECTOR2 SIZE_CAPTION = { SCREEN_WIDTH * 0.5f,100.0f };	// ���o���̃T�C�Y
const D3DXVECTOR3 POS_CAPTION = { SCRN_MID.x,SIZE_CAPTION.y,0.0f };	// ���o���̈ʒu
}

//=====================================================
// �R���X�g���N�^
//=====================================================
CRanking::CRanking()
{
	m_pCylinder = nullptr;
	m_pBehavior = nullptr;
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CRanking::~CRanking()
{

}

//=====================================================
// ����������
//=====================================================
HRESULT CRanking::Init(void)
{
	if (m_pCylinder == nullptr)
	{
		// �V�����_�[�̐���
		m_pCylinder = CMeshCylinder::Create();

		if (m_pCylinder != nullptr)
		{
			m_pCylinder->SetRadius(1500.0f);
			m_pCylinder->SetHeight(HEIGHT_CYLINDER);
			m_pCylinder->SetVtx();

			int nIdx = Texture::GetIdx("data\\TEXTURE\\BG\\result.jpg");
			m_pCylinder->SetIdxTexture(nIdx);
		}
	}

	// ���o���̐���
	CPolygon2D *pCaption = nullptr;

	pCaption = CPolygon2D::Create(4);

	if (pCaption != nullptr)
	{
		pCaption->SetPosition(POS_CAPTION);
		pCaption->SetSize(SIZE_CAPTION.x, SIZE_CAPTION.y);
		pCaption->SetVtx();

		int nIdx = Texture::GetIdx("data\\TEXTURE\\UI\\result.png");
		pCaption->SetIdxTexture(nIdx);
	}

	// �J�����ʒu�̐ݒ�
	CCamera *pCamera = CManager::GetCamera();

	if (pCamera == nullptr)
		return E_FAIL;

	CCamera::Camera *pInfoCamera = pCamera->GetCamera();

	pInfoCamera->posV = { 0.0f,HEIGHT_CYLINDER * 0.5f,0.0f };
	pInfoCamera->posR = { 0.0f,HEIGHT_CYLINDER * 0.5f,1.0f };

	LoadRanking();

	ChangeBehavior(new CResultPlayerScore);

	return S_OK;
}

//=====================================================
// �����L���O�̓Ǎ�
//=====================================================
void CRanking::LoadRanking(void)
{
	std::ifstream file(PATH_RANKING);

	if (file.is_open())
	{
		std::string temp;

		while (std::getline(file, temp))
		{// �ǂݍ��ނ��̂��Ȃ��Ȃ�܂œǍ�
			if (temp == "PLAYER")
			{// �v���C���[���Ǎ�
				SInfoRanking info;

				while (std::getline(file, temp))
				{
					std::istringstream iss(temp);
					std::string key;
					iss >> key;

					if (key == "RANK")
					{// �����N
						iss >> info.nRank;
					}

					if (key == "NAME")
					{// ���O
						iss >> info.name;
					}

					if (key == "REWARD")
					{// ��V�z
						iss >> info.nReward;
					}

					if (temp == "END_PLAYER")
					{
						m_listRanking.push_back(info);

						break;
					}
				}
			}
		}
	}
	else
	{
		assert(("�����L���O�t�@�C�����J���܂���ł���", false));
	}
}

//=====================================================
// �I������
//=====================================================
void CRanking::Uninit(void)
{
	if (m_pCylinder != nullptr)
	{
		m_pCylinder->Uninit();
		m_pCylinder = nullptr;
	}

	if (m_pBehavior != nullptr)
	{
		delete m_pBehavior;
		m_pBehavior = nullptr;
	}

	CObject::ReleaseAll();
}

//=====================================================
// �X�V����
//=====================================================
void CRanking::Update(void)
{
	if (m_pCylinder != nullptr)
	{
		D3DXVECTOR3 rot = m_pCylinder->GetRotation();

		rot.y += SPEED_ROTATION;

		universal::LimitRot(&rot.y);

		m_pCylinder->SetRotation(rot);
	}

	if (m_pBehavior != nullptr)
	{
		m_pBehavior->Update(this);
	}
}

//=====================================================
// �`�揈��
//=====================================================
void CRanking::Draw(void)
{

}

//=====================================================
// �r�w�C�r�A�̕ύX
//=====================================================
void CRanking::ChangeBehavior(CResultBehavior *pBehavior)
{
	if (m_pBehavior != nullptr)
	{
		m_pBehavior->Uninit(this);

		delete m_pBehavior;
	}

	m_pBehavior = pBehavior;

	if (m_pBehavior != nullptr)
	{
		m_pBehavior->Init(this);
	}
}