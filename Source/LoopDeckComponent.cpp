/*
  ==============================================================================

    LoopDeckComponent.cpp
    Created: 5 Nov 2020 10:53:23pm
    Author:  YakLion

  ==============================================================================
*/

#include <JuceHeader.h>
#include "LoopDeckComponent.h"


//==============================================================================
//LoopDeckComponent::LoopDeckComponent()
//{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

//}

LoopDeckComponent::LoopDeckComponent()
{
    //get total screen size
    //setSize(getBounds().getWidth(), getBounds().getHeight()); //can replace with x and y
    Rectangle<int> area = getLocalBounds();

    //get x and y values
    int x = area.getWidth();
    int y = area.getHeight();
    int buttonW = x / 4;
    int buttonH = y / 5;
    const int col = 4;
    const int row = 5;

    area = getLocalBounds().removeFromBottom(y / 5).removeFromLeft(x / 4);
    playing.resize(20, false);

    area.setCentre(x * 0.125, y * 0.1);
    for (size_t i = 0; i < int(col*row); ++i)
    {
        auto button = std::make_unique<LoopToggleButton>();
        addAndMakeVisible(button.get());
        
        button->setBounds(area);
        button->onClick = [this, i]()
        {
            playing[i] = playButtons[i]->getToggleState();            
        };
        playButtons.push_back(std::move(button));
    }
    

}

LoopDeckComponent::~LoopDeckComponent() {};

void LoopDeckComponent::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code.
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::darkolivegreen);
    auto a = getLocalBounds();
    a.reduce(10, 10);
    g.drawRect (a, 10);   // draw an outline around the component
    
}

void LoopDeckComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    //get total screen size
    Rectangle<int> area = getLocalBounds();

    //get x and y values
    int x = area.getWidth();
    int y = area.getHeight();
    area = area.removeFromBottom(y / 5).removeFromLeft(x / 4);
    int buttonW = x / 4;
    int buttonH = y / 5;
    int row = 5;
    int col = 4;
    int i = 0;
    area.setPosition(0, 0);
    for (auto &button : playButtons)
    {
        button->setBounds(area);
        button->setID(i);
        area.translate(buttonW, 0);
        i++;
        if (i % 4 == 0) {
            area.translate(-4 * buttonW, buttonH);
        }
    }
        

}


std::vector<bool> LoopDeckComponent::get_playing() {
    return playing;
}