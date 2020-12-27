/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class BlueSamplerAudioProcessorEditor  : public juce::AudioProcessorEditor,
                                         public FileDragAndDropTarget,
                                         public Slider::Listener
{
public:
    BlueSamplerAudioProcessorEditor (BlueSamplerAudioProcessor&);
    ~BlueSamplerAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
    bool isInterestedInFileDrag (const StringArray& files) override;
    void filesDropped (const StringArray& files, int x, int y) override;
    
    void sliderValueChanged(Slider* slider) override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    TextButton mLoadButton { "Load" };
    std::vector<float> mAudioPoints;
    bool mShouldBePainting {false};
    
    Slider mAttackSlider, mDecaySlider, mSustainSlider, mReleaseSlider;
    
    Label mAttackLabel, mDecayLabel, mSustainLabel, mReleaseLabel;
    
    BlueSamplerAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BlueSamplerAudioProcessorEditor)
};
