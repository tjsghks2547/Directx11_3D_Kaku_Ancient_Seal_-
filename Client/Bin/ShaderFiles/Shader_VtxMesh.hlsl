#include "../../../EngineSDK/hlsl/Engine_Shader_Defines.hlsli"

float4x4		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
Texture2D       g_DiffuseTexture;
Texture2D		g_NormalTexture;
Texture2D       g_NoiseTexture; 

float           g_Time;
float           g_DissolveAmount;   

struct VS_IN
{
	float3			vPosition : POSITION;	
	float3			vNormal   : NORMAL;
	float2			vTexcoord : TEXCOORD0;	
	float3			vTangent  : TANGENT;
};

struct VS_OUT
{
	float4			vPosition : SV_POSITION;
	float4			vNormal   : NORMAL;
	float2			vTexcoord : TEXCOORD0;
	float4			vWorldPos : TEXCOORD1;
    float4			vProjPos  : TEXCOORD2; 
	
	   
    float4			vTangent  : TANGENT;
    float4			vBinormal : BINORMAL;
};

VS_OUT VS_MAIN(VS_IN In)
{	
	VS_OUT			Out = (VS_OUT)0;

	matrix			matWV, matWVP;

	matWV  = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
    Out.vNormal   = normalize(mul(float4(In.vNormal, 0.f), g_WorldMatrix));
	Out.vTexcoord = In.vTexcoord;
	Out.vWorldPos = mul(float4(In.vPosition, 1.f), g_WorldMatrix);
    Out.vProjPos = Out.vPosition; 
	
    Out.vTangent  = normalize(mul(float4(In.vTangent, 0.f), g_WorldMatrix));	
    Out.vBinormal = vector(normalize(cross(Out.vNormal.xyz, Out.vTangent.xyz)), 0.f); // ���� ���� �߿��ϴ� �ֳ��ϸ� �����ٲ�� binormal�� - ������ �����Ǵϱ� 
	
	return Out;
}

struct VS_OUT_SHADOW    
{
    float4 vPosition : SV_POSITION; 
    float4 vProjPos : TEXCOORD0;    
};

VS_OUT_SHADOW VS_MAIN_SHADOW(VS_IN In)
{
    VS_OUT_SHADOW Out = (VS_OUT_SHADOW) 0;

    matrix matWV, matWVP;   

    matWV = mul(g_WorldMatrix, g_ViewMatrix);   
    matWVP = mul(matWV, g_ProjMatrix);  

    Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP); 
    Out.vProjPos = Out.vPosition;

    return Out;
}



struct PS_IN
{
	float4			vPosition : SV_POSITION;
	float4			vNormal   : NORMAL;
	float2			vTexcoord : TEXCOORD0;
	float4			vWorldPos : TEXCOORD1;
    float4			vProjPos  : TEXCOORD2;
	
		
    float4          vTangent  : TANGENT;
    float4          vBinormal : BINORMAL;
};

struct PS_OUT
{
    float4 vDiffuse : SV_TARGET0;
    float4 vNormal  : SV_TARGET1;
    float4 vDepth   : SV_TARGET2;
};


struct PS_IN_SHADOW
{
    float4 vPosition : SV_POSITION;
    float4 vProjPos : TEXCOORD0;
};

struct PS_OUT_SHADOW
{
    float4 vShadow : SV_TARGET0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

    vector vMtrlDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);
	
    if (vMtrlDiffuse.a < 0.1f)
        discard;
	
    float4 vNormalDesc  = g_NormalTexture.Sample(LinearSampler, In.vTexcoord);	
	

	/* ź��Ʈ �����̽��� �����ϴ� ����̴�. */	
    float3 vNormal = vNormalDesc.xyz * 2.f - 1.f;	
	
	
	/* ���� �����̽����� �븻�� ��ȯ����. */
    float3x3 WorldMatrix = float3x3(In.vTangent.xyz, In.vBinormal.xyz, In.vNormal.xyz);
    vNormal = normalize(mul(vNormal, WorldMatrix));
	

    Out.vDiffuse = vMtrlDiffuse;
    Out.vNormal = vector(vNormal * 0.5f + 0.5f, 0.f);	
    //Out.vNormal  = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
    Out.vDepth   = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w, 0.f, 0.f);	
	
	return Out;
}


