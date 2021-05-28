#include "TestComponent.h"
#include "LoopToggleButton.h"
//==============================================================================
TestComponent::TestComponent()
{
    //set size of all vectors to size of required elements
    setSize(500, 550);
    int n = 10;
    playing.resize(n, false);
    playBuffers.resize(n);
    playPositions.resize(n);
    playVolumes.resize(n, 0.5f);
    //iterate over the elements and create one play, load buttons and one volume slider 
    for (int i = 0; i < n; ++i)
    {

        //set up play toggle buttons, draw then and provide onClick functionality
        auto button = std::make_unique<LoopToggleButton>(); 
        button->setButtonText("PLAY "); 
        addAndMakeVisible(button.get()); 
        button->setBounds(10, 1 + i * 50, 200, 49); 
        button->onClick = [this, i]() 
        { 
            playing[i] = playButtons[i]->getToggleState();
        };
        //add all play buttons to vector
        playButtons.push_back(std::move(button));
        
        //set up load buttons, draw provide onClick loading using a windows FileChooser
        auto loadbutton = std::make_unique<juce::TextButton>();
        loadbutton->setButtonText("Load...");
        addAndMakeVisible(loadbutton.get());
        loadbutton->setBounds(215, 1 + i * 50, 105, 49);
        loadbutton->onClick = [this, i]()
        {
            /*/ WINDOWS FILECHOOSER REQUIRED - COMMENTED OUT TO COMPILE FOR ADNROID
            juce::FileChooser chooser("Load audio file...", juce::File::getSpecialLocation(juce::File::userHomeDirectory), "*.wav");
            auto file = juce::File();
            if (chooser.browseForFileToOpen())
            {
                file = chooser.getResult();
            }
            if (file != juce::File())
            {
                juce::AudioFormatManager manager;
                manager.registerBasicFormats();
                auto reader = std::unique_ptr<juce::AudioFormatReader>(manager.createReaderFor(file));
                if (reader)
                {
                    juce::AudioBuffer<float> tempBuffer(1, reader->lengthInSamples);
                    reader->read(&tempBuffer, 0, reader->lengthInSamples, 0, true, true);

                    playButtons[i]->setButtonText(file.getFileName());
                    playPositions[i] = 0;
                    {
                        std::swap(playBuffers[i], tempBuffer);
                    }
                };
            };*/
        };
            //add loadButtons to a vector
            loadButtons.push_back(std::move(loadbutton));

            //set up volume sliders, draw them
            auto volume = std::make_unique<juce::Slider>();
            volume->setRange(-30.0f, 0.0f);
            volume->setValue(0.0f, juce::dontSendNotification);
            addAndMakeVisible(volume.get());
            //provide volume slider onValueChange listener functionality to change the volume value in the playVolumes vector
            volume->onValueChange = [this, i]()
            {
                playVolumes[i] = juce::Decibels::decibelsToGain(volumeSliders[i]->getValue());
            };
            volume->setBounds(320, 1 + i * 50, 160, 49);
            //add sliders to vector
            volumeSliders.push_back(std::move(volume));
        };
   
    //open audio input/output devices
    setAudioChannels(2, 2);
    
}

TestComponent::~TestComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void TestComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    for (auto& position : playPositions)
        position = 0;
}

void TestComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    bufferToFill.clearActiveBufferRegion();
    for (int i = 0; i < playBuffers.size(); ++i)
    {
        if (playing[i] && playBuffers[i].getNumSamples() > 1)
        {
            for (int j = 0; j < bufferToFill.numSamples; ++j)
            {
                float currentsample = playBuffers[i].getSample(0, playPositions[i]);
                // multiply by volume
                currentsample *= playVolumes[i];
                // copy source sample from left channel to both channels, making a mono audioBuffer
                for (int channel = 0; channel < 2; ++channel)
                {
                    bufferToFill.buffer->addSample(channel, bufferToFill.startSample + j, currentsample);
                }
                playPositions[i] = playPositions[i] + 1;
            }
        }
    }
}

void TestComponent::releaseResources()
{

}

//==============================================================================
void TestComponent::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

    // You can add your drawing code here!
}

void TestComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
}