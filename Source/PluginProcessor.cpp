// WanjoDrum v1 - created by Robert J Baldwin.  Started 22/09/2023
// All pads saving sounds 17/10/2023
// Code Refactoring 22/10/2023
// Completed - my first proper music project.  25/10/2023
// Realised the difficulty of implememnting individual real-time pitch shifting.  That's why zPlane is used by the big guys.
// Not gonna purchase that for this learning project

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================

PadInfo::PadInfo(int keyAssign, int midiNoteNormalPitch, int padNum)
    :

    mKeyAssign{ keyAssign },
    mMidiNoteNormalPitch{ midiNoteNormalPitch },
    mPadNum{ padNum } {}

//==============================================================================


WanjoDrumAudioProcessor::WanjoDrumAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
                    : AudioProcessor (BusesProperties()
                        #if ! JucePlugin_IsMidiEffect
                        #if ! JucePlugin_IsSynth
                        .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                        #endif
                        .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                        #endif
                        ),
                    apvts{ *this, nullptr,"Parameters",createParameters()},
                    waveViewer(1)
               

#endif

{
    mFormatManager.registerBasicFormats();

    for (int i = 0; i < mNumVoices; i++)
    {
        mSampler.addVoice(new juce::SamplerVoice());
    }

    waveViewer.setRepaintRate(30);
    waveViewer.setBufferSize(256);
    


}


WanjoDrumAudioProcessor::~WanjoDrumAudioProcessor()
{
    mFormatReader = nullptr;
}



//==============================================================================
const juce::String WanjoDrumAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool WanjoDrumAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool WanjoDrumAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool WanjoDrumAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double WanjoDrumAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int WanjoDrumAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int WanjoDrumAudioProcessor::getCurrentProgram()
{
    return 0;
}

void WanjoDrumAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String WanjoDrumAudioProcessor::getProgramName (int index)
{
    return {};
}

void WanjoDrumAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void WanjoDrumAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    mSampler.setCurrentPlaybackSampleRate(sampleRate);

}

void WanjoDrumAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool WanjoDrumAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else

    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif




//=============================PROCESS BLOCK====================================================================


void WanjoDrumAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();


    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
    
        buffer.clear(i, 0, buffer.getNumSamples());

        auto g = apvts.getRawParameterValue("MASTERGAIN");
       
       
        // output the sampler contents
        mSampler.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
        
        // apply gain values to buffer contents
        buffer.applyGain(*g);

        waveViewer.pushBuffer(buffer);
        
       
}




//==============================================================================
bool WanjoDrumAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* WanjoDrumAudioProcessor::createEditor()
{
    return new WanjoDrumAudioProcessorEditor (*this);
}




//==============================================================================



void WanjoDrumAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{

    // The host will call this method when it wants to save the processor's internal state.
    // This must copy any info about the processor's state into the block of memory provided, 
    // so that the host can store this and later restore it using setStateInformation().

    //DBG("GetStateInfo called\n");

    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());

    // Add pad parameters to the XML
    xml->setAttribute("PAD1_FILEPATH", pad1Info.mFilePath);
    xml->setAttribute("PAD1_KEYASSIGNMENT", pad1Info.mKeyAssign);
    xml->setAttribute("PAD1_MIDINOTE", pad1Info.mMidiNoteNormalPitch);

    xml->setAttribute("PAD2_FILEPATH", pad2Info.mFilePath);
    xml->setAttribute("PAD2_KEYASSIGNMENT", pad2Info.mKeyAssign);
    xml->setAttribute("PAD2_MIDINOTE", pad2Info.mMidiNoteNormalPitch);

    xml->setAttribute("PAD3_FILEPATH", pad3Info.mFilePath);
    xml->setAttribute("PAD3_KEYASSIGNMENT", pad3Info.mKeyAssign);
    xml->setAttribute("PAD3_MIDINOTE", pad3Info.mMidiNoteNormalPitch);

    xml->setAttribute("PAD4_FILEPATH", pad4Info.mFilePath);
    xml->setAttribute("PAD4_KEYASSIGNMENT", pad4Info.mKeyAssign);
    xml->setAttribute("PAD4_MIDINOTE", pad4Info.mMidiNoteNormalPitch);

    xml->setAttribute("PAD5_FILEPATH", pad5Info.mFilePath);
    xml->setAttribute("PAD5_KEYASSIGNMENT", pad5Info.mKeyAssign);
    xml->setAttribute("PAD5_MIDINOTE", pad5Info.mMidiNoteNormalPitch);

    xml->setAttribute("PAD6_FILEPATH", pad6Info.mFilePath);
    xml->setAttribute("PAD6_KEYASSIGNMENT", pad6Info.mKeyAssign);
    xml->setAttribute("PAD6_MIDINOTE", pad6Info.mMidiNoteNormalPitch);

    xml->setAttribute("PAD7_FILEPATH", pad7Info.mFilePath);
    xml->setAttribute("PAD7_KEYASSIGNMENT", pad7Info.mKeyAssign);
    xml->setAttribute("PAD7_MIDINOTE", pad7Info.mMidiNoteNormalPitch);

    xml->setAttribute("PAD8_FILEPATH", pad8Info.mFilePath);
    xml->setAttribute("PAD8_KEYASSIGNMENT", pad8Info.mKeyAssign);
    xml->setAttribute("PAD8_MIDINOTE", pad8Info.mMidiNoteNormalPitch);

    xml->setAttribute("PAD9_FILEPATH", pad9Info.mFilePath);
    xml->setAttribute("PAD9_KEYASSIGNMENT", pad9Info.mKeyAssign);
    xml->setAttribute("PAD9_MIDINOTE", pad9Info.mMidiNoteNormalPitch);
    


    // Convert the XML to a string
    juce::String xmlString = xml->toString();

    // Print the XML to the console
    //DBG("XML Content:\n" << xmlString);

    copyXmlToBinary(*xml, destData);

 

    
}

void WanjoDrumAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // This must restore the processor's state from a block of data previously created using getStateInformation(). 
    //DBG("SetStateInfo called\n");

    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    if (xmlState.get() != nullptr)
    {
        if (xmlState->hasTagName(apvts.state.getType()))
            apvts.replaceState(juce::ValueTree::fromXml(*xmlState));
        // Retrieve the details from the ValueTree
        pad1Info.mFilePath = xmlState->getStringAttribute("PAD1_FILEPATH");
        pad1Info.mKeyAssign = xmlState->getIntAttribute("PAD1_KEYASSIGNMENT", 0);
        pad1Info.mMidiNoteNormalPitch = xmlState->getIntAttribute("PAD1_MIDINOTE", 0);

        pad2Info.mFilePath = xmlState->getStringAttribute("PAD2_FILEPATH");
        pad2Info.mKeyAssign = xmlState->getIntAttribute("PAD2_KEYASSIGNMENT", 0);
        pad2Info.mMidiNoteNormalPitch = xmlState->getIntAttribute("PAD2_MIDINOTE", 0);

        pad3Info.mFilePath = xmlState->getStringAttribute("PAD3_FILEPATH");
        pad3Info.mKeyAssign = xmlState->getIntAttribute("PAD3_KEYASSIGNMENT", 0);
        pad3Info.mMidiNoteNormalPitch = xmlState->getIntAttribute("PAD3_MIDINOTE", 0);

        pad4Info.mFilePath = xmlState->getStringAttribute("PAD4_FILEPATH");
        pad4Info.mKeyAssign = xmlState->getIntAttribute("PAD4_KEYASSIGNMENT", 0);
        pad4Info.mMidiNoteNormalPitch = xmlState->getIntAttribute("PAD4_MIDINOTE", 0);

        pad5Info.mFilePath = xmlState->getStringAttribute("PAD5_FILEPATH");
        pad5Info.mKeyAssign = xmlState->getIntAttribute("PAD5_KEYASSIGNMENT", 0);
        pad5Info.mMidiNoteNormalPitch = xmlState->getIntAttribute("PAD5_MIDINOTE", 0);

        pad6Info.mFilePath = xmlState->getStringAttribute("PAD6_FILEPATH");
        pad6Info.mKeyAssign = xmlState->getIntAttribute("PAD6_KEYASSIGNMENT", 0);
        pad6Info.mMidiNoteNormalPitch = xmlState->getIntAttribute("PAD6_MIDINOTE", 0);

        pad7Info.mFilePath = xmlState->getStringAttribute("PAD7_FILEPATH");
        pad7Info.mKeyAssign = xmlState->getIntAttribute("PAD7_KEYASSIGNMENT", 0);
        pad7Info.mMidiNoteNormalPitch = xmlState->getIntAttribute("PAD7_MIDINOTE", 0);

        pad8Info.mFilePath = xmlState->getStringAttribute("PAD8_FILEPATH");
        pad8Info.mKeyAssign = xmlState->getIntAttribute("PAD8_KEYASSIGNMENT", 0);
        pad8Info.mMidiNoteNormalPitch = xmlState->getIntAttribute("PAD8_MIDINOTE", 0);

        pad9Info.mFilePath = xmlState->getStringAttribute("PAD9_FILEPATH");
        pad9Info.mKeyAssign = xmlState->getIntAttribute("PAD9_KEYASSIGNMENT", 0);
        pad9Info.mMidiNoteNormalPitch = xmlState->getIntAttribute("PAD9_MIDINOTE", 0);




        // Reload the sampler
        reloadSampler(pad1Info.mKeyAssign, pad1Info.mMidiNoteNormalPitch, pad1Info.mPadNum);
        reloadSampler(pad2Info.mKeyAssign, pad2Info.mMidiNoteNormalPitch, pad2Info.mPadNum);
        reloadSampler(pad3Info.mKeyAssign, pad3Info.mMidiNoteNormalPitch, pad3Info.mPadNum);
        reloadSampler(pad4Info.mKeyAssign, pad4Info.mMidiNoteNormalPitch, pad4Info.mPadNum);
        reloadSampler(pad5Info.mKeyAssign, pad5Info.mMidiNoteNormalPitch, pad5Info.mPadNum);
        reloadSampler(pad6Info.mKeyAssign, pad6Info.mMidiNoteNormalPitch, pad6Info.mPadNum);
        reloadSampler(pad7Info.mKeyAssign, pad7Info.mMidiNoteNormalPitch, pad7Info.mPadNum);
        reloadSampler(pad8Info.mKeyAssign, pad8Info.mMidiNoteNormalPitch, pad8Info.mPadNum);
        reloadSampler(pad9Info.mKeyAssign, pad9Info.mMidiNoteNormalPitch, pad9Info.mPadNum);


        // Convert the XML to a string
        juce::String xmlString = xmlState->toString();

        // Print the XML content to the console
       // DBG("XML Content - setState called: \n" << xmlString << '\n');
 
    }

   

}



