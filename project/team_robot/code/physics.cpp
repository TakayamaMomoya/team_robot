//================================================================
//
// BulletPhysics[Myphysics.cpp]
// Author �匴�叫
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
	const btScalar GRAVITY = -100;  // �d��
	const btVector3 WORLDMIN = btVector3(-10000, -10000, -10000);  // ���[���h�̍L��
	const btVector3 WORLDMAX = btVector3(10000, 10000, 10000);     // ���[���h�̍L��
	const int MAXPROXIES = 1024;  // �v���L�V�̍ő吔
}

/// This is a Hello World program for running a basic Bullet physics simulation

//================================================================
// �R���X�g���N�^
//================================================================
CPhysics::CPhysics()
{
	// �l���N���A
	allFrame = 0;
}

//================================================================
// �f�X�g���N�^
//================================================================
CPhysics::~CPhysics()
{

}

//================================================================
// ����������
//================================================================
void CPhysics::Init(void)
{
	// ���[���h�̍L��
	btVector3 worldAabbMin(WORLDMIN);
	btVector3 worldAabbMax(WORLDMAX);

	// �v���L�V�̍ő吔�i�Փ˕��̂̂悤�Ȃ��́j
	int maxProxies = MAXPROXIES;

	// broadphase�̍쐬�iSAP�@�j
	m_pBroadphase = new btAxisSweep3(worldAabbMin, worldAabbMax, maxProxies);

	// �f�t�H���g�̏Փːݒ�ƃf�B�X�p�b�`���̍쐬
	m_pCollisionConfiguration = new btDefaultCollisionConfiguration();
	m_pDispatcher = new btCollisionDispatcher(m_pCollisionConfiguration);

	// �Փˉ����\���o
	m_pSolver = new btSequentialImpulseConstraintSolver;

	// ���U���I���E�̍쐬
	m_pDynamicsWorld = new btDiscreteDynamicsWorld(m_pDispatcher, m_pBroadphase, m_pSolver, m_pCollisionConfiguration);

	// �d�͂̐ݒ�
	m_pDynamicsWorld->setGravity(btVector3(0, GRAVITY, 0));

	// �n��
	{
		// �n�ʂ̏Փˌ`��̍쐬�ibtStaticPlaneShape�F�������ʁj
		btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0, 1, 0), 1);
		// �n�ʂ�MotionState�̐ݒ�
		btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -1, 0)));

		// �n�ʂ̏�������ݒ�
		btRigidBody::btRigidBodyConstructionInfo
			groundRigidBodyCI(0, groundMotionState, groundShape, btVector3(0, 0, 0));
		// �n�ʂ̍��̂̍쐬
		btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);
		// ���[���h�ɒn�ʂ̍��̂�ǉ�
		m_pDynamicsWorld->addRigidBody(groundRigidBody);
	}
}

//================================================================
// �I������
//================================================================
void CPhysics::Uninit(void)
{
	// �_�C�i�~�b�N���[���h�̔j��
	if (m_pDynamicsWorld != nullptr)
	{
		delete m_pDynamicsWorld;
		m_pDynamicsWorld = nullptr;
	}

	// �u���[�h�̔j��
	if (m_pBroadphase != nullptr)
	{
		delete m_pBroadphase;
		m_pBroadphase = nullptr;
	}

	// �f�t�H���g�̏Փːݒ��j��
	if (m_pCollisionConfiguration != nullptr)
	{
		delete m_pCollisionConfiguration;
		m_pCollisionConfiguration = nullptr;
	}

	// �f�B�X�p�b�`���[�̔j��
	if (m_pDispatcher != nullptr)
	{
		delete m_pDispatcher;
		m_pDispatcher = nullptr;
	}

	// �Փˉ����\���o�̔j��
	if (m_pSolver != nullptr)
	{
		delete m_pSolver;
		m_pSolver = nullptr;
	}
}

//================================================================
// �`�揈��
//================================================================
void CPhysics::Draw(void)
{

}