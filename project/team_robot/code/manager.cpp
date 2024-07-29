//*****************************************************
//
// �}�l�[�W���[����[manager.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "manager.h"
#include "renderer.h"
#include "polygon2D.h"
#include "inputkeyboard.h"
#include "inputmouse.h"
#include "debugproc.h"
#include "sound.h"
#include "object.h"
#include "camera.h"
#include "light.h"
#include "polygon3D.h"
#include "objectX.h"
#include "model.h"
#include "texture.h"
#include "particle.h"
#include "fade.h"
#include "inputManager.h"
#include "block.h"
#include "debrisSpawner.h"
#include "MyEffekseer.h"
#include "physics.h"

//*****************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************
CCamera *CManager::m_pCamera = nullptr;	// �J�����̃|�C���^
CLight *CManager::m_pLight = nullptr;	// ���C�g�̃|�C���^
CEffekseer* CManager::m_pMyEffekseer = nullptr;  // �G�t�F�N�V�A�̃|�C���^
CPhysics* CManager::m_pPhysics = nullptr;        // �����V�~�����[�V�����̃|�C���^
CScene *CManager::m_pScene = nullptr;	// �V�[���ւ̃|�C���^
CScene::MODE CManager::m_mode = CScene::MODE_GAME;	// ���݂̃��[�h
int CManager::m_nScore = 0;	// �X�R�A�ۑ��p
float CManager::m_fDeltaTime = 0.0f;	// �O��̃t���[������o�߂�������
CManager *CManager::m_pManager = nullptr;	// ���g�̃|�C���^

