/*
  ==============================================================================

    MainComponent.cpp
    Created: 5 Nov 2020 11:03:55pm
    Author:  YakLion

  ==============================================================================
*/

#include <JuceHeader.h>
#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent() : playButton("Play"), stopButton("Stop"), recordButton("Record-Not working"), exportButton("Export Last Recording-Not working"),
                                 loadButton("Bank 1"), loadButton2("Bank 2"), loadButton3("Bank 3"), loadButton4("Bank 4")
{
    //initialise starting state
    state = Stopped;
    bool recording = false;
   
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

    formatManager.registerBasicFormats();
    
    //set up lambda functions for button click calls
    
    loadButton.onClick = [this] {loadButtonClicked(1);};
    loadButton2.onClick = [this] {loadButtonClicked(2);};
    loadButton3.onClick = [this] {loadButtonClicked(3);};
    loadButton4.onClick = [this] {loadButtonClicked(4);};
    playButton.onClick = [this] {playButtonClicked();};
    recordButton.onClick = [this] {recordButtonClicked();};
    exportButton.onClick = [this] {exportButtonClicked(exportBuffer);};
    stopButton.onClick = [this] {stopButtonClicked();};

    stopButton.setEnabled(false);
    playButton.setEnabled(false);
    recordButton.setEnabled(true);

    setSize(450, 800);
    slider_1.setSliderStyle(Slider::SliderStyle::LinearBarVertical);
    slider_1.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxAbove, true,50,20);
    


    //draw child components
    addAndMakeVisible(&loadButton);
    addAndMakeVisible(&loadButton2);
    addAndMakeVisible(&loadButton3);
    addAndMakeVisible(&loadButton4);
    addAndMakeVisible(&exportButton);
    addAndMakeVisible(&ld);
    //addAndMakeVisible(&audio_app);
    addAndMakeVisible(&slider_1);
    addAndMakeVisible(&playButton);
    addAndMakeVisible(&stopButton);
    addAndMakeVisible(&recordButton);
    //ProgressBar progBar = ProgressBar(loopProgress);
    //addAndMakeVisible(&progBar);

    //additional required setup
    this->playing = ld.get_playing();
    samples.resize(20);
    playPositions.resize(20);
    playVolumes.resize(5);


    //set up 5 volume slider loop, add functionality
    for (int i = 0; i < 5; i++) {
        auto volumeSlider = std::make_unique<juce::Slider>();
        volumeSlider->setSliderStyle(Slider::SliderStyle::LinearBarVertical);
        volumeSlider->setRange(-48.0f, 0.0f);
        volumeSlider->setValue(-6.0f);
        volumeSlider->setTextBoxIsEditable(false);
        volumeSlider->setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
        addAndMakeVisible(volumeSlider.get());
        volumeSlider->onValueChange = [this, i] {
            playVolumes[i] = juce::Decibels::decibelsToGain(volumeSliders[i]->getValue());
        };
        if (i == 0) {
            volumeSlider->setColour(juce::Slider::trackColourId, juce::Colours::forestgreen);
        }
        else if (i == 1) {
            volumeSlider->setColour(juce::Slider::trackColourId, juce::Colours::limegreen);
        }
        else if (i == 2) {
            volumeSlider->setColour(juce::Slider::trackColourId, juce::Colours::lightgreen);
        }
        else if (i == 3) {
            volumeSlider->setColour(juce::Slider::trackColourId, juce::Colours::yellowgreen);
        }
        else {
            volumeSlider->setColour(juce::Slider::trackColourId, juce::Colours::orange);
        }
        volumeSlider->setBounds(600, 20 + i * 50, 20, 50);
        volumeSliders.push_back(std::move(volumeSlider));
    }
    resized();
    loadButtonClicked(1);
    
    //setAudioChannels starts the audio playback - immediately calling getNextAudioBlock
    //all non-thread safe work is done before this

    
    //open the required input and output channels
    setAudioChannels(2, 2);
    //Logger::getCurrentLogger()->writeToLog("opened 2 input/output channels");
    
}

MainComponent::~MainComponent()
{
    //turn off audio in destructor
    shutdownAudio();
}

void MainComponent::paint (juce::Graphics& g)
{
    // set button colour
    loadButton.setColour(TextButton::buttonColourId, Colours::pink);
    loadButton2.setColour(TextButton::buttonColourId, Colours::blueviolet);
    loadButton3.setColour(TextButton::buttonColourId, Colours::mediumvioletred);
    loadButton4.setColour(TextButton::buttonColourId, Colours::darkviolet);
    playButton.setColour(TextButton::buttonColourId, Colours::green);
    recordButton.setColour(TextButton::buttonColourId, Colours::darkred);
    exportButton.setColour(TextButton::buttonColourId, Colours::orange);
    stopButton.setColour(TextButton::buttonColourId, Colours::red);

    g.fillAll (Colours::darkslateblue);   // clear the background
    g.setColour(juce::Colours::black);
    g.drawRect (getLocalBounds(), 5);   // draw an outline around the component
}

