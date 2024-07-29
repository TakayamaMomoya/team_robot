//===========================================================
//
// �}�C�G�t�F�N�V�A[MyEffekseer.cpp]
// Author �匴�叫
//
//===========================================================

#include <iostream>
#include <string>

#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "MyEffekseer.h"
#include "camera.h"
#include "debugproc.h"

// �G�t�F�N�g�̖��O
const char* CEffekseer::m_apEfkName[CEffekseer::TYPE_MAX] =
{
	"",                               // �Ȃ���Ȃ�
	"data\\EFFEKSEER\\Effect\\drift.efkefc",  // �h���t�g���̉ΉԂƉ�
	"data\\EFFEKSEER\\Effect\\spark.efkefc",  // �ǏՓˎ��̉Ή�
	"data\\EFFEKSEER\\Effect\\bikefire.efkefc", // �r�C���̉�
	"data\\EFFEKSEER\\Effect\\slash00.efkefc",	// �a��
	"data\\EFFEKSEER\\Effect\\parry.efkefc",	// �p���B
	"data\\EFFEKSEER\\Effect\\flash00.efkefc",	// �藠���̌�
	"data\\EFFEKSEER\\Effect\\shuriken.efkefc",	// �藠���̓���
	"data\\EFFEKSEER\\Effect\\WindShuriken.efkefc",	// �藠���̉�]��
	"data\\EFFEKSEER\\Effect\\grapple.efkefc",	// ���
	"data\\EFFEKSEER\\Effect\\smoke.efkefc",	// �G�P�ގ��̉�
	"data\\EFFEKSEER\\Effect\\gate00.efkefc",	// �Q�[�g
};

//===========================================================
// �R���X�g���N�^
//===========================================================
CEffekseer::CEffekseer()
{
	m_nNum = 0;
}

//===========================================================
// �f�X�g���N�^
//===========================================================
CEffekseer::~CEffekseer()
{

}

//===========================================================
// ����������
//===========================================================
void CEffekseer::Init(void)
{
	// �G�t�F�N�g�̃}�l�[�W���[�̍쐬
	m_efkManager = ::Effekseer::Manager::Create(8000);

	// Specify a position of view
	// ���_�ʒu���m��
	viewerPosition = ::Effekseer::Vector3D(0.0f, 0.0f, 0.0f);

	// ���W�n��ݒ肷��B�A�v���P�[�V�����ƈ�v������K�v������B
	m_efkManager->SetCoordinateSystem(Effekseer::CoordinateSystem::LH);

	// Setup effekseer modules
	// Effekseer�̃��W���[�����Z�b�g�A�b�v����
	SetupEffekseerModules(m_efkManager);

	// Specify a projection matrix
	// ���e�s���ݒ�
	projectionMatrix.PerspectiveFovLH(90.0f / 180.0f * 3.14f, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 1.0f, 500.0f);

	// Specify a camera matrix
		// �J�����s���ݒ�
	cameraMatrix.LookAtLH(viewerPosition, ::Effekseer::Vector3D(0.0f, 0.0f, 0.0f), ::Effekseer::Vector3D(0.0f, 1.0f, 0.0f));
}

//===========================================================
// �I������
//===========================================================
void CEffekseer::Uninit(void)
{
	for (auto it = m_listEffect.begin(); it != m_listEffect.end();)
	{
		auto itNext = std::next(it);

		if (itNext == m_listEffect.end())
			break;

		ReleaseEffect((*it));

		it = itNext;
	}

	m_nNum = 0;
}

//===========================================================
// �X�V����
//===========================================================
void CEffekseer::Update(void)
{
	for (auto it = m_listEffect.begin();it != m_listEffect.end();)
	{
		// �G�t�F�N�g�̈ړ�
		Effekseer::Handle handle = (*it)->GetHandle();
		Effekseer::Vector3D pos = (*it)->GetPosition();
		m_efkManager->SetLocation(handle, pos);

		// �G�t�F�N�g�̌������f
		Effekseer::Vector3D rot = (*it)->GetRotation();
		m_efkManager->SetRotation(handle, rot.X, rot.Y, rot.Z);

		// �G�t�F�N�g�̃X�P�[�����f
		Effekseer::Vector3D scale = (*it)->GetScale();
		m_efkManager->SetScale(handle, scale.X, scale.Y, scale.Z);

		// ���C���[�p�����[�^�̐ݒ�
		Effekseer::Manager::LayerParameter layerParameter;
		layerParameter.ViewerPosition = viewerPosition;
		m_efkManager->SetLayerParameter(0, layerParameter);

		// �}�l�[�W���[�̍X�V
		Effekseer::Manager::UpdateParameter updateParameter;
		m_efkManager->Update(updateParameter);

		// ���Ԃ��X�V����
		int32_t time = (*it)->GetTime();
		m_efkRenderer->SetTime((float)time / 60.0f);
		time++;
		(*it)->SetTime(time);

		auto itNext = std::next(it);

		if (itNext == m_listEffect.end())
			break;

		// ���t���[���A�G�t�F�N�g���Đ��I�����Ă��邩�m�F����
		if (m_efkManager->Exists(handle) == false)
		{
			(*it)->Uninit();

			// ���X�g���珜�O
			m_listEffect.remove((*it));
		}

		it = itNext;
	}
}