//=====================================================
// �R���X�g���N�^
//=====================================================
CManager::CManager()
{
	m_pScene = nullptr;
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CManager::~CManager()
{

}

//=====================================================
// ����������
//=====================================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	m_pManager = this;

	// �����_���[�̐���
	CRenderer::Create(hWnd, bWindow);

	// ���̓}�l�[�W���[����
	CInputManager::Create(hInstance, hWnd);

	// �f�o�b�O�\���̐���
	CDebugProc::Create();

	// �T�E���h����
	CSound::Create(hWnd);

	if (m_pCamera == nullptr)
	{// �J��������
		m_pCamera = new CCamera;

		if (m_pCamera != nullptr)
		{
			// ������
			if (FAILED(m_pCamera->Init()))
			{// �������Ɏ��s�����ꍇ
				return E_FAIL;
			}
		}
	}

	if (m_pLight == nullptr)
	{// ���C�g����
		m_pLight = new CLight;

		if (m_pLight != nullptr)
		{
			// ������
			if (FAILED(m_pLight->Init()))
			{// �������Ɏ��s�����ꍇ
				return E_FAIL;
			}
		}
	}

	if (m_pMyEffekseer == nullptr)
	{// �G�t�F�N�V�A����

		m_pMyEffekseer = new CEffekseer;

		// ������
		m_pMyEffekseer->Init();
	}

	if (m_pPhysics == nullptr)
	{
		m_pPhysics = new CPhysics;

		// ������
		m_pPhysics->Init();
	}

	// �e�N�X�`���Ǘ��̐���
	CTexture::Create();

	// �t�F�[�h�̐���
	CFade::Create();

	// �p�[�e�B�N���̓Ǎ�
	CParticle::Load();

	// �j�ЃX�|�i�[�̓Ǎ�
	CDebrisSpawner::Load();

	SetMode(m_mode);

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CManager::Uninit(void)
{
	m_pManager = nullptr;

	if (m_pScene != nullptr)
	{
		m_pScene->Uninit();
	}

	// �t�F�[�h�I��
	CFade *pFade = CFade::GetInstance();

	if (pFade != nullptr)
	{
		pFade->Uninit();
	}

	// �p�[�e�B�N�����j��
	CParticle::Unload();

	// �j�ЃX�|�i�[�j��
	CDebrisSpawner::Unload();

	// �����_���[�I��
	CRenderer *pRenderer = CRenderer::GetInstance();

	if (pRenderer != nullptr)
	{
		pRenderer->Uninit();
	}

	// ���̓}�l�[�W���[�I��
	CInputManager *pInputManager = CInputManager::GetInstance();

	if (pInputManager != nullptr)
	{
		pInputManager->Uninit();
	}

	CSound *pSound = CSound::GetInstance();

	if (pSound != nullptr)
	{// �T�E���h�̏I���E�j��
		pSound->Uninit();
		pSound = nullptr;
	}

	// �f�o�b�O�v���V�[�W���̏I��
	CDebugProc *pDebugProc = CDebugProc::GetInstance();

	if (pDebugProc != nullptr)
	{
		pDebugProc->Uninit();
	}

	if (m_pLight != nullptr)
	{// ���C�g�̏I���E�j��
		m_pLight->Uninit();

		delete m_pLight;
		m_pLight = nullptr;
	}

	if (m_pMyEffekseer != nullptr)
	{// �G�t�F�N�V�A�̏I���E�j��
		m_pMyEffekseer->Uninit();

		delete m_pMyEffekseer;
		m_pMyEffekseer = nullptr;
	}

	if (m_pPhysics != nullptr)
	{// �����V�~�����[�V�����̏I���E�j��
		m_pPhysics->Uninit();

		delete m_pPhysics;
		m_pPhysics = nullptr;
	}

	if (m_pCamera != nullptr)
	{// �J�����̏I���E�j��
		m_pCamera->Uninit();

		delete m_pCamera;
		m_pCamera = nullptr;
	}

	// �e�N�X�`���̏I���E�j��
	CTexture *pTexture = CTexture::GetInstance();

	if (pTexture != nullptr)
	{
		pTexture->Unload();

		delete pTexture;
		pTexture = nullptr;
	}

	// ���f���j��
	CModel::Unload();
}

//=====================================================
// �X�V����
//=====================================================
void CManager::Update(void)
{
#ifdef _DEBUG
	ImGui_ImplDX9_NewFrame();
	ImGui::NewFrame();

	ImGui::SetNextWindowPos(ImVec2(500, 60), ImGuiCond_Appearing);

	ImGui::SetNextWindowSize(ImVec2(500, 500), ImGuiCond_Appearing);

	ImGui::Begin("Edit");
#endif

	// �t�F�[�h�X�V
	CFade *pFade = CFade::GetInstance();

	if (pFade != nullptr)
	{
		pFade->Update();
	}

	if (m_pScene != nullptr)
	{
		m_pScene->Update();
	}

	// ���̓}�l�[�W���[�X�V
	CInputManager *pInputManager = CInputManager::GetInstance();

	if (pInputManager != nullptr)
	{
		pInputManager->Update();
	}

	CSound *pSound = CSound::GetInstance();

	if (pSound != nullptr)
	{// �T�E���h�̍X�V
		pSound->Update();
	}

	// �f�o�b�O�v���V�[�W���̍X�V
	CDebugProc *pDebugProc = CDebugProc::GetInstance();

	if (pDebugProc != nullptr)
	{
		pDebugProc->Update();
	}

	if (m_pLight != nullptr)
	{
		// ���C�g�̍X�V
		m_pLight->Update();
	}

	if (CInputKeyboard::GetInstance() != nullptr)
	{
		if (CInputKeyboard::GetInstance()->GetTrigger(DIK_F3))
		{// �������Z�b�g
			CManager::SetMode(CScene::MODE_TITLE);
		}
	}

#ifdef _DEBUG
	ImGui::End();
#endif
}

//=====================================================
// �`�揈��
//=====================================================
void CManager::Draw(void)
{
	if (m_pScene != nullptr)
	{
		m_pScene->Draw();
	}

	// �����_���[�`��
	CRenderer *pRenderer = CRenderer::GetInstance();

	if (pRenderer != nullptr)
	{
		pRenderer->Draw();
	}
}

//=====================================================
// ���[�h�ݒ�
//=====================================================
void CManager::SetMode(CScene::MODE mode)
{
	CSound *pSound = CSound::GetInstance();

	if (pSound != nullptr)
	{// ���̒�~
		pSound->Stop();
	}

	if (m_pCamera != nullptr)
	{
		m_pCamera->Init();
	}

	// �V�[����j��
	if (m_pScene != nullptr)
	{
		m_pScene->Uninit();

		delete m_pScene;
		m_pScene = nullptr;
	}

	// �V�[������
	m_pScene = CScene::Create(mode);

	// ���[�h�ݒ�
	m_mode = mode;
}