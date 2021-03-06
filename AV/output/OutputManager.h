#ifndef OUTPUTMANAGER_H
#define OUTPUTMANAGER_H

#include <deque>
#include <memory>
#include <thread>
#include <atomic>

#include "OutputSettings.h"
#include "MutexDataPair.h"
#include "AVWrapper.h"
#include "VideoEncoder.h"
#include "AudioEncoder.h"
#include "Muxer.h"
#include "Synchronizer.h"

class OutputManager
{
private:
    struct SharedData {
      //frame queue used while switching between fragments
        std::deque<std::unique_ptr<AVFrameWrapper>> m_video_frame_queue;
        std::deque<std::unique_ptr<AVFrameWrapper>> m_audio_frame_queue;
        unsigned int m_fragment_number;

        //muxer and encoders
        std::unique_ptr<Muxer> m_muxer;
        VideoEncoder *m_video_encoder;
        AudioEncoder *m_audio_encoder;
    };
    typedef MutexDataPair<SharedData>::Lock SharedLock;

private:
    static const size_t THROTTLE_THRESHOLD_FRAMES, THROTTLE_THRESHOLD_PACKETS;

private:
    OutputSettings m_output_settings;
    OutputFormat m_output_format;

    bool m_fragmented;
    int64_t m_fragment_length;

    std::unique_ptr<Synchronizer> m_synchronizer;

    std::thread m_thread;
    MutexDataPair<SharedData> m_shared_data;
    std::atomic<bool> m_should_stop, m_should_finish, m_is_done, m_error_occurred;

public:
    OutputManager(const OutputSettings& output_settings);
    ~OutputManager();

    void Finish();

    bool isFinished();

    void AddVideoFrame(std::unique_ptr<AVFrameWrapper> frame);

    void AddAudioFrame(std::unique_ptr<AVFrameWrapper> frame);

    int64_t GetVideoFrameDelay();

    unsigned int GetTotalQueuedFrameCount();

    double GetActualFrameRate();

    double GetActualBitRate();

    uint64_t GetTotalBytes();

private:
    void Init();
    void Free();

    void StartFragment();
    void StopFragment();

    void FragmentThread();

public:
    inline const OutputSettings* GetOutputSettings() {return &m_output_settings; }
    inline const OutputFormat* GetOutputFormat() {return &m_output_format; }
    inline Synchronizer* GetSynchronizer() {return m_synchronizer.get(); }

};

#endif // OUTPUTMANAGER_H
