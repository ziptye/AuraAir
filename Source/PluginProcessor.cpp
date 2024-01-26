/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AuraAirAudioProcessor::AuraAirAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), apvts(*this, nullptr, "Parameters", createParameters()),
                        airFilter(juce::dsp::IIR::Coefficients<float>::makeHighShelf(48000, 10000.f, 0.5f, 1.0f)),
                        airCut(juce::dsp::IIR::Coefficients<float>::makePeakFilter(48000, 8500.f, 5.0f, 1.0f)),
                        midFilter(juce::dsp::IIR::Coefficients<float>::makePeakFilter(48000, 1500.f, 5.0f, 1.0f)),
                        midCut(juce::dsp::IIR::Coefficients<float>::makePeakFilter(48000, 700.f, 5.0f, 1.0f)),
                        lowFilter(juce::dsp::IIR::Coefficients<float>::makePeakFilter(48000, 250.f, 5.0f, 1.0f)),
                        lowCut(juce::dsp::IIR::Coefficients<float>::makePeakFilter(48000, 150.f, 5.0f, 1.0f))
#endif
{
}

AuraAirAudioProcessor::~AuraAirAudioProcessor()
{
}

//==============================================================================
const juce::String AuraAirAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool AuraAirAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool AuraAirAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool AuraAirAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double AuraAirAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int AuraAirAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int AuraAirAudioProcessor::getCurrentProgram()
{
    return 0;
}

void AuraAirAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String AuraAirAudioProcessor::getProgramName (int index)
{
    return {};
}

void AuraAirAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void AuraAirAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumOutputChannels();
    spec.sampleRate = sampleRate;
    
    //======================================================
    rmsLevelLeft.reset(sampleRate, 0.5);
    rmsLevelRight.reset(sampleRate, 0.5);
    reset(); // resets all components L/M/H/HPF/LPF/Blend/Gain/etc.
    
    //======================================================
    updateFilter();
    updateCompressor();
    //======================================================
    airFilter.prepare(spec);
    airCut.prepare(spec);
    midFilter.prepare(spec);
    midCut.prepare(spec);
    lowFilter.prepare(spec);
    lowCut.prepare(spec);
    LPFilter.prepare(spec);
    HPFilter.prepare(spec);
    compressor.prepare(spec);
    gainKnob.prepare(spec);
    LPFilter.setType(juce::dsp::StateVariableTPTFilterType::lowpass);
    HPFilter.setType(juce::dsp::StateVariableTPTFilterType::highpass);
    
    //======================================================
    rmsLevelLeft.setCurrentAndTargetValue(-100.f);
    rmsLevelRight.setCurrentAndTargetValue(-100.f);
    //======================================================
}

void AuraAirAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool AuraAirAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
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

void AuraAirAudioProcessor::updateCompressor()
{
    float thres = *apvts.getRawParameterValue("COMPRESSOR");
    compressor.setRatio(2.0f);
    compressor.setAttack(10.0f);
    compressor.setRelease(250.f);
    compressor.setThreshold(thres);
}
void AuraAirAudioProcessor::updateFilter()
{
    //======================================================
    // Air Band
    double sampleRate = 48000;
    float cutOffFreq = 10000.0f;
    float quality = 0.5f;
    float gainNew = *apvts.getRawParameterValue("AIR");
    *airFilter.state = *dsp::IIR::Coefficients<float>::makeHighShelf(sampleRate, cutOffFreq, quality, juce::Decibels::decibelsToGain(gainNew));
    //======================================================
    // AirCut Band
    float airCutGain = 1.0f;

    if (gainNew >= 6)
        airCutGain = 1.5f - (gainNew / 12.0f);
    else
        airCutGain = 1.0f;
    *airCut.state = *dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate, 4500.f, 5.0f, airCutGain);
    //======================================================
    // MidFilter + MidCut
    float midFilterGain = *apvts.getRawParameterValue("MID");
    *midFilter.state = *dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate, 3000.f, 0.5f, midFilterGain); // NEED TO EDIT THIS FREQ!!!
    
    float midCutGain = 1.0f;
    if (midFilterGain >= 3)
        midCutGain = 1.5f - (midFilterGain / 12.0f);
    else
        midCutGain = 1.0f;
    *midCut.state = *dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate, 750.f, 5.0f, midCutGain);
    
    //======================================================
    // LowFilter + LowCut
    
    float lowFilterGain = *apvts.getRawParameterValue("LOW");
    *lowFilter.state = *dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate, 300.f, 0.69f, lowFilterGain);
    
    float lowCutGain = 1.0f;
    if (lowFilterGain >= 3)
        lowCutGain = 1.5f - (lowFilterGain / 12.0f);
    else
        lowCutGain = 1.0f;
    *lowCut.state = *dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate, 250.f, 5.0f, lowCutGain);
}
void AuraAirAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    rmsLevelLeft.skip(buffer.getNumSamples());
    rmsLevelRight.skip(buffer.getNumSamples());
    
    
    {
    const auto value = Decibels::gainToDecibels(buffer.getRMSLevel(0, 0, buffer.getNumSamples()));
        
        if (value < rmsLevelLeft.getCurrentValue())
            rmsLevelLeft.setTargetValue(value);
        else
            rmsLevelLeft.setCurrentAndTargetValue(value);
    }
    
    {
    const auto value = Decibels::gainToDecibels(buffer.getRMSLevel(1, 0, buffer.getNumSamples()));
        
        if (value < rmsLevelRight.getCurrentValue())
            rmsLevelRight.setTargetValue(value);
        else
            rmsLevelRight.setCurrentAndTargetValue(value);
    }
    //======================================================
    auto LPF = apvts.getRawParameterValue("LPF");
    LPFilter.setCutoffFrequency(LPF->load());
    
    auto HPF = apvts.getRawParameterValue("HPF");
    HPFilter.setCutoffFrequency(HPF->load());
    
    //======================================================
    float gain = *apvts.getRawParameterValue("GAIN");
    gainKnob.setGainDecibels(gain);
    gainKnob.setRampDurationSeconds(0.25);
    //======================================================
    //Creates audioBlock and Context for Filters to process
    auto audioBlock = juce::dsp::AudioBlock<float> (buffer); // this is an alias to the audio buffer.
    auto context = juce::dsp::ProcessContextReplacing<float>(audioBlock);
    updateFilter();
    updateCompressor();
    //======================================================
    LPFilter.process(context);
    HPFilter.process(context);
    airFilter.process(context);
    airCut.process(context);
    midFilter.process(context);
    midCut.process(context);
    lowFilter.process(context);
    lowCut.process(context);
    
    gainKnob.process(context);
    
    //    compressor.process(context);
    //======================================================
}

