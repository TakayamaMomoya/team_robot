//*****************************************************
//
// �u���[�̏���[blur.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "blur.h"
#include "renderer.h"
#include "manager.h"
#include "camera.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const D3DXCOLOR COL_POLYGON_INITIAL = { 1.0f,1.0f,1.0f,0.1f };  // �����̏d�˂�|���S���̐F
}

//*****************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************
CBlur *CBlur::m_pBlur = nullptr;    // ���g�̃|�C���^

//=====================================================
// �R���X�g���N�^
//=====================================================
CBlur::CBlur() : m_colPolygon{}, m_apRenderMT{}, m_apTextureMT{}, m_viewpotrMT(), m_pZBuffMT(nullptr), m_pVtxBuffMT(nullptr), m_fAddSizePolygon(0.0f),
                    m_pRenderDef(nullptr), m_pZBuffDef(nullptr),m_viewportDef(),m_mtxProjDef(),m_mtxViewDef()
{

}

//=====================================================
// �f�X�g���N�^
//=====================================================
CBlur::~CBlur()
{

}

//=====================================================
// ��������
//=====================================================
CBlur *CBlur::Create(void)
{
    if (m_pBlur == nullptr)
    {
        m_pBlur = new CBlur;

        if (m_pBlur != nullptr)
        {
            m_pBlur->Init();
        }
    }

    return m_pBlur;
}

