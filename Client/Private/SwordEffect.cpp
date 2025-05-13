#include "pch.h"    
#include "SwordEffect.h"   
#include "GameInstance.h"
#include "Shader.h"
#include "Client_Defines.h" 

CSwordEffect::CSwordEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CGameObject(pDevice, pContext) 
{
}




HRESULT CSwordEffect::Initialize(void* pArg)
{

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;


    //m_pTransformCom->Scaling(_float3{ 10.f,10.f,10.f });

    return S_OK;    
}

void CSwordEffect::Priority_Update(_float fTimeDelta)
{

}

void CSwordEffect::Update(_float fTimeDelta)
{

}

void CSwordEffect::Late_Update(_float fTimeDelta)
{
    /* ���⼭ ���� ������Ʈ ����߰ڴ�. */

    if (m_vecTrailPoint.size() > 1)
    {
        // �׷��� ������ ���� // 
        _uint NumDrawBuffer =  m_vecTrailPoint.size();       
        
   

        for(_uint i=0; i< NumDrawBuffer-1; i++)
        {
            const TrailPoint& p0 = m_vecTrailPoint[i];      
            const TrailPoint& p1 = m_vecTrailPoint[i + 1];  

            // ���� 4�� ����
            // v0: tip_i
            // v1: hilt_i
            // v2: tip_(i+1)
            // v3: hilt_(i+1)

            TrailVertex v0, v1, v2, v3; 

            v0.position = p0.tip;   
            v1.position = p0.hilt;  
            v2.position = p1.tip;       
            v3.position = p1.hilt;  

            // �����ϰ� UV�� age ����(����)
            // ���� �������� v=0~1, �ʺ� �������� u=0 �Ǵ� u=1 ��
            // �Ǵ� tip/hilt�� 0,1 �־ ��.

            // ù��° ���׸�Ʈ(i=0)���� ���������� ���� ���ϴ� ȿ��(���� ��)�� �ְ� �ʹٸ�
            // age�� �״�� �ΰų�, ���� ��� ���� �����ص� �˴ϴ�.

            float u0 = 0.0f; // tip 
            float u1 = 1.0f; // hilt    
            float v0Tex = (float)i / (float)(NumDrawBuffer - 1);    
            float v1Tex = (float)(i + 1) / (float)(NumDrawBuffer - 1);  

            v0.texcoord = XMFLOAT2(u0, v0Tex);    
            v1.texcoord = XMFLOAT2(u1, v0Tex);      
            v2.texcoord = XMFLOAT2(u0, v1Tex);      
            v3.texcoord = XMFLOAT2(u1, v1Tex);      

            // age: ������ ���׸�Ʈ ������ ���� �������� �ʡ� ����� �ְų�
            //      tip/hilt ���� age �� �־ ��.
            // ���⼭�� p0�� p1�� �߰��� ������ �� ���� ����.
            float avgAge = (p0.age + p1.age) * 0.5f;    

            v0.age = avgAge;        
            v1.age = avgAge;
            v2.age = avgAge;
            v3.age = avgAge;

            // �ε��� ���: v0, v1, v2, v2, v1, v3
            // ������� ���� ���� ���� = vertices.size()

            UINT baseIndex = static_cast<UINT>(m_vecVertices.size());        
            // �ﰢ�� �� ��: (0,1,2), (2,1,3)    
            m_vecIndices.push_back(baseIndex + 0);       
            m_vecIndices.push_back(baseIndex + 1);      
            m_vecIndices.push_back(baseIndex + 2);      
                 
            m_vecIndices.push_back(baseIndex + 2);       
            m_vecIndices.push_back(baseIndex + 1);  
            m_vecIndices.push_back(baseIndex + 3);  

            // ���� vertices�� v0,v1,v2,v3 �߰�
            m_vecVertices.push_back(v0);    
            m_vecVertices.push_back(v1);    
            m_vecVertices.push_back(v2);    
            m_vecVertices.push_back(v3);    
        }


        m_NumVertices = m_vecVertices.size();   
        m_NumIndices = m_vecIndices.size(); 

        

        /* ���ؽ� ���� Map, UnMap */
        D3D11_MAPPED_SUBRESOURCE mappedResource;    
        ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));  

        HRESULT hr = m_pContext->Map(m_pViBufferCom->Get_VB_Buffer(), 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &mappedResource);
        if (FAILED(hr)) 
            int a = 4;

        VTXPOSTEXAGE* pVertices = reinterpret_cast<VTXPOSTEXAGE*>(mappedResource.pData);

        if (SUCCEEDED(hr))
        {
            memcpy(pVertices, m_vecVertices.data(), sizeof(TrailVertex) * m_NumVertices);
            m_pContext->Unmap(m_pViBufferCom->Get_VB_Buffer(), 0);
        }

        /* �ε��� ���� Map, UnMap */
        hr = m_pContext->Map(m_pViBufferCom->Get_IB_Buffer(), 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &mappedResource);
        if (FAILED(hr)) 
            int a = 4;  

        _uint* pIndices = reinterpret_cast<_uint*>(mappedResource.pData);   

        if (SUCCEEDED(hr))  
        {
            memcpy(pIndices, m_vecIndices.data(), sizeof(_uint) * m_NumIndices);    
            m_pContext->Unmap(m_pViBufferCom->Get_IB_Buffer(), 0);          
        }   
        
        m_pGameInstance->Add_RenderGroup(CRenderer::RG_BLEND, this);

    }

 
}

