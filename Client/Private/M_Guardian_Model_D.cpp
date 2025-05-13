#include "pch.h"
#include "M_Guardian_Model_D.h"
#include "GameInstance.h"
#include "Animation.h"
#include "MonsterHpBar.h"
#include "MonsterStunBar.h"
#include "M_Guardian_Model_D_Arm.h"



CM_Guardian_Model_D::CM_Guardian_Model_D(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CMonster(pDevice, pContext)
{
}

CM_Guardian_Model_D::CM_Guardian_Model_D(const CM_Guardian_Model_D& Prototype)
	:CMonster(Prototype)
{
}

HRESULT CM_Guardian_Model_D::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CM_Guardian_Model_D::Initialize(void* pArg)
{
	/*if (m_szName == nullptr)
		strcpy_s(m_szName, "Prototype_GameObject_M_Guardian_Model_D");	

	_char* NoName = " ";*/	

	if (pArg != nullptr)	
	{
		CGameObject::GAMEOBJECT_DESC* pObjectDesc = static_cast<GAMEOBJECT_DESC*>(pArg);	
		{
			//if (strcmp(pObjectDesc->szName, NoName))	
			strcpy_s(m_szName, pObjectDesc->szName);	
		}

	}

	m_iHp = 10;	

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;


	m_pTransformCom->Set_Speed(10.f);

	m_iState = STATE_IDLE;

	m_pModelCom->SetUp_Animation(2, false);


	

	return S_OK;
}

void CM_Guardian_Model_D::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);

	if (m_bMonsterActive)
	{

		if (m_iState == STATE_DEAD)
		{
			m_fAccTimeDeltaDissove += fTimeDelta;
			if (m_fDissolveAmount > 1.f)
				m_fDissolveAmount = 1.f;
			else
			{
				m_fDissolveAmount += fTimeDelta * 0.4f;
			}
		}

		m_fAccTimeDelta += fTimeDelta;
		m_fTimeDelta = fTimeDelta;



		_vector pPlayerPos = m_pPlayer->Get_Transfrom()->Get_State(CTransform::STATE_POSITION);
		_vector MonsterPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

		float distance = XMVectorGetX(XMVector3Length(pPlayerPos - MonsterPos));


		_bool AnimationFinish = m_pModelCom->Get_VecAnimation().at(m_pModelCom->Get_Current_Animation_Index())->isAniMationFinish();


		if (m_iHp <= 0 && !m_bDead)
		{
			m_bDead = true;
			m_pModelCom->Get_VecAnimation().at(0)->SetLerpTime(5.f);
			m_pModelCom->Get_VecAnimation().at(1)->SetLerpTime(5.f);
			m_pModelCom->Get_VecAnimation().at(2)->SetLerpTime(5.f);
			m_pModelCom->Get_VecAnimation().at(3)->SetLerpTime(5.f);
			m_pModelCom->SetUp_Animation(2, true);
			m_iState = STATE_DEAD;
			m_fAnimationSpeed = 5.f;

		}

		else if (!m_bDead)
		{

			if (distance > 300.f)
			{
				//m_iState = 0;
				//m_iState = STATE_IDLE;
				m_pModelCom->SetUp_Animation(2, true);
				m_fAnimationSpeed = 1.f;
			}

			if (distance > 100.f && distance < 300.f && !m_bStart)
			{
				m_bStart = true;
				//m_iState = 0;
				//m_iState = STATE_IDLE;
				m_pModelCom->SetUp_Animation(4, false);
				m_fAnimationSpeed = 1.f;
			}


			/* 여기다가 어택 추가하면 되겠네 */

			else if (distance < 100.f && AnimationFinish)
			{

				m_iState = 0;
				m_iState = STATE_WALK;
				m_pModelCom->SetUp_Animation(3, true);
				m_fAnimationSpeed = 1.f;
			}

			else if (distance < 30.f && m_fAccTimeDelta > 12.0f)
			{
				m_iState = 0;
				m_iState = STATE_ATTACK;
				m_pModelCom->SetUp_Animation(0, false);
				m_fAccTimeDelta = 0.f;
				m_fAnimationSpeed = 1.f;
			}


			if (m_iState == STATE_WALK)
				m_fAccTimeDelta += fTimeDelta;
		}

		m_pMonsterHpBar->Priority_Update(fTimeDelta);
		m_pMonsterStunBar->Priority_Update(fTimeDelta);
	}
}

