//*****************************************************
//
// �����_���[�̏���[renderer.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "renderer.h"
#include "manager.h"
#include "debugproc.h"
#include "object.h"
#include "fade.h"
#include "block.h"
#include "blur.h"
#include "player.h"
#include "inputkeyboard.h"

//*****************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************
CRenderer *CRenderer::m_pRenderer = nullptr;	// ���g�̃|�C���^

//=====================================================
// �R���X�g���N�^
//=====================================================
CRenderer::CRenderer()
{
	// �ϐ��̃N���A
	m_pD3D = nullptr;
	m_pD3DDevice = nullptr;
	ZeroMemory(&m_fogInfo, sizeof(SInfoFog));
	m_bDispimGui = false;
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CRenderer::~CRenderer()
{

}

//=====================================================
// ��������
//=====================================================
CRenderer *CRenderer::Create(HWND hWnd, BOOL bWindow)
{
	if (m_pRenderer == nullptr)
	{
		m_pRenderer = new CRenderer;

		if (m_pRenderer != nullptr)
		{
			m_pRenderer->Init(hWnd, bWindow);
		}
	}

	return m_pRenderer;
}

//=====================================================
// ����������
//=====================================================
HRESULT CRenderer::Init(HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;						// �f�B�X�v���C���[�h�̐ݒ�
	D3DPRESENT_PARAMETERS d3dpp;						//�f�B�X�v���C���[�h�̐ݒ�

	// Direct3D�I�u�W�F�N�g����
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (m_pD3D == nullptr)
	{
		return E_FAIL;
	}
	// ���݂̃f�B�X�v���C���[�h���擾
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// �f�o�C�X�̃v���[���e�[�V�����p�����[�^�[�̐ݒ�
	ZeroMemory(&d3dpp, sizeof(d3dpp));								// �p�����[�^�[�̃[���N���A

	d3dpp.BackBufferWidth = SCREEN_WIDTH;							// �Q�[����ʂ̕�
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;							// ����
	d3dpp.BackBufferFormat = d3ddm.Format;							// �`��
	d3dpp.BackBufferCount = 1;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;						// �f���M���ɓ���
	d3dpp.EnableAutoDepthStencil = TRUE;							// �f�v�X�ƃX�e���V���o�b�t�@���쐬
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;					// �f�v�X�o�b�t�@��8bit�Ŏg��
	d3dpp.Windowed = bWindow;										// �E�B���h�E���[�h
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;		// ���t���b�V�����[�g
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;		// �C���^�[�o��

	if (!bWindow)
	{
		// �E�B���h�E�X�^�C�����擾
		LONG lStyle = GetWindowLong(hWnd, GWL_STYLE);

		// �E�B���h�E�X�^�C����WS_POPUP�ɕύX
		SetWindowLong(hWnd, GWL_STYLE, lStyle & ~WS_OVERLAPPEDWINDOW | WS_POPUP);

		// �E�B���h�E�̈ʒu�ƃT�C�Y����ʑS�̂ɐݒ�
		SetWindowPos
		(
			hWnd,
			HWND_TOP,
			0, 0, // x, y ���W
			GetSystemMetrics(SCREEN_WIDTH), // ��ʂ̕�
			GetSystemMetrics(SCREEN_HEIGHT), // ��ʂ̍���
			SWP_FRAMECHANGED | SWP_SHOWWINDOW
		);
	}

	// Direct3D�f�o�C�X�̐���
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&m_pD3DDevice)))
	{
		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&m_pD3DDevice)))
		{
			if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&m_pD3DDevice)))
			{
				return E_FAIL;
			}
		}
	}
	
	// �����_�[�X�e�[�g�̐ݒ�
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//�T���v�X�e�[�g�̐ݒ�
	m_pD3DDevice->SetSamplerState(0,
		D3DSAMP_MIPFILTER,
		D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0,
		D3DSAMP_MINFILTER,
		D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0,
		D3DSAMP_MAGFILTER,
		D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0,
		D3DSAMP_ADDRESSU,
		D3DTADDRESS_WRAP);
	m_pD3DDevice->SetSamplerState(0,
		D3DSAMP_ADDRESSV,
		D3DTADDRESS_WRAP);

	//�e�N�X�`���X�e�[�W�X�e�[�g�̐ݒ�
	m_pD3DDevice->SetTextureStageState(0,
		D3DTSS_ALPHAOP,
		D3DTOP_MODULATE);
	m_pD3DDevice->SetTextureStageState(0,
		D3DTSS_ALPHAARG1,
		D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0,
		D3DTSS_ALPHAARG2,
		D3DTA_CURRENT);

	// �I�u�W�F�N�g�̃����[�X����
	CObject::ReleaseAll();

	// �u���[�̐���
	CBlur::Create();

