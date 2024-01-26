/*
  ==============================================================================

    LPFSlider.cpp
    Created: 9 Sep 2023 9:15:27am
    Author:  Zachary Pennington

  ==============================================================================
*/

#include "LPFSlider.h"

LPFSlider::LPFSlider() : juce::Slider()
{
    setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
//    setRange(0.0, 1.0, 0.001);
//    setValue(0.000);
    
    // COLORS:
    setColour(Slider::ColourIds::thumbColourId, juce::Colours::white);
    setColour(Slider::ColourIds::rotarySliderOutlineColourId, juce::Colours::transparentBlack);
    setColour(Slider::ColourIds::rotarySliderFillColourId, juce::Colours::white);
    
    setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, false, 0, 0);
    setSliderSnapsToMousePosition(true);
}

LPFSlider::~LPFSlider()
{
    
}
