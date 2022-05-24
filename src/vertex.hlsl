cbuffer BufferPerFrame : register(b0) {
   matrix camera_matrix;
}

struct ShaderInput {
   float3 position_local : POS;
};

struct ShaderOutput {
   float4 position_clip : SV_POSITION;
};

ShaderOutput main(ShaderInput input) {
   ShaderOutput output;
   output.position_clip = float4(input.position_local, 1.0);
   return output;
}