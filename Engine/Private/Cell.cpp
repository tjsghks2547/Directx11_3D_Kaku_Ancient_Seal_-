#include "Cell.h"
#include "VIBuffer_Cell.h"

CCell::CCell(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :m_pDevice(pDevice)
    ,m_pContext(pContext)
{
    Safe_AddRef(m_pDevice);
    Safe_AddRef(m_pContext);        
}

HRESULT CCell::Initialize(const _float3* pPoints, _int iIndex)
{
    memcpy(m_vPoints, pPoints, sizeof(_float3) * POINT_END);

    m_iIndex = iIndex; 

    XMStoreFloat4(&m_vPlane,
        XMPlaneFromPoints(XMLoadFloat3(&pPoints[POINT_A]), XMLoadFloat3(&pPoints[POINT_B]), XMLoadFloat3(&pPoints[POINT_C])));


#ifdef _DEBUG
    m_pVIBuffer = CVIBuffer_Cell::Create(m_pDevice, m_pContext, m_vPoints); 
    if (nullptr == m_pVIBuffer)
        return E_FAIL;
#endif 

    return S_OK;
}

_bool CCell::isIn(_fvector vPosition, _int* pNeighborIndex)
{
    for(size_t i =0; i< LINE_END; i++)
    {
        _vector  vDir  = XMVector3Normalize(vPosition - XMLoadFloat3(&m_vPoints[i]));
        _vector  vLine = XMLoadFloat3(&m_vPoints[(i + 1) % 3]) - XMLoadFloat3(&m_vPoints[i]); // �ð���� ( 0�� �� �� �ﰢ�� ������ ����) 
        _vector  vNoraml = XMVector3Normalize(XMVectorSet(XMVectorGetZ(vLine) * -1.f, 0.f, XMVectorGetX(vLine), 0.f));
       
        if(XMVectorGetX(XMVector3Dot(vDir, vNoraml)) > 0) // ������ ���� 0���� ������ ����  0���� ũ�� �ܺ�
        {
            *pNeighborIndex = m_iNeighborIndices[i]; // i =0 �� �� ����AB�� �ش� �÷��̾��� ���⺤�� �� �����ؼ� 0���� ������  �̿� 
            return false;
        }
    }
    
    return true;    
}

_bool CCell::Compare_Points(_fvector vSourPoint, _fvector vDestPoint)
{
    //XMVectorEqual() �� �ϳ��ϳ� ������ �� ��� x,y,z,w�� ���ؼ� ������ 1 �Ȱ����� 0�� ��ȯ

    if (true == XMVector3Equal(vSourPoint, XMLoadFloat3(&m_vPoints[POINT_A])))
    {
        if (true == XMVector3Equal(vDestPoint, XMLoadFloat3(&m_vPoints[POINT_B])))
            return true;
        if (true == XMVector3Equal(vDestPoint, XMLoadFloat3(&m_vPoints[POINT_C])))
            return true;
    }

    if (true == XMVector3Equal(vSourPoint, XMLoadFloat3(&m_vPoints[POINT_B])))
    {
        if (true == XMVector3Equal(vDestPoint, XMLoadFloat3(&m_vPoints[POINT_C])))
            return true;
        if (true == XMVector3Equal(vDestPoint, XMLoadFloat3(&m_vPoints[POINT_A])))
            return true;
    }

    if (true == XMVector3Equal(vSourPoint, XMLoadFloat3(&m_vPoints[POINT_C])))  
    {
        if (true == XMVector3Equal(vDestPoint, XMLoadFloat3(&m_vPoints[POINT_A])))  
            return true;
        if (true == XMVector3Equal(vDestPoint, XMLoadFloat3(&m_vPoints[POINT_B])))
            return true;
    }

    return false;   

}

_float CCell::Compute_Height(_fvector vPosition)
{
    // ax+ by + cz + d =0;
    // y = (-ax - cz -d ) / b 

    return (-m_vPlane.x * vPosition.m128_f32[0] - m_vPlane.z * vPosition.m128_f32[2] - m_vPlane.w) / m_vPlane.y;    
}

#ifdef _DEBUG
HRESULT CCell::Render()
{
    m_pVIBuffer->Bind_InputAssembler(); 

    m_pVIBuffer->Render();  

    return S_OK;    
}

#endif

CCell* CCell::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _float3* pPoints, _int iIndex)
{
    CCell* pInstance = new CCell(pDevice, pContext);

    if(FAILED(pInstance->Initialize(pPoints, iIndex)))
    {
        MSG_BOX("Failed to Created : CCell");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CCell::Free()
{
    __super::Free();

#ifdef _DEBUG
    
    Safe_Release(m_pVIBuffer);

#endif // _DEBUG

    Safe_Release(m_pContext);   
    Safe_Release(m_pDevice);

}
