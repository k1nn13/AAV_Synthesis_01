#pragma once

#include "ofMain.h"
#include "ofxMidi.h"
#include "ofxMaxim.h"
#include "ofxGui.h"
#include "ofxXmlSettings.h"

#define NUMCHANNELS 3


class ofApp : public ofBaseApp, public ofxMidiListener {

	public:
		void setup();
		void update();
		void draw();
		//-------------------MAXIM---------------------------------//
		void audioOut(float * output, int bufferSize, int nChannels);
		//---------------------------------------------------------//
		void drawWaveform();
		//-------------------MIDI----------------------------------//
		void newMidiMessage(ofxMidiMessage &message);
		void midiDebug();
		void exit();
		//-------------------GUI----------------------------------//
		void saveButtonPressed();
		void loadButtonPressed();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		//---------------------------------------------------------//
		//--XML--//
		ofxXmlSettings XML;
		string message;
		string xmlStructure;//??
		int numADSR;

		//--------------------------------------------------------------------------------
		//--SETUP-MIDI--//
		ofxMidiIn midi;
		vector <ofxMidiMessage> messages;
		vector <int> midiNotes;
		convert mtof;						 //convert midi notes to frequency
		int midiTrigger, midiNote;

		maxiClock clock;
		int n;
		int seqPitch[16];
		int playHead;
		int position;
		int seqTrigger;
		int seq_1[16] = { 1,0,0,1, 1,0,0,1, 1,0,1,0, 1,0,0,1 };
		int seq_2[16] = { 1,1,0,1, 1,1,0,1, 1,0,1,0, 1,0,1,1 };

		//--------------------------------------------------------------------------------
		//--SETUP-SOUND--//
		ofSoundStream soundStream;
		unsigned bufferSize, sampleRate;
		double currentSample;
		double mix_0, mix_1, mix_2;							//channel mix
		double masterMix[2], 
			   outputs_0[2], outputs_1[2], outputs_2[2];	//stereo mix

		//analysis
		ofxMaxiFFT fft;
		ofxMaxiFFTOctaveAnalyzer oct;
		maxiMix channelMix[NUMCHANNELS];				


		//--------------------------------------------------------------------------------
		//--GUI--//
		ofxPanel gui;
		//Sound Settings
		ofParameter<double> masterGainSlider;
		ofxVec4Slider adsrSlider;
		ofParameter<double> AM_mod, FM_mod, autoPan;
		ofParameter<double> pan_0, pan_1, pan_2;
		ofParameter<double> gain_0, gain_1, gain_2;
		ofParameter<double> filterFreq;
		
		//XML settings
		ofxButton saveButton, loadButton;
		ofxToggle AM_button;
		ofxIntSlider numPresets;
		ofParameter<int> selectPreset;
		
		//--------------------------------------------------------------------------------
		//--SYNTHESIS--//
		//Maxi stuff
		maxiOsc osc[NUMCHANNELS];
		maxiOsc lfo[NUMCHANNELS];
		maxiEnv env[NUMCHANNELS];
		maxiFilter filter[NUMCHANNELS];
		maxiDelayline delay[NUMCHANNELS];
		//outputs
		double oscOut[NUMCHANNELS];
		double lfoOut[NUMCHANNELS];
		double envOut[NUMCHANNELS];
		double pitch[NUMCHANNELS];
		double delayOut[NUMCHANNELS];
		double filterOut[NUMCHANNELS];
	
		//--------------------------------------------------------------------------------
		//--DRAW-WAVEFROM--//
		vector <float> volHistory;
		float smoothedVol;
		float scaledVol;
		float soundBuffer[512];


};
