#include "OBJShaderHeader.hlsli"

Texture2D<float4> tex : register(t0); // 0番スロットに設定されたテクスチャ
SamplerState smp : register(s0); // 0番スロットに設定されたサンプラー

float4 main(VSOutput input) : SV_TARGET
{
	float4 texcolor = tex.Sample(smp, input.uv * tiling) * color;
	if (texcolor.a <= 0.f) { discard; }

	float3 light = normalize(float3(1, -1, 1)); // 右下奥　向きのライト
	float light_diffuse = saturate(dot(-light, input.normal));
	float3 shade_color;
	shade_color = m_ambient; // アンビエント項
	shade_color += m_diffuse * light_diffuse; // ディフューズ項
	return float4(pow(texcolor.rgb * shade_color, 1.f / 2.2f), texcolor.a * m_alpha);
}