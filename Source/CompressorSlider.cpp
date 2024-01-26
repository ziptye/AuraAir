/*
  ==============================================================================

    CompressorSlider.cpp
    Created: 8 Sep 2023 1:20:16pm
    Author:  Zachary Pennington

  ==============================================================================
*/

#include "CompressorSlider.h"


CompressorSlider::CompressorSlider() : juce::Slider()
{
    
    setSliderStyle(SliderStyle::LinearHorizontal);
    setRange(0.0, 1.0, 0.001);
    setValue(0.000);
    
    
    
    setColour(Slider::thumbColourId, juce::Colours::white);
    setColour(Slider::ColourIds::backgroundColourId, juce::Colours::black);
    setColour(Slider::trackColourId, juce::Colours::white);
    setSliderSnapsToMousePosition(true);
    setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, false, 0, 0);
    
}

CompressorSlider::~CompressorSlider()
{
    
}
