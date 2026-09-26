#pragma once
#include <atomic>
#include "../includes.hpp"
#include "ffmpeg/events.hpp"

enum AudioMode {
    Off = 0,
    Song = 1,
    Record = 2
};

class MyRenderTexture {
public:
    unsigned width, height;
    int old_fbo, old_rbo;
    unsigned fbo;
    geode::prelude::CCTexture2D* texture = nullptr;
    void begin();
    void capture(std::mutex& lock, std::vector<uint8_t>& data, std::atomic_bool& lul);
};

class Renderer {
public:

    Renderer() : width(1920), height(1080), fps(60) {}

    std::atomic_bool frameHasData{false};
    std::atomic_bool workerActive{false};
    bool levelFinished = false;
    std::atomic_bool recording{false};
    std::atomic_bool pause{false};
    int audioMode = 0;
    float ogMusicVol;
    float ogSFXVol;
    float SFXVolume = 1.f;
    float musicVolume = 1.f;

    bool usingApi = false;
    bool dontRender = false;
    bool dontRecordAudio = false;
    std::atomic_bool recordingAudio{false};
    bool startedAudio = false;
    bool isPlatformer = false;
    int finishFrame = 0;
    int levelStartFrame = 0;

    float stopAfter = 3.f;
    float timeAfter = 0.f;
    unsigned width, height;
    unsigned fps;
    double lastFrame_t, extra_t;
    int pauseAttempts = 0;

    MyRenderTexture renderer;
    ffmpeg::events::Recorder ffmpeg;
    std::vector<uint8_t> currentFrame;
    std::mutex lock;
    std::string codec = "", bitrate = "12M", extraArgs = "", videoArgs = "", extraAudioArgs = "", path = "";
    #ifdef GEODE_IS_MACOS
    std::string ffmpegPath = "/opt/homebrew/bin/ffmpeg";
    #elif defined(GEODE_IS_WINDOWS)
    std::string ffmpegPath = (geode::dirs::getGameDir() / "ffmpeg.exe").string();
    #else
    std::string ffmpegPath;
    #endif
    std::unordered_set<int> renderedFrames;

    FMODAudioEngine* fmod = nullptr;
    cocos2d::CCSize ogRes = {0, 0};
    float ogScaleX = 1.f;
    float ogScaleY = 1.f;

    void captureFrame();
    void changeRes(bool og);

    void start();
    void startAudio(PlayLayer* pl);

    void stop(int frame = 0);
    void stopAudio();

    void handleRecording(PlayLayer* pl, int frame);
    void handleAudioRecording(PlayLayer* pl, int frame);
    
    static bool toggle();
    static bool shouldUseAPI();
    bool tryPause();
};
