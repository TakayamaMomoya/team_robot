//*****************************************************
//
// リザルトのBehavior[resultBehavior.h]
// Author:高山桃也
//
//*****************************************************

#ifndef _RESULTBEHAVIOR_H_
#define _RESULTBEHAVIOR_H_

//*****************************************************
// インクルード
//*****************************************************

//*****************************************************
// 前方宣言
//*****************************************************
class CRanking;
class CNumber;
class CPolygon2D;

//*****************************************************
// クラスの定義
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
		TYPE_ALL = 0,	// 総額
		TYPE_ADD,	// 加算報酬
		TYPE_REPAIR,	// 修理費
		TYPE_RESULT,	// 総額
		TYPE_MAX
	};

	CResultPlayerScore();
	~CResultPlayerScore() override;

private:
	void Init(CRanking *pResult) override;
	void Update(CRanking *pResult) override;
	void Uninit(CRanking *pResult) override;

	CNumber *m_apNumber[TYPE_MAX];	// 数値
	CPolygon2D *m_apCaption[TYPE_MAX];	// 項目の見出し
	CPolygon2D *m_pFrame;	// フレーム
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