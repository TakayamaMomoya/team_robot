//*****************************************************
//
// �X�J�C�{�b�N�X�̏���[skybox.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "skybox.h"
#include "manager.h"
#include "renderer.h"
#include "player.h"
#include "texture.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const float CHANGE_TEX_LINE = 100000.0f;	// �e�N�X�`����ς��郉�C��
}

//*****************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************
CSkybox *CSkybox::m_pSkybox = nullptr;	// ���g�̃|�C���^

//=====================================================
// �R���X�g���N�^
//=====================================================
CSkybox::CSkybox(int nPriority) : CObjectX(nPriority)
{

}

//=====================================================
// �f�X�g���N�^
//=====================================================
CSkybox::~CSkybox()
{

}

//=====================================================
// ����������
//=====================================================
HRESULT CSkybox::Init(void)
{
	// �p���N���X�̏�����
	CObjectX::Init();

	// ���f���̓Ǎ�
	int nIdx = CModel::Load("data\\MODEL\\skybox\\sky00.x");
	BindModel(nIdx);

	EnableLighting(true);
	EnableFog(false);

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CSkybox::Uninit(void)
{
	// �p���N���X�̏I��
	CObjectX::Uninit();
}

//=====================================================
// �X�V����
//=====================================================
void CSkybox::Update(void)
{
	// �p���N���X�̍X�V
	CObjectX::Update();

	// �v���C���[�̒Ǐ]
	FollowPlayer();

	// �e�N�X�`���ύX�̌��m
	DetectionChangeTexture();
}

//=====================================================
// �v���C���[�̒Ǐ]
//=====================================================
void CSkybox::FollowPlayer(void)
{
	CPlayer *pPlayer = CPlayer::GetInstance();

	if (pPlayer == nullptr)
		return;

	SetPosition(pPlayer->GetPosition());
}

//=====================================================
// �e�N�X�`���ύX�̌��m
//=====================================================
void CSkybox::DetectionChangeTexture(void)
{
	CPlayer *pPlayer = CPlayer::GetInstance();

	if (pPlayer == nullptr)
		return;

	D3DXVECTOR3 posPlayer = pPlayer->GetPosition();
	D3DXVECTOR3 movePlayer = pPlayer->GetMove();
	D3DXVECTOR3 posPlayerNext = posPlayer + movePlayer;

	if (posPlayer.x < CHANGE_TEX_LINE && posPlayerNext.x >= CHANGE_TEX_LINE)
	{
		// �e�N�X�`���𓻂ɂ���
		CModel::Model *pModel = CModel::GetModel(GetIdxModel());

		if (pModel->pIdxTexture == nullptr)
			return;

		int nIdx = Texture::GetIdx("data\\TEXTURE\\BG\\sky01.jpg");

		*pModel->pIdxTexture = nIdx;
	}
}

//=====================================================
// �`�揈��
//=====================================================
void CSkybox::Draw(void)
{
	// �f�o�C�X�̎擾
	CRenderer *pRenderer = CRenderer::GetInstance();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// ���C�e�B���O�𖳌���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// �p���N���X�̕`��
	CObjectX::Draw();

	// ���C�e�B���O��L����
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//=====================================================
// ��������
//=====================================================
CSkybox *CSkybox::Create()
{
	CSkybox *pSkybox = nullptr;

	if (pSkybox == nullptr)
	{
		pSkybox = new CSkybox;

		if (pSkybox != nullptr)
		{
			// ������
			pSkybox->Init();
		}
	}

	return pSkybox;
}