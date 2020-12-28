/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
BlueSamplerAudioProcessorEditor::BlueSamplerAudioProcessorEditor (BlueSamplerAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    mLoadButton.onClick = [&]() { audioProcessor.loadFile(); };
    addAndMakeVisible (mLoadButton);
    
    // Attack Slider
    mAttackSlider.setSliderStyle (Slider::SliderStyle::RotaryVerticalDrag);
    mAttackSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 40, 20);
    mAttackSlider.setRange (0.0f, 5.0f, 0.01f);
    mAttackSlider.addListener(this);
    addAndMakeVisible (mAttackSlider);
    
    mAttackLabel.setFont(10.0f);
    mAttackLabel.setText("Attack", NotificationType::dontSendNotification);
    mAttackLabel.setJustificationType(Justification::centredTop);
    mAttackLabel.attachToComponent(&mAttackSlider, false);
    
    // Decay Slider
    mDecaySlider.setSliderStyle (Slider::SliderStyle::RotaryVerticalDrag);
    mDecaySlider.setTextBoxStyle(Slider::TextBoxBelow, true, 40, 20);
    mDecaySlider.setRange (0.0f, 1.0f, 0.01f);
    mDecaySlider.addListener(this);
    addAndMakeVisible (mDecaySlider);
    
    mDecayLabel.setFont(10.0f);
    mDecayLabel.setText("Decay", NotificationType::dontSendNotification);
    mDecayLabel.setJustificationType(Justification::centredTop);
    mDecayLabel.attachToComponent(&mDecaySlider, false);
    
    // Sustain Slider
    mSustainSlider.setSliderStyle (Slider::SliderStyle::RotaryVerticalDrag);
    mSustainSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 40, 20);
    mSustainSlider.setRange (0.0f, 1.0f, 0.01f);
    mSustainSlider.addListener(this);
    addAndMakeVisible (mSustainSlider);
    
    mSustainLabel.setFont(10.0f);
    mSustainLabel.setText("Sustain", NotificationType::dontSendNotification);
    mSustainLabel.setJustificationType(Justification::centredTop);
    mSustainLabel.attachToComponent(&mSustainSlider, false);
    
    // Release Slider
    mReleaseSlider.setSliderStyle (Slider::SliderStyle::RotaryVerticalDrag);
    mReleaseSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 40, 20);
    mReleaseSlider.setRange (0.0f, 1.0f, 0.01f);
    mReleaseSlider.addListener(this);
    addAndMakeVisible (mReleaseSlider);
    
    mReleaseLabel.setFont(10.0f);
    mReleaseLabel.setText("Release", NotificationType::dontSendNotification);
    mReleaseLabel.setJustificationType(Justification::centredTop);
    mReleaseLabel.attachToComponent(&mReleaseSlider, false);
    
    setSize (600, 200);
}

BlueSamplerAudioProcessorEditor::~BlueSamplerAudioProcessorEditor()
{
}

//==============================================================================
void BlueSamplerAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(Colours::black);
    
    g.setColour(Colours::white);
    
    if (mShouldBePainting) {
        Path p;
        mAudioPoints.clear();
        
        auto waveform = audioProcessor.getWaveForm();
        auto ratio = waveform.getNumSamples() / getWidth();
        auto buffer = waveform.getReadPointer(0);
        
        // scale audio file to window on x axis
        for(int sample = 0; sample < waveform.getNumSamples(); sample += ratio)
        {
            mAudioPoints.push_back(buffer[sample]);
        }
        
        p.startNewSubPath(0, getHeight() / 2);
        
        for (int sample = 0; sample < mAudioPoints.size(); ++sample)
        {
            // scale on y axis
            auto point = jmap<float> (mAudioPoints[sample], -1.0f, 1.0f, 200, 0);
            p.lineTo(sample, point);
            
        }
        
        g.strokePath(p, PathStrokeType(2));

        mShouldBePainting = false;
    }

    
    
//    g.setColour(Colours::white);
//    g.setFont(15.0f);
//    
//    if(audioProcessor.getNumSamplerSounds() > 0)
//    {
//        g.fillAll (Colours::red);
//        g.drawText("Sound Loaded", getWidth() / 2 - 50, getHeight() / 2 - 10, 100, 20, Justification::centred);
//    }
//    else
//    {
//        g.drawText("Load a Sound", getWidth() / 2 - 50, getHeight() / 2 - 10, 100, 20, Justification::centred);
//    }
//    
//    //if we have a sound
//        //"sound loaded"
//    //else
//        //"load a sound"
}

void BlueSamplerAudioProcessorEditor::resized()
{
    const auto startX = 0.6f;
    const auto startY = 0.6f;
    const auto dialWidth = 0.1f;
    const auto dialHeight = 0.4f;
    
    mAttackSlider.setBoundsRelative (startX, startY, dialWidth, dialHeight);
    mDecaySlider.setBoundsRelative (startX + dialWidth, startY, dialWidth, dialHeight);
    mSustainSlider.setBoundsRelative (startX + (dialWidth * 2), startY, dialWidth, dialHeight);
    mReleaseSlider.setBoundsRelative (startX + (dialWidth * 3), startY, dialWidth, dialHeight);
    
//    mLoadButton.setBounds(getWidth() / 2 - 50, getHeight() / 2 - 50, 100, 100);
//
}

bool BlueSamplerAudioProcessorEditor::isInterestedInFileDrag (const StringArray& files)
{
    for (auto file : files)
    {
        if (file.contains(".wav") || file.contains(".mp3") || file.contains(".aif"))
        {
            return true;
        }
    };
    
    return false;
}


void BlueSamplerAudioProcessorEditor::filesDropped(const StringArray& files, int x, int y)
{
    for (auto file : files) {
        if (isInterestedInFileDrag(file)) {
            mShouldBePainting = true;
            audioProcessor.loadFile(file);
        }
    }
    
    repaint();
}


void BlueSamplerAudioProcessorEditor::sliderValueChanged (Slider* slider)
{
    if (slider == &mAttackSlider) {
        audioProcessor.getADSRParams().attack = mAttackSlider.getValue();
    }
    else if (slider == &mDecaySlider)
    {
        audioProcessor.getADSRParams().decay = mDecaySlider.getValue();
    }
    else if (slider == &mSustainSlider)
    {
        audioProcessor.getADSRParams().sustain = mSustainSlider.getValue();
    }
    else if (slider == &mReleaseSlider)
    {
        audioProcessor.getADSRParams().release = mReleaseSlider.getValue();
    }
    
    audioProcessor.updateADSR();
}
