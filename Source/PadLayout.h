/*
  ==============================================================================

    PadLayout.h
    Created: 25 Oct 2023 9:23:22am
    Author:  rjbal

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
class PadLayout : public juce::Component
{
public:
    PadLayout(WanjoDrumAudioProcessor& processor);
    ~PadLayout() override;

    void paint(juce::Graphics&) override;
    void resized() override;

    void setupPadButton(juce::TextButton& padButton, const PadInfo& padInfo);

private:

    WanjoDrumAudioProcessor& audioProcessor;

    // interface buttons
    juce::TextButton pad1;
    juce::TextButton pad2;
    juce::TextButton pad3;
    juce::TextButton pad4;
    juce::TextButton pad5;
    juce::TextButton pad6;
    juce::TextButton pad7;
    juce::TextButton pad8;
    juce::TextButton pad9;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PadLayout)
};
