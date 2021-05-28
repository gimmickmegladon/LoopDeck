/*
  ==============================================================================

    LoopDeckComponent.h
    Created: 5 Nov 2020 10:53:23pm
    Author:  YakLion

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "LoopToggleButton.h"



class LoopDeckComponent : public juce::Component
{

public:
    LoopDeckComponent();
        

    ~LoopDeckComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    std::vector<bool> get_playing();
        

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LoopDeckComponent)

    std::vector<bool> playing;
public:
    std::vector<std::unique_ptr<LoopToggleButton>> playButtons;
private:
    std::vector<LoopToggleButton> newButtons;
    

};