// =============== load a file ================================================================================ 

void WanjoDrumAudioProcessor::loadFile(int keyAssignment, int midiNoteForNormalPitch, int padNumber)
{
  

    juce::FileChooser chooser{ "Load File" };

    if (chooser.browseForFileToOpen())
    {
        auto file = chooser.getResult(); // Get the selected file
        auto filePathName = file.getFullPathName();
  
         // Check file format
        try
        {
            if (!file.hasFileExtension("wav") && !file.hasFileExtension("aif") && !file.hasFileExtension("mp3"))
            {
                // Handle invalid file format
                // Show a message to the user
                juce::AlertWindow::showMessageBox(juce::AlertWindow::WarningIcon,
                    "Invalid File Format",
                    "Please select a WAV or AIFF file.");
                return; // Return early, don't proceed with loading
            }

            // Check file length 

            if (file.getSize() > maxSampleLengthSecs * 44100) // Assuming 44100 samples per second
            {
                // Handle file too long
                juce::AlertWindow::showMessageBox(juce::AlertWindow::WarningIcon,
                    "File Too Long",
                    "The selected file is greater than 240 seconds. Please select a shorter file.");
                return; // Return early, don't proceed with loading
            }

        }

        catch (const std::exception& e)
        {
            juce::AlertWindow::showMessageBox(juce::AlertWindow::WarningIcon,
                "Error",
                e.what());
        }


        if (padNumber == pad1Info.mPadNum)
        {
            getPadParamsAssign(pad1Info, filePathName, keyAssignment, midiNoteForNormalPitch);
        }

        else if (padNumber == pad2Info.mPadNum)
        {
            getPadParamsAssign(pad2Info, filePathName, keyAssignment, midiNoteForNormalPitch);
        }

        else if (padNumber == pad3Info.mPadNum)
        {
            getPadParamsAssign(pad3Info, filePathName, keyAssignment, midiNoteForNormalPitch);
        }

        else if (padNumber == pad4Info.mPadNum)
        {
            getPadParamsAssign(pad4Info, filePathName, keyAssignment, midiNoteForNormalPitch);
        }

        else if (padNumber == pad5Info.mPadNum)
        {
            getPadParamsAssign(pad5Info, filePathName, keyAssignment, midiNoteForNormalPitch);
        }

        else if (padNumber == pad6Info.mPadNum)
        {
            getPadParamsAssign(pad6Info, filePathName, keyAssignment, midiNoteForNormalPitch);
        }

        else if (padNumber == pad7Info.mPadNum)
        {
            getPadParamsAssign(pad7Info, filePathName, keyAssignment, midiNoteForNormalPitch);
        }

        else if (padNumber == pad8Info.mPadNum)
        {
            getPadParamsAssign(pad8Info, filePathName, keyAssignment, midiNoteForNormalPitch);
        }

        else if (padNumber == pad9Info.mPadNum)
        {
            getPadParamsAssign(pad9Info, filePathName, keyAssignment, midiNoteForNormalPitch);
        }


        fillSampler(file, keyAssignment, midiNoteForNormalPitch, padNumber);
    }

}


