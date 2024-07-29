//*****************************************************
//
// �I���W�i�����C�u����[myLib.h.h]
// Author:���R����
//
//*****************************************************

#ifndef _MYLIB_H_
#define _MYLIB_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "main.h"

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CSpline
{// ���R�X�v���C��
public:
    CSpline() {};
    void Init(const std::vector<double>& x, const std::vector<double>& y);

    double Interpolate(double xi,int nIdx = -1);

private:
    std::vector<double> x;
    std::vector<double> y;
    std::vector<double> a;
    std::vector<double> b;
    std::vector<double> c;
    std::vector<double> d;
};

class CCutMullSpline
{// �J�b�g�}���E�X�v���C��
public:
    CCutMullSpline() {};    // �R���X�g���N�^
    ~CCutMullSpline();    // �f�X�g���N�^

    // �����o�֐�
    void Init(const std::vector<D3DXVECTOR3> vPos); // ����������
    D3DXVECTOR3 Interpolate(float t, int nIdx);   // ��ԏ���
    bool IsEmpty(void) { return m_vPos.size() == 0; }; // �󂩂ǂ����̔���
    float GetLength(int nIdx,int nDiv);   // �Ȑ��̒����̎擾

private:
    // �����o�ϐ�
    std::vector<D3DXVECTOR3> m_vPos;    // �_�̔z��
};
#endif