//===========================================================
// �`�揈��
//===========================================================
void CEffekseer::Draw(void)
{
	if (m_efkRenderer != nullptr)
	{
		// Specify a projection matrix
		// ���e�s���ݒ�
		m_efkRenderer->SetProjectionMatrix(projectionMatrix);

		// �J�����̏��擾
		CCamera* pCamera = CManager::GetCamera();

		// �r���[�}�g���b�N�X
		auto ViewMatrix = pCamera->GetCamera()->mtxView;

		// �v���W�F�N�V�����}�g���b�N�X
		auto Projection = pCamera->GetCamera()->mtxProjection;

		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				projectionMatrix.Values[i][j] = Projection.m[i][j];
				cameraMatrix.Values[i][j] = ViewMatrix.m[i][j];
			}
		}

		// Specify a camera matrix
		// �J�����s���ݒ�
		m_efkRenderer->SetCameraMatrix(cameraMatrix);

		// Begin to rendering effects
		// �G�t�F�N�g�̕`��J�n�������s���B
		m_efkRenderer->BeginRendering();

		// Render effects
		// �G�t�F�N�g�̕`����s���B
		Effekseer::Manager::DrawParameter drawParameter;
		drawParameter.ZNear = 0.0f;
		drawParameter.ZFar = 1.0f;
		drawParameter.ViewProjectionMatrix = m_efkRenderer->GetCameraProjectionMatrix();
		m_efkManager->Draw(drawParameter);

		// Finish to rendering effects
		// �G�t�F�N�g�̕`��I���������s���B
		m_efkRenderer->EndRendering();
	}
}

//===========================================================
// �G�t�F�N�g�̐���
//===========================================================
CEffekseerEffect *CEffekseer::CreateEffect(const char* FileName, ::Effekseer::Vector3D pos, ::Effekseer::Vector3D rot, ::Effekseer::Vector3D scale)
{
	CEffekseerEffect *pEffect = nullptr;

	pEffect = new CEffekseerEffect;

	if (pEffect == nullptr)
		return nullptr;

	// �G�t�F�N�g�̓Ǎ�
	// char16_t�ɕϊ�
	std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> converter;
	std::u16string string16t = converter.from_bytes(FileName);

	Effekseer::EffectRef effect = Effekseer::Effect::Create(m_efkManager, string16t.c_str());

	pEffect->SetEffect(effect);
	
	// Play an effect
	// �G�t�F�N�g�̍Đ�
	Effekseer::Handle handle = m_efkManager->Play(effect, 0, 0, 0);

	pEffect->SetHandle(handle);

	pEffect->Init(pos, rot, scale);

	m_listEffect.push_back(pEffect);

	return pEffect;
}

//===========================================================
// ���W���[���ݒ�
//===========================================================
void CEffekseer::SetupEffekseerModules(::Effekseer::ManagerRef efkManager)
{
		// Create a  graphics device
	// �`��f�o�C�X�̍쐬
	::Effekseer::Backend::GraphicsDeviceRef graphicsDevice;
	graphicsDevice = ::EffekseerRendererDX9::CreateGraphicsDevice(CRenderer::GetInstance()->GetDevice());

	// Create a renderer of effects
	// �G�t�F�N�g�̃����_���[�̍쐬
	m_efkRenderer = ::EffekseerRendererDX9::Renderer::Create(graphicsDevice, 8000);

	// Sprcify rendering modules
	// �`�惂�W���[���̐ݒ�
	efkManager->SetSpriteRenderer(m_efkRenderer->CreateSpriteRenderer());
	efkManager->SetRibbonRenderer(m_efkRenderer->CreateRibbonRenderer());
	efkManager->SetRingRenderer(m_efkRenderer->CreateRingRenderer());
	efkManager->SetTrackRenderer(m_efkRenderer->CreateTrackRenderer());
	efkManager->SetModelRenderer(m_efkRenderer->CreateModelRenderer());

	// Specify a texture, model, curve and material loader
	// It can be extended by yourself. It is loaded from a file on now.
	// �e�N�X�`���A���f���A�J�[�u�A�}�e���A�����[�_�[�̐ݒ肷��B
	// ���[�U�[���Ǝ��Ŋg���ł���B���݂̓t�@�C������ǂݍ���ł���B
	efkManager->SetTextureLoader(m_efkRenderer->CreateTextureLoader());
	efkManager->SetModelLoader(m_efkRenderer->CreateModelLoader());
	efkManager->SetMaterialLoader(m_efkRenderer->CreateMaterialLoader());
	efkManager->SetCurveLoader(Effekseer::MakeRefPtr<Effekseer::CurveLoader>());
}

