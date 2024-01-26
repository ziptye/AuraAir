/*
  ==============================================================================

    LowRotarySlider.cpp
    Created: 8 Sep 2023 3:18:16pm
    Author:  Zachary Pennington

  ==============================================================================
*/

#include "LowRotarySlider.h"

LowRotarySlider::LowRotarySlider() : juce::Slider()
{
    setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
//    setRange(0.0, 1.0, 0.001);
//    setValue(0.000);
    
    //Sets colors of the slider (interior & exterior outline)
    setColour(Slider::thumbColourId, juce::Colours::white);
    setColour(Slider::ColourIds::rotarySliderOutlineColourId, juce::Colours::transparentBlack);
    setColour(Slider::ColourIds::rotarySliderFillColourId, juce::Colours::white);
    
    setSliderSnapsToMousePosition(true);
    setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, false, 0, 0);
}

LowRotarySlider::~LowRotarySlider()
{
    
}
