#pragma once

#include <xaudio2.h>
#include <cstdint>
#include <wrl.h>
#include <map>
#include <string>

/// <summary>
/// �I�[�f�B�I
/// </summary>

class GameSound
{

public:
    // �`�����N�w�b�_
    struct ChunkHeader
    {
        char id[4]; // �`�����N����ID
        int32_t size;  // �`�����N�T�C�Y
    };

    // RIFF�w�b�_�`�����N
    struct RiffHeader
    {
        ChunkHeader chunk;   // "RIFF"
        char type[4]; // "WAVE"
    };

    // FMT�`�����N
    struct FormatChunk
    {
        ChunkHeader chunk; // "fmt "
        WAVEFORMATEX fmt; // �g�`�t�H�[�}�b�g
    };

    // �����f�[�^
    struct SoundData
    {
        // �g�`�t�H�[�}�b�g
        WAVEFORMATEX wfex;
        // �o�b�t�@�̐擪�A�h���X
        BYTE* pBuffer;
        // �o�b�t�@�̃T�C�Y
        unsigned int bufferSize;
    };

    static GameSound* GetInstance();

    /// <summary>
    /// ������
    /// </summary>
    void Initialize(const std::string& directoryPath="Resources/sound/");

    /// <summary>
    /// ��������@�I��
    /// </summary>
    void Finalize();

    /// <summary>
    /// �����ǂݍ���
    /// </summary>
    void LoadWave(const std::string& filename);

    /// <summary>
    /// �T�E���h�f�[�^���
    /// </summary>
    /// <param name="filename"></param>
    void Unload(SoundData* soundData);

    /// <summary>
    /// �����Đ�
    /// </summary>
    /// <param name="filename">WAV�t�@�C����</param>
    void PlayWave(const std::string& filename);

private:
    Microsoft::WRL::ComPtr<IXAudio2> xAudio2_;
    //�T�E���h�f�[�^�A�z�z��
    std::map<std::string, SoundData> soundDatas_;

    //�T�E���h�i�[�f�B���N�g��
    std::string directoryPath_;
};

