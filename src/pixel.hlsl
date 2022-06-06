struct InputData {
   float2 uv : TEXCOORD;
};

Texture2D    Texture : register(t0);
SamplerState Sampler : register(s0);

float4 main(InputData IN) : SV_TARGET {
   return Texture.Sample(Sampler, IN.uv);
}