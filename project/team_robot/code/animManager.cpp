//*****************************************************
//
// �A�j���[�V�����}�l�[�W���[[animManager.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "animManager.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "anim3D.h"

//*****************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************
CAnimManager *CAnimManager::s_pAnimEffect3D = nullptr;	// ���g�̃|�C���^

//*****************************************************
// �}�N����`
//*****************************************************
#define FILE_PATH	"data\\TEXT\\animEffect3D.txt"	// �t�@�C���̃p�X

//=====================================================
// �R���X�g���N�^
//=====================================================
CAnimManager::CAnimManager(int nPriority) : CObject(nPriority), m_apAnimEffect{}
{

}

//=====================================================
// �f�X�g���N�^
//=====================================================
CAnimManager::~CAnimManager()
{

}

//=====================================================
// ����������
//=====================================================
HRESULT CAnimManager::Init(void)
{
	// �Ǎ�
	Load();

	return S_OK;
}

//=====================================================
// �Ǎ�����
//=====================================================
void CAnimManager::Load(void)
{
	char cTemp[256];
	int nCntEffect = 0;

	// �t�@�C������ǂݍ���
	FILE *pFile = fopen(FILE_PATH, "r");

	if (pFile != nullptr)
	{// �t�@�C�����J�����ꍇ
		while (true)
		{
			// �����ǂݍ���
			(void)fscanf(pFile, "%s", &cTemp[0]);

			if (strcmp(cTemp, "EFFECTSET") == 0)
			{// �Ǎ��J�n
				if (m_apAnimEffect[nCntEffect] == nullptr)
				{
					// �C���X�^���X����
					m_apAnimEffect[nCntEffect] = new S_InfoAnimEffect;

					ZeroMemory(m_apAnimEffect[nCntEffect],sizeof(S_InfoAnimEffect));
				}

				while (true)
				{
					// �����ǂݍ���
					(void)fscanf(pFile, "%s", &cTemp[0]);

					if (strcmp(cTemp, "END_EFFECTSET") == 0)
					{// �I������
						nCntEffect++;

						break;
					}

					if (strcmp(cTemp, "PATH") == 0)
					{// �p�X
						(void)fscanf(pFile, "%s", &cTemp[0]);
						
						(void)fscanf(pFile, "%s", &m_apAnimEffect[nCntEffect]->acPath[0]);
					}

					if (strcmp(cTemp, "NUM_ANIM") == 0)
					{// �A�j���[�V������
						(void)fscanf(pFile, "%s", &cTemp[0]);

						(void)fscanf(pFile, "%d", &m_apAnimEffect[nCntEffect]->nNumAnim);
					}

					if (strcmp(cTemp, "SPEED_ANIM") == 0)
					{// �A�j���[�V�������x
						(void)fscanf(pFile, "%s", &cTemp[0]);

						(void)fscanf(pFile, "%d", &m_apAnimEffect[nCntEffect]->nSpeedAnim);
					}

					if (strcmp(cTemp, "SIZE") == 0)
					{// �T�C�Y
						(void)fscanf(pFile, "%s", &cTemp[0]);

						(void)fscanf(pFile, "%f", &m_apAnimEffect[nCntEffect]->fSize);
					}

					if (strcmp(cTemp, "COL") == 0)
					{// �F�ǂݍ���
						(void)fscanf(pFile, "%s", &cTemp[0]);

						(void)fscanf(pFile, "%f", &m_apAnimEffect[nCntEffect]->col.r);
						(void)fscanf(pFile, "%f", &m_apAnimEffect[nCntEffect]->col.g);
						(void)fscanf(pFile, "%f", &m_apAnimEffect[nCntEffect]->col.b);
						(void)fscanf(pFile, "%f", &m_apAnimEffect[nCntEffect]->col.a);
					}

					if (strcmp(cTemp, "IS_ADD") == 0)
					{// ���Z�������邩�ǂ���
						int i;

						(void)fscanf(pFile, "%s", &cTemp[0]);

						(void)fscanf(pFile, "%d", &i);

						if (i == 1)
						{
							m_apAnimEffect[nCntEffect]->bAdd = true;
						}
						else
						{
							m_apAnimEffect[nCntEffect]->bAdd = false;
						}
					}

					if (strcmp(cTemp, "IS_LOOP") == 0)
					{// ���[�v���邩�ǂ���
						int i;

						(void)fscanf(pFile, "%s", &cTemp[0]);

						(void)fscanf(pFile, "%d", &i);

						if (i == 1)
						{
							m_apAnimEffect[nCntEffect]->bLoop = true;
						}
						else
						{
							m_apAnimEffect[nCntEffect]->bLoop = false;
						}
					}

					if (strcmp(cTemp, "IS_BILLBOARD") == 0)
					{// �r���{�[�h�ɂ��邩�ǂ���
						int i;

						(void)fscanf(pFile, "%s", &cTemp[0]);

						(void)fscanf(pFile, "%d", &i);

						if (i == 1)
						{
							m_apAnimEffect[nCntEffect]->bBillboard = true;
						}
						else
						{
							m_apAnimEffect[nCntEffect]->bBillboard = false;
						}
					}
				}
			}

			if (strcmp(cTemp, "END_SCRIPT") == 0)
			{// �I������
				break;
			}
		}

		// �t�@�C�������
		fclose(pFile);
	}
}