//=====================================================
// ������
//=====================================================
void CBlur::Init(void)
{
    m_colPolygon = COL_POLYGON_INITIAL;

    // �f�o�C�X�̎擾
    LPDIRECT3DDEVICE9 pDevice = Renderer::GetDevice();

    for (int i = 0; i < Blur::NUM_RENDER - 1; i++)
    {
        // �����_�[�^�[�Q�b�g�p�̃e�N�X�`������
        if (SUCCEEDED(pDevice->CreateTexture(SCREEN_WIDTH,
            SCREEN_HEIGHT,
            1,
            D3DUSAGE_RENDERTARGET,
            D3DFMT_X8R8G8B8,
            D3DPOOL_DEFAULT,
            &m_apTextureMT[i],
            nullptr)))
        {
            // �^�[�Q�b�g�����_�����O�p�C���^�[�t�F�C�X����
            if (FAILED(m_apTextureMT[i]->GetSurfaceLevel(0, &m_apRenderMT[i])))
            {
                assert(("�^�[�Q�b�g�����_�����O�p�C���^�[�t�F�[�X�����Ɏ��s", false));
            }
        }
        else
        {
            assert(("�����_�[�^�[�Q�b�g�p�̃e�N�X�`�������Ɏ��s", false));
        }
    }

    // �����_�[�^�[�Q�b�g�p�̃e�N�X�`������
    if (SUCCEEDED(pDevice->CreateTexture(SCREEN_WIDTH,
        SCREEN_HEIGHT,
        1,
        D3DUSAGE_RENDERTARGET,
        D3DFMT_A8R8G8B8,
        D3DPOOL_DEFAULT,
        &m_apTextureMT[Blur::NUM_RENDER - 1],
        nullptr)))
    {
        // �^�[�Q�b�g�����_�����O�p�C���^�[�t�F�C�X����
        if (FAILED(m_apTextureMT[Blur::NUM_RENDER - 1]->GetSurfaceLevel(0, &m_apRenderMT[Blur::NUM_RENDER - 1])))
        {
            assert(("�^�[�Q�b�g�����_�����O�p�C���^�[�t�F�[�X�����Ɏ��s", false));
        }
    }
    else
    {
        assert(("�����_�[�^�[�Q�b�g�p�̃e�N�X�`�������Ɏ��s", false));
    }

    // �^�[�Q�b�g�����_�����O�pZ�o�b�t�@����
    if (FAILED(pDevice->CreateDepthStencilSurface(SCREEN_WIDTH,
        SCREEN_HEIGHT,
        D3DFMT_D16,
        D3DMULTISAMPLE_NONE,
        0,
        TRUE,
        &m_pZBuffMT,
        nullptr)))
    {
        assert(("�^�[�Q�b�g�����_�����O�pZ�o�b�t�@�����Ɏ��s", false));
    }

    // �����_�����O�^�[�Q�b�g��Z�o�b�t�@��ۑ�
    LPDIRECT3DSURFACE9 pRenderDef, pZBuffDef;

    pDevice->GetRenderTarget(0, &pRenderDef);
    pDevice->GetDepthStencilSurface(&pZBuffDef);

    // Z�o�b�t�@�̐ݒ�
    pDevice->SetDepthStencilSurface(m_pZBuffMT);

    for (int i = 0; i < Blur::NUM_RENDER; i++)
    {
        // �����_�����O�^�[�Q�b�g��ݒ�
        pDevice->SetRenderTarget(0, m_apRenderMT[i]);

        // �N���A����
        pDevice->Clear(0, nullptr,
            (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
            D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);
    }

    // �����_�����O�^�[�Q�b�g��Z�o�b�t�@�����ɖ߂�
    pDevice->SetRenderTarget(0, pRenderDef);
    pDevice->SetDepthStencilSurface(pZBuffDef);

    // �r���[�|�[�g������
    m_viewpotrMT.X = 0;
    m_viewpotrMT.Y = 0;
    m_viewpotrMT.Width = SCREEN_WIDTH;
    m_viewpotrMT.Height = SCREEN_HEIGHT;
    m_viewpotrMT.MinZ = 0.0f;
    m_viewpotrMT.MaxZ = 1.0f;

    // �|���S���̐���
    CreatePolygon();
}

//=====================================================
// �I��
//=====================================================
void CBlur::Uninit(void)
{
    m_pBlur = nullptr;

    for (int i = 0; i < Blur::NUM_RENDER; i++)
    {
        if (m_apRenderMT[i] != nullptr)
        {
            m_apRenderMT[i]->Release();
            m_apRenderMT[i] = nullptr;
        }

        if (m_apTextureMT[i] != nullptr)
        {
            m_apTextureMT[i]->Release();
            m_apTextureMT[i] = nullptr;
        }
    }

    if (m_pZBuffMT != nullptr)
    {
        m_pZBuffMT->Release();
        m_pZBuffMT = nullptr;
    }

    if (m_pVtxBuffMT != nullptr)
    {
        m_pVtxBuffMT->Release();
        m_pVtxBuffMT = nullptr;
    }

    if (m_pRenderDef != nullptr)
    {
        m_pRenderDef->Release();
        m_pRenderDef = nullptr;
    }

    if (m_pZBuffDef != nullptr)
    {
        m_pZBuffDef->Release();
        m_pZBuffDef = nullptr;
    }
}

//=====================================================
// �|���S���̐���
//=====================================================
void CBlur::CreatePolygon(void)
{
    // �f�o�C�X�̎擾
    LPDIRECT3DDEVICE9 pDevice = Renderer::GetDevice();

    // ���_�o�b�t�@�̐���
    if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
        D3DUSAGE_WRITEONLY,
        FVF_VERTEX_3D,
        D3DPOOL_MANAGED,
        &m_pVtxBuffMT,
        NULL)))
    {
        assert(("���_�o�b�t�@�̐����Ɏ��s", false));
    }

    // ���_���̃|�C���^
    VERTEX_2D *pVtx;
    
    // ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
    m_pVtxBuffMT->Lock(0, 0, (void**)&pVtx, 0);

    // ���_���W�̐ݒ�
    pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
    pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
    pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

    // �@���x�N�g���̐ݒ�
    pVtx[0].rhw = 1.0f;
    pVtx[1].rhw = 1.0f;
    pVtx[2].rhw = 1.0f;
    pVtx[3].rhw = 1.0f;

    // ���_�J���[�̐ݒ�
    pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
    pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
    pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
    pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

    // �e�N�X�`�����W
    pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
    pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
    pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
    pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

    // ���_�o�b�t�@���A�����b�N
    m_pVtxBuffMT->Unlock();
}

//=====================================================
// �`��ɕK�v�ȏ��̕ۑ�
//=====================================================
void CBlur::SaveRenderInfo(void)
{
    // �f�o�C�X�̎擾
    LPDIRECT3DDEVICE9 pDevice = Renderer::GetDevice();

    // �������ۑ�
    pDevice->GetRenderTarget(0, &m_pRenderDef);
    pDevice->GetDepthStencilSurface(&m_pZBuffDef);
    pDevice->GetViewport(&m_viewportDef);
    pDevice->GetTransform(D3DTS_VIEW, &m_mtxViewDef);
    pDevice->GetTransform(D3DTS_PROJECTION, &m_mtxProjDef);
}