void  WanjoDrumAudioProcessor::padParamsAssign(PadInfo& pad,
    juce::String filename,
    const juce::String& filepath,
    const int& keyAssignment,
    const int& midiNoteForNormalPitch,

    int& keyAssignmentValue,
    int& midiNoteValue)
{
    pad.mFileName = filename;
    pad.mFilePath = filepath;
    pad.mKeyAssign = keyAssignment;
    pad.mMidiNoteNormalPitch = midiNoteForNormalPitch;

    keyAssignmentValue = pad.mKeyAssign;
    midiNoteValue = pad.mMidiNoteNormalPitch;
}

void  WanjoDrumAudioProcessor::getPadParamsAssign(PadInfo& pad,
    const juce::String& filePathName,
    const int& keyAssignment,
    const int& midiNoteForPitch
)
{
    pad.mFilePath = filePathName;
    pad.mKeyAssign = keyAssignment;
    pad.mMidiNoteNormalPitch = midiNoteForPitch;
}




//=============Audio Parameter Scaffolding======================================

juce::AudioProcessorValueTreeState::ParameterLayout WanjoDrumAudioProcessor::createParameters()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    params.push_back(std::make_unique<juce::AudioParameterFloat>("MASTERGAIN", "Master Gain", 0.0f, 1.0f, 0.5f));


    return{ params.begin(), params.end() };
}



// =============== fill the sampler ================================================================================ 

void WanjoDrumAudioProcessor::fillSampler(const juce::File& file, int keyAssignment, int midiNoteForNormalPitch, int padNumber)
{
    // create the audioformatreader and give it the file
    mFormatReader = mFormatManager.createReaderFor(file);

    // Get filename and assign to variable
    juce::String fileName = file.getFileNameWithoutExtension();
    // Get filepath and assign to variable
    juce::String filePath = file.getFullPathName();

    // Get the key assignment and midi note for normal pitch based on pad number
    int keyAssignmentValue, midiNoteValue;

    if (padNumber == pad1Info.mPadNum)
    {
        padParamsAssign(pad1Info, fileName, filePath, keyAssignment, midiNoteForNormalPitch, pad1Info.mKeyAssign, pad1Info.mMidiNoteNormalPitch);
    }
    else if (padNumber == pad2Info.mPadNum)
    {
        padParamsAssign(pad2Info, fileName, filePath, keyAssignment, midiNoteForNormalPitch, pad2Info.mKeyAssign, pad2Info.mMidiNoteNormalPitch);
    }
    else if (padNumber == pad3Info.mPadNum)
    {
        padParamsAssign(pad3Info, fileName, filePath, keyAssignment, midiNoteForNormalPitch, pad3Info.mKeyAssign, pad3Info.mMidiNoteNormalPitch);
    }
    else if (padNumber == pad4Info.mPadNum)
    {
        padParamsAssign(pad4Info, fileName, filePath, keyAssignment, midiNoteForNormalPitch, pad4Info.mKeyAssign, pad4Info.mMidiNoteNormalPitch);
    }

    else if (padNumber == pad5Info.mPadNum)
    {
        padParamsAssign(pad5Info, fileName, filePath, keyAssignment, midiNoteForNormalPitch, pad5Info.mKeyAssign, pad5Info.mMidiNoteNormalPitch);
    }
    else if (padNumber == pad6Info.mPadNum)
    {
        padParamsAssign(pad6Info, fileName, filePath, keyAssignment, midiNoteForNormalPitch, pad6Info.mKeyAssign, pad6Info.mMidiNoteNormalPitch);
    }
    else if (padNumber == pad7Info.mPadNum)
    {
        padParamsAssign(pad7Info, fileName, filePath, keyAssignment, midiNoteForNormalPitch, pad7Info.mKeyAssign, pad7Info.mMidiNoteNormalPitch);
    }
    else if (padNumber == pad8Info.mPadNum)
    {
        padParamsAssign(pad8Info, fileName, filePath, keyAssignment, midiNoteForNormalPitch, pad8Info.mKeyAssign, pad8Info.mMidiNoteNormalPitch);
    }
    else if (padNumber == pad9Info.mPadNum)
    {
        padParamsAssign(pad9Info, fileName, filePath, keyAssignment, midiNoteForNormalPitch, pad9Info.mKeyAssign, pad9Info.mMidiNoteNormalPitch);
    }







    juce::BigInteger range;
    const double attackTimeSecs{ 0.0 };
    const double releaseTimeSecs{ 0.0 };


    range.setRange(keyAssignment, 1, true);
    auto* sample = new juce::SamplerSound("Sample", *mFormatReader, range, midiNoteForNormalPitch, attackTimeSecs, releaseTimeSecs, maxSampleLengthSecs);
    mSampler.addSound(sample);

    if (mSampler.getNumSounds() > 0)
    {
        //DBG("SamplerSound added successfully");
    }
    else
    {
        //DBG("Failed to add SamplerSound");
    }
}


