#include <JuceHeader.h>
#include "PadLayout.h"

// function to set each pad up with parameters 
void PadLayout::setupPadButton(juce::TextButton& padButton, const PadInfo& padInfo)
{
    addAndMakeVisible(&padButton);
    padButton.setButtonText("Load..");

    const int padKeyAss = padInfo.mKeyAssign;
    const int padMidiNoteForNormalPitch = padInfo.mMidiNoteNormalPitch;

    const juce::String& filePath = padInfo.mFilePath;
    if (!filePath.isEmpty())
    {
        padButton.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::darkred);
        padButton.setButtonText(padInfo.mFileName);
    }

    padButton.onClick = [this, padKeyAss, padMidiNoteForNormalPitch, &padInfo, &padButton]
        {
            audioProcessor.loadFile(padKeyAss, padMidiNoteForNormalPitch, padInfo.mPadNum);
            padButton.setButtonText(padInfo.mFileName);
            padButton.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::darkred);
            audioProcessor.reloadSampler(padKeyAss, padMidiNoteForNormalPitch, padInfo.mPadNum);
        };
}



//==============================================================================
PadLayout::PadLayout(WanjoDrumAudioProcessor& processor) : audioProcessor(processor)
{

    // set up pads
    setupPadButton(pad1, audioProcessor.pad1Info);
    setupPadButton(pad2, audioProcessor.pad2Info);
    setupPadButton(pad3, audioProcessor.pad3Info);
    setupPadButton(pad4, audioProcessor.pad4Info);
    setupPadButton(pad5, audioProcessor.pad5Info);
    setupPadButton(pad6, audioProcessor.pad6Info);
    setupPadButton(pad7, audioProcessor.pad7Info);
    setupPadButton(pad8, audioProcessor.pad8Info);
    setupPadButton(pad9, audioProcessor.pad9Info);

}

PadLayout::~PadLayout()
{
}

void PadLayout::paint(juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll(juce::Colours::darkgrey);   // clear the background

    g.setColour(juce::Colours::black);
    g.drawRect(getLocalBounds(), 0);   // draw an outline around the component

    g.setColour(juce::Colours::black);

}

void PadLayout::resized()
{
    pad1.setBounds(5, 5, 80, 80);
    pad2.setBounds(95, 5, 80, 80);
    pad3.setBounds(185, 5, 80, 80);
    pad4.setBounds(5, 95, 80, 80);
    pad5.setBounds(95, 95, 80, 80);
    pad6.setBounds(185, 95, 80, 80);
    pad7.setBounds(5, 185, 80, 80);
    pad8.setBounds(95, 185, 80, 80);
    pad9.setBounds(185, 185, 80, 80);


}
