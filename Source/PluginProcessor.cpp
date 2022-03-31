/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
TestVAudioProcessor::TestVAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
}

TestVAudioProcessor::~TestVAudioProcessor()
{
}

//==============================================================================
const juce::String TestVAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool TestVAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool TestVAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool TestVAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double TestVAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int TestVAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int TestVAudioProcessor::getCurrentProgram()
{
    return 0;
}

void TestVAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String TestVAudioProcessor::getProgramName (int index)
{
    return {};
}

void TestVAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void TestVAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
   
    
    
    /**
     
    FILTERS
     
     =================================================
     */
    
    
    synth2.setSampleRate(sampleRate);
    synth2.setSampleRate(sampleRate);
       
   
    
    /**
     
     DELAY
     
     =================================================
     */
    
    
    
    delay.setSize(sampleRate);
    delay.setDelayTime(sampleRate * 0.009f);
       
    /**
     
    [ OSC ] calls out waves from the OSCILLATOR classes
     
     =================================================
     */
     
    square.setSampleRate(sampleRate);
    square.setFrequency(100.0f);
    triangle.setSampleRate(sampleRate);
    triangle.setFrequency(93.0f);
    
    
    
    /**
    
     
     [ OSC ] calls out waves from the OSCILLATOR classes [ for  High and Mid tones ]
     
     =================================================
     */
    
    
    lowSine.setSampleRate(sampleRate);
    lowSine.setFrequency(40.0f);
    
    highSine.setSampleRate(sampleRate);
    highSine.setFrequency(106.0f);
    
    midSquare.setSampleRate(sampleRate);
    midSquare.setFrequency(73.0f);
    
    
    
    subNew.setSampleRate(sampleRate);
    subNew.setFrequency(55.0f);
    
    
    triNew.setSampleRate(sampleRate);
    triNew.setFrequency(68.0f);
    
    
    
    
    
    oscNew2.setSampleRate(sampleRate);
    oscNew2.setFrequency(134.0f);
    
    
    
    /**
     
       calls out LFOs from the two Oscillator classes [ High and Mid tones ]
     
     =================================================
     */
    
    
    lfoI.setSampleRate(sampleRate);
    lfoI.setFrequency(0.78f);
       
    lfoII.setSampleRate(sampleRate);
    lfoII.setFrequency(0.015f);
   
    lfoIII.setSampleRate(sampleRate);
    lfoIII.setFrequency(0.09f);
       
    lfoIV.setSampleRate(sampleRate);
    lfoIV.setFrequency(0.94f);
    
    lfoV.setSampleRate(sampleRate);
    lfoV.setFrequency(0.08f);
    
    lfoVI.setSampleRate(sampleRate);
    lfoVI.setFrequency(0.55f);
       
    /**
     
     [REVERB ] calls out JUCE's inbuilt Reverb
     
     =================================================
     */
    
    
    juce::Reverb::Parameters reverbParams;
       
       reverbParams.width = 1.6 * lfoII.process4();
       
       reverbParams.roomSize = 0.92f * lfoV.process7();
       
       reverbParams.wetLevel = 1.4f;     // * lfoII.process4()
       reverbParams.damping = 1.1f;
       
       reverb.setParameters(reverbParams);
       
       reverb.reset();
    
    
    
    

}

void TestVAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool TestVAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void TestVAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    
        
    /**
     
     
     OUR DSP
     
     =================================================
     */
    
    
    int numSamples = buffer.getNumSamples();
            
            float* left = buffer.getWritePointer(0);
            float* right = buffer.getWritePointer(1);
            
            
            for (int i=0; i<numSamples; i++)
            {
               // float freq = 220 + (rand() % 440);
                
                
                
                
                float sample = (lfoI.process5() * midSquare.process2() * 0.1f * triNew.process2()) + ( synth2.processmain2() + triNew.process2()) ;
                
                float sample2 = (lfoII.process4() * triangle.process6() * 0.1f + highSine.process2() * lfoIV.process7()) + (lfoII.process7() * lfoVI.process4() * 0.2f);
                
                float sample3 = square.process3() * lfoV.process5() + triangle.process6() * lfoII.process4() * 0.2f;
                
                float sample4 = ((lowSine.process4() * 0.8f) * lfoII.process7() * lfoII.process5()) * 0.2f;
                
                float sample5 = (highSine.process1() * 0.1f * lfoIV.process4()  * triNew.process3() * synth2.processmain2() * lfoVI.process6() * 0.2f);
                
                float delayMod = lfoI.process7();
                
                
                int delayTime = delayMod * 10000 + 20000;
                
                
                delay.setDelayTime(delayTime);
                
                
                float delayedSample = delay.process2(sample);
                
                
                float sample6  = synth2.processmain2();
                
                
                float sampleSemiMain = (((sample2 * sample3 + sample5 * sample6) * delayedSample)) + (lfoV.process7() + lfoII.process4() + lfoII.process6()) * 0.1f;
                
                
                float sampleSemiMain2 = ((sample6 + sample6) * delayedSample) + (lfoV.process6())  + (lfoV.process7() + lfoII.process6()) * 0.1f;
                
                
                float sampleMain = (sampleSemiMain * 0.3f) + ((sample5 + sample4 + sample6) * 0.2) + sample4 * 0.1f;
                
                float sampleMain2 = (sampleSemiMain2 * 0.3f) + ((sample5 + sample6) * 0.2f) + sample * 0.1f;
                
                left[i] = (sample + sample5 + sample6  + delayedSample * 0.3f) * lfoV.process7();
                
                right[i] = sample4 + sample5 + sample2 + delayedSample * 0.3f * lfoVI.process4();
                
            }

    /**
     
     
   REVERB OUT
     
     =================================================
     */
    
    
    reverb.processStereo(left, right, numSamples);
    
    
    
    
    
    
    
    
    
}

//==============================================================================
bool TestVAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* TestVAudioProcessor::createEditor()
{
    return new TestVAudioProcessorEditor (*this);
}

//==============================================================================
void TestVAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void TestVAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new TestVAudioProcessor();
}
