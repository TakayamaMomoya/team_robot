//*****************************************************
//
// オリジナルライブラリ[myLib.h.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "myLib.h"
#include "debugproc.h"

//=================================================================================
// スプライン
//=================================================================================
//=====================================================
// 初期化処理
//=====================================================
void CSpline::Init(const std::vector<double>& x, const std::vector<double>& y)
{
    // 引数の受け取り
    this->x = x;
    this->y = y;
    int nSize = x.size();

    // 配列の初期化
    a = y;
    b.resize(nSize, 0.0);
    c.resize(nSize, 0.0);
    d.resize(nSize, 0.0);

    // 区間の距離計算
    std::vector<double> h(nSize - 1);
    for (int i = 0; i < nSize - 1; ++i)
    {
        h[i] = x[i + 1] - x[i];
    }

    // 三重対角系のセットアップ
    std::vector<double> alpha(nSize - 1);

    for (int i = 1; i < nSize - 1; ++i)
    {
        alpha[i] = 3.0 / h[i] * (a[i + 1] - a[i]) - 3.0 / h[i - 1] * (a[i] - a[i - 1]);
    }

    // cの三重対角系を解く
    std::vector<double> l(nSize + 1);
    std::vector<double> mu(nSize + 1);
    std::vector<double> z(nSize + 1);
    l[0] = 1.0;
    mu[0] = 0.0;
    z[0] = 0.0;

    for (int i = 1; i < nSize - 1; ++i)
    {
        l[i] = 2.0 * (x[i + 1] - x[i - 1]) - h[i - 1] * mu[i - 1];
        mu[i] = h[i] / l[i];
        z[i] = (alpha[i] - h[i - 1] * z[i - 1]) / l[i];
    }

    l[nSize - 1] = 1.0;
    z[nSize - 1] = 0.0;
    c[nSize - 1] = 0.0;

    for (int j = nSize - 2; j >= 0; j--)
    {
        c[j] = z[j] - mu[j] * c[j + 1];
        b[j] = (a[j + 1] - a[j]) / h[j] - h[j] * (c[j + 1] + 2.0 * c[j]) / 3.0;
        d[j] = (c[j + 1] - c[j]) / (3.0 * h[j]);
    }
}

//=====================================================
// 高さの計算
//=====================================================
double CSpline::Interpolate(double xi, int nIdx)
{
    int n = x.size();
    int i = 0;

    if (n < 2)
        return 0.0;

    if (nIdx != -1)
    {
        i = nIdx;
    }
    else
    {
        while (i < n - 1 && xi > x[i + 1])
        {
            i++;
        }

        if (i >= n)
        {
            i = n - 1;
        }
    }

    double h = xi - x[i];

    return a[i] + b[i] * h + c[i] * h * h + d[i] * h * h * h;
}

//=====================================================
// デストラクタ
//=====================================================
CCutMullSpline::~CCutMullSpline()
{
    m_vPos.clear();
}

//=====================================================
// 初期化処理
//=====================================================
void CCutMullSpline::Init(const std::vector<D3DXVECTOR3> vPos)
{
    // 配列のリサイズ
    int nSize = vPos.size();
    m_vPos.resize(nSize + 2);

    for (int i = 0; i < nSize; i++)
    {// 引数の受け取り
        m_vPos[i + 1] = vPos[i];
    }

    // ダミーの最初と最後の位置情報を追加
    m_vPos[0] = vPos[0];
    //m_vPos[0].x -= 50.0f;
    m_vPos[nSize + 1] = vPos[nSize - 1];
    //m_vPos[nSize + 1].x += 50.0f;
}

//=====================================================
// 補間
//=====================================================
D3DXVECTOR3 CCutMullSpline::Interpolate(float t,int nIdx)
{
    D3DXVECTOR3 a1, a2, a3, a4;
    D3DXVECTOR3 pos;

    // a4の決定
    a4 = m_vPos[nIdx];

    // a3の決定
    a3 = (m_vPos[nIdx] - m_vPos[nIdx - 1]) / 2;

    // a1の決定
    a1 = ((m_vPos[nIdx + 2] - m_vPos[nIdx]) / 2) - 2 * m_vPos[nIdx + 1] + a3 + 2 * a4;

    // a2の決定
    a2 = 3 * m_vPos[nIdx + 1] - ((m_vPos[nIdx + 2] - m_vPos[nIdx]) / 2) - 2 * a3 - 3 * a4;

    // 計算結果を返す
    pos = (a1 * t * t * t) + (a2 * t * t) + (a3 * t) + a4;

    return pos;
}

//=====================================================
// 曲線の長さの取得
//=====================================================
float CCutMullSpline::GetLength(int nIdx, int nDiv)
{
    if (nDiv <= 0)
        assert(("スプラインの長さ取得で不正な分割数を入力しています",false)); // 分割数が0より少ないエラー

    float fLengthResult = 0;

    for (int i = 1; i < nDiv; i++)
    {// 区分ごとに差分距離を取得して加算する
        float fRate = (float)i / nDiv;
        float fRateLast = (float)(i - 1) / nDiv;

        D3DXVECTOR3 pos = Interpolate(fRate, nIdx);
        D3DXVECTOR3 posLast = Interpolate(fRateLast, nIdx);

        D3DXVECTOR3 vecDiff = posLast - pos;
        float fLength = D3DXVec3Length(&vecDiff);

        fLengthResult += fLength;
    }

    return fLengthResult;
}