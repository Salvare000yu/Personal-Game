#include "PostEffect.hlsli"

Texture2D<float4> tex0 : register(t0); // 0番スロットに設定されたテクスチャ
Texture2D<float4> tex1 : register(t1);
SamplerState smp : register(s0); // 0番スロットに設定されたサンプラー

float Gaussian(float2 drawUV, float2 pickUV, float sigma)
{
    float d = distance(drawUV, pickUV);
    return exp(-(d * d) / (2 * sigma * sigma));
}

float4 main(VSOutput input) : SV_TARGET
{
	
    float4 texcolor0 = tex0.Sample(smp, input.uv);
    float4 texcolor1 = tex1.Sample(smp, input.uv);
	
    float4 color = texcolor0;
    if (fmod(input.uv.y, 0.05f) < 0.02f)
    {
       //color = texcolor0.r; //斜線
    }
    float dist = vignettePow * 2.f * distance(float2(0.5f, 0.5f), input.uv);
    color.gb *= 1.f - dist;

    return float4(color.rgb, 1);
}
