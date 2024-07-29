//*****************************************************
//
// ���U���g��Behavior[resultBehavior.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "resultBehavior.h"
#include "number.h"
#include "ranking.h"
#include "inputManager.h"
#include "fade.h"
#include "texture.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const int NUM_PLACE = 6;
const D3DXVECTOR3 POS_NUMBER[CResultPlayerScore::TYPE_MAX] =
{// �����\���X�R�A�̐��l�̈ʒu
	{ SCRN_MID.x, SCRN_MID.y - 250.0f, 0.0f },
	{ SCRN_MID.x, SCRN_MID.y - 130.0f, 0.0f },
	{ SCRN_MID.x, SCRN_MID.y + 30.0f, 0.0f },
	{ SCRN_MID.x, SCRN_MID.y + 210.0f, 0.0f },
};
const D3DXVECTOR2 SIZE_NUMBER = { 25.0f,50.0f };	// �����̃T�C�Y
const int COST_REPAIR = 50;	// ������1%�ɂ�����C����p
const char* PATH_TEXTURE_CAPTION[CResultPlayerScore::TYPE_MAX] =
{// ���ڂ̌��o���̃e�N�X�`���p�X
	"data\\TEXTURE\\UI\\resultCaption00.png",
	"data\\TEXTURE\\UI\\resultCaption01.png",
	"data\\TEXTURE\\UI\\resultCaption02.png",
	"data\\TEXTURE\\UI\\resultCaption03.png",
};
const D3DXVECTOR2 SIZE_CAPTION = { 70.0f,50.0f };	// ���ڌ��o���̃T�C�Y
}

//=====================================================
// �R���X�g���N�^
//=====================================================
CResultBehavior::CResultBehavior()
{

}

//=====================================================
// �f�X�g���N�^
//=====================================================
CResultBehavior::~CResultBehavior()
{

}

//=====================================================
// �v���C���[�X�R�A�\��
//=====================================================
CResultPlayerScore::CResultPlayerScore()
{// �R���X�g���N�^
	ZeroMemory(&m_apNumber[0], sizeof(m_apNumber));
	ZeroMemory(&m_apCaption[0], sizeof(m_apCaption));
	m_pFrame = nullptr;
}

CResultPlayerScore::~CResultPlayerScore()
{// �f�X�g���N�^

}

void CResultPlayerScore::Init(CRanking *pResult)
{// ������

}

void CResultPlayerScore::Uninit(CRanking *pResult)
{// �I��
	for (int i = 0; i < TYPE_NUMBER::TYPE_MAX; i++)
	{
		if (m_apNumber[i] != nullptr)
		{
			m_apNumber[i]->Uninit();
			m_apNumber[i] = nullptr;
		}

		if (m_apCaption[i] != nullptr)
		{
			m_apCaption[i]->Uninit();
			m_apCaption[i] = nullptr;
		}
	}

	if (m_pFrame != nullptr)
	{
		m_pFrame->Uninit();
		m_pFrame = nullptr;
	}
}

void CResultPlayerScore::Update(CRanking *pResult)
{// �X�V
	// �t�F�[�h���鑀��
	CInputManager *pInputManager = CInputManager::GetInstance();

	if (pInputManager != nullptr)
	{
		if (pInputManager->GetTrigger(CInputManager::BUTTON_ENTER))
		{
			CFade *pFade = CFade::GetInstance();

			if (pFade != nullptr)
			{
				pFade->SetFade(CScene::MODE_TITLE);
			}
		}

		return;
	}
}

//=====================================================
// �����L���O�\��
//=====================================================
CResultRanking::CResultRanking()
{// �R���X�g���N�^

}

CResultRanking::~CResultRanking()
{// �f�X�g���N�^

}

void CResultRanking::Init(CRanking *pResult)
{// ������

}

void CResultRanking::Update(CRanking *pResult)
{// �X�V
	// �t�F�[�h���鑀��
	CInputManager *pInputManager = CInputManager::GetInstance();

	if (pInputManager != nullptr)
	{
		if (pInputManager->GetTrigger(CInputManager::BUTTON_ENTER))
		{
			CFade *pFade = CFade::GetInstance();

			if (pFade != nullptr)
			{
				pFade->SetFade(CScene::MODE_TITLE);
			}
		}

		if (pInputManager->GetTrigger(CInputManager::BUTTON_BACK))
		{// ���X�R�A�\���ɖ߂�
			pResult->ChangeBehavior(new CResultPlayerScore);

			return;
		}
	}
}