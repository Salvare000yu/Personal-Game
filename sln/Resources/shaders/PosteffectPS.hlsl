#include "PostEffect.hlsli"

Texture2D<float4> tex0 : register(t0); // 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
Texture2D<float4> tex1 : register(t1);
SamplerState smp : register(s0); // 0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

float Gaussian(float2 drawUV, float2 pickUV, float sigma)
{
    float d = distance(drawUV, pickUV);
    return exp(-(d * d) / (2 * sigma * sigma));
}

// �����x�����𒊏o�������̂��ڂ���
float4 GaussianBlurHighLumi(Texture2D<float4> tex, float2 uv, float sigma = 0.005f, float stepWidth = 0.002f, float threthreshold = 0.8f)
{
    // ���v�łǂ̂��炢�����������L�^����ϐ�
    float totalWeight = 0;

	// �ŏI�I�ȐF������ϐ�
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
