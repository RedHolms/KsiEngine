struct InputData {
   float4 position : SV_POSITION;
   float2 uv : TEXCOORD;
};

Texture2D    Texture : register(t0);
SamplerState Sampler : register(s0);

float4 main(InputData IN) : SV_TARGET {
   return Texture.Sample(Sampler, IN.uv);
   // return float4(1.0, 1.0, 1.0, 1.0);
}