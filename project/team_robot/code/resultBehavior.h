//*****************************************************
//
// ���U���g��Behavior[resultBehavior.h]
// Author:���R����
//
//*****************************************************

#ifndef _RESULTBEHAVIOR_H_
#define _RESULTBEHAVIOR_H_

//*****************************************************
// �C���N���[�h
//*****************************************************

//*****************************************************
// �O���錾
//*****************************************************
class CRanking;
class CNumber;
class CPolygon2D;

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CResultBehavior
{
public:
	CResultBehavior();
	virtual ~CResultBehavior();

	virtual void Init(CRanking *pResult) = 0;
	virtual void Update(CRanking *pResult) = 0;
	virtual void Uninit(CRanking *pResult) = 0;
private:

};

class CResultPlayerScore : public CResultBehavior
{
public:
	enum TYPE_NUMBER
	{
		TYPE_ALL = 0,	// ���z
		TYPE_ADD,	// ���Z��V
		TYPE_REPAIR,	// �C����
		TYPE_RESULT,	// ���z
		TYPE_MAX
	};

	CResultPlayerScore();
	~CResultPlayerScore() override;

private:
	void Init(CRanking *pResult) override;
	void Update(CRanking *pResult) override;
	void Uninit(CRanking *pResult) override;

	CNumber *m_apNumber[TYPE_MAX];	// ���l
	CPolygon2D *m_apCaption[TYPE_MAX];	// ���ڂ̌��o��
	CPolygon2D *m_pFrame;	// �t���[��
};

class CResultRanking : public CResultBehavior
{
public:
	CResultRanking();
	~CResultRanking() override;

private:
	void Init(CRanking *pResult) override;
	void Update(CRanking *pResult) override;
	void Uninit(CRanking *pResult) override {};
};

#endif