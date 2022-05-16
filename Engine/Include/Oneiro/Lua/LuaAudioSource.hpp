//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "HazelAudio/HazelAudio.h"

namespace oe::Lua
{
    class AudioSource
    {
    public:
        AudioSource(const std::string& path) { mAudioSource.LoadFromFile(path); }
        void PrePlay()
        {
            VisualNovel::GetInstructions().push_back({VisualNovel::PLAY_MUSIC, {}, {}, this});
        }

        void Play()
        {
            mAudioSource.Play();
        }

        void PreStop()
        {
            VisualNovel::GetInstructions().push_back({VisualNovel::STOP_MUSIC, {}, {}, this});
        }

        void Stop()
        {
            mAudioSource.Stop();
        }
    private:
        Hazel::Audio::Source mAudioSource;
    };
}