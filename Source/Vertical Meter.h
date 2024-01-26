/*
  ==============================================================================

    VerticalGradientMeter.h
    Created: 16 Jun 2023 3:01:22pm
    Author:  Zachary Pennington

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
using namespace juce;

namespace Gui
{
    class VerticalGradientMeter : public Component, public Timer
{
public:
    VerticalGradientMeter(std::function<float()>&& valueFunction) : valueSupplier(std::move(valueFunction))
    {
        startTimerHz(24);
    }
    
    void paint(Graphics& g) override
    {
        const auto level = valueSupplier();
        
        auto bounds = getLocalBounds().toFloat();
        
//        g.setColour(Colours::black);
        g.fillRoundedRectangle(bounds, 5.f);
        g.setColour(Colours::white);
        
        
//        g.setGradientFill(gradient);
        const auto scaledY = jmap(level, -60.f, 0.f, 0.f, static_cast<float>(getHeight()));
        g.fillRoundedRectangle(bounds.removeFromBottom(scaledY), 3.f);
        
        
    }
    
    void resized() override
    {
        const auto bounds = getLocalBounds().toFloat();
        gradient = ColourGradient {
            Colours::blue,
            bounds.getBottomLeft(),
            Colours::orange,
            bounds.getTopLeft(),
            false
        };
        gradient.addColour(0.99, Colours::red);
    }
    
    void timerCallback() override
    {
        repaint();
    }
private:
    std::function<float()> valueSupplier;
    ColourGradient gradient{};
};
}