PS_OUT PS_MAIN_DISSOLVE(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    vector vMtrlDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);
    
    if (vMtrlDiffuse.a < 0.1f)  
        discard;
   
    /* Dissolve ���� */ 
    
    // ������ �ؽ�ó���� ���ø� (UV ������, Ÿ�� ���� �̿��� ���� ����)
    float2 noiseUV = In.vTexcoord + float2(0.0f, g_Time * 0.1);
    float noiseValue = g_NoiseTexture.Sample(LinearSampler, noiseUV).r;
    
    
    //(����) Dissolve ��踦 ������ Edge Color�� ���� 
    // ���� (noiseValue)�� gDissolveAmount ��ó�� ���� �� 
    // ��) edgeWidth ���� �������� ȿ�� 
    float g_EdgeWidth = 0.3f; 
    float edgeFactor = smoothstep(g_DissolveAmount - g_EdgeWidth, g_DissolveAmount, noiseValue);        
    
    //edgeFactor�� �̿��� ��� ���� ���� 
    //edgeFactor�� 1�̸� ��� ��(����), 0�̸� ��� �� (���� Ŭ��)    
    //��� = edgeFactor�� 0~1 ���̸� ������ �κ�
    //(�� �� �����ϰ� �Ϸ��� saturate, lerp�� �̿�) 
    
    float4 g_EdgeColor = { 1.f, 1.f, 0.f, 1.f }; 
    float  edgeStrength = 1.0 - edgeFactor; // ��� �αٿ��� 1
    float4 edgeBlend = lerp(vMtrlDiffuse, g_EdgeColor, edgeStrength);       
    float4 finalColor = lerp(vMtrlDiffuse, edgeBlend, edgeStrength);
    
    // Dissolve ���� 
    // noiseValue < gDissolveAmount �̸� �ȼ��� ������ ( ���� ó�� ) 
    // clip(x)�� ȣ���ϸ� x < 0 �� �� �ش� �ȼ��� ������. 
    if (noiseValue < g_DissolveAmount)
    {
           // ���� ���, noiseValue�� g_DissolveAmount ��ó�� ���� edge ȿ���� �����ϰ�,
            // �׺��� ���� ������ �����ϰ� ����.
        float alphaFactor = saturate((noiseValue - (g_DissolveAmount - g_EdgeWidth)) / g_EdgeWidth);
        finalColor.a *= alphaFactor;
        
        

         // �ʿ信 ����, alpha�� �ʹ� ������ ������ ����� ���� ����
         if (finalColor.a < 0.01f) 
            clip(-1);  
       
    }
    
    // (����) Dissolve ��踦 ������ Edge Color�� ���� 
    // ���� (noiseValue)�� gDissolveAmount ��ó�� ���� �� 
    // ��) edgeWidth ���� �������� ȿ�� 
    //float edgeFactor = smoothstep(g_DissolveAmount - g_EdgeWidth, g_DissolveAmount, noiseValue);    
    
    // edgeFactor�� �̿��� ��� ���� ���� 
    // edgeFactor�� 1�̸� ��� ��(����), 0�̸� ��� �� (���� Ŭ��) 
    // ��� = edgeFactor�� 0~1 ���̸� ������ �κ�
    // (�� �� �����ϰ� �Ϸ��� saturate, lerp�� �̿�)
    
    //float  edgeStrength = 1.0 - edgeFactor; // ��� �αٿ��� 1
    //float4 edgeBlend = lerp(vMtrlDiffuse, g_EdgeColor, edgeStrength);    
    //float4 finalColor = lerp(vMtrlDiffuse, edgeBlend, edgeStrength);
    
    /* ////////////////////////////////// */
    
    float4 vNormalDesc = g_NormalTexture.Sample(LinearSampler, In.vTexcoord);
	
    /* ź��Ʈ �����̽��� �����ϴ� ����̴�. ���� (0~1 ) UnNormal�� ���� �Ǿ� ���� */  
    float3 vNormal = vNormalDesc.xyz * 2.f - 1.f;

   /* ���� �����̽����� �븻�� ��ȯ����. */
    float3x3 WorldMatrix = float3x3(In.vTangent.xyz, In.vBinormal.xyz, In.vNormal.xyz);
    
    vNormal = normalize(mul(vNormal, WorldMatrix));
    
    //Out.vDiffuse = vMtrlDiffuse;
    /* ������ ���� */
    float blendFactor = 0.5; // 0 ~ 1 ������ ��, ȿ���� ���� ����
    Out.vDiffuse = lerp(vMtrlDiffuse, finalColor, blendFactor);
    Out.vNormal = vector(vNormal * 0.5f + 0.5f, 0.f);
    
    //Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w, 0.f, 0.f);



    return Out;
}

PS_OUT_SHADOW PS_MAIN_SHADOW(PS_IN_SHADOW In)   
{
    PS_OUT_SHADOW Out = (PS_OUT_SHADOW) 0;  

    Out.vShadow = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w, 0.f, 0.f);   

    return Out; 
}


technique11 DefaultTechnique
{
	pass DefaultPass
	{
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

    
    pass DissolvePass
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);       
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);  

        VertexShader = compile vs_5_0 VS_MAIN();    
        GeometryShader = NULL;  
        PixelShader = compile ps_5_0 PS_MAIN_DISSOLVE();    
    }

    pass Shadow
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN_SHADOW();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_SHADOW();
    }

}
