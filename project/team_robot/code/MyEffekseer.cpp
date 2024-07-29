//===========================================================
//
// マイエフェクシア[MyEffekseer.cpp]
// Author 大原怜将
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

// エフェクトの名前
const char* CEffekseer::m_apEfkName[CEffekseer::TYPE_MAX] =
{
	"",                               // なんもない
	"data\\EFFEKSEER\\Effect\\drift.efkefc",  // ドリフト時の火花と煙
	"data\\EFFEKSEER\\Effect\\spark.efkefc",  // 壁衝突時の火花
	"data\\EFFEKSEER\\Effect\\bikefire.efkefc", // 排気口の火
	"data\\EFFEKSEER\\Effect\\slash00.efkefc",	// 斬撃
	"data\\EFFEKSEER\\Effect\\parry.efkefc",	// パリィ
	"data\\EFFEKSEER\\Effect\\flash00.efkefc",	// 手裏剣の光
	"data\\EFFEKSEER\\Effect\\shuriken.efkefc",	// 手裏剣の投げ
	"data\\EFFEKSEER\\Effect\\WindShuriken.efkefc",	// 手裏剣の回転風
	"data\\EFFEKSEER\\Effect\\grapple.efkefc",	// 鉤縄
	"data\\EFFEKSEER\\Effect\\smoke.efkefc",	// 敵撤退時の煙
	"data\\EFFEKSEER\\Effect\\gate00.efkefc",	// ゲート
};

//===========================================================
// コンストラクタ
//===========================================================
CEffekseer::CEffekseer()
{
	m_nNum = 0;
}

//===========================================================
// デストラクタ
//===========================================================
CEffekseer::~CEffekseer()
{

}

//===========================================================
// 初期化処理
//===========================================================
void CEffekseer::Init(void)
{
	// エフェクトのマネージャーの作成
	m_efkManager = ::Effekseer::Manager::Create(8000);

	// Specify a position of view
	// 視点位置を確定
	viewerPosition = ::Effekseer::Vector3D(0.0f, 0.0f, 0.0f);

	// 座標系を設定する。アプリケーションと一致させる必要がある。
	m_efkManager->SetCoordinateSystem(Effekseer::CoordinateSystem::LH);

	// Setup effekseer modules
	// Effekseerのモジュールをセットアップする
	SetupEffekseerModules(m_efkManager);

	// Specify a projection matrix
	// 投影行列を設定
	projectionMatrix.PerspectiveFovLH(90.0f / 180.0f * 3.14f, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 1.0f, 500.0f);

	// Specify a camera matrix
		// カメラ行列を設定
	cameraMatrix.LookAtLH(viewerPosition, ::Effekseer::Vector3D(0.0f, 0.0f, 0.0f), ::Effekseer::Vector3D(0.0f, 1.0f, 0.0f));
}

//===========================================================
// 終了処理
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
// 更新処理
//===========================================================
void CEffekseer::Update(void)
{
	for (auto it = m_listEffect.begin();it != m_listEffect.end();)
	{
		// エフェクトの移動
		Effekseer::Handle handle = (*it)->GetHandle();
		Effekseer::Vector3D pos = (*it)->GetPosition();
		m_efkManager->SetLocation(handle, pos);

		// エフェクトの向き反映
		Effekseer::Vector3D rot = (*it)->GetRotation();
		m_efkManager->SetRotation(handle, rot.X, rot.Y, rot.Z);

		// エフェクトのスケール反映
		Effekseer::Vector3D scale = (*it)->GetScale();
		m_efkManager->SetScale(handle, scale.X, scale.Y, scale.Z);

		// レイヤーパラメータの設定
		Effekseer::Manager::LayerParameter layerParameter;
		layerParameter.ViewerPosition = viewerPosition;
		m_efkManager->SetLayerParameter(0, layerParameter);

		// マネージャーの更新
		Effekseer::Manager::UpdateParameter updateParameter;
		m_efkManager->Update(updateParameter);

		// 時間を更新する
		int32_t time = (*it)->GetTime();
		m_efkRenderer->SetTime((float)time / 60.0f);
		time++;
		(*it)->SetTime(time);

		auto itNext = std::next(it);

		if (itNext == m_listEffect.end())
			break;

		// 毎フレーム、エフェクトが再生終了しているか確認する
		if (m_efkManager->Exists(handle) == false)
		{
			(*it)->Uninit();

			// リストから除外
			m_listEffect.remove((*it));
		}

		it = itNext;
	}
}

//===========================================================
// 描画処理
//===========================================================
void CEffekseer::Draw(void)
{
	if (m_efkRenderer != nullptr)
	{
		// Specify a projection matrix
		// 投影行列を設定
		m_efkRenderer->SetProjectionMatrix(projectionMatrix);

		// カメラの情報取得
		CCamera* pCamera = CManager::GetCamera();

		// ビューマトリックス
		auto ViewMatrix = pCamera->GetCamera()->mtxView;

		// プロジェクションマトリックス
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
		// カメラ行列を設定
		m_efkRenderer->SetCameraMatrix(cameraMatrix);

		// Begin to rendering effects
		// エフェクトの描画開始処理を行う。
		m_efkRenderer->BeginRendering();

		// Render effects
		// エフェクトの描画を行う。
		Effekseer::Manager::DrawParameter drawParameter;
		drawParameter.ZNear = 0.0f;
		drawParameter.ZFar = 1.0f;
		drawParameter.ViewProjectionMatrix = m_efkRenderer->GetCameraProjectionMatrix();
		m_efkManager->Draw(drawParameter);

		// Finish to rendering effects
		// エフェクトの描画終了処理を行う。
		m_efkRenderer->EndRendering();
	}
}