HRESULT CSwordEffect::Render()
{
    if(m_vecVertices.size() > 0  ||  m_vecIndices.size() > 0)
    {
        if (FAILED(Bind_ShaderResources()))
            return E_FAIL;

        m_pShaderCom->Begin(0);


        m_pViBufferCom->Bind_InputAssembler();
        m_pViBufferCom->Render_NumIndices(m_vecIndices.size());
        //m_pViBufferCom->Render();
    }
   
    return S_OK;    
}

void CSwordEffect::OnCollisionEnter(CGameObject* _pOther)
{
}

void CSwordEffect::OnCollision(CGameObject* _pOther)
{
}

void CSwordEffect::OnCollisionExit(CGameObject* _pOther)
{
}

HRESULT CSwordEffect::Ready_Components()
{
    /* Com_Model */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Sword_Trail_Buffer"),
        TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pViBufferCom))))     
        return E_FAIL;  

    /* Com_Shader */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxPosTexAge"),  
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    /* Com_Texture */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Sword_Trail"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))      
        return E_FAIL;          

    return S_OK;    
}

HRESULT CSwordEffect::Bind_ShaderResources()
{
    
    if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))    
        return E_FAIL;  

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW)))) 
        return E_FAIL;  
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ)))) 
        return E_FAIL;  

    if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))    
        return E_FAIL;  

    return S_OK;
}

HRESULT CSwordEffect::Add_Trail_Point(TrailPoint _pTrailPoint)
{
    m_vecTrailPoint.push_back(_pTrailPoint);        

    return S_OK;        
}

void CSwordEffect::ReSet_Trail()
{
    if (m_vecTrailPoint.size() > 1)
    {
        /* ���ؽ� ���� Map, UnMap */
        D3D11_MAPPED_SUBRESOURCE mappedResource;
        ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

        HRESULT hr = m_pContext->Map(m_pViBufferCom->Get_VB_Buffer(), 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &mappedResource);
        if (FAILED(hr))
            int a = 4;

        VTXPOSTEXAGE* pVertices = reinterpret_cast<VTXPOSTEXAGE*>(mappedResource.pData);

        if (SUCCEEDED(hr))
        {
            ZeroMemory(mappedResource.pData, sizeof(TrailVertex) * m_NumVertices);
            m_pContext->Unmap(m_pViBufferCom->Get_VB_Buffer(), 0);
        }

        /* �ε��� ���� Map, UnMap */
        hr = m_pContext->Map(m_pViBufferCom->Get_IB_Buffer(), 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &mappedResource);
        if (FAILED(hr))
            int a = 4;

        _uint* pIndices = reinterpret_cast<_uint*>(mappedResource.pData);

        if (SUCCEEDED(hr))
        {
            ZeroMemory(mappedResource.pData, sizeof(_uint) * m_NumIndices);
            m_pContext->Unmap(m_pViBufferCom->Get_IB_Buffer(), 0);
        }

        m_vecTrailPoint.clear();
        m_NumVertices = 0;
        m_NumIndices = 0;

        m_vecIndices.clear();
        m_vecVertices.clear();
    }
}

CSwordEffect* CSwordEffect::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CSwordEffect* pInstance = new CSwordEffect(pDevice, pContext); 

    if (FAILED(pInstance->Initialize(nullptr)))     
    {
        MSG_BOX("Failed to Created : CSword_Effect");   
        Safe_Release(pInstance);    
    }

    return pInstance;   
}

CGameObject* CSwordEffect::Clone(void* pArg)    
{
    return nullptr; 
}

void CSwordEffect::Free()
{
    __super::Free();

    Safe_Release(m_pTextureCom);
    Safe_Release(m_pViBufferCom);
    Safe_Release(m_pShaderCom);
}
