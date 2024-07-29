//*****************************************************
//
// ���b�V���G�f�B�^�[�̏���[editMesh.h]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "edit.h"
#include "meshRoad.h"

//*****************************************************
// �O���錾
//*****************************************************
class CPolygon3D;
class CObjectX;
class CStateEditMesh;
class CEnemySpawner;

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CEditMesh : public CEdit
{
public:
	CEditMesh();
	~CEditMesh();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void ChangeState(CStateEditMesh *pState);
	const char* GetSavePath(void) { return &m_aPathSave[0]; }
	const char* GetSavePathEnemy(void) { return &m_aPathSaveEnemy[0]; }
	void SaveEnemy(const char* pPath);
	void LoadEnemy(const char* pPath);
	void AddSpawner(CEnemySpawner* pSpawner) { m_listEnemySpawner.push_back(pSpawner); }

private:
	// �����o�֐�
	std::vector<CMeshRoad::SInfoRoadPoint>::iterator SelectIterator(void);	// �C�e���[�^�[�̑I��
	void MoveCurrentRP(void);	// �I�����[�h�|�C���g�̈ړ�
	void CollideRPRay(std::vector<CMeshRoad::SInfoRoadPoint>::iterator it, D3DXVECTOR3 posFar, D3DXVECTOR3 posNear, D3DXVECTOR3 vecDiff);	// ���[�h�|�C���g�ƃ��C�̔���

	// �����o�ϐ�
	CPolygon3D *m_pMesh;	// ���b�V���̃|�C���^
	CObjectX *m_pManipulator;	// �}�j�s�����[�^�[
	CStateEditMesh *m_pState;	// �X�e�C�g
	std::vector<CMeshRoad::SInfoRoadPoint>::iterator m_itCurrent;	// �I�����Ă�C�e���C�^�[
	bool m_bCurrent;	// �I���t���O
	D3DXVECTOR3 m_posCurrent;	// �I���u���b�N�ʒu
	char m_aPathSave[256] = {};	// �ۑ��p�X
	char m_aPathSaveEnemy[256] = {};	// �G�ۑ��p�X
	std::list<CEnemySpawner*> m_listEnemySpawner;	// �G�X�|�i�[�̃��X�g
};

//*****************************************************
// �X�e�C�g�N���X�̒�`
//*****************************************************
class CStateEditMesh
{// ���b�V���G�f�B�b�g�̃X�e�C�g���N���X
public:
	CStateEditMesh() {};
	virtual ~CStateEditMesh() {};

	virtual void Update(CEditMesh *pEdit) = 0;
};

class CStateEditMeshCreateMesh : public CStateEditMesh
{// ���b�V���̐���
public:
	CStateEditMeshCreateMesh() {};
	virtual ~CStateEditMeshCreateMesh() {};

	void Update(CEditMesh *pEdit) override;

private:
};

class CStateEditMeshCreateTunnel : public CStateEditMesh
{// �g���l���̐���
public:
	CStateEditMeshCreateTunnel();	// �R���X�g���N�^
	virtual ~CStateEditMeshCreateTunnel() {};	// �f�X�g���N�^

	// �����o�֐�
	void Update(CEditMesh *pEdit) override;	// �X�V����

private:
	// �����o�֐�
	void SetTunnel(std::vector<CMeshRoad::SInfoRoadPoint>::iterator it);

	// �����o�ϐ�
	bool m_bEnd;	// �X�^�[�g�̑I�����ǂ���
	std::vector<CMeshRoad::SInfoRoadPoint>::iterator m_itStart;	// �g���l���̊J�n�C�e���C�^�[
	std::vector<CMeshRoad::SInfoRoadPoint>::iterator m_itEnd;	// �g���l���̏I���C�e���C�^�[
};

class CStateEditMeshCreateGR : public CStateEditMesh
{// �K�[�h���[���̐���
public:
	CStateEditMeshCreateGR();	// �R���X�g���N�^
	virtual ~CStateEditMeshCreateGR() {};	// �f�X�g���N�^

	// �����o�֐�
	void Update(CEditMesh *pEdit) override;	// �X�V����

private:
	// �����o�֐�
	void SetGR(std::vector<CMeshRoad::SInfoRoadPoint>::iterator it);

	// �����o�ϐ�
	bool m_bEnd;	// �X�^�[�g�̑I�����ǂ���
	float m_fHeight;	// ����
	float m_bLeft;	// �����ɂ��邩�ǂ���
	std::vector<CMeshRoad::SInfoRoadPoint>::iterator m_itStart;	// �J�n�C�e���C�^�[
	std::vector<CMeshRoad::SInfoRoadPoint>::iterator m_itEnd;	// �I���C�e���C�^�[
};

class CStateEditMeshSelectGR : public CStateEditMesh
{// �K�[�h���[���̑I��
public:
	CStateEditMeshSelectGR();	// �R���X�g���N�^
	virtual ~CStateEditMeshSelectGR() {};	// �f�X�g���N�^

	// �����o�֐�
	void Update(CEditMesh *pEdit) override;	// �X�V����

private:
	// �����o�ϐ�
	std::vector<CGuardRail*>::iterator m_it;	// �I���C�e���[�^�[
};

class CStateEditMeshAdjustRoadPoint : public CStateEditMesh
{// ���[�h�|�C���g�̒���
public:
	CStateEditMeshAdjustRoadPoint();
	virtual ~CStateEditMeshAdjustRoadPoint() {};

	void Update(CEditMesh *pEdit) override;
};

class CStateEditMeshDeleteRoadPoint : public CStateEditMesh
{// ���[�h�|�C���g�̍폜
public:
	CStateEditMeshDeleteRoadPoint();
	virtual ~CStateEditMeshDeleteRoadPoint() {};

	void Update(CEditMesh *pEdit) override;
};