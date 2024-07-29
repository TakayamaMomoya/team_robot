//*****************************************************
//
// �I���W�i�����C�u����[myLib.h.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "myLib.h"
#include "debugproc.h"

//=================================================================================
// �X�v���C��
//=================================================================================
//=====================================================
// ����������
//=====================================================
void CSpline::Init(const std::vector<double>& x, const std::vector<double>& y)
{
    // �����̎󂯎��
    this->x = x;
    this->y = y;
    int nSize = x.size();

    // �z��̏�����
    a = y;
    b.resize(nSize, 0.0);
    c.resize(nSize, 0.0);
    d.resize(nSize, 0.0);

    // ��Ԃ̋����v�Z
    std::vector<double> h(nSize - 1);
    for (int i = 0; i < nSize - 1; ++i)
    {
        h[i] = x[i + 1] - x[i];
    }

    // �O�d�Ίp�n�̃Z�b�g�A�b�v
    std::vector<double> alpha(nSize - 1);

    for (int i = 1; i < nSize - 1; ++i)
    {
        alpha[i] = 3.0 / h[i] * (a[i + 1] - a[i]) - 3.0 / h[i - 1] * (a[i] - a[i - 1]);
    }

    // c�̎O�d�Ίp�n������
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
// �����̌v�Z
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
// �f�X�g���N�^
//=====================================================
CCutMullSpline::~CCutMullSpline()
{
    m_vPos.clear();
}

//=====================================================
// ����������
//=====================================================
void CCutMullSpline::Init(const std::vector<D3DXVECTOR3> vPos)
{
    // �z��̃��T�C�Y
    int nSize = vPos.size();
    m_vPos.resize(nSize + 2);

    for (int i = 0; i < nSize; i++)
    {// �����̎󂯎��
        m_vPos[i + 1] = vPos[i];
    }

    // �_�~�[�̍ŏ��ƍŌ�̈ʒu����ǉ�
    m_vPos[0] = vPos[0];
    //m_vPos[0].x -= 50.0f;
    m_vPos[nSize + 1] = vPos[nSize - 1];
    //m_vPos[nSize + 1].x += 50.0f;
}

//=====================================================
// ���
//=====================================================
D3DXVECTOR3 CCutMullSpline::Interpolate(float t,int nIdx)
{
    D3DXVECTOR3 a1, a2, a3, a4;
    D3DXVECTOR3 pos;

    // a4�̌���
    a4 = m_vPos[nIdx];

    // a3�̌���
    a3 = (m_vPos[nIdx] - m_vPos[nIdx - 1]) / 2;

    // a1�̌���
    a1 = ((m_vPos[nIdx + 2] - m_vPos[nIdx]) / 2) - 2 * m_vPos[nIdx + 1] + a3 + 2 * a4;

    // a2�̌���
    a2 = 3 * m_vPos[nIdx + 1] - ((m_vPos[nIdx + 2] - m_vPos[nIdx]) / 2) - 2 * a3 - 3 * a4;

    // �v�Z���ʂ�Ԃ�
    pos = (a1 * t * t * t) + (a2 * t * t) + (a3 * t) + a4;

    return pos;
}

//=====================================================
// �Ȑ��̒����̎擾
//=====================================================
float CCutMullSpline::GetLength(int nIdx, int nDiv)
{
    if (nDiv <= 0)
        assert(("�X�v���C���̒����擾�ŕs���ȕ���������͂��Ă��܂�",false)); // ��������0��菭�Ȃ��G���[

    float fLengthResult = 0;

    for (int i = 1; i < nDiv; i++)
    {// �敪���Ƃɍ����������擾���ĉ��Z����
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