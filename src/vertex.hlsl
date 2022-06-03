cbuffer ConstantBuffer : register(b0) {
   matrix projectionMatrix;
   matrix viewMatrix;
}

cbuffer ObjectBuffer : register(b1) {
   matrix objectMatrix;
}

struct InputData {
   float3 position : POSITION;
   float3 color: COLOR;
};

struct OutputData {
   float4 color : COLOR;
   float4 position : SV_POSITION;
};

OutputData main(InputData IN) {
   OutputData OUT;

   matrix mvp = mul(mul(projectionMatrix, viewMatrix), objectMatrix);
   OUT.position = mul(mvp, float4(IN.position, 1.0f));
   OUT.color = float4(IN.color, 1.0f);

   return OUT;
}