

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "PadLayout.h"

//==============================================================================
/**
*/
class WanjoDrumAudioProcessorEditor  : public juce::AudioProcessorEditor, public juce::Slider::Listener
                                       
                                       
{
public:
    WanjoDrumAudioProcessorEditor (WanjoDrumAudioProcessor&);
    ~WanjoDrumAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    void sliderValueChanged(juce::Slider* slider) override;

  

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    WanjoDrumAudioProcessor& audioProcessor; 

    juce::Label gainLabel;
    juce::Label titleLabel;
    juce::Image background;
    
    juce::Slider gainSlider;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> gainSliderAttachment;

    // pads interface
    PadLayout padInterface;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WanjoDrumAudioProcessorEditor)
};