//=====================================================
// �����_�[�^�[�Q�b�g�̕ύX
//=====================================================
void CBlur::ChangeTarget(void)
{
    CCamera *pCamera = CManager::GetCamera();

    if (pCamera == nullptr)
        return;

    // �f�o�C�X�̎擾
    LPDIRECT3DDEVICE9 pDevice = Renderer::GetDevice();

    D3DXMATRIX mtxView, mtxProj;
    float fAspect;

    // �����_�����O�^�[�Q�b�g��Z�o�b�t�@��ݒ�
    pDevice->SetRenderTarget(0, m_apRenderMT[0]);
    pDevice->SetDepthStencilSurface(m_pZBuffMT);

    // �e�N�X�`���p�r���[�|�[�g�ɐݒ�
    pDevice->SetViewport(&m_viewpotrMT);

    //�v���W�F�N�V�����}�g���b�N�X������
    D3DXMatrixIdentity(&mtxProj);

    fAspect = (float)m_viewpotrMT.Width / (float)m_viewpotrMT.Height;

    //�v���W�F�N�V�����}�g���b�N�X�쐬
    D3DXMatrixPerspectiveFovLH(&mtxProj,
        D3DXToRadian(45.0f),
        fAspect,
        MIN_DRAW,
        MAX_DRAW);

    //�}�g���b�N�X�ݒ�
    pDevice->SetTransform(D3DTS_PROJECTION, &mtxProj);

    //�r���[�}�g���b�N�X������
    D3DXMatrixIdentity(&mtxView);

    CCamera::Camera *pInfoCamera = pCamera->GetCamera();

    //�r���[�}�g���b�N�X�쐬
    D3DXMatrixLookAtLH(&mtxView,
        &pInfoCamera->posV,
        &pInfoCamera->posR,
        &pInfoCamera->vecU);

    //�r���[�}�g���b�N�X�ݒ�
    pDevice->SetTransform(D3DTS_VIEW, &pInfoCamera->mtxView);
}

//=====================================================
// �O��̃e�N�X�`�����d�˂�
//=====================================================
void CBlur::OverlapLastTexture(void)
{
    // �f�o�C�X�̎擾
    LPDIRECT3DDEVICE9 pDevice = Renderer::GetDevice();

    // ���_���̃|�C���^
    VERTEX_2D *pVtx;

    // ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
    m_pVtxBuffMT->Lock(0, 0, (void**)&pVtx, 0);

    // ���_���W�̐ݒ�
    pVtx[0].pos = D3DXVECTOR3(m_fAddSizePolygon, m_fAddSizePolygon, 0.0f);
    pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH - m_fAddSizePolygon, m_fAddSizePolygon, 0.0f);
    pVtx[2].pos = D3DXVECTOR3(m_fAddSizePolygon, SCREEN_HEIGHT - m_fAddSizePolygon, 0.0f);
    pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH - m_fAddSizePolygon, SCREEN_HEIGHT - m_fAddSizePolygon, 0.0f);

    // ���_�J���[�̐ݒ�
    pVtx[0].col = m_colPolygon;
    pVtx[1].col = m_colPolygon;
    pVtx[2].col = m_colPolygon;
    pVtx[3].col = m_colPolygon;

    // ���_�o�b�t�@���A�����b�N
    m_pVtxBuffMT->Unlock();

    // ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
    pDevice->SetStreamSource(0, m_pVtxBuffMT, 0, sizeof(VERTEX_2D));

    // ���_�t�H�[�}�b�g�̐ݒ�
    pDevice->SetFVF(FVF_VERTEX_2D);

    LPDIRECT3DTEXTURE9 pTexture = m_apTextureMT[1];

    // �e�N�X�`���ݒ�
    pDevice->SetTexture(0, pTexture);

    // �`��
    pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//=====================================================
// �����_�[�^�[�Q�b�g�̕���
//=====================================================
void CBlur::RestoreTarget(void)
{
    // �f�o�C�X�̎擾
    LPDIRECT3DDEVICE9 pDevice = Renderer::GetDevice();

    pDevice->SetRenderTarget(0, m_pRenderDef);
    pDevice->SetDepthStencilSurface(m_pZBuffDef);
    pDevice->SetViewport(&m_viewportDef);
    //pDevice->SetTransform(D3DTS_VIEW, &m_mtxViewDef);
    //pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjDef);
}