void MainComponent::resized()
{
    // This is where drawing of all children of the component should be done
    
    //get total screen size
    Rectangle<int> area = getLocalBounds();
    
    //get x and y values
    int x = area.getWidth();
    int y = area.getHeight();
    
    //set size of all child components
    Rectangle<int> ldArea = area.removeFromBottom(y*0.65).removeFromLeft(x*0.96);
    Rectangle<int> sl1Area = area.removeFromBottom(y*0.16).removeFromLeft(x*0.15);
    Rectangle<int> pButtonArea(x * 0.02, y * 0.01, x * 0.18, y * 0.1);
    Rectangle<int> sButtonArea(x * 0.21, y * 0.01, x * 0.18, y * 0.1);
    Rectangle<int> rButtonArea(x * 0.40, y * 0.01, x * 0.18, y * 0.1);
    Rectangle<int> eButtonArea(x * 0.02, y * 0.115, x * 0.56, y * 0.05);
    Rectangle<int> lButtonArea(x * 0.60, y * 0.01, x * 0.175, y * 0.075);
    Rectangle<int> l2ButtonArea(x * 0.79, y * 0.01, x * 0.175, y * 0.075);
    Rectangle<int> l3ButtonArea(x * 0.60, y * 0.09, x * 0.175, y * 0.075);
    Rectangle<int> l4ButtonArea(x * 0.79, y * 0.09, x * 0.175, y * 0.075);
    //Rectangle<int> progressArea(x * 0.02, y * 0.02, x * 0.96, y * 0.01);
    
    //set centre of LoopDeck and slider components
    ldArea.setCentre(x/2, y/2);
    //set position for 5 main audio sliders
    sl1Area.setCentre(x * 0.10, y * 0.91);
    //set bounds of all child components
    //progBar.setBounds(progressArea);
    playButton.setBounds(pButtonArea);
    stopButton.setBounds(sButtonArea);
    recordButton.setBounds(rButtonArea);
    exportButton.setBounds(eButtonArea);
    loadButton.setBounds(lButtonArea);
    loadButton2.setBounds(l2ButtonArea);
    loadButton3.setBounds(l3ButtonArea);
    loadButton4.setBounds(l4ButtonArea);
    ld.setBounds(ldArea);
    for (auto &vs : volumeSliders) {
        vs->setBounds(sl1Area);
        sl1Area.translate(x*0.2 , 0);
    }

    //writing logs
    //Logger::getCurrentLogger()->writeToLog("testLogging");

}


void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    //this function is called when the audio device is started or when settings are changed
    
    for(auto& position : playPositions){
        position = 0;
    }
    exportBuffer.setSize(2, 864000);
    
}
void MainComponent::getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill)
{
    if (state == Stopped) {
        this->playing = ld.get_playing();
    }
    //audio processing code goes here
    bufferToFill.clearActiveBufferRegion();
    int count = 0;
    for (int i = 0; i < samples.size(); ++i)
    {
        if (playing[i] && samples[i].getNumSamples() > 1 && state == Playing)
        {
            for (int j = 0; j < bufferToFill.numSamples; ++j)
            {
                for (int channel = 0; channel < 2; ++channel)
                {
                float current_sample = samples[i].getSample(channel, playPositions[i]);
                // apply play volume. note that this doesn't implement smoothing, so there can be crackles
                current_sample *= playVolumes[i/4];
                //if (recording) { commented because this functionality was cut from the final product
                //    exportBuffer.addSample(channel, (count * bufferToFill.numSamples) + j, current_sample);
                //    count = count + 1;
                //}
                // copy source samples to both output channels
                
                    bufferToFill.buffer->addSample(channel, bufferToFill.startSample + j, current_sample);
                }
                // increment source buffer play position and handle looping
                ++playPositions[i];
                if (playPositions[i] >= samples[i].getNumSamples()) {
                    
                    this->playing = ld.get_playing();
                    
                    //playPositions[i] = 0;
                    for (auto& position : playPositions) {
                        position = 0;
                    }
                }
                
            }
        }
        //float progress = playPositions[i] / samples[i].getNumSamples();
        //updateProgressBar(progress);
    }
}
void MainComponent::releaseResources()
{
    //this is called when the audio device stops or is being restarted due to a setting change
    transportSource.releaseResources();
}

void MainComponent::updateProgressBar(double progress) {
    //loopProgress = progress;
}

void MainComponent::recordButtonClicked() {
    if (recording) {
        recording = false;
        recordButton.setEnabled(true);
    }
    if(!recording) {
        recording = true;
        recordButton.setEnabled(false);
    }
}



