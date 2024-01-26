/*
  ==============================================================================

    AirRotarySlider.cpp
    Created: 8 Sep 2023 4:51:16pm
    Author:  Zachary Pennington

  ==============================================================================
*/

#include "AirRotarySlider.h"

AirRotarySlider::AirRotarySlider() : juce::Slider()
{
    setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
//    setRange(0.0, 1.0, 0.001);
//    setValue(0.000);
    
    // COLORS
    setColour(Slider::ColourIds::thumbColourId, juce::Colours::white);
    setColour(Slider::ColourIds::rotarySliderOutlineColourId, juce::Colours::transparentBlack);
    setColour(Slider::ColourIds::rotarySliderFillColourId, juce::Colours::white);
    
    setSliderSnapsToMousePosition(true);
    setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, false, 0, 0);
}

AirRotarySlider::~AirRotarySlider()
{
    
}
