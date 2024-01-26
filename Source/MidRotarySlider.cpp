/*
  ==============================================================================

    MidRotarySlider.cpp
    Created: 8 Sep 2023 3:59:42pm
    Author:  Zachary Pennington

  ==============================================================================
*/

#include "MidRotarySlider.h"

MidRotarySlider::MidRotarySlider() : juce::Slider()
{
    setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
//    setRange(0.0, 1.0, 0.001);
//    setValue(0.000);
    
    //SETS THE COLOR OF THE OUTLINE OF THE SLIDER
    setColour(Slider::ColourIds::thumbColourId, juce::Colours::white);
    setColour(Slider::ColourIds::rotarySliderFillColourId, juce::Colours::white);
    setColour(Slider::ColourIds::rotarySliderOutlineColourId, juce::Colours::transparentBlack);
    
    setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, false, 0, 0);
    setSliderSnapsToMousePosition(true);
}

MidRotarySlider::~MidRotarySlider()
{
    
}
