//
// Generated by Microsoft (R) HLSL Shader Compiler 9.29.952.3111
//
// Parameters:
//
//   float4 DNEResolution;
//   sampler2D PixelTexture2D_0;
//   sampler2D PixelTexture2D_1;
//   sampler2D PixelTexture2D_2;
//   sampler2D SceneColorTexture;
//   float4 ScreenPositionScaleBias;
//   float4 UniformPixelVector_0;
//   float4 UniformPixelVector_1;
//   float4 UniformPixelVector_2;
//
//
// Registers:
//
//   Name                    Reg   Size
//   ----------------------- ----- ----
//   DNEResolution           c0       1   //0.001041667	-0.001851852	-1	1
//   ScreenPositionScaleBias c1       1
//   UniformPixelVector_0    c7       1
//   UniformPixelVector_1    c8       1  //0.2853794	0.4268951	0.4268951	0.4268951
//   UniformPixelVector_2    c9       1  //-0.4280701	0.5684052	0.5684052	0.5684052
//   SceneColorTexture       s0       1
//   PixelTexture2D_0        s1       1
//   PixelTexture2D_1        s2       1
//   PixelTexture2D_2        s3       1
//

ps_3_0
def c2, 14, 0, 2, 15
def c3, -9.99999997e-007, 2, 1.5, 0.800000012
def c4, 2.20000005, 1.64999998, 0, 0
dcl_texcoord2 v0
dcl_texcoord v1.xy
dcl_texcoord3 v2.yz
dcl_texcoord4 v3.w
dcl_texcoord5 v4.w
dcl vPos.xy
dcl_2d s0
dcl_2d s1
dcl_2d s2
dcl_2d s3
//mov r0.yz, c3
r0.yz = (2,1.5)
r0.xy = v1.xy * (2, 1.5) + (0.2853794	0.4268951)
//mad r0.xy, v1, r0.yzzw, c8
//texld r0, r0, s3
r0 = texld(s3, r0.xy)
//mov r1.xy, c4
r1.xy = (2.2,1.65)
//mad r0.yz, v1.xxyw, r1.xxyw, c9.xxyw
r0.yz = v1.xy * (2.2, 1.65) + (-0.4280701	0.5684052)
texld r1, r0.yzzw, s3
r1 = texld(s3, r0.yz)
add r0.x, r0.x, r1.x
mul r0.x, r0.x, c3.w
r0.x = (r0.x + r1.x) * 0.8
add r0.y, r0_abs.x, c3.x
r0.y = (r0.x + r1.x) * 0.8
pow r1.x, r0_abs.x, v2.y
cmp r0.x, r0.y, r1.x, c2.y
texld r1, v1, s2
add r0.y, r1_abs.x, c3.x
pow r0.z, r1_abs.x, v2.z
cmp r0.y, r0.y, r0.z, c2.y
add r0.x, r0.x, r0.y
texld r1, v1, s1
mul_sat r0.y, r1.x, c2.w
mul r0.x, r0.x, r0.y
mul_pp r0.x, r0.x, v0.w
mad r0.yz, vPos.xxyw, c0.xxyw, c0.xzww
mul r0.yz, r0, v4.w
rcp r0.w, v4.w
mul r0.yz, r0.w, r0
mad r0.yz, r0, c1.xxyw, c1.xwzw
texld r1, r0.yzzw, s0
mul r0.yzw, r1.xxyz, r1.xxyz
mad r0.yzw, r0, c2.x, r1.xxyz
max r1.xyz, r0.yzww, c2.y
min r0.yzw, r1.xxyz, c2.z
add r0.yzw, r0, v0.xxyz
add_pp r0.yzw, r0, c7.xxyz
mul r0.yzw, r0, v3.w
mul_pp oC0.xyz, r0.x, r0.yzww
mov oC0.w, c2.y

// approximately 39 instruction slots used (5 texture, 34 arithmetic)