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
	
	//時間でRGBシフト
    float4 texcolor0 = tex0.Sample(smp, input.uv);
    texcolor0.r = tex0.Sample(smp, input.uv + float2(0.003 * sin(1.6 * time * 3.141592653589793f), 0)).r;
    texcolor0.g = tex0.Sample(smp, input.uv + float2(0, 0.003 * sin(1.4*time * 3.141592653589793f))).g;
    texcolor0.b = tex0.Sample(smp, input.uv + float2(0.002 * sin(1.2*time * 3.141592653589793f), 0.01)).b;
    float4 texcolor1 = tex1.Sample(smp, input.uv);

    float4 color = texcolor0;
    if (fmod(input.uv.y, 0.1f) < 0.05f)
    {
        //color = texcolor1;//斜線
    }

    return float4(color.rgb, 1);

	//ぼかし↓
	//float totalWeight = 0, _Sigma = 0.005, _StepWidth = 0.001;
	//float4 col = float4(0, 0, 0, 0);

	//for (float py = -_Sigma * 2; py <= _Sigma * 2; py += _StepWidth)
	//{
	//	for (float px = -_Sigma * 2; px <= _Sigma * 2; px += _StepWidth)
	//	{
 //           float2 pickUV = input.uv + float2(px, py);
 //           float weight = Gaussian(input.uv, pickUV, _Sigma);
	//		col += tex0.Sample(smp, pickUV) * weight;
			
	//		totalWeight += weight;
	//	}
	//}

	//col.rgb = col.rgb / totalWeight;
	//return col;
}
