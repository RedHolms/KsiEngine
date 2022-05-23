struct px_in {
   float4 position_clip : SV_POSITION;
};

float4 main(px_in input) : SV_TARGET {
   return float4(1.0, 0.0, 1.0, 1.0);
}