void MainComponent::playButtonClicked()
{
    bool selected = false;
    this->playing = ld.get_playing();
    for (int i = 0; i < playing.size(); i++) {
        if (playing[i] == true) {
            selected = true;
        }
    }
    if(selected){
        playButton.setEnabled(false);
        stopButton.setEnabled(true);
        loadButton.setEnabled(false);
        loadButton2.setEnabled(false);
        loadButton3.setEnabled(false);
        loadButton4.setEnabled(false);
        state = Playing;
    }
}

void MainComponent::stopButtonClicked()
{
    stopButton.setEnabled(false);
    playButton.setEnabled(true);
    recordButton.setEnabled(true);
    recording = false;
    state = Stopped;
    for (auto& position : playPositions) {
        position = 0;
    }
    if (bank == 1) {
        loadButton.setEnabled(false);
    }
    else {
        loadButton.setEnabled(true);
    }
    if (bank == 2) {
        loadButton2.setEnabled(false);
    }
    else {
        loadButton2.setEnabled(true);
    }
    if (bank == 3) {
        loadButton3.setEnabled(false);
    }
    else {
        loadButton3.setEnabled(true);
    }
    if (bank == 4) {
        loadButton4.setEnabled(false);
    }
    else {
        loadButton4.setEnabled(true);
    }
}

void MainComponent::loadButtonClicked(int banks)
{
    samples.clear();
    names.clear();
    // attempting to find a sounds directory
    auto directory = juce::File::getSpecialLocation(juce::File::SpecialLocationType::userDocumentsDirectory).getFullPathName();
    directory += R"(\LoopDeckAudio\base_samples\)"; 
    if (banks == 1) {
        directory += R"(\bank_1\)";
        bank = banks;
        loadButton.setEnabled(false);
        stopButtonClicked();
    }
    else if (banks == 2) {
        directory += R"(\bank_2\)";
        bank = banks;
        loadButton2.setEnabled(false);
        stopButtonClicked();
    }
    else if (banks == 3) {
        directory += R"(\bank_3\)";
        bank = banks;
        loadButton3.setEnabled(false);
        stopButtonClicked();
    }
    else if (banks == 4) {
        directory += R"(\bank_4\)"; 
        bank = banks;
        loadButton4.setEnabled(false);
        stopButtonClicked();
    }
    // loading audioSamples from a specific directory
    juce::File file(directory);
    if (file.exists()) { 
        for (juce::DirectoryEntry entry : juce::RangedDirectoryIterator(file, true))
            if (!entry.isDirectory()) {
                File audioSampleFile = entry.getFile();
                names.push_back(audioSampleFile.getFileNameWithoutExtension());
                if (audioSampleFile.exists()) {
                    auto reader = formatManager.createReaderFor(audioSampleFile);
                    if (reader != nullptr) {
                        auto numChannels = (int)reader->numChannels;
                        juce::AudioBuffer<float> tempBuffer(reader->numChannels, reader->lengthInSamples);
                        reader->read(&tempBuffer, 0, reader->lengthInSamples, 0, true, true);
                        samples.push_back(tempBuffer);
                        delete reader;
                        
                    }
                    else {
                        Logger::getCurrentLogger()->writeToLog("reader is nullptr");
                    }
                }
            }
    }
    if(!samples.empty()){
    playButton.setEnabled(true);
    }
    for (int i = 0; i < samples.size(); i++) {
        if(samples[i].getNumSamples() < 1){
            ld.playButtons[i]->setEnabled(false);
            ld.playButtons[i]->setButtonText("missing");
        }
        else{
            ld.playButtons[i]->setEnabled(true);
        }
        ld.playButtons[i]->setName(names[i]);
        ld.playButtons[i]->triggerClick();
        ld.playButtons[i]->triggerClick();
        ld.playButtons[i]->resized();


    }
}

void MainComponent::exportButtonClicked(AudioBuffer<float> exBuff)
{
    //this function has been commented out because it ran into an error which cannot be easily fixed,
    //therefore, this functionality is not present

    //export to WAV file code
    //AudioBuffer<float> exportBuffer = exBuff;
    //auto directory = juce::File::getSpecialLocation(juce::File::SpecialLocationType::userDocumentsDirectory).getFullPathName();
    //directory += R"(\LoopDeckAudio\exports\test.wav)";
    //File file(directory);
    //file.deleteFile();

    //WavAudioFormat wav_format;
    //auto output = std::unique_ptr<FileOutputStream> (file.createOutputStream());
    //auto writer = wav_format.createWriterFor(output.get(), 48000, exportBuffer.getNumChannels(), 32, {}, 0);
    //output.release();

    //if (writer != nullptr) {
    //   writer->writeFromAudioSampleBuffer(exportBuffer, 0, exportBuffer.getNumSamples());
    //    writer = nullptr;
    //}
    //else {
    //    Logger::getCurrentLogger()->writeToLog("writer is nullptr");
    //}

    //writer.reset(wav_format.createWriterFor(file.createOutputStream(), 48000, exportBuffer.getNumChannels(), 32, {}, 0));

}