// ====================== reload samples =================================

void WanjoDrumAudioProcessor::reloadSampler(int keyAssignment, int midiNoteForNormalPitch, int padNumber)
{
    if (padNumber == pad1Info.mPadNum && !pad1Info.mFilePath.isEmpty())
    {
        juce::File file(pad1Info.mFilePath);
        fillSampler(file, keyAssignment, midiNoteForNormalPitch, padNumber);


    }
    else if (padNumber == pad2Info.mPadNum && !pad2Info.mFilePath.isEmpty())
    {
        juce::File file2(pad2Info.mFilePath);
        fillSampler(file2, keyAssignment, midiNoteForNormalPitch, padNumber);


    }
    else if (padNumber == pad3Info.mPadNum && !pad3Info.mFilePath.isEmpty())
    {
        juce::File file3(pad3Info.mFilePath);
        fillSampler(file3, keyAssignment, midiNoteForNormalPitch, padNumber);


    }
    else if (padNumber == pad4Info.mPadNum && !pad4Info.mFilePath.isEmpty())
    {
        juce::File file4(pad4Info.mFilePath);
        fillSampler(file4, keyAssignment, midiNoteForNormalPitch, padNumber);


    }
    else if (padNumber == pad5Info.mPadNum && !pad5Info.mFilePath.isEmpty())
    {
        juce::File file5(pad5Info.mFilePath);
        fillSampler(file5, keyAssignment, midiNoteForNormalPitch, padNumber);

    }
    else if (padNumber == pad6Info.mPadNum && !pad6Info.mFilePath.isEmpty())
    {
        juce::File file6(pad6Info.mFilePath);
        fillSampler(file6, keyAssignment, midiNoteForNormalPitch, padNumber);

    }
    else if (padNumber == pad7Info.mPadNum && !pad7Info.mFilePath.isEmpty())
    {
        juce::File file7(pad7Info.mFilePath);
        fillSampler(file7, keyAssignment, midiNoteForNormalPitch, padNumber);

    }
    else if (padNumber == pad8Info.mPadNum && !pad8Info.mFilePath.isEmpty())
    {
        juce::File file8(pad8Info.mFilePath);
        fillSampler(file8, keyAssignment, midiNoteForNormalPitch, padNumber);

    }
    else if (padNumber == pad9Info.mPadNum && !pad9Info.mFilePath.isEmpty())
    {
        juce::File file9(pad9Info.mFilePath);
        fillSampler(file9, keyAssignment, midiNoteForNormalPitch, padNumber);

    }
}



//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new WanjoDrumAudioProcessor();
}