//==============================================================================
bool AuraAirAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* AuraAirAudioProcessor::createEditor()
{
    return new AuraAirAudioProcessorEditor (*this);
}

//==============================================================================
void AuraAirAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    
//    juce::MemoryOutputStream memOut(destData, true);
//    apvts.state.writeToStream(memOut);
}

void AuraAirAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    
//    auto valTree = juce::ValueTree::readFromData(data, sizeInBytes); // Stores the tree and all of it's children in Binary format.
//    if (valTree.isValid())
//    {
//        apvts.replaceState(valTree); // replaces the state of the value tree
//    }
}

float AuraAirAudioProcessor::getRMSValue(const int channel) const
{
    jassert(channel == 0 || channel == 1);
    
    if (channel == 0)
        return rmsLevelLeft.getCurrentValue();
    if (channel == 1)
        return rmsLevelRight.getCurrentValue();
    return 0.f;
}

juce::AudioProcessorValueTreeState::ParameterLayout AuraAirAudioProcessor::createParameters()
{
    std::vector<std::unique_ptr<RangedAudioParameter>> params;
    
    auto LMRange = juce::NormalisableRange<float>(1.0f, 6.0f, 0.25);
    auto AirRange = juce::NormalisableRange<float>(1.0f, 12.0f, 0.25f);
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("LOW", 1), "Low", LMRange, 1.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("MID", 1), "Mid", LMRange, 1.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("AIR", 1), "Air", AirRange, 1.0f));
    
    
    auto gainRange = juce::NormalisableRange<float>(-10.0f, 10.0f, 0.25f);
    auto blendRange = juce::NormalisableRange<float>(0.0f, 100.0f, 1.0f);
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("GAIN", 1), "Gain", gainRange, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("BLEND", 1), "Blend", blendRange, 100.0f));
    
    
    auto LPFRange = juce::NormalisableRange<float>(20.0f, 20000.0f, 1.0f, 0.25f);
    auto HPFRange = juce::NormalisableRange<float>(20.0f, 20000.0f, 1.0f, 0.25f);
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("LPF", 1), "LPF", LPFRange, 20000.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("HPF", 1), "HPF", HPFRange, 20.0f));
    
    auto CompRange = juce::NormalisableRange<float>(-60.0f, 12.0f, 1.0f);
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("COMPRESSOR", 1), "Compressor", CompRange, 0.0f));
    
    
    return { params.begin(), params.end()} ;
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AuraAirAudioProcessor();
}

void AuraAirAudioProcessor::reset()
{
    LPFilter.reset();
    HPFilter.reset();
    
    airFilter.reset();
    airCut.reset();
    midFilter.reset();
    midCut.reset();
    lowFilter.reset();
    lowCut.reset();
    
    compressor.reset();
    
    gainKnob.reset();
}