void CM_Guardian_Model_D::Update(_float fTimeDelta)
{
	//if (true == m_pModelCom->Play_Animation(fTimeDelta * m_fAnimationSpeed))
	//	int a = 10;

	_bool AnimationFinish = m_pModelCom->Get_VecAnimation().at(m_pModelCom->Get_Current_Animation_Index())->isAniMationFinish();




	_vector pPlayerPos = m_pPlayer->Get_Transfrom()->Get_State(CTransform::STATE_POSITION);
	_vector MonsterPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	
	_vector Dir = pPlayerPos - MonsterPos;
	
	
	_vector		vCurPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_vector		test = { 0.f,0.f,0.f,1.f };
	/* 루트 모션 애니메션 코드 */
	m_pRootMatrix = m_pModelCom->Get_RootMotionMatrix("Dummy001");
	
	
	if (m_iState != STATE_WALK )	
	{
		//m_pTransformCom->Go_Straight(fTimeDelta, m_pNavigationCom);
		if (!XMVector4Equal(XMLoadFloat4x4(m_pRootMatrix).r[3], test) && m_pModelCom->Get_LerpFinished())	
		{
			if (m_pNavigationCom->isMove(vCurPosition))	
					m_pTransformCom->Set_MulWorldMatrix(m_pRootMatrix);	
		}
	}

	if (true == m_pModelCom->Play_Animation(fTimeDelta * m_fAnimationSpeed))
		int a = 10;


	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, vCurPosition);
	_vector		vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);	
	
	
	if (m_iState == STATE_WALK)
	{
		if (m_pNavigationCom->isMove(m_pTransformCom->Get_State(CTransform::STATE_POSITION)))
		{
			//외적 구해야겟네 
			_vector vLook = XMVector2Normalize(_fvector{ m_pTransformCom->Get_State(CTransform::STATE_LOOK).m128_f32[0],m_pTransformCom->Get_State(CTransform::STATE_LOOK).m128_f32[2] });
			_vector vDir = XMVector2Normalize(_fvector{ XMVectorGetX(Dir),XMVectorGetZ(Dir) });	
	
			float dotResult = XMVectorGetX(XMVector2Dot(vLook, vDir));
	
			dotResult = max(-1.0f, min(dotResult, 1.0f));
			float Radian = acosf(dotResult);
	
			_vector crossResult = XMVector2Cross(vDir, vLook);
			float crossY = XMVectorGetY(crossResult);
			if (crossY < 0.0f) {
				Radian = -Radian;
			}
	
			if (fabsf(Radian) > XMConvertToRadians(0.1f))
				m_pTransformCom->Turn_Degree(_fvector{ 0.f,1.f,0.f,0.f }, Radian);
	
	
	
			m_pTransformCom->Go_Straight(fTimeDelta, m_pNavigationCom);
			//_vector		vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
			//m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetY(vPosition, m_pNavigationCom->Compute_Height(vPosition)));
	
		}
	}

	m_pColliderCom->Update(XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()));
	
	m_pMonsterArm->Update(fTimeDelta);		
	//m_pColliderAttackCom->Update(XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()));
	m_pMonsterHpBar->Update(fTimeDelta);	


}

void CM_Guardian_Model_D::Late_Update(_float fTimeDelta)
{	
	if (m_bMonsterActive)
	{
		m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this);
		m_pGameInstance->Add_RenderGroup(CRenderer::RG_SHADOW, this);
	}

	if (m_iState != STATE_DEAD)
	{
		m_pMonsterHpBar->Late_Update(fTimeDelta);
		m_pMonsterStunBar->Late_Update(fTimeDelta);
	}
}

HRESULT CM_Guardian_Model_D::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	_uint			iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (size_t i = 0; i < iNumMeshes; i++)
	{

		if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_DIFFUSE, "g_DiffuseTexture", 0)))
			return E_FAIL;

		if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_NORMALS, "g_NormalTexture", 0)))
			return E_FAIL;



		if (FAILED(m_pModelCom->Bind_BoneMatrices(m_pShaderCom, i, "g_BoneMatrices")))
			return E_FAIL;

		m_pShaderCom->Begin(1);
		m_pModelCom->Render(i);


	}

#ifdef _DEBUG
	m_pNavigationCom->Render();
	m_pColliderCom->Render();
	m_pMonsterArm->Render();
#endif 
	//m_pMonsterHpBar->Render();

	return S_OK;
}

HRESULT CM_Guardian_Model_D::Render_Shadow()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Bind_Shadow_Matrices(m_pShaderCom, "g_ViewMatrix", "g_ProjMatrix")))
		return E_FAIL;	

	_uint			iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (size_t i = 0; i < iNumMeshes; i++)
	{
		if (FAILED(m_pModelCom->Bind_BoneMatrices(m_pShaderCom, i, "g_BoneMatrices")))
			return E_FAIL;

		m_pShaderCom->Begin(2);
		m_pModelCom->Render(i);
	}

	return S_OK;
}

