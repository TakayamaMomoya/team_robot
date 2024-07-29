//*****************************************************
//
// �RD�A�j���[�V�����G�t�F�N�g����[animEffect3D.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "animEffect3D.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "anim3D.h"

//*****************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************
CAnimEffect3D *CAnimEffect3D::m_pAnimEffect3D = nullptr;	// ���g�̃|�C���^

//*****************************************************
// �}�N����`
//*****************************************************
#define FILE_PATH	"data\\TEXT\\animEffect3D.txt"	// �t�@�C���̃p�X

//=====================================================
// �R���X�g���N�^
//=====================================================
CAnimEffect3D::CAnimEffect3D(int nPriority) : CObject(nPriority)
{
	for (int i = 0; i < TYPE_MAX; i++)
	{
		m_apAnimEffect[i] = nullptr;
	}
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CAnimEffect3D::~CAnimEffect3D()
{

}

//=====================================================
// ����������
//=====================================================
HRESULT CAnimEffect3D::Init(void)
{
	// �Ǎ�
	Load();

	return S_OK;
}

//=====================================================
// �Ǎ�����
//=====================================================
void CAnimEffect3D::Load(void)
{
	// �ϐ��錾
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
					m_apAnimEffect[nCntEffect] = new SInfoAnimEffect;

					ZeroMemory(m_apAnimEffect[nCntEffect],sizeof(SInfoAnimEffect));
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
void CAnimEffect3D::Uninit(void)
{
	m_pAnimEffect3D = nullptr;

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
void CAnimEffect3D::Update(void)
{

}

//=====================================================
// �`�揈��
//=====================================================
void CAnimEffect3D::Draw(void)
{

}

//=====================================================
// ��������
//=====================================================
CAnimEffect3D *CAnimEffect3D::Create(void)
{
	if (m_pAnimEffect3D == nullptr)
	{
		m_pAnimEffect3D = new CAnimEffect3D;

		if (m_pAnimEffect3D != nullptr)
		{
			// ����������
			m_pAnimEffect3D->Init();
		}
	}

	return m_pAnimEffect3D;
}

//=====================================================
// �A�j���[�V�����̐���
//=====================================================
CAnim3D *CAnimEffect3D::CreateEffect(D3DXVECTOR3 pos, TYPE type)
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
CAnim3D *CreateAnim(D3DXVECTOR3 pos, CAnimEffect3D::TYPE type)
{
	CAnimEffect3D *pManager = CAnimEffect3D::GetInstance();

	if (pManager == nullptr)
		return nullptr;

	CAnim3D *pAnim = pManager->CreateEffect(pos, type);

	return pAnim;
}
}