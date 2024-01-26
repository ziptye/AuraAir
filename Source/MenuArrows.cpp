/*
  ==============================================================================

    MenuArrows.cpp
    Created: 10 Sep 2023 11:38:52am
    Author:  Zachary Pennington

  ==============================================================================
*/

#include "MenuArrows.h"

MenuArrows::MenuArrows()
{
    setClickingTogglesState(true);
    setColour(TextButton::ColourIds::buttonOnColourId, juce::Colours::lightgoldenrodyellow);
    setColour(TextButton::ColourIds::textColourOnId, juce::Colours::black);
    setColour(TextButton::ColourIds::buttonColourId, juce::Colours::black);
    setColour(TextButton::ColourIds::textColourOffId, juce::Colours::white);
}
