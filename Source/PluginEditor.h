/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Vertical Meter.h"
#include "CompressorSlider.h"
#include "LowRotarySlider.h"
#include "MidRotarySlider.h"
#include "AirRotarySlider.h"
#include "GainRotarySlider.h"
#include "BlendRotarySlider.h"
#include "LPFSlider.h"
#include "HPFSlider.h"
#include "MenuBox.h"
#include "MenuArrows.h"

using namespace juce;

//==============================================================================
/**
*/
class AuraAirAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    AuraAirAudioProcessorEditor (AuraAirAudioProcessor&);
    ~AuraAirAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    std::vector<Component*>getComps();

private:
    
    AuraAirAudioProcessor& audioProcessor;
    
    Image background;
    Gui::VerticalGradientMeter verticalGradientMeterL, verticalGradientMeterR;
    
    MenuBox menu;
    MenuArrows arrowLeft;
    MenuArrows arrowRight;
    
    LowRotarySlider lowRotarySlider;
    MidRotarySlider midRotarySlider;
    AirRotarySlider airRotarySlider;
    
    GainRotarySlider gainRotarySlider;
    BlendRotarySlider blendRotarySlider;
    
    LPFSlider lpfSlider;
    HPFSlider hpfSlider;
    
    CompressorSlider compSlider;
    //SLIDER ATTACHEMENTS MUST DELETE BEFORE SLIDERS THEMSELVES... MUST REAMIN HERE!!!
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> lowSliderAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> midSliderAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> airSliderAttachment;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> gainSliderAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> blendSliderAttachment;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> lpfSliderAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> hpfSliderAttachment;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> compSliderAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AuraAirAudioProcessorEditor)
};
