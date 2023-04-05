#include "PostEffect.hlsli"

Texture2D<float4> tex0 : register(t0); // 0番スロットに設定されたテクスチャ
Texture2D<float4> tex1 : register(t1);
SamplerState smp : register(s0); // 0番スロットに設定されたサンプラー

float4 ChromaticAberration(float2 uv, float level = 3.f, float spread = 0.02f)
{
	float2 offset = (uv - 0.5f) * float2(1.f, -1.f);
	float3 sum = float3(0.f, 0.f, 0.f);
	float4 ret = float4(0.f, 0.f, 0.f, 1.f);

	for (float i = 0; i < level; i++)
	{
		float t = 2.f * i / float(level - 1); // range 0.0->2.0
		float3 slice = float3(1.f - t, 1.f - abs(t - 1.f), t - 1.f);
		slice = max(slice, 0.f);
		sum += slice;
		float2 slice_offset = (t - 1.f) * spread * offset;

		ret.rgb += slice * tex0.Sample(smp, saturate(uv + slice_offset)).rgb;
	}
	ret.rgb /= sum;
	return float4(ret.rgb, 1.f);
}

float Gaussian(float2 drawUV, float2 pickUV, float sigma)
{
	float d = distance(drawUV, pickUV);
	return exp(-(d * d) / (2 * sigma * sigma));
}

// 高光度部分を抽出したものをぼかす
float4 GaussianBlurHighLumi(Texture2D<float4> tex, float2 uv, float sigma = 0.005f, float stepWidth = 0.002f, float threthreshold = 0.9f)
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
	float4 texcolor1 = texcolor0 + ChromaticAberration(input.uv);

	float4 color = texcolor1;

	float dist = vignettePow * 2.f * distance(float2(0.5f, 0.5f), input.uv);
	color.gb *= 1.f - dist;


	return float4(color.rgb, 1);
}