HRESULT CM_Guardian_Model_D::Ready_Components()
{
	/* Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxAnimMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_M_Guardian_Model_D"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	CNavigation::NAVIGATION_DESC   Desc{};


	//Desc.iCurrentCellIndex = 51;

	if(!strcmp(m_szName , "M_Guardian_Model_D_01"))
		Desc.iCurrentCellIndex = 226;	

	//Desc.iCurrentCellIndex = 0;
	/* Com_Navigation*/
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Navigation"),
		TEXT("Com_Navigation"), reinterpret_cast<CComponent**>(&m_pNavigationCom), &Desc)))
		return E_FAIL;

	/* Com_Collider */
	CBounding_Sphere::BOUNDING_SPHERE_DESC SphereDesc{};


	SphereDesc.fRadius = 10.f;
	SphereDesc.vCenter = _float3(0.f, SphereDesc.fRadius, 0.f);


	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_SPHERE"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &SphereDesc)))
		return E_FAIL;

	m_pColliderCom->Set_Collider_Name("M_Guardian_Model_D");
	m_pGameInstance->Add_ObjCollider(GROUP_TYPE::MONSTER, this);

	/* Com_HpBar */

	m_pMonsterHpBar = CMonsterHpBar::Create(m_pDevice, m_pContext);	
	m_pMonsterHpBar->SetMonsterPtr(this);	
	m_pMonsterHpBar->Initialize(nullptr);

	
	/* Com_StunBar */

	m_pMonsterStunBar = CMonsterStunBar::Create(m_pDevice, m_pContext);			
	m_pMonsterStunBar->SetMonsterPtr(this);		
	m_pMonsterStunBar->Initialize(nullptr);		



	/* Monster Parts*/

	m_pMonsterArm = CM_Guardian_Model_D_Arm::Create(m_pDevice, m_pContext);
	m_pMonsterArm->SetTransform(m_pTransformCom);	
	m_pMonsterArm->Set_Parent_State_Ptr(&m_iState);	
	//m_pGameInstance->Add_ObjCollider(GROUP_TYPE::MONSTER, m_pMonsterArm	);		


	/* Noise Texture */
	/* Com Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Noise"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}


void CM_Guardian_Model_D::OnCollisionEnter(CGameObject* _pOther)
{
	_bool AnimationFinish = m_pModelCom->Get_VecAnimation().at(m_pModelCom->Get_Current_Animation_Index())->isAniMationFinish();

	if (!strcmp(_pOther->Get_Name(), "PLAYER_WEAPON"))
	{
		if (m_iHp > 0)
		{
			
			m_iHp--; // 애니메이션이 한번 진행될때 한번만 데미지가 입혀지도록 해야함.    
			_vector vLook = XMVector3Normalize(m_pPlayer->Get_Transfrom()->Get_State(CTransform::STATE_LOOK));
			float pushStrength = 5.0f; // 밀리는 힘 크기		
			_vector pushDirection = vLook * pushStrength;

			_vector MonsterPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
			MonsterPos += pushDirection;

			// 새로운 위치 적용
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, MonsterPos);



		}
	}

	if (!strcmp(_pOther->Get_Name(), "PLAYER"))
	{
		

		_vector vLook = XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_LOOK));
		float pushStrength = 10.0f; // 밀리는 힘 크기	
		_vector pushDirection = vLook * pushStrength * m_fTimeDelta;

		_vector playerPos = m_pPlayer->Get_Transfrom()->Get_State(CTransform::STATE_POSITION);
		playerPos += pushDirection;

		// 새로운 위치 적용
		m_pPlayer->Get_Transfrom()->Set_State(CTransform::STATE_POSITION, playerPos);

	}


}

void CM_Guardian_Model_D::OnCollision(CGameObject* _pOther)
{
	_bool AnimationFinish = m_pModelCom->Get_VecAnimation().at(m_pModelCom->Get_Current_Animation_Index())->isAniMationFinish();

	if (!strcmp(_pOther->Get_Name(), "PLAYER"))
	{


		_vector vLook = XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_LOOK));
		float pushStrength = 10.0f; // 밀리는 힘 크기	
		_vector pushDirection = vLook * pushStrength * m_fTimeDelta;

		_vector playerPos = m_pPlayer->Get_Transfrom()->Get_State(CTransform::STATE_POSITION);
		playerPos += pushDirection;

		// 새로운 위치 적용
		m_pPlayer->Get_Transfrom()->Set_State(CTransform::STATE_POSITION, playerPos);

		if (m_iHp <= 0)
		{
			// 여기서 게임 오브젝트에서 삭제를 다뤄야겠네 
			if (AnimationFinish && m_iState == STATE_DEAD && m_bDead)
			{
				m_pGameInstance->Sub_ObjCollider(GROUP_TYPE::MONSTER, this);
				m_pGameInstance->Sub_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Monster"), this);
			}
		}

	}




}


void CM_Guardian_Model_D::OnCollisionExit(CGameObject* _pOther)
{
	int a = 4;
}



HRESULT CM_Guardian_Model_D::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_NoiseTexture", 0)))	
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_DissolveAmount", &m_fDissolveAmount, sizeof(_float))))	
		return E_FAIL;	
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Time", &m_fAccTimeDeltaDissove, sizeof(_float))))	
		return E_FAIL;


	return S_OK;
}

CM_Guardian_Model_D* CM_Guardian_Model_D::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CM_Guardian_Model_D* pInstance = new CM_Guardian_Model_D(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CM_Guardian_Model_D");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CM_Guardian_Model_D::Clone(void* pArg)
{
	CM_Guardian_Model_D* pInstance = new CM_Guardian_Model_D(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CM_Guardian_Model_D");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CM_Guardian_Model_D::Free()
{
	__super::Free();
	
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pMonsterArm);	

	Safe_Release(m_pMonsterHpBar);		
	Safe_Release(m_pMonsterStunBar);	

}