//=====================================================
// �I������
//=====================================================
void CAnimManager::Uninit(void)
{
	s_pAnimEffect3D = nullptr;

	for (int i = 0; i < TYPE_MAX; i++)
	{
		if (m_apAnimEffect[i] != nullptr)
		{
			delete m_apAnimEffect[i];
			m_apAnimEffect[i] = nullptr;
		}
	}

	Release();
}

//=====================================================
// �X�V����
//=====================================================
void CAnimManager::Update(void)
{

}

//=====================================================
// �`�揈��
//=====================================================
void CAnimManager::Draw(void)
{

}

//=====================================================
// ��������
//=====================================================
CAnimManager *CAnimManager::Create(void)
{
	if (s_pAnimEffect3D == nullptr)
	{
		s_pAnimEffect3D = new CAnimManager;

		if (s_pAnimEffect3D != nullptr)
		{
			// ����������
			s_pAnimEffect3D->Init();
		}
	}

	return s_pAnimEffect3D;
}

//=====================================================
// �A�j���[�V�����̐���
//=====================================================
CAnim3D *CAnimManager::CreateEffect(D3DXVECTOR3 pos, E_TYPE type)
{
	if ((type >= TYPE_MAX || type < 0 ) &&
		m_apAnimEffect[type] != nullptr)
	{// �G���[
		return nullptr;
	}

	CAnim3D *pAnim3D = nullptr;

	// �C���X�^���X����
	pAnim3D = CAnim3D::Create(pos, m_apAnimEffect[type]->nNumAnim, m_apAnimEffect[type]->nSpeedAnim, m_apAnimEffect[type]->bLoop);

	if (pAnim3D != nullptr)
	{
		// �F�̐ݒ�
		pAnim3D->SetColor(m_apAnimEffect[type]->col);

		pAnim3D->SetSize(m_apAnimEffect[type]->fSize, m_apAnimEffect[type]->fSize);
		pAnim3D->EnableAdd(m_apAnimEffect[type]->bAdd);

		CTexture *pTexture = CTexture::GetInstance();

		if (pTexture != nullptr)
		{// �e�N�X�`���̐ݒ�
			int nIdx = pTexture->Regist(&m_apAnimEffect[type]->acPath[0]);
			pAnim3D->SetIdxTexture(nIdx);

			if (m_apAnimEffect[type]->bBillboard)
			{
				pAnim3D->SetMode(CAnim3D::MODE_BILLBOARD);
			}

			pAnim3D->SetVtx();
		}
	}

	return pAnim3D;
}

namespace Anim3D
{
CAnim3D *CreateAnim(D3DXVECTOR3 pos, CAnimManager::E_TYPE type)
{
	CAnimManager *pManager = CAnimManager::GetInstance();

	if (pManager == nullptr)
		return nullptr;

	CAnim3D *pAnim = pManager->CreateEffect(pos, type);

	return pAnim;
}
}