//===========================================================
// �G�t�F�N�g�̃����[�X
//===========================================================
void CEffekseer::Release(int idx)
{

}

//===========================================================
// �G�t�F�N�g�̃����[�X
//===========================================================
void CEffekseer::ReleaseEffect(CEffekseerEffect *pEffect)
{
	if (pEffect == nullptr)
		return;

	// �G�t�F�N�g�̏I������
	pEffect->Uninit();

	// ���X�g���珜�O
	m_listEffect.remove(pEffect);
}

//===========================================================
// �G�t�F�N�g�t�@�C���̃p�X�擾
//===========================================================
const char* CEffekseer::GetPathEffect(CEffekseer::TYPE type)
{
	if (type <= TYPE::TYPE_NONE || type >= TYPE::TYPE_MAX)
		return nullptr;

	return m_apEfkName[type];
}

//**************************************************************************************
// �G�t�F�N�g�N���X
//**************************************************************************************
//===========================================================
// �R���X�g���N�^
//===========================================================
CEffekseerEffect::CEffekseerEffect()
{

}

//===========================================================
// �f�X�g���N�^
//===========================================================
CEffekseerEffect::~CEffekseerEffect()
{

}

//===========================================================
// ������
//===========================================================
void CEffekseerEffect::Init(::Effekseer::Vector3D pos, ::Effekseer::Vector3D rot, ::Effekseer::Vector3D scale)
{
	m_pos = pos;
	m_rot = rot;
	m_scale = scale;
	m_time = 0;

	CEffekseer *pEffekseer = CManager::GetMyEffekseer();

	if (pEffekseer == nullptr)
		return;

	Effekseer::ManagerRef manager = pEffekseer->GetEfkManager();

	// �ʒu�A�����A�傫���ݒ�
	manager->SetLocation(m_efkHandle, m_pos);
	manager->SetRotation(m_efkHandle, { 0.0f, 1.0f, 0.0f }, rot.Y);
	manager->SetScale(m_efkHandle, m_scale.X, m_scale.Y, m_scale.Z);
}

//===========================================================
// �I��
//===========================================================
void CEffekseerEffect::Uninit()
{
	CEffekseer *pEffekseer = CManager::GetMyEffekseer();

	if (pEffekseer == nullptr)
		return;

	Effekseer::ManagerRef efkManager = pEffekseer->GetEfkManager();

	// �G�t�F�N�g�̉��
	efkManager->StopEffect(m_efkHandle);
}

//===========================================================
// �ʒu�̒Ǐ]
//===========================================================
CEffekseerEffect *CEffekseerEffect::FollowPosition(D3DXVECTOR3 pos)
{
	CEffekseer *pEffekseer = CManager::GetMyEffekseer();

	if (pEffekseer == nullptr)
		return nullptr;

	Effekseer::ManagerRef efkManager = pEffekseer->GetEfkManager();

	// ���t���[���A�G�t�F�N�g���Đ��I�����Ă��邩�m�F����
	if (efkManager->Exists(m_efkHandle) == false)
	{
		return nullptr;
	}

	SetPosition(Effekseer::Vector3D(pos.x, pos.y, pos.z));

	return this;
}

namespace MyEffekseer
{
CEffekseerEffect *CreateEffect(CEffekseer::TYPE type, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale)
{
	CEffekseer *pEffekseer = CManager::GetMyEffekseer();

	if (pEffekseer == nullptr)
		return nullptr;

	// �p�X�̎擾
	const char* pPath = pEffekseer->GetPathEffect(type);

	if (pPath == nullptr)
		return nullptr;

	CEffekseerEffect *pEffect = pEffekseer->CreateEffect(pPath, Effekseer::Vector3D(pos.x, pos.y, pos.z), Effekseer::Vector3D(rot.x, rot.y, rot.z), Effekseer::Vector3D(scale.x, scale.y, scale.z));

	return pEffect;
}
}