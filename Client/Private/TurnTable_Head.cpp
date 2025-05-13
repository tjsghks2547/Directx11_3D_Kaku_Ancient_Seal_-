#include "pch.h"
#include "TurnTable_Head.h"
#include "Model.h"
#include "Shader.h"
#include "GameInstance.h"
#include "InterActionUi.h"
#include "Water.h"  
#include "Client_Defines.h" 


CTurnTable_Head::CTurnTable_Head(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CGameObject(pDevice, pContext)
{
}

CTurnTable_Head::CTurnTable_Head(const  CTurnTable_Head& Prototype)
    :CGameObject(Prototype)
{

}

HRESULT  CTurnTable_Head::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;




    return S_OK;
}

HRESULT  CTurnTable_Head::Initialize(void* pArg)
{
    if (m_szName == nullptr)
        strcpy_s(m_szName, "Prototype_GameObject_TurnTable_Head");

    _char* NoName = " ";    

    if (pArg != nullptr)    
    {
        CGameObject::GAMEOBJECT_DESC* pObjectDesc = static_cast<GAMEOBJECT_DESC*>(pArg);    
        {
            if (strcmp(pObjectDesc->szName, NoName))    
                strcpy_s(m_szName, pObjectDesc->szName);    
        }   

    }   

    if (FAILED(__super::Initialize(pArg)))  
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_pTransformCom->Set_RotationSpeed(1.f);
    //m_pTransformCom->Scaling(_float3{ 0.001f,0.001f,0.001f });

    m_fAngle = -1.f;



    if (!strcmp(m_szName, "Handle_3"))
    {
        if (FAILED(m_pGameInstance->Add_EventObject(TEXT("Stage_3"), this)))    
            return E_FAIL;

        m_pTransformCom->Set_Speed(10.f);   
    }

    return S_OK;
}

void  CTurnTable_Head::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);
}

void  CTurnTable_Head::Update(_float fTimeDelta)
{
    //m_pModelCom->Play_Animation(fTimeDelta);
    __super::Update(fTimeDelta);

    m_fTimeDelta = fTimeDelta;

    if (m_fAccTimeDelta > 6.f)
    {
        if (!m_bStage_1)/* 딱 한번이여야함 시발 뭐함 너 왜이렇게 코드짬 이새끼 ㅇㅇ;;*/
        {
            m_pGameInstance->Event_Activate(TEXT("Stage_1"));
            m_bStage_1 = true;  
        }
    }

    if (m_fAccTimeDeltaHandle2 > 6.f)
        if (!m_bStage_2_Event2)
        {
            m_pGameInstance->Event_Activate(TEXT("Stage_2_Event2"));
            m_bStage_2_Event2 = false; 
        }

    if (m_fAccTimeDeltaHandle3 > 0.1f)
        if (!m_bStage_3)
        {
            m_pGameInstance->Event_Activate(TEXT("Stage_3"));
            m_bStage_3 = true; 
        }
       

    if (m_bTriggerOnOff)        
        IsTriggerOn(TEXT("Stage_3"));       


    m_pColliderCom->Update(XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()));   



}

void  CTurnTable_Head::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this);
    if (m_isCollision) // 접근시에만 Ui 나타나도록 설정 
        m_pInterActionUi->Late_Update(fTimeDelta);
}

HRESULT  CTurnTable_Head::Render()
{
    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    _uint  iNumMeshes = m_pModelCom->Get_NumMeshes();

    for (size_t i = 0; i < iNumMeshes; i++)
    {
        if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_DIFFUSE, "g_DiffuseTexture", 0)))
            return E_FAIL;

        if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_NORMALS, "g_NormalTexture", 0)))
            return E_FAIL;

        m_pShaderCom->Begin(0);

        m_pModelCom->Render(i);
    }


    //m_pColliderCom->Render();

    

    return S_OK;
}

void CTurnTable_Head::OnCollisionEnter(CGameObject* _pOther)
{
}

