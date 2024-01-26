/*
  ==============================================================================

    BlendRotarySlider.cpp
    Created: 8 Sep 2023 5:03:21pm
    Author:  Zachary Pennington

  ==============================================================================
*/

#include "BlendRotarySlider.h"

BlendRotarySlider::BlendRotarySlider() : juce::Slider()
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

BlendRotarySlider::~BlendRotarySlider()
{
    
}
