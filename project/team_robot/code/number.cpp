//*****************************************************
//
// �����̏���[number.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "number.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "UI.h"

//*****************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************
LPDIRECT3DTEXTURE9 CNumber::m_pTexture = nullptr;	// �e�N�X�`���̃|�C���^

//=====================================================
// �R���X�g���N�^
//=====================================================
CNumber::CNumber(int nPriority) : CObject(nPriority)
{
	m_nValue = 0;
	m_nNumPlace = 0;
	m_pos = { 0.0f,0.0f,0.0f };
	m_nNumAll = 0;
	m_nIdxTexture = -1;
	ZeroMemory(&m_apObject[0],sizeof(m_apObject));
}

//=====================================================
//	�f�X�g���N�^
//=====================================================
CNumber::~CNumber()
{

}

//=====================================================
// ����������
//=====================================================
HRESULT CNumber::Init(void)
{
	for (int nCount = 0; nCount < m_nNumPlace; nCount++)
	{
		if (m_apObject[nCount] != nullptr)
		{
			m_apObject[nCount]->SetSize(40.0f, 100.0f);
			m_apObject[nCount]->SetPosition(D3DXVECTOR3(GetPosition().x + 80.0f * nCount, GetPosition().y, GetPosition().z));
		}
	}

	// �e�N�X�`���ԍ��擾
	m_nIdxTexture = CTexture::GetInstance()->Regist("data\\TEXTURE\\UI\\number01.png");

	for (int nCnt = 0; nCnt < m_nNumPlace; nCnt++)
	{// �e�N�X�`���ԍ��擾
		if (m_apObject[nCnt] != nullptr)
		{
			m_apObject[nCnt]->SetIdxTexture(m_nIdxTexture);
		}
	}

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CNumber::Uninit(void)
{
	for (int nCount = 0; nCount < MAX_PLACE; nCount++)
	{
		if (m_apObject[nCount] != nullptr)
		{
			m_apObject[nCount]->Uninit();
			m_apObject[nCount] = nullptr;
		}
	}

	Release();
}

//=====================================================
// �X�V����
//=====================================================
void CNumber::Update(void)
{

}

//=====================================================
// �l�ݒ菈��
//=====================================================
void CNumber::SetValue(int nValue,int nNumPlace)
{
	// �ϐ��錾
	int aTexU[MAX_PLACE];
	int aData1, aData2;

	m_nNumPlace = nNumPlace;

	m_nValue = nValue;

	for (int nCount = 0; nCount < m_nNumPlace; nCount++)
	{
		if (m_apObject[nCount] != nullptr)
		{
			aData1 = (int)pow(10.0f, m_nNumPlace - nCount);
			aData2 = (int)pow(10.0f, m_nNumPlace - nCount - 1);

			aTexU[nCount] = m_nValue % aData1 / aData2;

			D3DXVECTOR2 leftUp = { 0.0f + 0.1f * aTexU[nCount], 0.0f };
			D3DXVECTOR2 rightDown = { 0.1f + 0.1f * aTexU[nCount], 1.0f };


			m_apObject[nCount]->SetVtx();

			m_apObject[nCount]->SetTex(leftUp, rightDown);
		}
	}
}

//=====================================================
// �ʒu�ݒ菈��
//=====================================================
void CNumber::SetPosition(D3DXVECTOR3 pos)
{
	m_pos = pos;

	for (int nCount = 0; nCount < m_nNumPlace; nCount++)
	{
		if (m_apObject[nCount] != nullptr)
		{
			m_apObject[nCount]->SetPosition(D3DXVECTOR3(pos.x + m_apObject[nCount]->GetWidth() * nCount * 2, pos.y, pos.z));
			m_apObject[nCount]->SetVtx();
		}
	}
}

//=====================================================
// �T�C�Y�ݒ菈��
//=====================================================
void CNumber::SetSizeAll(float width, float height)
{
	for (int nCount = 0; nCount < m_nNumPlace; nCount++)
	{
		if (m_apObject[nCount] != nullptr)
		{
			m_apObject[nCount]->SetSize(width,height);
			m_apObject[nCount]->SetPosition(D3DXVECTOR3(GetPosition().x + width * nCount * 2, GetPosition().y, GetPosition().z));
			m_apObject[nCount]->SetVtx();
		}
	}
}

//=====================================================
// �e�N�X�`���ݒ菈��
//=====================================================
void CNumber::SetTex(const char* pFileName)
{
	for (int nCnt = 0; nCnt < m_nNumPlace; nCnt++)
	{// �e�N�X�`���ԍ��擾

		if (m_apObject[nCnt] != nullptr)
		{
			CTexture* pTexture = CTexture::GetInstance();

			if (pTexture != nullptr)
			{
				m_nIdxTexture = pTexture->Regist(pFileName);

				m_apObject[nCnt]->SetIdxTexture(m_nIdxTexture);
				m_apObject[nCnt]->SetVtx();
			}
		}
	}


}
//=====================================================
// �F�ݒ菈��
//=====================================================
void CNumber::SetColor(D3DXCOLOR col)
{
	for (int nCount = 0; nCount < m_nNumPlace; nCount++)
	{
		if (m_apObject[nCount] != nullptr)
		{
			m_apObject[nCount]->SetCol(col);
		}
	}
}

//=====================================================
// �F�擾����
//=====================================================
D3DXCOLOR CNumber::GetColor(void)
{
	if (m_apObject[0] != nullptr)
	{
		return m_apObject[0]->GetCol();
	}

	return D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
}

//=====================================================
// ��������
//=====================================================
CNumber *CNumber::Create(int nNumPlace, int nValue)
{
	CNumber *pNumber = nullptr;

	pNumber = new CNumber;

	if (pNumber != nullptr)
	{
		pNumber->m_nNumPlace = nNumPlace;

		pNumber->m_nValue = nValue;

		for (int nCnt = 0; nCnt < nNumPlace; nCnt++)
		{// �����p�̃C���X�^���X�𐶐�
			pNumber->m_apObject[nCnt] = nullptr;

			if (pNumber->m_apObject[nCnt] == nullptr)
			{
				pNumber->m_apObject[nCnt] = CUI::Create();
			}
		}

		// ����������
		pNumber->Init();

		pNumber->SetValue(nValue, nNumPlace);
	}

	return pNumber;
}