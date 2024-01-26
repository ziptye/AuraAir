/*
  ==============================================================================

    MenuBox.cpp
    Created: 10 Sep 2023 11:23:40am
    Author:  Zachary Pennington

  ==============================================================================
*/

#include "MenuBox.h"

MenuBox::MenuBox() : juce::ComboBox()
{
    setColour(juce::ComboBox::ColourIds::backgroundColourId, juce::Colours::white);
    setColour(juce::ComboBox::ColourIds::arrowColourId, juce::Colours::black);
    setColour(juce::ComboBox::ColourIds::textColourId, juce::Colours::black);
    
    setJustificationType(juce::Justification::centred);
    
    setText("Default Preset");
    
    addItem("Default Preset", 1);
    addItem("Preset 1", 2);
    addItem("Preset 2", 3);
    addItem("Preset 3", 4);
}

MenuBox::~MenuBox()
{
    
}