#ifdef _DEBUG
	m_bDispimGui = true;
#else
	m_bDispimGui = false;
#endif

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CRenderer::Uninit(void)
{
	// �u���[�̔j��
	CBlur *pBlur = CBlur::GetInstance();

	if (pBlur != nullptr)
	{
		pBlur->Uninit();
		delete pBlur;
	}

	// �I�u�W�F�N�g�̃����[�X
	CObject::ReleaseAll();

	if (m_pD3DDevice != nullptr)
	{// Direct3D�f�o�C�X�j��
		m_pD3DDevice->Release();
		m_pD3DDevice = nullptr;
	}

	if (m_pD3D != nullptr)
	{// Direct3D�I�u�W�F�N�g�̔j��
		m_pD3D->Release();
		m_pD3D = nullptr;
	}

	delete this;
}

//=====================================================
// �X�V����
//=====================================================
void CRenderer::Update(void)
{
	// �I�u�W�F�N�g�̍X�V
	CObject::UpdateAll();

#ifdef _DEBUG
	CInputKeyboard *pKeyboard = CInputKeyboard::GetInstance();

	if (pKeyboard != nullptr)
	{
		if (pKeyboard->GetTrigger(DIK_F4))
		{
			m_bDispimGui = m_bDispimGui ? false : true;
		}
	}
#endif
}

//=====================================================
// �`�揈��
//=====================================================
void CRenderer::Draw(void)
{
	// �t�F�[�h�擾
	CFade *pFade = CFade::GetInstance();

	// ��ʃN���A
	m_pD3DDevice->Clear(0, nullptr,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	float fStart = 5000;
	float fEnd = 20000;

	// �t�H�O��L����
	//m_pD3DDevice->SetRenderState(D3DRS_FOGENABLE, m_fogInfo.bEnable);

	// �t�H�O�̐F��ݒ�
	m_pD3DDevice->SetRenderState(D3DRS_FOGCOLOR, D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));

	// �t�H�O�̏�Ԃ�ݒ�
	m_pD3DDevice->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_NONE);
	m_pD3DDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_LINEAR);

	// �t�H�O�̔����͈͂�ݒ�
	m_pD3DDevice->SetRenderState(D3DRS_FOGSTART, *(DWORD*)(&fStart));
	m_pD3DDevice->SetRenderState(D3DRS_FOGEND, *(DWORD*)(&fEnd));

	// �`��J�n
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{// �`�揈��
		// FPS�\��
		DrawFPS();
		
		// �I�u�W�F�N�g�̕`��
		CObject::DrawAll();

		if (pFade != nullptr)
		{// �t�F�[�h�`��
			pFade->Draw();
		}

		CDebugProc::GetInstance()->Draw();

		CBlur *pBlur = CBlur::GetInstance();

#ifdef _DEBUG
		// imgui�`��
		ImGui::Render();

		if(m_bDispimGui)
			ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
#endif

		// �`��I��
		m_pD3DDevice->EndScene();
	}

	// �o�b�N�E�t�����g�o�b�t�@�����ւ���
	m_pD3DDevice->Present(nullptr, nullptr, nullptr, nullptr);
}

//==================================================================================================
//�f�o�b�O�\������
//==================================================================================================
void CRenderer::DrawFPS(void)
{
	//������ɑ��
	CDebugProc::GetInstance()->Print("FPS:%d\n", GetFPS());
	CDebugProc::GetInstance()->Print("�I�u�W�F�N�g����:[%d]\n", CObject::GetNumAll());
}

namespace Renderer
{
LPDIRECT3DDEVICE9 GetDevice(void)
{
	CRenderer *pRenderer = CRenderer::GetInstance();
	LPDIRECT3DDEVICE9 pDevice = nullptr;

	if (pRenderer != nullptr)
	{
		pDevice = pRenderer->GetDevice();
	}
	else
	{
		assert(("�f�o�C�X�̎擾�Ɏ��s", false));
	}

	return pDevice;
}
}