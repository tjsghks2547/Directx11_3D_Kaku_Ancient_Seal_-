#pragma once

#include "Base.h"

/* ���� ������ ���� ��, ��������� �����Ѵ�. */
/* �ݺ����� ������ ���� �� ����� ����ĵ� ���Ͽ� �����Ѵ�. */
/* Set, Get�Լ��� ���� ����� ���� �Ǵ� ���t����. */

BEGIN(Engine)

class CPipeLine final : public CBase
{
public:
	enum D3DTRANSFORMSTATE {D3DTS_VIEW, D3DTS_PROJ, D3DTS_END };
private:
	CPipeLine();
	virtual ~CPipeLine() = default;

public:
	_float4x4 Get_Transform_Float4x4(D3DTRANSFORMSTATE eState) const {
		return m_TransformationMatrices[eState];
	}
	_matrix Get_Transform_Matrix(D3DTRANSFORMSTATE eState) const {
		return XMLoadFloat4x4(&m_TransformationMatrices[eState]);
	}
	_float4x4 Get_Transform_Float4x4_Inverse(D3DTRANSFORMSTATE eState) const {
		return m_TransformationInverseMatrices[eState];
	}
	_matrix Get_Transform_Matrix_Inverse(D3DTRANSFORMSTATE eState)const {
		return XMLoadFloat4x4(&m_TransformationInverseMatrices[eState]);
	}

	_float4 Get_CamPosition() const {
		return m_vCamPosition;
	}

public:
	void Set_Transform(D3DTRANSFORMSTATE eState, _fmatrix TransformMatrix) {
		XMStoreFloat4x4(&m_TransformationMatrices[eState], TransformMatrix);
	}

public:
	HRESULT Initialize();
	/* ���� ������ ���� ��, ��������� ������� ���ؼ� �����Ѵ�. */
	/* �ϴ� �迡 ī�޶� ��ġ�� ���س��´�. */
	void Update();

private:
	_float4x4				m_TransformationMatrices[D3DTS_END] = {};
	_float4x4				m_TransformationInverseMatrices[D3DTS_END] = {};
	_float4					m_vCamPosition = {};

public:
	static CPipeLine* Create();
	virtual void Free() override;	
};

END