//===========================================================
// エフェクトの生成
//===========================================================
CEffekseerEffect *CEffekseer::CreateEffect(const char* FileName, ::Effekseer::Vector3D pos, ::Effekseer::Vector3D rot, ::Effekseer::Vector3D scale)
{
	CEffekseerEffect *pEffect = nullptr;

	pEffect = new CEffekseerEffect;

	if (pEffect == nullptr)
		return nullptr;

	// エフェクトの読込
	// char16_tに変換
	std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> converter;
	std::u16string string16t = converter.from_bytes(FileName);

	Effekseer::EffectRef effect = Effekseer::Effect::Create(m_efkManager, string16t.c_str());

	pEffect->SetEffect(effect);
	
	// Play an effect
	// エフェクトの再生
	Effekseer::Handle handle = m_efkManager->Play(effect, 0, 0, 0);

	pEffect->SetHandle(handle);

	pEffect->Init(pos, rot, scale);

	m_listEffect.push_back(pEffect);

	return pEffect;
}

//===========================================================
// モジュール設定
//===========================================================
void CEffekseer::SetupEffekseerModules(::Effekseer::ManagerRef efkManager)
{
		// Create a  graphics device
	// 描画デバイスの作成
	::Effekseer::Backend::GraphicsDeviceRef graphicsDevice;
	graphicsDevice = ::EffekseerRendererDX9::CreateGraphicsDevice(CRenderer::GetInstance()->GetDevice());

	// Create a renderer of effects
	// エフェクトのレンダラーの作成
	m_efkRenderer = ::EffekseerRendererDX9::Renderer::Create(graphicsDevice, 8000);

	// Sprcify rendering modules
	// 描画モジュールの設定
	efkManager->SetSpriteRenderer(m_efkRenderer->CreateSpriteRenderer());
	efkManager->SetRibbonRenderer(m_efkRenderer->CreateRibbonRenderer());
	efkManager->SetRingRenderer(m_efkRenderer->CreateRingRenderer());
	efkManager->SetTrackRenderer(m_efkRenderer->CreateTrackRenderer());
	efkManager->SetModelRenderer(m_efkRenderer->CreateModelRenderer());

	// Specify a texture, model, curve and material loader
	// It can be extended by yourself. It is loaded from a file on now.
	// テクスチャ、モデル、カーブ、マテリアルローダーの設定する。
	// ユーザーが独自で拡張できる。現在はファイルから読み込んでいる。
	efkManager->SetTextureLoader(m_efkRenderer->CreateTextureLoader());
	efkManager->SetModelLoader(m_efkRenderer->CreateModelLoader());
	efkManager->SetMaterialLoader(m_efkRenderer->CreateMaterialLoader());
	efkManager->SetCurveLoader(Effekseer::MakeRefPtr<Effekseer::CurveLoader>());
}

//===========================================================
// エフェクトのリリース
//===========================================================
void CEffekseer::Release(int idx)
{

}

//===========================================================
// エフェクトのリリース
//===========================================================
void CEffekseer::ReleaseEffect(CEffekseerEffect *pEffect)
{
	if (pEffect == nullptr)
		return;

	// エフェクトの終了処理
	pEffect->Uninit();

	// リストから除外
	m_listEffect.remove(pEffect);
}

//===========================================================
// エフェクトファイルのパス取得
//===========================================================
const char* CEffekseer::GetPathEffect(CEffekseer::TYPE type)
{
	if (type <= TYPE::TYPE_NONE || type >= TYPE::TYPE_MAX)
		return nullptr;

	return m_apEfkName[type];
}

//**************************************************************************************
// エフェクトクラス
//**************************************************************************************
//===========================================================
// コンストラクタ
//===========================================================
CEffekseerEffect::CEffekseerEffect()
{

}

//===========================================================
// デストラクタ
//===========================================================
CEffekseerEffect::~CEffekseerEffect()
{

}

//===========================================================
// 初期化
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

	// 位置、向き、大きさ設定
	manager->SetLocation(m_efkHandle, m_pos);
	manager->SetRotation(m_efkHandle, { 0.0f, 1.0f, 0.0f }, rot.Y);
	manager->SetScale(m_efkHandle, m_scale.X, m_scale.Y, m_scale.Z);
}

//===========================================================
// 終了
//===========================================================
void CEffekseerEffect::Uninit()
{
	CEffekseer *pEffekseer = CManager::GetMyEffekseer();

	if (pEffekseer == nullptr)
		return;

	Effekseer::ManagerRef efkManager = pEffekseer->GetEfkManager();

	// エフェクトの解放
	efkManager->StopEffect(m_efkHandle);
}

//===========================================================
// 位置の追従
//===========================================================
CEffekseerEffect *CEffekseerEffect::FollowPosition(D3DXVECTOR3 pos)
{
	CEffekseer *pEffekseer = CManager::GetMyEffekseer();

	if (pEffekseer == nullptr)
		return nullptr;

	Effekseer::ManagerRef efkManager = pEffekseer->GetEfkManager();

	// 毎フレーム、エフェクトが再生終了しているか確認する
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

	// パスの取得
	const char* pPath = pEffekseer->GetPathEffect(type);

	if (pPath == nullptr)
		return nullptr;

	CEffekseerEffect *pEffect = pEffekseer->CreateEffect(pPath, Effekseer::Vector3D(pos.x, pos.y, pos.z), Effekseer::Vector3D(rot.x, rot.y, rot.z), Effekseer::Vector3D(scale.x, scale.y, scale.z));

	return pEffect;
}
}