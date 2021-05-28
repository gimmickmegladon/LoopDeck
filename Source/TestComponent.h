#pragma once

#include <JuceHeader.h>

class TestComponent : public juce::AudioAppComponent
{
public:
    //==============================================================================
    TestComponent();
    ~TestComponent() override;

    //==============================================================================
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint(juce::Graphics& g) override;
    void resized() override;
private:
    std::vector<juce::AudioBuffer<float>> playBuffers;
    std::vector<int> playPositions;
    std::vector<bool> playing;
    std::vector<float> playVolumes;
    std::vector<std::unique_ptr<juce::ToggleButton>> playButtons;
    std::vector<std::unique_ptr<juce::TextButton>> loadButtons;
    std::vector<std::unique_ptr<juce::Slider>> volumeSliders;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TestComponent)
};