//=====================================================
// �o�b�N�o�b�t�@�ւ̕`��
//=====================================================
void CBlur::DrawBuckBuffer(void)
{
    // �f�o�C�X�̎擾
    LPDIRECT3DDEVICE9 pDevice = Renderer::GetDevice();

    // ���_���̃|�C���^
    VERTEX_2D *pVtx;

    //���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
    m_pVtxBuffMT->Lock(0, 0, (void**)&pVtx, 0);

    //���_���W�̐ݒ�
    pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
    pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
    pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

    //���_�J���[�̐ݒ�
    pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

    //���_�o�b�t�@���A�����b�N
    m_pVtxBuffMT->Unlock();

    // ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
    pDevice->SetStreamSource(0, m_pVtxBuffMT, 0, sizeof(VERTEX_2D));

    // ���_�t�H�[�}�b�g�̐ݒ�
    pDevice->SetFVF(FVF_VERTEX_2D);

    LPDIRECT3DTEXTURE9 pTexture = m_apTextureMT[0];

    // �e�N�X�`���ݒ�
    pDevice->SetTexture(0, pTexture);

    // �`��
    pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

    // �e�N�X�`���ݒ�
    pDevice->SetTexture(0, m_apTextureMT[2]);

    // �`��
    pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//=====================================================
// �o�b�t�@�̓���ւ�
//=====================================================
void CBlur::SwapBuffer(void)
{
    // ����ւ��p�̃o�b�t�@
    LPDIRECT3DSURFACE9 pRednerWk;
    LPDIRECT3DTEXTURE9 pTextureWk;

    pRednerWk = m_apRenderMT[0];
    m_apRenderMT[0] = m_apRenderMT[1];
    m_apRenderMT[1] = pRednerWk;

    pTextureWk = m_apTextureMT[0];
    m_apTextureMT[0] = m_apTextureMT[1];
    m_apTextureMT[1] = pTextureWk;
}

//=====================================================
// �u���[���Ȃ������_�[�ւ̕ύX
//=====================================================
void CBlur::SetRenderToNotBlur(void)
{
    // �f�o�C�X�̎擾
    LPDIRECT3DDEVICE9 pDevice = Renderer::GetDevice();

    pDevice->SetRenderTarget(0, m_apRenderMT[2]);
}

//=====================================================
// �u���[���Ȃ������_�[�̃��Z�b�g
//=====================================================
void CBlur::ClearNotBlur(void)
{
    // �f�o�C�X�̎擾
    LPDIRECT3DDEVICE9 pDevice = Renderer::GetDevice();

    LPDIRECT3DSURFACE9 m_pRenderDef;

    pDevice->GetRenderTarget(0, &m_pRenderDef);

    pDevice->SetRenderTarget(0, m_apRenderMT[2]);

    // ��ʃN���A
    pDevice->Clear(0, nullptr,
        (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
        D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

    pDevice->SetRenderTarget(0, m_pRenderDef);
}

//=====================================================
// �u���[����^�[�Q�b�g�ɖ߂�
//=====================================================
void CBlur::RestoreTargetBlur(void)
{
    // �f�o�C�X�̎擾
    LPDIRECT3DDEVICE9 pDevice = Renderer::GetDevice();

    pDevice->SetRenderTarget(0, m_apRenderMT[0]);
}

namespace Blur
{
//=====================================================
// �p�����[�^�[�̐ݒ�
//=====================================================
void SetBlur(float fSize, float fDensity)
{
    CBlur *pBlur = CBlur::GetInstance();

    if (pBlur != nullptr)
    {
        pBlur->SetAddSizePolygon(fSize);
        pBlur->SetPolygonColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, fDensity));
    }
}

//=====================================================
// �p�����[�^�[�̃��Z�b�g
//=====================================================
void ResetBlur(void)
{
    CBlur *pBlur = CBlur::GetInstance();

    if (pBlur != nullptr)
    {
        pBlur->SetAddSizePolygon(0.0f);
        pBlur->SetPolygonColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
    }
}

//=====================================================
// �p�����[�^�[�̉��Z
//=====================================================
void AddParameter(float fAddSize, float fAddDensity, float fMaxSize, float fMinSize, float fMaxDesity, float fMinDensity)
{
    CBlur *pBlur = CBlur::GetInstance();

    if (pBlur != nullptr)
    {
        float fSize = pBlur->GetAddSizePolygon();
        D3DXCOLOR col = pBlur->GetPolygonColor();

        fSize += fAddSize;
        col.a += fAddDensity;

        universal::LimitValuefloat(&fSize, fMaxSize, fMinSize);
        universal::LimitValuefloat(&col.a, fMaxDesity, fMinDensity);

        pBlur->SetAddSizePolygon(fSize);
        pBlur->SetPolygonColor(col);
    }
}
}