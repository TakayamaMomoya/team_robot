//================================================================
//
// BulletPhysics[Myphysics.cpp]
// Author 大原怜将
//
//================================================================
/*
Bullet Continuous Collision Detection and Physics Library
Copyright (c) 2003-2007 Erwin Coumans  https://bulletphysics.org

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it freely,
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

///-----includes_start-----
#include "physics.h"
#include "manager.h"
#include "debugproc.h"
#include "objectX.h"
#include "renderer.h"
#include <stdio.h>

#if defined (_MSC_VER)

#ifdef _DEBUG
#pragma comment(lib, "BulletCollision_Debug.lib")
#else
#pragma comment(lib, "BulletCollision.lib")
#endif

#ifdef _DEBUG
#pragma comment(lib, "BulletDynamics_Debug.lib")
#else
#pragma comment(lib, "BulletDynamics.lib")
#endif

#ifdef _DEBUG
#pragma comment(lib, "LinearMath_Debug.lib")
#else
#pragma comment(lib, "LinearMath.lib")
#endif

#endif

namespace
{
	const btScalar GRAVITY = -100;  // 重力
	const btVector3 WORLDMIN = btVector3(-10000, -10000, -10000);  // ワールドの広さ
	const btVector3 WORLDMAX = btVector3(10000, 10000, 10000);     // ワールドの広さ
	const int MAXPROXIES = 1024;  // プロキシの最大数
}

/// This is a Hello World program for running a basic Bullet physics simulation

//================================================================
// コンストラクタ
//================================================================
CPhysics::CPhysics()
{
	// 値をクリア
	allFrame = 0;
}

//================================================================
// デストラクタ
//================================================================
CPhysics::~CPhysics()
{

}

//================================================================
// 初期化処理
//================================================================
void CPhysics::Init(void)
{
	// ワールドの広さ
	btVector3 worldAabbMin(WORLDMIN);
	btVector3 worldAabbMax(WORLDMAX);

	// プロキシの最大数（衝突物体のようなもの）
	int maxProxies = MAXPROXIES;

	// broadphaseの作成（SAP法）
	m_pBroadphase = new btAxisSweep3(worldAabbMin, worldAabbMax, maxProxies);

	// デフォルトの衝突設定とディスパッチャの作成
	m_pCollisionConfiguration = new btDefaultCollisionConfiguration();
	m_pDispatcher = new btCollisionDispatcher(m_pCollisionConfiguration);

	// 衝突解決ソルバ
	m_pSolver = new btSequentialImpulseConstraintSolver;

	// 離散動的世界の作成
	m_pDynamicsWorld = new btDiscreteDynamicsWorld(m_pDispatcher, m_pBroadphase, m_pSolver, m_pCollisionConfiguration);

	// 重力の設定
	m_pDynamicsWorld->setGravity(btVector3(0, GRAVITY, 0));

	// 地面
	{
		// 地面の衝突形状の作成（btStaticPlaneShape：無限平面）
		btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0, 1, 0), 1);
		// 地面のMotionStateの設定
		btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -1, 0)));

		// 地面の初期情報を設定
		btRigidBody::btRigidBodyConstructionInfo
			groundRigidBodyCI(0, groundMotionState, groundShape, btVector3(0, 0, 0));
		// 地面の剛体の作成
		btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);
		// ワールドに地面の剛体を追加
		m_pDynamicsWorld->addRigidBody(groundRigidBody);
	}
}

//================================================================
// 終了処理
//================================================================
void CPhysics::Uninit(void)
{
	// ダイナミックワールドの破棄
	if (m_pDynamicsWorld != nullptr)
	{
		delete m_pDynamicsWorld;
		m_pDynamicsWorld = nullptr;
	}

	// ブロードの破棄
	if (m_pBroadphase != nullptr)
	{
		delete m_pBroadphase;
		m_pBroadphase = nullptr;
	}

	// デフォルトの衝突設定を破棄
	if (m_pCollisionConfiguration != nullptr)
	{
		delete m_pCollisionConfiguration;
		m_pCollisionConfiguration = nullptr;
	}

	// ディスパッチャーの破棄
	if (m_pDispatcher != nullptr)
	{
		delete m_pDispatcher;
		m_pDispatcher = nullptr;
	}

	// 衝突解決ソルバの破棄
	if (m_pSolver != nullptr)
	{
		delete m_pSolver;
		m_pSolver = nullptr;
	}
}

//================================================================
// 描画処理
//================================================================
void CPhysics::Draw(void)
{

}