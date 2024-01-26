/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AuraAirAudioProcessorEditor::AuraAirAudioProcessorEditor (AuraAirAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p),
    verticalGradientMeterL([&](){return audioProcessor.getRMSValue(0);}),
    verticalGradientMeterR([&](){return audioProcessor.getRMSValue(1);})
{
    //======================================
    // MAKES COMPONENTS VISABLE
    
    for(auto* comp : getComps())
    {
        addAndMakeVisible(comp); // RENDERS ALL COMPONENTS
    }
    
    //======================================================
    // SETS BACKGROUND SIZE + IMAGE
    
    setSize (1355, 735);
    background = juce::ImageCache::getFromMemory(BinaryData::Aura_Air_png, BinaryData::Aura_Air_pngSize);
    
    //======================================================
    // CONNECTS SLIDERS TO APVTS
    
    lowSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "LOW", lowRotarySlider);
    
    midSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "MID", midRotarySlider);
    
    airSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "AIR", airRotarySlider);
    
    gainSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "GAIN", gainRotarySlider);
    
    blendSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "BLEND", blendRotarySlider);
    
    lpfSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "LPF", lpfSlider);
    
    hpfSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "HPF", hpfSlider);
    
    compSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "COMPRESSOR", compSlider);
    //======================================================
    arrowLeft.setButtonText("<");
    arrowRight.setButtonText(">");
    //======================================================
}

AuraAirAudioProcessorEditor::~AuraAirAudioProcessorEditor()
{
}

//==============================================================================
void AuraAirAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    
    g.drawImageAt(background, 0, 0); // DRAWS BG IMAGE
    
}

void AuraAirAudioProcessorEditor::resized()
{
    verticalGradientMeterL.setBounds(452, 48, 32, 236); // x y w h
    verticalGradientMeterR.setBounds(871, 48, 32, 236); // x y w h
    
    lowRotarySlider.setBounds(135, 42, 267, 267);
    midRotarySlider.setBounds(545, 47, 267, 267);
    airRotarySlider.setBounds(947, 47, 267, 267);
    
    gainRotarySlider.setBounds(372, 310, 192, 192);
    blendRotarySlider.setBounds(790, 310, 192, 192);
    
    lpfSlider.setBounds(198, 427, 140, 140);
    hpfSlider.setBounds(1010, 427, 140, 140);
    
    compSlider.setBounds(427, 507, 500, 300);
    
    menu.setBounds(60, 645, 250, 30);
    arrowLeft.setBounds(15, 645, 30, 30);
    arrowRight.setBounds(325, 645, 30, 30);
    
}

std::vector<juce::Component*>AuraAirAudioProcessorEditor::getComps()
{
    return {
        &verticalGradientMeterL,
        &verticalGradientMeterR,
        &compSlider,
        &lowRotarySlider,
        &midRotarySlider,
        &airRotarySlider,
        &gainRotarySlider,
        &blendRotarySlider,
        &lpfSlider,
        &hpfSlider,
        &menu,
        &arrowLeft,
        &arrowRight,
    };
}
