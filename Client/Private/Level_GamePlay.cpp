#include "pch.h" 
#include "..\Public\Level_GamePlay.h"
#include "GameInstance.h"
#include "Camera_Free.h"
#include "Layer.h"	
#include "HpBar.h"
#include "Stamina_Bar.h"
#include "SC_Object_StoneBall.h"
#include "SC_TotemB.h"
#include "Boss.h"
#include "S_HpFlower_Ui.h"
#include "FadeInOut.h"

CLevel_GamePlay::CLevel_GamePlay(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CLevel { pDevice, pContext }
{

}

HRESULT CLevel_GamePlay::Initialize()
{
	/*if (FAILED(Ready_Lights()))
		return E_FAIL;*/

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Structure(TEXT("Layer_Structure"))))
		return E_FAIL;	

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Monster(TEXT("Layer_Monster"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Effect(TEXT("Layer_Effect"))))	
		return E_FAIL;

	if (FAILED(Ready_Layer_UI(TEXT("Layer_UI"))))
		return E_FAIL;

	if (FAILED(Ready_Lights()))	
		return E_FAIL;	

	m_pGameInstance->CheckGroup(GROUP_TYPE::PLAYER,        GROUP_TYPE::MONSTER);		
	m_pGameInstance->CheckGroup(GROUP_TYPE::PLAYER_WEAPON, GROUP_TYPE::MONSTER);		
	m_pGameInstance->CheckGroup(GROUP_TYPE::PLAYER,		   GROUP_TYPE::OBJECT);	
	

	m_pGameInstance->PlayBGM(L"A_BGM_Temple.ogg", 0.2f);
	
	return S_OK;
}

void CLevel_GamePlay::Update(_float fTimeDelta)
{
	
}

