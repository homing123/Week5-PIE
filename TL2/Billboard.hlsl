//cbuffer
//	struct BillboardBufferType
//{
//    FVector CompPosition;
//    FMatrix View;
//    FMatrix Proj;
//    FMatrix InverseViewMat;
//};

//struct VS_INPUT
//{
//    float3 centerPos : WORLDPOSITION;
//    float2 size : SIZE;
//    float4 uvRect : UVRECT;
//    uint vertexId : SV_VertexID; // GPU가 자동으로 부여하는 고유 정점 ID
//};

//struct PS_INPUT
//{
//    float4 pos_screenspace : SV_POSITION;
//    float2 tex : TEXCOORD0;
//};

//Texture2D g_DiffuseTexColor : register(t0);
//SamplerState g_Sample : register(s0);

float4 main(float4 pos : POSITION) : SV_POSITION
{
    return pos;
}