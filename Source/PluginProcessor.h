/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "Delay.h"
#include "Oscillator.h"
#include "FilterSynth.h"
#include "OscillatorII.h"
#include "FilterSynthII.h"

//==============================================================================
/**
*/
class TestVAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    TestVAudioProcessor();
    ~TestVAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

private:
    
   
    
    /**
     
   Calling and assigning CLASS members to form  waves for OSCILLATORS and LFOs.
     
     =================================================
     */
    
    
    Oscillator square;
    OscillatorII triangle;
    
    
    OscillatorII lowSine;
    Oscillator midSquare;
    Oscillator highSine;
    
    
    Oscillator triNew;
    Oscillator subNew;
    
    
    Oscillator oscNew;
    Oscillator oscNew2;
    
    
    OscillatorII lfoI;
    OscillatorII lfoII;
    Oscillator lfoIII;
    OscillatorII lfoIV;
    OscillatorII lfoV;
    OscillatorII lfoVI;
    
    /**
     
   Calling and assigning CLASS members to add FILTER SYNTH, DELAY & REVERB
     
     =================================================
     */
    FilterSynth synth;
    
    FilterSynthII synth2;
    
    
    Delay delay;
    
   // juce::IIRFilter filter;
    
    juce::Reverb reverb;
    
    
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TestVAudioProcessor)
};
