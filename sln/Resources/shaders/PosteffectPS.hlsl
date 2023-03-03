#include "PostEffect.hlsli"

Texture2D<float4> tex0 : register(t0); // 0番スロットに設定されたテクスチャ
Texture2D<float4> tex1 : register(t1);
SamplerState smp : register(s0); // 0番スロットに設定されたサンプラー

float Gaussian(float2 drawUV, float2 pickUV, float sigma)
{
    float d = distance(drawUV, pickUV);
    return exp(-(d * d) / (2 * sigma * sigma));
}

// 高光度部分を抽出したものをぼかす
float4 GaussianBlurHighLumi(Texture2D<float4> tex, float2 uv, float sigma = 0.005f, float stepWidth = 0.002f, float threthreshold = 0.8f)
{
    // 合計でどのくらい混ぜたかを記録する変数
    float totalWeight = 0;

	// 最終的な色を入れる変数
    float4 col = float4(0, 0, 0, 1);

    for (float py = -sigma * 2; py <= sigma * 2; py += stepWidth)
    {
        for (float px = -sigma * 2; px <= sigma * 2; px += stepWidth)
        {
            float2 pickUV = uv + float2(px, py);

            float weight = Gaussian(uv, pickUV, sigma);

            float4 texCol = step(threthreshold, tex.Sample(smp, pickUV));

            col.rgb += texCol * weight;

            totalWeight += weight;
        }
    }

    col.rgb = col.rgb / totalWeight;
    return col;
}

float4 main(VSOutput input) : SV_TARGET
{
	
    float4 texcolor0 = GaussianBlurHighLumi(tex0, input.uv);
    float4 texcolor1 = texcolor0+tex0.Sample(smp, input.uv);
	
    float4 color = texcolor1;
    float dist = vignettePow * 2.f * distance(float2(0.5f, 0.5f), input.uv);
    color.gb *= 1.f - dist;
    
    

    return float4(color.rgb, 1);
}