HRESULT CLevel_GamePlay::Render() 
{
#ifdef _DEBUG
	SetWindowText(g_hWnd, TEXT("게임플레이 레벨입니다."));
#endif

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Lights()
{
	//m_pPlayer = m_pGameInstance->Get_Player_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Player"));	

	if (FAILED(m_pGameInstance->SetUp_ShadowLight(XMVectorSet(232.f, 200.f, 150.f, 1.f), XMVectorSet(232.f, 62.f, 60.f, 1.f),	
		XMConvertToRadians(60.f), g_iWinSizeX / static_cast<_float>(g_iWinSizeY), 0.1f, 800.f	
		, m_pGameInstance->Get_Player_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Player")))))	
		return E_FAIL;

	LIGHT_DESC				LightDesc{};

	/* 2월 8일 빛 */
	LightDesc.eType = LIGHT_DESC::TYPE_DIRECTIONAL;
	LightDesc.vDirection = _float4(1.f, 1.f, 1.f, 0.f);
	LightDesc.vDiffuse   = _float4(0.9f, 0.9f, 0.9f, 1.f);
	LightDesc.vAmbient   = _float4(0.6f, 0.6f, 0.6f, 1.f);
	LightDesc.vSpecular  = _float4(0.3f, 0.3f, 0.3f, 1.f);


	/*LightDesc.eType = LIGHT_DESC::TYPE_DIRECTIONAL;	
	LightDesc.vDirection = _float4(1.f, 1.f, 1.f, 0.f);	
	LightDesc.vDiffuse   = _float4(0.9f, 0.9f, 0.9f, 1.f);	
	LightDesc.vAmbient   = _float4(0.5f, 0.5f, 0.5f, 1.f);	
	LightDesc.vSpecular  = _float4(0.3f, 0.3f, 0.3f, 1.f);	*/

	if (FAILED(m_pGameInstance->Add_Light(LightDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	//if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Terrain"), LEVEL_GAMEPLAY, pLayerTag, nullptr)))
	//	return E_FAIL;

	//for (size_t i = 0; i < 3; i++)
	//{
	//	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_ForkLift"), LEVEL_GAMEPLAY, pLayerTag, nullptr)))
	//		return E_FAIL;
	//}

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Sky"), LEVEL_GAMEPLAY, pLayerTag, nullptr)))
		return E_FAIL;



	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Structure(const _tchar* pLayerTag)
{
	/* 여기서 맵 파일 하나하나 다 읽어와야함 */

	_ulong dwByte = {}; 
	//HANDLE hFile = CreateFile(TEXT("../Map_File/real76.bin"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	HANDLE hFile = CreateFile(TEXT("../Map_File/real143.bin"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	while (true)
	{
		_tchar Layer_Name[MAX_PATH] = {}; //레이어 이름										
		ReadFile(hFile, Layer_Name, MAX_PATH, &dwByte, nullptr);

		if (dwByte == 0)
			break;
		/* 이름 작업 */
		_char   Prototype_Name[MAX_PATH] = {};

		ReadFile(hFile, Prototype_Name, MAX_PATH, &dwByte, nullptr);


		_float4x4 WorldMatrix = {};
		ReadFile(hFile, &WorldMatrix, sizeof(_float4x4), &dwByte, nullptr);
		//int a = 4;

		_tchar Translate_wchar[MAX_PATH] = {};
		MultiByteToWideChar(CP_ACP, 0, Prototype_Name, strlen(Prototype_Name), Translate_wchar, MAX_PATH);

		/* 이제 TRANSFORM만 건들면 될듯함.*/
		//int b = 4;
		if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, Translate_wchar, LEVEL_GAMEPLAY, Layer_Name)))
			return E_FAIL;

		//CTransform* pTrasnform = dynamic_cast<CTransform*>(
		//* Find GaemObject 만들어야 할듯
		// 구분할 수 있는 방법을 생각해봐야할듯.
		map<const _wstring, class CLayer*>* Level_Layers = m_pGameInstance->Get_Layers();		

		auto& Level_GamePlay = Level_Layers[3];

		for (auto& Layers : Level_GamePlay)
		{
			//auto& iter = find(Level_GamePlay.begin(), Level_GamePlay.end(), Layer_Name);	
			auto iter = Level_GamePlay.find(Layer_Name);

			if (iter == Level_GamePlay.end())
				return E_FAIL;

			else
			{
				CTransform* pTranform = dynamic_cast<CTransform*>(
					iter->second->Get_GameObject_List().back()->Find_Component(TEXT("Com_Transform")));	

				pTranform->Set_WorldMatrix(WorldMatrix);	
			}
		}
	}
	CloseHandle(hFile);
	
	CGameObject::GAMEOBJECT_DESC ObjectDesc{};	
	
#pragma region 깨지는 블럭들 
	ObjectDesc.fPosition = { -7.325f, 25.404f, 532.795f ,1.f};
	ObjectDesc.fScaling = { 0.15f,0.05f,0.15f };
	/* 깨지는 블럭들 설치 */
	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_ST_LiftTable_a2_sui_00"), LEVEL_GAMEPLAY, TEXT("Layer_Temple"), &ObjectDesc)))
		return E_FAIL;

	ObjectDesc.fPosition = { -7.325f, 25.404f, 570.295f ,1.f };
	ObjectDesc.fScaling = { 0.15f,0.05f,0.15f };
	/* 깨지는 블럭들 설치 */
	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_ST_LiftTable_a2_sui_00"), LEVEL_GAMEPLAY, TEXT("Layer_Temple"), &ObjectDesc)))	
		return E_FAIL;


	ObjectDesc.fPosition = { -7.325f, 25.404f, 607.964f ,1.f };
	ObjectDesc.fScaling = { 0.15f,0.05f,0.15f };
	/* 깨지는 블럭들 설치 */
	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_ST_LiftTable_a2_sui_00"), LEVEL_GAMEPLAY, TEXT("Layer_Temple"), &ObjectDesc)))	
		return E_FAIL;	

	ObjectDesc.fPosition = { -44.826f, 35.404f, 532.964f ,1.f };
	ObjectDesc.fScaling = { 0.15f,0.05f,0.15f };	
	/* 깨지는 블럭들 설치 */
	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_ST_LiftTable_a2_sui_00"), LEVEL_GAMEPLAY, TEXT("Layer_Temple"), &ObjectDesc)))
		return E_FAIL;

	ObjectDesc.fPosition = { -44.826f, 35.404f, 570.464f ,1.f };
	ObjectDesc.fScaling = { 0.15f,0.05f,0.15f };
	/* 깨지는 블럭들 설치 */
	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_ST_LiftTable_a2_sui_00"), LEVEL_GAMEPLAY, TEXT("Layer_Temple"), &ObjectDesc)))
		return E_FAIL;
		
	ObjectDesc.fPosition = { -44.826f, 35.404f, 607.964f ,1.f };
	ObjectDesc.fScaling = { 0.15f,0.05f,0.15f };	
	/* 깨지는 블럭들 설치 */
	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_ST_LiftTable_a2_sui_00"), LEVEL_GAMEPLAY, TEXT("Layer_Temple"), &ObjectDesc)))
		return E_FAIL;

	// 3번째 줄 
	ObjectDesc.fPosition = { -82.327f, 35.404f, 532.964f ,1.f };
	ObjectDesc.fScaling = { 0.15f,0.05f,0.15f };
	/* 깨지는 블럭들 설치 */
	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_ST_LiftTable_a2_sui_00"), LEVEL_GAMEPLAY, TEXT("Layer_Temple"), &ObjectDesc)))
		return E_FAIL;


	ObjectDesc.fPosition = { -82.327f, 35.404f, 570.464f ,1.f };
	ObjectDesc.fScaling = { 0.15f,0.05f,0.15f };
	/* 깨지는 블럭들 설치 */
	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_ST_LiftTable_a2_sui_00"), LEVEL_GAMEPLAY, TEXT("Layer_Temple"), &ObjectDesc)))
		return E_FAIL;

	ObjectDesc.fPosition = { -82.327f, 35.404f, 607.964f ,1.f };
	ObjectDesc.fScaling = { 0.15f,0.05f,0.15f };
	/* 깨지는 블럭들 설치 */
	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_ST_LiftTable_a2_sui_00"), LEVEL_GAMEPLAY, TEXT("Layer_Temple"), &ObjectDesc)))
		return E_FAIL;

	// 4번째 줄 

	ObjectDesc.fPosition = { -119.828f, 35.404f, 532.964f ,1.f };
	ObjectDesc.fScaling = { 0.15f,0.05f,0.15f };
	/* 깨지는 블럭들 설치 */
	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_ST_LiftTable_a2_sui_00"), LEVEL_GAMEPLAY, TEXT("Layer_Temple"), &ObjectDesc)))
		return E_FAIL;


	ObjectDesc.fPosition = { -119.828f, 35.404f, 570.464f ,1.f };
	ObjectDesc.fScaling = { 0.15f,0.05f,0.15f };
	/* 깨지는 블럭들 설치 */
	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_ST_LiftTable_a2_sui_00"), LEVEL_GAMEPLAY, TEXT("Layer_Temple"), &ObjectDesc)))
		return E_FAIL;

	ObjectDesc.fPosition = { -119.828f, 35.404f, 607.964f ,1.f };
	ObjectDesc.fScaling = { 0.15f,0.05f,0.15f };
	/* 깨지는 블럭들 설치 */
	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_ST_LiftTable_a2_sui_00"), LEVEL_GAMEPLAY, TEXT("Layer_Temple"), &ObjectDesc)))
		return E_FAIL;


	// 5번째 줄 
	ObjectDesc.fPosition = { -157.329f, 35.404f, 532.964f ,1.f };
	ObjectDesc.fScaling = { 0.15f,0.05f,0.15f };
	/* 깨지는 블럭들 설치 */
	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_ST_LiftTable_a2_sui_00"), LEVEL_GAMEPLAY, TEXT("Layer_Temple"), &ObjectDesc)))
		return E_FAIL;


	ObjectDesc.fPosition = { -157.329f, 35.404f, 570.464f ,1.f };
	ObjectDesc.fScaling = { 0.15f,0.05f,0.15f };
	/* 깨지는 블럭들 설치 */
	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_ST_LiftTable_a2_sui_00"), LEVEL_GAMEPLAY, TEXT("Layer_Temple"), &ObjectDesc)))
		return E_FAIL;

	ObjectDesc.fPosition = { -157.329f, 35.404f, 607.964f ,1.f };
	ObjectDesc.fScaling = { 0.15f,0.05f,0.15f };
	/* 깨지는 블럭들 설치 */
	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_ST_LiftTable_a2_sui_00"), LEVEL_GAMEPLAY, TEXT("Layer_Temple"), &ObjectDesc)))
		return E_FAIL;



	// 6번째 줄 
	ObjectDesc.fPosition = { -194.830f, 35.404f, 532.964f ,1.f };
	ObjectDesc.fScaling = { 0.15f,0.05f,0.15f };
	/* 깨지는 블럭들 설치 */
	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_ST_LiftTable_a2_sui_00"), LEVEL_GAMEPLAY, TEXT("Layer_Temple"), &ObjectDesc)))
		return E_FAIL;


	ObjectDesc.fPosition = { -194.830f, 35.404f, 570.464f ,1.f };
	ObjectDesc.fScaling = { 0.15f,0.05f,0.15f };
	/* 깨지는 블럭들 설치 */
	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_ST_LiftTable_a2_sui_00"), LEVEL_GAMEPLAY, TEXT("Layer_Temple"), &ObjectDesc)))
		return E_FAIL;

	ObjectDesc.fPosition = { -194.830f, 35.404f, 607.964f ,1.f };
	ObjectDesc.fScaling = { 0.15f,0.05f,0.15f };
	/* 깨지는 블럭들 설치 */
	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_ST_LiftTable_a2_sui_00"), LEVEL_GAMEPLAY, TEXT("Layer_Temple"), &ObjectDesc)))
		return E_FAIL;

	// 7번째 줄 

	ObjectDesc.fPosition = { -232.331f, 35.404f, 532.964f ,1.f };
	ObjectDesc.fScaling = { 0.15f,0.05f,0.15f };
	/* 깨지는 블럭들 설치 */
	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_ST_LiftTable_a2_sui_00"), LEVEL_GAMEPLAY, TEXT("Layer_Temple"), &ObjectDesc)))
		return E_FAIL;


	ObjectDesc.fPosition = { -232.331f, 35.404f, 570.464f ,1.f };
	ObjectDesc.fScaling = { 0.15f,0.05f,0.15f };
	/* 깨지는 블럭들 설치 */
	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_ST_LiftTable_a2_sui_00"), LEVEL_GAMEPLAY, TEXT("Layer_Temple"), &ObjectDesc)))
		return E_FAIL;

	ObjectDesc.fPosition = { -232.331f, 35.404f, 607.964f ,1.f };
	ObjectDesc.fScaling = { 0.15f,0.05f,0.15f };
	/* 깨지는 블럭들 설치 */
	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_ST_LiftTable_a2_sui_00"), LEVEL_GAMEPLAY, TEXT("Layer_Temple"), &ObjectDesc)))
		return E_FAIL;


	// 8번째 줄 


	ObjectDesc.fPosition = { -269.832f, 35.404f, 532.964f ,1.f };
	ObjectDesc.fScaling = { 0.15f,0.05f,0.15f };
	/* 깨지는 블럭들 설치 */
	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_ST_LiftTable_a2_sui_00"), LEVEL_GAMEPLAY, TEXT("Layer_Temple"), &ObjectDesc)))
		return E_FAIL;


	ObjectDesc.fPosition = { -269.832f, 35.404f, 570.464f ,1.f };
	ObjectDesc.fScaling = { 0.15f,0.05f,0.15f };
	/* 깨지는 블럭들 설치 */
	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_ST_LiftTable_a2_sui_00"), LEVEL_GAMEPLAY, TEXT("Layer_Temple"), &ObjectDesc)))
		return E_FAIL;

	ObjectDesc.fPosition = { -269.832f, 35.404f, 607.964f ,1.f };
	ObjectDesc.fScaling = { 0.15f,0.05f,0.15f };
	/* 깨지는 블럭들 설치 */
	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_ST_LiftTable_a2_sui_00"), LEVEL_GAMEPLAY, TEXT("Layer_Temple"), &ObjectDesc)))
		return E_FAIL;





	// 9번째 줄 


	ObjectDesc.fPosition = { -307.334f, 35.404f, 532.964f ,1.f };
	ObjectDesc.fScaling = { 0.15f,0.05f,0.15f };
	/* 깨지는 블럭들 설치 */
	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_ST_LiftTable_a2_sui_00"), LEVEL_GAMEPLAY, TEXT("Layer_Temple"), &ObjectDesc)))
		return E_FAIL;


	ObjectDesc.fPosition = { -307.334f, 35.404f, 570.464f ,1.f };
	ObjectDesc.fScaling = { 0.15f,0.05f,0.15f };
	/* 깨지는 블럭들 설치 */
	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_ST_LiftTable_a2_sui_00"), LEVEL_GAMEPLAY, TEXT("Layer_Temple"), &ObjectDesc)))
		return E_FAIL;

	ObjectDesc.fPosition = { -307.334f, 35.404f, 607.964f ,1.f };
	ObjectDesc.fScaling = { 0.15f,0.05f,0.15f };
	/* 깨지는 블럭들 설치 */
	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_ST_LiftTable_a2_sui_00"), LEVEL_GAMEPLAY, TEXT("Layer_Temple"), &ObjectDesc)))
		return E_FAIL;




	// 10번째 줄 

	ObjectDesc.fPosition = { -344.835f, 35.404f, 532.964f ,1.f };
	ObjectDesc.fScaling = { 0.15f,0.05f,0.15f };
	/* 깨지는 블럭들 설치 */
	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_ST_LiftTable_a2_sui_00"), LEVEL_GAMEPLAY, TEXT("Layer_Temple"), &ObjectDesc)))
		return E_FAIL;


	ObjectDesc.fPosition = { -344.835f, 35.404f, 570.464f ,1.f };
	ObjectDesc.fScaling = { 0.15f,0.05f,0.15f };
	/* 깨지는 블럭들 설치 */
	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_ST_LiftTable_a2_sui_00"), LEVEL_GAMEPLAY, TEXT("Layer_Temple"), &ObjectDesc)))
		return E_FAIL;

	ObjectDesc.fPosition = { -344.835f, 35.404f, 607.964f ,1.f };
	ObjectDesc.fScaling = { 0.15f,0.05f,0.15f };
	/* 깨지는 블럭들 설치 */
	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_ST_LiftTable_a2_sui_00"), LEVEL_GAMEPLAY, TEXT("Layer_Temple"), &ObjectDesc)))
		return E_FAIL;



	/* 마지막 돌 이벤트 발판 설치 */
	//ObjectDesc.fPosition = { -344.835f, 35.404f, 607.964f ,1.f };	
	//ObjectDesc.fScaling = { 0.15f,0.05f,0.15f };	
	///* 깨지는 블럭들 설치 */
	//if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_ST_LiftTable_a2_sui_00"), LEVEL_GAMEPLAY, TEXT("Layer_Temple"), &ObjectDesc)))
	//	return E_FAIL;
#pragma endregion 
#pragma region 물 요소 설치 
	/* 물 요소 설치 */
	ObjectDesc.fPosition = { -800.0f, -240.0f, 408.352f ,1.f };
	ObjectDesc.fScaling = { 1.5f ,1.0f ,1.5f };
	ObjectDesc.szName = "Water_Stage_01";		
	//ObjectDesc.fRotationPerSec =
	//_char* ObjectName = "Water_Test";
	
	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Water"), LEVEL_GAMEPLAY, TEXT("Layer_Test"),&ObjectDesc)))
		return E_FAIL;		


	/* 물 요소 설치 */
	ObjectDesc.fPosition = { -1200.0f, -175.0f, 500.f ,1.f };
	ObjectDesc.fScaling = { 1.0f ,1.0f ,1.3f };
	ObjectDesc.szName = "Water_Stage_02";
	//ObjectDesc.fRotationPerSec =
	//_char* ObjectName = "Water_Test";

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Water"), LEVEL_GAMEPLAY, TEXT("Layer_Test"), &ObjectDesc)))
		return E_FAIL;

#pragma endregion 
#pragma region 핸들 요소 설치 
	/* 핸들 요소 설치 */
	ObjectDesc.fPosition = { -359.421f, -226.473f, 646.744f ,1.f };
	ObjectDesc.fScaling = { 0.1f ,0.1f ,0.1f };
	ObjectDesc.szName = "Handle_1";
	

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_TurnTable_Head"), LEVEL_GAMEPLAY, TEXT("Layer_Control"), &ObjectDesc)))
		return E_FAIL;


	ObjectDesc.fPosition = { -969.125f, -141.208f, 865.543f ,1.f };
	ObjectDesc.fScaling = { 0.1f ,0.1f ,0.1f };
	ObjectDesc.szName = "Handle_2";


	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_TurnTable_Head"), LEVEL_GAMEPLAY, TEXT("Layer_Control"), &ObjectDesc)))
		return E_FAIL;

	ObjectDesc.fPosition = { -1015.263f, -174.918f, 1051.395f ,1.f };
	ObjectDesc.fScaling = { 0.1f ,0.1f ,0.1f };
	ObjectDesc.szName = "Handle_3";

		
	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_TurnTable_Head"), LEVEL_GAMEPLAY, TEXT("Layer_Control"), &ObjectDesc)))	
		return E_FAIL;	


#pragma endregion 
#pragma region 핸들 바디 설치 

	ObjectDesc.fPosition = { -1014.43f, -174.918f, 1051.395f ,1.f };	
	ObjectDesc.fScaling = { 0.1f ,0.1f ,0.1f };	
	ObjectDesc.szName = "Body_4";


	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_TurnTable_Body"), LEVEL_GAMEPLAY, TEXT("Layer_Control"), &ObjectDesc)))
		return E_FAIL;

#pragma endregion 
#pragma region 연꽃 발판 설치 
	/* 발판 설치 */

	ObjectDesc.fPosition = { -475.656f, -238.656f, 587.802f ,1.f };
	ObjectDesc.fScaling = { 0.1f ,0.1f ,0.1f };
	ObjectDesc.szName = "Water_Step_01";
	//ObjectDesc.fRotationPerSec =
	//_char* ObjectName = "Water_Test";

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SM_LotusFlower_07a1"), LEVEL_GAMEPLAY, TEXT("Layer_Control"), &ObjectDesc)))
		return E_FAIL;



	ObjectDesc.fPosition = { -545.745f, -238.656f, 543.126f ,1.f };
	ObjectDesc.fScaling = { 0.1f ,0.1f ,0.1f };
	ObjectDesc.szName = "Water_Step_02";

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SM_LotusFlower_07a1"), LEVEL_GAMEPLAY, TEXT("Layer_Control"), &ObjectDesc)))
		return E_FAIL;


	ObjectDesc.fPosition = { -631.298f, -238.656f, 595.470f ,1.f };
	ObjectDesc.fScaling = { 0.1f ,0.1f ,0.1f };
	ObjectDesc.szName = "Water_Step_03";

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SM_LotusFlower_07a1"), LEVEL_GAMEPLAY, TEXT("Layer_Control"), &ObjectDesc)))
		return E_FAIL;


	ObjectDesc.fPosition = { -696.388f, -238.656f, 543.279f ,1.f };
	ObjectDesc.fScaling = { 0.1f ,0.1f ,0.1f };
	ObjectDesc.szName = "Water_Step_04";

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SM_LotusFlower_07a1"), LEVEL_GAMEPLAY, TEXT("Layer_Control"), &ObjectDesc)))
		return E_FAIL;

#pragma endregion 
#pragma region Heavy_Box

	ObjectDesc.fPosition = { -1052.264f, -167.278f, 550.909f ,1.f };	
	ObjectDesc.fScaling = { 0.125f ,0.125f ,0.125f };	
	ObjectDesc.szName = "Heavy_Box_1";	

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SC_Stone_HeavyBOX_A_Mod"), LEVEL_GAMEPLAY, TEXT("Layer_Control"), &ObjectDesc)))	
		return E_FAIL;	

#pragma endregion 
#pragma region Box_Lift 
	ObjectDesc.fPosition = { -1027.431f, -170.642f, 550.909f ,1.f };		
	ObjectDesc.fScaling = { 0.063f ,0.063f ,0.063f };		
	ObjectDesc.szName = "Box_Lift_1";		

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Box_Lift"), LEVEL_GAMEPLAY, TEXT("Layer_Control"), &ObjectDesc)))
		return E_FAIL;	


	ObjectDesc.fPosition = { -1077.830f, -144.878f, 855.709f ,1.f };
	ObjectDesc.fScaling = { 0.063f ,0.063f ,0.063f };	
	ObjectDesc.szName = "Box_Lift_2";

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Box_Lift"), LEVEL_GAMEPLAY, TEXT("Layer_Control"), &ObjectDesc)))	
		return E_FAIL;	

#pragma endregion 
#pragma region LeverButton 
	ObjectDesc.fPosition = { -1050.597f, -166.943f, 527.176f ,1.f };
	ObjectDesc.fScaling = { 0.05f ,0.05f ,0.05f };	
	ObjectDesc.szName = "Lever_Button_1";

 	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_LeverButton"), LEVEL_GAMEPLAY, TEXT("Layer_Control"), &ObjectDesc)))
		return E_FAIL;


	ObjectDesc.fPosition = { -1050.033f, -125.477f, 892.089f ,1.f };
	ObjectDesc.fScaling = { 0.05f ,0.05f ,0.05f };	
	ObjectDesc.szName = "Lever_Button_2";

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_LeverButton"), LEVEL_GAMEPLAY, TEXT("Layer_Control"), &ObjectDesc)))	
		return E_FAIL;	


	/*ObjectDesc.fPosition = { 220.f, 60.f, 200.f ,1.f };
	ObjectDesc.fScaling = { 0.05f ,0.05f ,0.05f };
	ObjectDesc.szName = "Lever_Button_2";

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_LeverButton"), LEVEL_GAMEPLAY, TEXT("Layer_Control"), &ObjectDesc)))
		return E_FAIL;*/
#pragma endregion 
#pragma region SC_TempleDoor_05
	ObjectDesc.fPosition = { -1014.802f, -150.870f, 899.342f ,1.f };	
	ObjectDesc.fScaling = { 0.05f ,0.06f ,0.1f };	
	ObjectDesc.szName = "Temple_Door_01";	

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SC_TempleDoor_05"), LEVEL_GAMEPLAY, TEXT("Layer_Control"), &ObjectDesc)))	
		return E_FAIL;	

#pragma endregion 
#pragma region Moving Floor
	ObjectDesc.fPosition = { -1015.402f, -185.253f, 1053.062f ,1.f };	
	ObjectDesc.fScaling = { 0.075f ,0.075f ,0.075f };	
	ObjectDesc.szName = "Moving_Floor_01";		

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Moving_Floor"), LEVEL_GAMEPLAY, TEXT("Layer_Control"), &ObjectDesc)))	
		return E_FAIL;	

#pragma endregion 
#pragma region 떨어지는 돌들	

	CSC_Object_StoneBall::STONE_BALL_DESC StoneBallDesc{};

	/* 첫 번째 공자리 */
	StoneBallDesc.fPosition = { -488.785f, 204.432f, -435.003f ,1.f };	
	StoneBallDesc.fScaling = { 0.04f ,0.05f ,0.05f };	
	StoneBallDesc.startTime = 0.f;
	StoneBallDesc.LifeTime = 6.f; 
	StoneBallDesc.EventName = TEXT("Stone_Ball_Stage1");

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SC_Object_StoneBall"), LEVEL_GAMEPLAY, TEXT("Layer_Control"), &StoneBallDesc)))
		return E_FAIL;


	StoneBallDesc.fPosition = { -488.785f, 204.432f, -435.003f ,1.f };
	StoneBallDesc.fScaling = { 0.04f ,0.05f ,0.05f };
	StoneBallDesc.startTime = 7.f;
	StoneBallDesc.LifeTime = 13.f;
	StoneBallDesc.EventName = TEXT("Stone_Ball_Stage1");

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SC_Object_StoneBall"), LEVEL_GAMEPLAY, TEXT("Layer_Control"), &StoneBallDesc)))
		return E_FAIL;


	/* 두 번째 공자리 */
	StoneBallDesc.fPosition = { -526.829f, 204.432f, -435.003f ,1.f };
	StoneBallDesc.fScaling = { 0.04f ,0.05f ,0.05f };	
	StoneBallDesc.startTime = 0.5f;	
	StoneBallDesc.LifeTime = 6.5f;	
	StoneBallDesc.EventName = TEXT("Stone_Ball_Stage1");

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SC_Object_StoneBall"), LEVEL_GAMEPLAY, TEXT("Layer_Control"), &StoneBallDesc)))
		return E_FAIL;

	StoneBallDesc.fPosition = { -526.829f, 204.432f, -435.003f ,1.f };	
	StoneBallDesc.fScaling = { 0.04f ,0.05f ,0.05f };	
	StoneBallDesc.startTime = 4.5f;	
	StoneBallDesc.LifeTime = 10.5f;	
	StoneBallDesc.EventName = TEXT("Stone_Ball_Stage1");	

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SC_Object_StoneBall"), LEVEL_GAMEPLAY, TEXT("Layer_Control"), &StoneBallDesc)))
		return E_FAIL;

	StoneBallDesc.fPosition = { -526.829f, 204.432f, -435.003f ,1.f };
	StoneBallDesc.fScaling = { 0.04f ,0.05f ,0.05f };
	StoneBallDesc.startTime = 9.f;
	StoneBallDesc.LifeTime = 15.f;
	StoneBallDesc.EventName = TEXT("Stone_Ball_Stage1");

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SC_Object_StoneBall"), LEVEL_GAMEPLAY, TEXT("Layer_Control"), &StoneBallDesc)))
		return E_FAIL;

	/* 세 번째 공자리 */
	StoneBallDesc.fPosition = { -566.068f, 204.432f, -435.003f ,1.f };	
	StoneBallDesc.fScaling = { 0.04f ,0.05f ,0.05f };		
	StoneBallDesc.startTime = 1.f;		
	StoneBallDesc.LifeTime = 7.f;		
	StoneBallDesc.EventName = TEXT("Stone_Ball_Stage1");	

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SC_Object_StoneBall"), LEVEL_GAMEPLAY, TEXT("Layer_Control"), &StoneBallDesc)))
		return E_FAIL;	

	StoneBallDesc.fPosition = { -566.068f, 204.432f, -435.003f ,1.f };	
	StoneBallDesc.fScaling = { 0.04f ,0.05f ,0.05f };	
	StoneBallDesc.startTime = 4.f;	
	StoneBallDesc.LifeTime = 10.f;	
	StoneBallDesc.EventName = TEXT("Stone_Ball_Stage1");	

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SC_Object_StoneBall"), LEVEL_GAMEPLAY, TEXT("Layer_Control"), &StoneBallDesc)))
		return E_FAIL;


	StoneBallDesc.fPosition = { -566.068f, 204.432f, -435.003f ,1.f };
	StoneBallDesc.fScaling = { 0.04f ,0.05f ,0.05f };
	StoneBallDesc.startTime = 7.f;
	StoneBallDesc.LifeTime = 13.f;
	StoneBallDesc.EventName = TEXT("Stone_Ball_Stage1");

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SC_Object_StoneBall"), LEVEL_GAMEPLAY, TEXT("Layer_Control"), &StoneBallDesc)))
		return E_FAIL;


	/* 네 번째 공자리 */
	StoneBallDesc.fPosition = { -604.023f, 204.432f, -435.003f ,1.f };
	StoneBallDesc.fScaling = { 0.04f ,0.05f ,0.05f };
	StoneBallDesc.startTime = 1.5f;
	StoneBallDesc.LifeTime = 7.5f;
	StoneBallDesc.EventName = TEXT("Stone_Ball_Stage1");

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SC_Object_StoneBall"), LEVEL_GAMEPLAY, TEXT("Layer_Control"), &StoneBallDesc)))
		return E_FAIL;

	StoneBallDesc.fPosition = { -604.023f, 204.432f, -435.003f ,1.f };
	StoneBallDesc.fScaling = { 0.04f ,0.05f ,0.05f };
	StoneBallDesc.startTime = 3.5f;
	StoneBallDesc.LifeTime = 9.5f;
	StoneBallDesc.EventName = TEXT("Stone_Ball_Stage1");

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SC_Object_StoneBall"), LEVEL_GAMEPLAY, TEXT("Layer_Control"), &StoneBallDesc)))
		return E_FAIL;


	StoneBallDesc.fPosition = { -604.023f, 204.432f, -435.003f ,1.f };
	StoneBallDesc.fScaling = { 0.04f ,0.05f ,0.05f };
	StoneBallDesc.startTime = 9.f;
	StoneBallDesc.LifeTime = 15.f;
	StoneBallDesc.EventName = TEXT("Stone_Ball_Stage1");

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SC_Object_StoneBall"), LEVEL_GAMEPLAY, TEXT("Layer_Control"), &StoneBallDesc)))
		return E_FAIL;

	/* 다섯 번째 공자리 */
	StoneBallDesc.fPosition = { -643.499f, 204.432f, -435.003f ,1.f };
	StoneBallDesc.fScaling = { 0.04f ,0.05f ,0.05f };	
	StoneBallDesc.startTime = 3.f;	
	StoneBallDesc.LifeTime = 9.f;	
	StoneBallDesc.EventName = TEXT("Stone_Ball_Stage1");

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SC_Object_StoneBall"), LEVEL_GAMEPLAY, TEXT("Layer_Control"), &StoneBallDesc)))
		return E_FAIL;	

	StoneBallDesc.fPosition = { -643.499f, 204.432f, -435.003f ,1.f };
	StoneBallDesc.fScaling = { 0.04f ,0.05f ,0.05f };
	StoneBallDesc.startTime = 7.f;
	StoneBallDesc.LifeTime = 13.f;
	StoneBallDesc.EventName = TEXT("Stone_Ball_Stage1");

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SC_Object_StoneBall"), LEVEL_GAMEPLAY, TEXT("Layer_Control"), &StoneBallDesc)))
		return E_FAIL;

#pragma endregion 
#pragma region Npc 

	ObjectDesc.fPosition = { -636.515f, 40.649f, 185.957f ,1.f };
	ObjectDesc.fScaling = { 0.5f ,0.5f ,0.5f };
	ObjectDesc.szName = "Strong_Man_Npc_01";

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_StrongMan"), LEVEL_GAMEPLAY, TEXT("Layer_Control"), &ObjectDesc)))
		return E_FAIL;

#pragma endregion 
#pragma region 마지막 발판 위 덮개
	ObjectDesc.fPosition = { -564.243f, 166.563f,-370.724f ,1.f };
	ObjectDesc.fScaling = { 0.06f ,0.03f ,0.05f };
	ObjectDesc.szName = "Temple_Ground_01";	

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Temple_Ground"), LEVEL_GAMEPLAY, TEXT("Layer_Control"), &ObjectDesc)))
		return E_FAIL;
#pragma endregion 
#pragma region 마지막 돌 발판 
	ObjectDesc.fPosition = { -564.160f, 160.442f,-370.604f ,1.f };
	ObjectDesc.fScaling = { 0.06f ,0.06f ,0.05f };
	ObjectDesc.szName = "SC_StepBox_01";

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SC_StepBox"), LEVEL_GAMEPLAY, TEXT("Layer_Control"), &ObjectDesc)))
		return E_FAIL;
#pragma endregion
#pragma region SC_TotemB
	ObjectDesc.fPosition = { -559.491f, 310.635f, -652.746f ,1.f };
	ObjectDesc.fScaling = { 0.03f ,0.03f ,0.03f };
	ObjectDesc.szName = "SC_TotemB_01";

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SC_TotemB"), LEVEL_GAMEPLAY, TEXT("Layer_Control"), &ObjectDesc)))
		return E_FAIL;

#pragma endregion 

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Player(const _tchar* pLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Player"), LEVEL_GAMEPLAY, pLayerTag, nullptr)))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_HpBar"), LEVEL_GAMEPLAY, pLayerTag, nullptr)))	
		return E_FAIL;	

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Stamina_Bar"), LEVEL_GAMEPLAY, pLayerTag, nullptr)))
		return E_FAIL;
	

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Button_1"), LEVEL_GAMEPLAY, pLayerTag, nullptr)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Button_2"), LEVEL_GAMEPLAY, pLayerTag, nullptr)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Button_3"), LEVEL_GAMEPLAY, pLayerTag, nullptr)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Button_4"), LEVEL_GAMEPLAY, pLayerTag, nullptr)))
		return E_FAIL;


	//CUIObject::UIOBJECT_DESC UI_Desc{}; 

	//UI_Desc.fSizeX = 75.f;	
	//UI_Desc.fSizeY = 75.f;	
	//UI_Desc.fX =  100.f;	
	//UI_Desc.fY =  790.f;	
	//UI_Desc.fNear = 0.f;	
	//UI_Desc.fFar = 1.f;	
	//
	//UI_Desc.fSpeedPerSec = 5.f;	
	//UI_Desc.fRotationPerSec = XMConvertToRadians(90.f);	


	//if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_ItemBag"), LEVEL_GAMEPLAY, pLayerTag, &UI_Desc)))
	//	return E_FAIL;

	//UI_Desc.fSizeX = 75.f;
	//UI_Desc.fSizeY = 75.f;
	//UI_Desc.fX = 200.f;
	//UI_Desc.fY = 790.f;
	//UI_Desc.fNear = 0.f;
	//UI_Desc.fFar = 1.f;

	//UI_Desc.fSpeedPerSec = 5.f;
	//UI_Desc.fRotationPerSec = XMConvertToRadians(90.f);


	//if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_ItemBag"), LEVEL_GAMEPLAY, pLayerTag, &UI_Desc)))
	//	return E_FAIL;

	//UI_Desc.fSizeX = 75.f;
	//UI_Desc.fSizeY = 75.f;
	//UI_Desc.fX = 300.f;
	//UI_Desc.fY = 790.f;
	//UI_Desc.fNear = 0.f;
	//UI_Desc.fFar = 1.f;

	//UI_Desc.fSpeedPerSec = 5.f;
	//UI_Desc.fRotationPerSec = XMConvertToRadians(90.f);


	//if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_ItemBag"), LEVEL_GAMEPLAY, pLayerTag, &UI_Desc)))
	//	return E_FAIL;


	//UI_Desc.fSizeX = 75.f;
	//UI_Desc.fSizeY = 75.f;
	//UI_Desc.fX = 400.f;
	//UI_Desc.fY = 790.f;
	//UI_Desc.fNear = 0.f;
	//UI_Desc.fFar = 1.f;

	//UI_Desc.fSpeedPerSec = 5.f;
	//UI_Desc.fRotationPerSec = XMConvertToRadians(90.f);


	//if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_ItemBag"), LEVEL_GAMEPLAY, pLayerTag, &UI_Desc)))
	//	return E_FAIL;


	///* 아이템 ui (꽃, 버섯 ) */

	//UI_Desc.fSizeX = 70.f;	
	//UI_Desc.fSizeY = 70.f;	
	//UI_Desc.fX = 100.f;	
	//UI_Desc.fY = 790.f;	
	//UI_Desc.fNear = 0.f;	
	//UI_Desc.fFar = 1.f;	

	//UI_Desc.fSpeedPerSec = 5.f;
	//UI_Desc.fRotationPerSec = XMConvertToRadians(90.f);


	//if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_S_HpFlower_Ui"), LEVEL_GAMEPLAY, pLayerTag, &UI_Desc)))
	//	return E_FAIL;

	//UI_Desc.fSizeX = 70.f;
	//UI_Desc.fSizeY = 70.f;
	//UI_Desc.fX = 200.f;
	//UI_Desc.fY = 790.f;
	//UI_Desc.fNear = 0.f;
	//UI_Desc.fFar = 1.f;

	//UI_Desc.fSpeedPerSec = 5.f;
	//UI_Desc.fRotationPerSec = XMConvertToRadians(90.f);


	//if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SM_MushroomA_L_Ui"), LEVEL_GAMEPLAY, pLayerTag, &UI_Desc)))
	//	return E_FAIL;
	//

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Camera(const _tchar * pLayerTag)
{
	CCamera_Free::FREE_CAMERA_DESC		Desc = {};

	
	Desc.vEye = _float3(0.f, 10.f, -7.f);
	Desc.vAt = _float3(0.f, 0.f, 0.f);

	Desc.fFovy = XMConvertToRadians(60.f);
	Desc.fNear = 0.1f;
	Desc.fFar = 800.f;
	Desc.fMouseSensor = 0.05f;
	Desc.fSpeedPerSec = 25.f;
	Desc.fRotationPerSec = XMConvertToRadians(90.f);


	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Camera_Free"), LEVEL_GAMEPLAY, pLayerTag, &Desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Monster(const _tchar * pLayerTag)
{
	CGameObject::GAMEOBJECT_DESC ObjectDesc{};	
	
	ObjectDesc.fPosition = { -629.0f, 40.f, 521.0f ,1.f };	
	ObjectDesc.fScaling = { 1.f ,1.0f ,1.0f };		
	ObjectDesc.szName = "M_Guardian_Model_D_01";		
	
	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_M_Guardian_Model_D"), LEVEL_GAMEPLAY,pLayerTag, &ObjectDesc)))	
		return E_FAIL;	
	//
	//
	//
	ObjectDesc.fPosition = { -537.0f, 40.f, 529.0f ,1.f };
	ObjectDesc.fScaling = { 1.f ,1.0f ,1.0f };	
	ObjectDesc.szName = "M_Guardian_Model_C_01";
	
	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_M_Guardian_Model_C"), LEVEL_GAMEPLAY, pLayerTag, &ObjectDesc)))
		return E_FAIL;
		
	
	
	ObjectDesc.fPosition = { -567.0f, 40.f, 468.0f ,1.f };
	ObjectDesc.fScaling = { 1.f ,1.0f ,1.0f };
	ObjectDesc.szName = "M_Guardian_Model_A_01";
	
	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_M_Guardian_Model_A"), LEVEL_GAMEPLAY, pLayerTag, &ObjectDesc)))
		return E_FAIL;


	//CGameObject::GAMEOBJECT_DESC ObjectDesc{};	
	
	ObjectDesc.fPosition = { 232.654f, 88.017f, 1566.202f ,1.f };		
	ObjectDesc.fScaling = { 0.3f ,0.3f ,0.3f };			
	ObjectDesc.szName = "Boss";			
	
	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Boss"), LEVEL_GAMEPLAY,TEXT("Layer_Boss"), &ObjectDesc)))
		return E_FAIL;		

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Effect(const _tchar* pLayerTag)
{
	//if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Particle_Snow"), LEVEL_GAMEPLAY, pLayerTag)))
	//	return E_FAIL;	



	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_UI(const _tchar* pLayerTag)
{
	CUIObject::UIOBJECT_DESC UI_Desc{};

	UI_Desc.fSizeX = 75.f;
	UI_Desc.fSizeY = 75.f;
	UI_Desc.fX = 100.f;
	UI_Desc.fY = 790.f;
	UI_Desc.fNear = 0.f;
	UI_Desc.fFar = 1.f;

	UI_Desc.fSpeedPerSec = 5.f;
	UI_Desc.fRotationPerSec = XMConvertToRadians(90.f);


	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_ItemBag"), LEVEL_GAMEPLAY, pLayerTag, &UI_Desc)))
		return E_FAIL;

	UI_Desc.fSizeX = 75.f;
	UI_Desc.fSizeY = 75.f;
	UI_Desc.fX = 200.f;
	UI_Desc.fY = 790.f;
	UI_Desc.fNear = 0.f;
	UI_Desc.fFar = 1.f;

	UI_Desc.fSpeedPerSec = 5.f;
	UI_Desc.fRotationPerSec = XMConvertToRadians(90.f);


	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_ItemBag"), LEVEL_GAMEPLAY, pLayerTag, &UI_Desc)))
		return E_FAIL;

	UI_Desc.fSizeX = 75.f;
	UI_Desc.fSizeY = 75.f;
	UI_Desc.fX = 300.f;
	UI_Desc.fY = 790.f;
	UI_Desc.fNear = 0.f;
	UI_Desc.fFar = 1.f;

	UI_Desc.fSpeedPerSec = 5.f;
	UI_Desc.fRotationPerSec = XMConvertToRadians(90.f);


	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_ItemBag"), LEVEL_GAMEPLAY, pLayerTag, &UI_Desc)))
		return E_FAIL;


	UI_Desc.fSizeX = 75.f;
	UI_Desc.fSizeY = 75.f;
	UI_Desc.fX = 400.f;
	UI_Desc.fY = 790.f;
	UI_Desc.fNear = 0.f;
	UI_Desc.fFar = 1.f;

	UI_Desc.fSpeedPerSec = 5.f;
	UI_Desc.fRotationPerSec = XMConvertToRadians(90.f);


	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_ItemBag"), LEVEL_GAMEPLAY, pLayerTag, &UI_Desc)))
		return E_FAIL;


	/* 아이템 ui (꽃, 버섯 ) */

	UI_Desc.fSizeX = 70.f;
	UI_Desc.fSizeY = 70.f;
	UI_Desc.fX = 100.f;
	UI_Desc.fY = 790.f;
	UI_Desc.fNear = 0.f;
	UI_Desc.fFar = 1.f;

	UI_Desc.fSpeedPerSec = 5.f;
	UI_Desc.fRotationPerSec = XMConvertToRadians(90.f);


	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_S_HpFlower_Ui"), LEVEL_GAMEPLAY, pLayerTag, &UI_Desc)))
		return E_FAIL;

	UI_Desc.fSizeX = 70.f;
	UI_Desc.fSizeY = 70.f;
	UI_Desc.fX = 200.f;
	UI_Desc.fY = 790.f;
	UI_Desc.fNear = 0.f;
	UI_Desc.fFar = 1.f;

	UI_Desc.fSpeedPerSec = 5.f;
	UI_Desc.fRotationPerSec = XMConvertToRadians(90.f);


	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SM_MushroomA_L_Ui"), LEVEL_GAMEPLAY, pLayerTag, &UI_Desc)))
		return E_FAIL;


	/* 퀘스트 1 */

	UI_Desc.fSizeX = 450.f;	
	UI_Desc.fSizeY = 200.f;	
	UI_Desc.fX = 200.f;	
	UI_Desc.fY = 790.f;	
	UI_Desc.fNear = 0.f;	
	UI_Desc.fFar = 1.f;	

	UI_Desc.fSpeedPerSec = 5.f;	
	UI_Desc.fRotationPerSec = XMConvertToRadians(90.f);	
	UI_Desc.szName = "Quest_01";	


	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_QuestUI"), LEVEL_GAMEPLAY, pLayerTag, &UI_Desc)))
		return E_FAIL;


	/* 퀘스트 2 */

	UI_Desc.fSizeX = 450.f;
	UI_Desc.fSizeY = 200.f;
	UI_Desc.fX = 200.f;
	UI_Desc.fY = 790.f;
	UI_Desc.fNear = 0.f;
	UI_Desc.fFar = 1.f;

	UI_Desc.fSpeedPerSec = 5.f;
	UI_Desc.fRotationPerSec = XMConvertToRadians(90.f);
	UI_Desc.szName = "Quest_02";


	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_QuestUI"), LEVEL_GAMEPLAY, pLayerTag, &UI_Desc)))
		return E_FAIL;


	/* 보스  MASK UI  */

	UI_Desc.fSizeX = 100.f;	
	UI_Desc.fSizeY = 100.f;	
	UI_Desc.fX = 470.f;		
	UI_Desc.fY = 50.f;		
	UI_Desc.fNear = 0.f;		
	UI_Desc.fFar = 1.f;		
	
	UI_Desc.fSpeedPerSec = 5.f;			
	UI_Desc.fRotationPerSec = XMConvertToRadians(90.f);		
	UI_Desc.szName = "Boss_Mask_UI";			
	
	
	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_BossMaskUi"), LEVEL_GAMEPLAY, pLayerTag, &UI_Desc)))		
		return E_FAIL;		

	/* 보스 Hp UI */


	UI_Desc.fSizeX = 540.f;	
	UI_Desc.fSizeY = 20.f;	
	UI_Desc.fX = 800.f;	
	UI_Desc.fY = 55.f;
	UI_Desc.fNear = 0.f;
	UI_Desc.fFar = 1.f;

	UI_Desc.fSpeedPerSec = 5.f;
	UI_Desc.fRotationPerSec = XMConvertToRadians(90.f);		
	UI_Desc.szName = "Boss_Hp_UI";	


	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_BossHpUi"), LEVEL_GAMEPLAY, pLayerTag, &UI_Desc)))
		return E_FAIL;	




	/* Fade In Out*/
	
	//UI_Desc.fSizeX = 1600.f;
	//UI_Desc.fSizeY = 900.f;
	//UI_Desc.fX = 800.f;	
	//UI_Desc.fY = 450.f;	
	//UI_Desc.fNear = 0.f;	
	//UI_Desc.fFar = 1.f;	
	//
	//UI_Desc.fSpeedPerSec = 5.f;	
	//UI_Desc.fRotationPerSec = XMConvertToRadians(90.f);	
	//UI_Desc.szName = "Fade_InOut_UI";	
	//
	//
	//if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_FadeInOut"), LEVEL_GAMEPLAY, pLayerTag, &UI_Desc)))		
	//	return E_FAIL;	

	return S_OK;

}

CLevel_GamePlay * CLevel_GamePlay::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CLevel_GamePlay*	pInstance = new CLevel_GamePlay(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Created : CLevel_GamePlay");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CLevel_GamePlay::Free()
{
	__super::Free();

}
