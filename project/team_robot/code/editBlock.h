//*****************************************************
//
// �u���b�N�G�f�B�^�[�̏���[editBlock.h]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "block.h"
#include "edit.h"

//*****************************************************
// �O���錾
//*****************************************************
class CObjectX;
class CStateEditBlock;
class CPolygon3D;

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CEditBlock : public CEdit
{
public:
	CEditBlock();	// �R���X�g���N�^
	~CEditBlock();	// �f�X�g���N�^

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Save(void);
	void ChangeState(CStateEditBlock *pState);
	void AddBlockToMap(CBlock *pBlock);
	void SetMoveBlock(CBlock *pBlock) { m_pMoveBlock = pBlock; }

private:
	void CreateBlockIconAll(void);
	void DeleteAllIcon(void);
	void RaySelectBlock(void);
	void CollideBlockRay(CBlock *pBlock,D3DXVECTOR3 posFar, D3DXVECTOR3 posNear, D3DXVECTOR3 vecDiff);
	void MoveCurrentBlock(D3DXVECTOR3 posFar, D3DXVECTOR3 posNear);

	char m_aPath[256];
	CStateEditBlock *m_pState;	// �X�e�C�g�̃|�C���^
	D3DXVECTOR3 m_posCurrent;
	CBlock *m_pCurrentBlock;	// �I���u���b�N
	CBlock *m_pMoveBlock;	// �������u���b�N
	std::map<CBlock*, CPolygon3D*> m_aIcon;	// �A�C�R���̃|�C���^
};

//*****************************************************
// �X�e�C�g�N���X�̒�`
//*****************************************************
class CStateEditBlock
{// �u���b�N�G�f�B�b�g�̃X�e�C�g���N���X
public:
	CStateEditBlock() {};
	virtual ~CStateEditBlock() {};

	virtual void Init(CEditBlock *pEdit) = 0;
	virtual void Uninit(CEditBlock *pEdit) = 0;
	virtual void Update(CEditBlock *pEdit) = 0;
};

class CStateCreateBlockNormal : public CStateEditBlock
{// �ʏ�u���b�N�̐���
public:
	CStateCreateBlockNormal() : m_pObjectCursor(nullptr), m_nIdxObject(-1), m_type(CBlock::TYPE::TYPE_WALL), m_behaviour(CBlock::BEHAVIOUR::BEHAVIOUR_NORMAL) {};
	virtual ~CStateCreateBlockNormal() {};

	void Init(CEditBlock *pEdit) override;
	void Uninit(CEditBlock *pEdit) override;
	void Update(CEditBlock *pEdit) override;

private:
	void ChangeBlockBehaviour(void);
	void CreateBlock(D3DXVECTOR3 pos, CEditBlock *pEdit);
	CBlock *CheckDelete(void);
	void SelectBlock(void);

	int m_nIdxObject;
	CBlock::TYPE m_type;
	CObjectX *m_pObjectCursor;	// �J�[�\���̉��I�u�W�F�N�g
	CBlock::BEHAVIOUR m_behaviour;	// �u���b�N�̃r�w�C�r�A
	CBlock *m_pBlockCurrent;	// �I�����Ă���u���b�N�̃|�C���^
};

class CStateEditGrabBlock : public CStateEditBlock
{// �͂ރu���b�N�̕ҏW
public:
	CStateEditGrabBlock() {};
	virtual ~CStateEditGrabBlock() {};

	// �����o�֐�
	void Init(CEditBlock *pEdit) override;
	void Uninit(CEditBlock *pEdit) override;
	void Update(CEditBlock *pEdit) override;

private:
	// �����o�֐�
	void SelectGrabBlock(void);
	void EditGrabBlock(void);

	// �����o�ϐ�
	std::list<CBlockGrab*>::iterator m_it;	// �C�e���[�^�[
};