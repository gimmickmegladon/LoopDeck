/*
  ==============================================================================

    MainComponent.h
    Created: 5 Nov 2020 11:03:55pm
    Author:  YakLion

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "LoopDeckComponent.h" 
#include "AudioAppDemo.h"
//==============================================================================
/*
*/

class MainComponent  : public juce::AudioAppComponent
{
public:
    //======================================================================
    MainComponent();
    ~MainComponent() override;

    enum TransportState
    {
        Stopped,
        Starting,
        Playing,
        Stopping
    };
    //======================================================================
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //======================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)

    void playButtonClicked();
    void stopButtonClicked();
    void recordButtonClicked();
    void exportButtonClicked(AudioBuffer<float> exBuff);
    void loadButtonClicked(int banks);
    void updateProgressBar(double progress);
    //======================================================================
    // GUI ELEMENTS
    std::vector<bool> playing; 
    LoopDeckComponent ld;
    Slider slider_1;
    TextButton playButton;
    TextButton stopButton;
    TextButton recordButton;
    TextButton exportButton;
    TextButton loadButton;
    TextButton loadButton2;
    TextButton loadButton3;
    TextButton loadButton4;
    //double loopProgress = 0.0;
    //ProgressBar progBar;
    //AUDIO ELEMENTS
    int bank = 0;
    bool recording;
    TransportState state;
    juce::AudioFormatManager formatManager;
    WavAudioFormat wav_format;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;
    std::vector<juce::AudioSampleBuffer> samples;
    std::vector<int> playPositions;
    std::vector<float> playVolumes; 
    std::vector<String> names;
    std::vector<std::unique_ptr<juce::ToggleButton>> playButtons;
    std::vector<std::unique_ptr<juce::Slider>> volumeSliders;
    juce::AudioSampleBuffer mainBuffer;
    juce::AudioBuffer<float> exportBuffer;    
    //juce::Random random;
};
