//*****************************************************
//
// 3D�����̏���[number3D.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "number3D.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "polygon3D.h"

//*****************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************
LPDIRECT3DTEXTURE9 CNumber3D::m_pTexture = nullptr;	// �e�N�X�`���̃|�C���^

//=====================================================
// �R���X�g���N�^
//=====================================================
CNumber3D::CNumber3D(int nPriority) : CObject(nPriority)
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
CNumber3D::~CNumber3D()
{

}

//=====================================================
//	����������
//=====================================================
HRESULT CNumber3D::Init(void)
{
	// �e�N�X�`���ԍ��擾
	m_nIdxTexture = CTexture::GetInstance()->Regist("data\\TEXTURE\\UI\\number001.png");

	for (int nCount = 0; nCount < m_nNumPlace; nCount++)
	{
		if (m_apObject[nCount] != nullptr)
		{
			m_apObject[nCount]->SetSize(40.0f, 100.0f);
			m_apObject[nCount]->SetPosition(D3DXVECTOR3(GetPosition().x + 80.0f * nCount, GetPosition().y, GetPosition().z));
			m_apObject[nCount]->SetIdxTexture(m_nIdxTexture);
			m_apObject[nCount]->SetVtx();
			m_apObject[nCount]->EnableZtest(true);
			m_apObject[nCount]->SetMode(CPolygon3D::MODE_BILLBOARD);
		}
	}

	return S_OK;
}

//=====================================================
//	�I������
//=====================================================
void CNumber3D::Uninit(void)
{
	for (int nCount = 0; nCount < m_nNumPlace; nCount++)
	{
		m_apObject[nCount]->Uninit();
	}

	Release();
}

//=====================================================
//	�X�V����
//=====================================================
void CNumber3D::Update(void)
{
	SetValue(m_nValue, m_nNumPlace);
}

//=====================================================
//	�l�ݒ菈��
//=====================================================
void CNumber3D::SetValue(int nValue,int nNumPlace)
{
	// �ϐ��錾
	int aTexU[MAX_PLACE];
	int aData1, aData2;

	m_nNumPlace = nNumPlace;

	m_nValue = nValue;

	for (int nCount = 0; nCount < m_nNumPlace; nCount++)
	{
		aData1 = (int)pow(10.0f, m_nNumPlace - nCount);
		aData2 = (int)pow(10.0f, m_nNumPlace - nCount - 1);

		aTexU[nCount] = m_nValue % aData1 / aData2;

		D3DXVECTOR2 leftUp = { 0.0f + 0.1f * aTexU[nCount], 0.0f };
		D3DXVECTOR2 rightDown = { 0.1f + 0.1f * aTexU[nCount], 1.0f };

		if (m_apObject[nCount] != nullptr)
		{
			m_apObject[nCount]->SetVtx();

			m_apObject[nCount]->SetTex(leftUp, rightDown);
		}
	}
}

//=====================================================
// �ʒu�ݒ菈��
//=====================================================
void CNumber3D::SetPosition(D3DXVECTOR3 pos)
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
void CNumber3D::SetSizeAll(float width, float height)
{
	for (int nCount = 0; nCount < m_nNumPlace; nCount++)
	{
		if (m_apObject[nCount] != nullptr)
		{
			m_apObject[nCount]->SetSize(width,height);
			m_apObject[nCount]->SetPosition(D3DXVECTOR3(GetPosition().x + width * nCount * 2, GetPosition().y, GetPosition().z));
		}
	}
}

//=====================================================
// �F�ݒ菈��
//=====================================================
void CNumber3D::SetColor(D3DXCOLOR col)
{
	for (int nCount = 0; nCount < m_nNumPlace; nCount++)
	{
		if (m_apObject[nCount] != nullptr)
		{
			m_apObject[nCount]->SetColor(col);
		}
	}
}

//=====================================================
// ��������
//=====================================================
CNumber3D *CNumber3D::Create(int nNumPlace,int nValue)
{
	CNumber3D *pNumber = nullptr;

	if (pNumber == nullptr)
	{
		pNumber = new CNumber3D;

		if (pNumber != nullptr)
		{
			pNumber->m_nNumPlace = nNumPlace;

			pNumber->m_nValue = nValue;

			for (int nCnt = 0; nCnt < nNumPlace; nCnt++)
			{// �����p�̃C���X�^���X�𐶐�

				pNumber->m_apObject[nCnt] = nullptr;

				if (pNumber->m_apObject[nCnt] == nullptr)
				{
					pNumber->m_apObject[nCnt] = CPolygon3D::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

					pNumber->m_apObject[nCnt]->Init();
				}
			}

			// ����������
			pNumber->Init();

			pNumber->SetValue(nValue, nNumPlace);
		}
	}

	return pNumber;
}