/*
  ==============================================================================

    LoopToggleButton.h
    Created: 30 Dec 2020 2:10:45pm
    Author:  YakLion

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class LoopToggleButton  : public juce::ToggleButton
{
public:
    LoopToggleButton()
    {
        // In your constructor, you should add any child components, and
        // initialise any special settings that your component needs.
        //bool playing = false;
        id = 0;
        //name = "";
    }

    ~LoopToggleButton() override
    {
    }

    void paint(juce::Graphics& g) override
    {
        /* 
        * Button drawing code
        */
        auto a = getLocalBounds();
        a.reduce(10, 10);
        g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));   // clear the background
        // draw an outline around the component
        g.setColour(juce::Colours::black);
        g.drawRect(getLocalBounds(), 5);
        //fill background colour
        if (this->getToggleState()) {
            if(this->id / 4 == 0){
                g.setColour(juce::Colours::forestgreen);
            }
            else if(this->id / 4 == 1) {
                g.setColour(juce::Colours::limegreen);
            }
            else if (this->id / 4 == 2) {
                g.setColour(juce::Colours::lightgreen);
            }
            else if (this->id / 4 == 3) {
                g.setColour(juce::Colours::yellowgreen);
            }
            else {
                g.setColour(juce::Colours::orange);
            }
            g.fillRect(a);
        }
        else {
            g.setColour(juce::Colours::darkslategrey);
            g.fillRect(a);

        }

        g.setColour (juce::Colours::white);
        g.setFont (14.0f);
        g.drawText (getName(), getLocalBounds(),
                    juce::Justification::centred, true);   // draw some placeholder text
    }

    void setID(int newID) {
        this->id = newID;
    }

    String getName() {
        return this->name;
    }
    void setName(String newName) {
        this->name = newName;
    }

    void resized() override
    {
        //has no child components so is left empty
    }


private:
    int id;
    String name;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LoopToggleButton)
};
