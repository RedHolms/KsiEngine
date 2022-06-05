cbuffer ConstantBuffer : register(b0) {
   matrix projectionMatrix;
   matrix viewMatrix;
}

cbuffer ObjectBuffer : register(b1) {
   matrix objectMatrix;
   float4 objectPosition;
}

struct InputData {
   float3 position : POSITION;
   float3 color: COLOR;
   float2 uv : TEXCOORD;
};

struct OutputData {
   float4 position : SV_POSITION;
   float4 color : COLOR;
   float2 uv : TEXCOORD;
};

OutputData main(InputData IN) {
   OutputData OUT;

   matrix mvp = mul(projectionMatrix, viewMatrix);
   
   float4 rotatedPosition = mul(float4(IN.position, 1.0), objectMatrix);
   rotatedPosition += float4(objectPosition.xyz, 0);

   OUT.position = mul(mvp, rotatedPosition);
   OUT.color = float4(IN.color, 0.0);
   OUT.uv = IN.uv;

   return OUT;
}