void CTurnTable_Head::OnCollision(CGameObject* _pOther)
{
    if (!strcmp(m_szName, "Handle_1"))
    {
        CTransform* pPlayerTransfrom = _pOther->Get_Transfrom();
        _vector     vPlayerPos = pPlayerTransfrom->Get_State(CTransform::STATE_POSITION);
        _vector     vPlayerDir = pPlayerTransfrom->Get_State(CTransform::STATE_LOOK);
        _vector     vPivot = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

        // _vector     vDir =  vPlayerPos - vCurObjectPos; 


        //_float      vLength = XMVectorGetX(XMVector2Length(vDir));  

        _float      radius = 11.f;
        _float      fspeed = 1.f;
        //m_fangle = 0.f; 

        m_isCollision = true;

        if (GetKeyState('G') & 0x8000 && GetKeyState('W') & 0x8000)
        {
            m_fAngle += fspeed * m_fTimeDelta;

            float x = vPivot.m128_f32[0] + radius * cos(m_fAngle);
            float z = vPivot.m128_f32[2] + radius * sin(m_fAngle);

            vPlayerPos.m128_f32[0] = x;
            vPlayerPos.m128_f32[2] = z;

            pPlayerTransfrom->Set_RotationSpeed(1.f);
            pPlayerTransfrom->Turn(_fvector{ 0.f,-1.f,0.f,0.f }, m_fTimeDelta);

            pPlayerTransfrom->Set_State(CTransform::STATE_POSITION, vPlayerPos);

            m_pTransformCom->Turn(_fvector{ 0.f,-1.f,0.f,0.f }, m_fTimeDelta);

            m_fAccTimeDelta += m_fTimeDelta;

        }
    }


    else if (!strcmp(m_szName, "Handle_2"))
    {
        CTransform* pPlayerTransfrom = _pOther->Get_Transfrom();    
        _vector     vPlayerPos = pPlayerTransfrom->Get_State(CTransform::STATE_POSITION);   
        _vector     vPlayerDir = pPlayerTransfrom->Get_State(CTransform::STATE_LOOK);   
        _vector     vPivot = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
            
    
        _float      radius = 11.f;
        _float      fspeed = 1.f;

        m_isCollision = true;   

        if (GetKeyState('G') & 0x8000 && GetKeyState('W') & 0x8000) 
        {
            m_fAngle += fspeed * m_fTimeDelta;  

            float x = vPivot.m128_f32[0] + radius * cos(m_fAngle);  
            float z = vPivot.m128_f32[2] + radius * sin(m_fAngle);  

            vPlayerPos.m128_f32[0] = x; 
            vPlayerPos.m128_f32[2] = z; 

            pPlayerTransfrom->Set_RotationSpeed(1.f);
            pPlayerTransfrom->Turn(_fvector{ 0.f,-1.f,0.f,0.f }, m_fTimeDelta); 

            pPlayerTransfrom->Set_State(CTransform::STATE_POSITION, vPlayerPos);    

            m_pTransformCom->Turn(_fvector{ 0.f,-1.f,0.f,0.f }, m_fTimeDelta);  

            m_fAccTimeDeltaHandle2 += m_fTimeDelta; 

        }
    }


    else if (!strcmp(m_szName, "Handle_3"))
    {
        CTransform* pPlayerTransfrom = _pOther->Get_Transfrom();
        _vector     vPlayerPos = pPlayerTransfrom->Get_State(CTransform::STATE_POSITION);
        _vector     vPlayerDir = pPlayerTransfrom->Get_State(CTransform::STATE_LOOK);
        _vector     vPivot = m_pTransformCom->Get_State(CTransform::STATE_POSITION);


        _float      radius = 11.f;
        _float      fspeed = 1.f;

        m_isCollision = true;

        if (GetKeyState('G') & 0x8000 && GetKeyState('W') & 0x8000)
        {
            m_fAngle += fspeed * m_fTimeDelta;

            float x = vPivot.m128_f32[0] + radius * cos(m_fAngle);
            float z = vPivot.m128_f32[2] + radius * sin(m_fAngle);

            vPlayerPos.m128_f32[0] = x;
            vPlayerPos.m128_f32[2] = z;

            pPlayerTransfrom->Set_RotationSpeed(1.f);
            pPlayerTransfrom->Turn(_fvector{ 0.f,-1.f,0.f,0.f }, m_fTimeDelta);

            pPlayerTransfrom->Set_State(CTransform::STATE_POSITION, vPlayerPos);

            m_pTransformCom->Turn(_fvector{ 0.f,-1.f,0.f,0.f }, m_fTimeDelta);

            m_fAccTimeDeltaHandle3 += m_fTimeDelta;

        }


        
    }

}

void CTurnTable_Head::OnCollisionExit(CGameObject* _pOther)
{
    m_isCollision = false;  
    m_fAngle = -1.f;
}

HRESULT  CTurnTable_Head::Ready_Components()
{
    /* Com_Shader */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxMesh"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    /* Com_Model */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_TurnTable_Head"),
        TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
        return E_FAIL;

    /* Com_InterAction_UI */
    m_pInterActionUi = CInterActionUi::Create(m_pDevice, m_pContext);
    m_pInterActionUi->Initialize(nullptr);  

    /* Com_Collider */
    CBounding_Sphere::BOUNDING_SPHERE_DESC SphereDesc{};


    SphereDesc.fRadius = 200.f; 
    SphereDesc.vCenter = _float3(0.f, SphereDesc.fRadius-30.f, 0.f); 


    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_SPHERE"),
        TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &SphereDesc)))
        return E_FAIL;


    m_pColliderCom->Set_Collider_Name("TurnTable_Head");    
    m_pGameInstance->Add_ObjCollider(GROUP_TYPE::OBJECT, this); 

    return S_OK;    
}

HRESULT  CTurnTable_Head::Bind_ShaderResources()
{
    if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
        return E_FAIL;


    return S_OK;
}

void CTurnTable_Head::IsTriggerOn(_wstring _EventName)  
{
    _vector vCurPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);


    if (!strcmp(m_szName, "Handle_3") && _EventName == TEXT("Stage_3"))     
    {
        m_bTriggerOnOff = true; 

        m_fMoveHeightDistance -= m_fTimeDelta * 5.f;     // 이것도 수정  

        if (m_fMoveHeightDistance > 0.1f)       
        {
            m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetY(vCurPos, vCurPos.m128_f32[1] + m_fTimeDelta * 5.f));
        }
        else
            m_bTriggerOnOff = false;    

    }


}

CTurnTable_Head* CTurnTable_Head::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CTurnTable_Head* pInstance = new  CTurnTable_Head(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : TurnTable_Head");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CTurnTable_Head::Clone(void* pArg)
{
    CTurnTable_Head* pInstance = new  CTurnTable_Head(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : TurnTable_Head");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void  CTurnTable_Head::Free()
{
    __super::Free();
    
    Safe_Release(m_pColliderCom);
    Safe_Release(m_pInterActionUi);
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pModelCom);
}
