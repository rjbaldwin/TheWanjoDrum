/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>


//==============================================================================

struct PadInfo
{
    PadInfo(int keyAssign, int midiNoteNormalPitch, int padNum);

    juce::String mFileName;
    juce::String mFilePath;
    int mKeyAssign;
    int mMidiNoteNormalPitch;
    int mPadNum;


};


//==============================================================================

class WanjoDrumAudioProcessor  : public juce::AudioProcessor
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif
{
public:
    //==============================================================================
    WanjoDrumAudioProcessor();
    ~WanjoDrumAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    //==============================================================================
  
 

    // pad loadfiles
    void loadFile(int keyAssignment, int midiNoteForNomalPitch, int padNumber);
    void padParamsAssign(PadInfo& pad,
        juce::String filename,
        const juce::String& filepath,
        const int& keyAssignment,
        const int& midiNoteForNormalPitch,

        int& keyAssignmentValue,
        int& midiNoteValue);

    void getPadParamsAssign(PadInfo& pad, const juce::String& filePathName, const int& keyAssignment, const int& midiNoteForPitch);

    int getNumSamplerSounds() { return mSampler.getNumSounds(); }
    void fillSampler(const juce::File& file, int keyAssignment, int midiNoteForNomalPitch, int padNumber);
    void reloadSampler(int keyAssignment, int midiNoteForNomalPitch, int padNumber);

    // to do
    juce::String getPadFilePath(int padNumber);
    int getPadKeyAssignment(int padNumber);
    int getPadMidiNote(int padNumber);

    //==================

    float gain;

    juce::AudioProcessorValueTreeState apvts;

    juce::AudioVisualiserComponent waveViewer;

    PadInfo pad1Info{ 60,60,1 };
    PadInfo pad2Info{ 61,61,2 };
    PadInfo pad3Info{ 62,62,3 };
    PadInfo pad4Info{ 63,63,4 };
    PadInfo pad5Info{ 64,64,5 };
    PadInfo pad6Info{ 65,65,6 };
    PadInfo pad7Info{ 66,66,7 };
    PadInfo pad8Info{ 67,67,8 };
    PadInfo pad9Info{ 68,68,9 };

   
private:
    //==============================================================================

  
    juce::Synthesiser mSampler;
    const int mNumVoices{ 9 }; // 9 voice - 1 per pad
    
    juce::AudioFormatManager mFormatManager;
    juce::AudioFormatReader* mFormatReader{ nullptr };
    
    juce::AudioProcessorValueTreeState::ParameterLayout createParameters();

    const double maxSampleLengthSecs{ 240 };

  

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WanjoDrumAudioProcessor)
};
