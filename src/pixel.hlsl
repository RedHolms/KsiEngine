struct InputData {
   float4 color : COLOR;
};

float4 main(InputData IN) : SV_TARGET {
   return IN.color;
}