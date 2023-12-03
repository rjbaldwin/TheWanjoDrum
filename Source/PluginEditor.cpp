

#include "PluginProcessor.h"
#include "PluginEditor.h"



//==============================================================================
WanjoDrumAudioProcessorEditor::WanjoDrumAudioProcessorEditor (WanjoDrumAudioProcessor& p)
                                                : AudioProcessorEditor{&p},
                                                  audioProcessor{p},
                                                  padInterface{p}
{

    // title label
   



    // main interface
    addAndMakeVisible(padInterface);

    setSize(370, 410);

    // wave display

    addAndMakeVisible(audioProcessor.waveViewer);
    audioProcessor.waveViewer.setColours(juce::Colours::black, juce::Colours::green.withAlpha(0.5f));

 //============================================================================

 // master gain slider
    gainSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    gainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 200, 50);
    gainSlider.addListener(this);
    addAndMakeVisible(gainSlider);

    // gain label
    addAndMakeVisible(gainLabel);
    gainLabel.setText("Gain", juce::dontSendNotification);
    gainLabel.attachToComponent(&gainSlider, false);


    gainSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "MASTERGAIN", gainSlider);

}

WanjoDrumAudioProcessorEditor::~WanjoDrumAudioProcessorEditor()
{
}

//==============================================================================
void WanjoDrumAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    
   // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(juce::Colours::grey);
    g.setColour(juce::Colours::black);
    g.setFont(15.0f);

    background = juce::ImageCache::getFromMemory(BinaryData::back_jpg, BinaryData::back_jpgSize);
    g.drawImageWithin(background, 0, 0, getWidth(), getHeight(), juce::RectanglePlacement::doNotResize);
    

}

void WanjoDrumAudioProcessorEditor::resized()
{
    titleLabel.setBounds(10, 10, getWidth() - 20, 30);
    gainSlider.setBounds(300, 70, 70, 150);
    // main interface
    padInterface.setBounds(20, 50, 270, 270);

    audioProcessor.waveViewer.setBounds(20,320,270,70);
}



//==============================================================================
// Rob, remember to try to read this like a sentence!
// 
// For log gain slider values 
void WanjoDrumAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    // if the gainSlider is changed, assign the new values to rawVolume variable for processing in the process block!
    if (slider == &gainSlider)
    {
        audioProcessor.gain = pow(10, gainSlider.getValue() / 20);

        //output lider value debug info.  This slider works, judging by the console out values!
        //DBG("Gain: " << audioProcessor.gain);
    }

}



