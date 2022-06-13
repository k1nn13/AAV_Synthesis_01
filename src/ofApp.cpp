#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	//-----OF-SETUP-------//
	ofSetFrameRate(60);
	ofSetVerticalSync(true);
	ofEnableAlphaBlending();
	ofEnableSmoothing();

	//---------------XML---------------//
	message = "loading settings.xml!....";
	if (XML.loadFile("settings.xml")) {
		message = "settings.xml loaded!";
	} else {
		message = "unable to load settings.xml check data/ folder";
	}
	XML.pushTag("GUI");

	//-------------------GUI------------------------------//
	saveButton.addListener(this, &ofApp::saveButtonPressed);     // setup save / load button
	loadButton.addListener(this, &ofApp::loadButtonPressed);

	gui.setup();
	gui.add(masterGainSlider.set("master gain", 0.5, 0.0, 1));
	gui.add(adsrSlider.setup("ADSR", ofVec4f(10, 5, 5, 1000), ofVec4f(0, 0, 0, 0), ofVec4f(5000, 5000, 5000, 20000)));
	
	//GUI Voice 1 settings
	gui.add(gain_0.set("Ch 0: Gain", 0.5, 0.0, 1));
	gui.add(pan_0.set("Ch 0: Pan", 0.5, 0.0, 1));
	gui.add(FM_mod.set("Ch 0: FM modulation", 0, 0.0, 100));

	//GUI Voice 2 settings
	gui.add(gain_1.set("Ch 1: Gain", 0.5, 0.0, 1));
	gui.add(pan_1.set("Ch 1: Pan", 0.5, 0.0, 1));
	gui.add(AM_mod.set("Ch 1: AM modulation", 0, 0.0, 100));

	//GUI Voice 3 settings
	gui.add(gain_2.set("Ch 2: Gain", 0.1, 0.0, 1));
	gui.add(filterFreq.set("Ch 2: Filter Freq", 500, 1.0, 2000));
	gui.add(autoPan.set("Ch 2: Auto Pan Speed", 0.25, 0.0, 100));


	//GUI XML Save Conrtols
	gui.add(saveButton.setup("Save Preset"));
	gui.add(numPresets.setup("Number of Presets", 0, 0, 0));	 // displays number of presets saved maximum 10 from xml
	gui.add(selectPreset.set("Select Preset", 0, 0, 9));
	gui.add(loadButton.setup("Load Preset"));

	//--MIDI-SETUP--//
	midi.listInPorts();			  //display midi instrument name in terminal
	midi.openPort("ReMOTE SL 0"); //select midi port string or int
	midi.addListener(this);

	//---MAXIM-SETUP---//
	fft.setup(1024, 512, 256);
	oct.setup(4800, 1024, 10);
	sampleRate = 48000;
	bufferSize = 512;

	//--SoundStream-Settings--//
	ofSoundStreamListDevices();
	//ofSoundStreamSettings settings;
	//settings.setApi(ofSoundDevice::MS_ASIO);  //select sound card
	ofSoundStreamSetup(2, 2, this, sampleRate, bufferSize, 4);
	/*ofxMaxiSettings::setup(sampleRate, 2, bufferSize);*/

	//ofSoundStream stream; 
		//ofSoundStreamSettings streamSettings; 
		//streamSettings.numInputChannels = 2; 
		//streamSettings.numOutputChannels = 2; 
		//streamSettings.sampleRate = 44100; 
		//streamSettings.bufferSize = 512; 
		//streamSettings.numBuffers = 4; 
		//streamSettings.setApi(ofSoundDevice::Api::MS_ASIO); //MS_WASAPI, MS_DS, MS_ASIO|
		//ofSoundStreamSetup(streamSettings);


	//ofSoundStream settings; 
	//settings.setApi(ofSoundDevice::Api::MS_WASAPI);
	//std::vector<ofSoundDevice> devices = soundStream.getDeviceList(ofSoundDevice::Api::MS_WASAPI);
	////settings.setInDevice(devices[1]);
	//settings.setInListener(this);
	//settings.bufferSize = bufferSize;
	//settings.sampleRate = 44100;
	//settings.numOutputChannels = 2;
	//settings.numInputChannels = 2;
	//ofSoundStreamSetup(settings);
	//soundStream.printDeviceList();
	//=======================================================//

}

//--------------------------------------------------------------
void ofApp::saveButtonPressed() {

	numADSR = XML.getNumTags("settings");					// check the number of tags
	if (numADSR < 10) {										// set maximum save number

		if (XML.tagExists("settings", selectPreset)) {			
			XML.clearTagContents("settings", selectPreset);     // if tag exists clear its content
			XML.pushTag("settings", selectPreset);				// fill tag with new values

			XML.addValue("X", adsrSlider->x);
			XML.addValue("Y", adsrSlider->y);
			XML.addValue("Z", adsrSlider->z);
			XML.addValue("W", adsrSlider->w);

			XML.addValue("gainZero", gain_0);
			XML.addValue("gainOne", gain_1);
			XML.addValue("gainTwo", gain_2);
			XML.addValue("panZero", pan_0);
			XML.addValue("panOne", pan_1);

			XML.addValue("FM", FM_mod);
			XML.addValue("AM", AM_mod);
			XML.addValue("filterFreq", filterFreq);
			XML.addValue("autoPan", autoPan);
			XML.saveFile("settings.xml");						// save xml
			XML.popTag();

		} else {

			XML.addTag("settings");								// create new save information
			XML.pushTag("settings", selectPreset);

			XML.addValue("X", adsrSlider->x);
			XML.addValue("Y", adsrSlider->y);
			XML.addValue("Z", adsrSlider->z);
			XML.addValue("W", adsrSlider->w);

			XML.addValue("gainZero", gain_0);
			XML.addValue("gainOne", gain_1);
			XML.addValue("gainTwo", gain_2);
			XML.addValue("panZero", pan_0);
			XML.addValue("panOne", pan_1);

			XML.addValue("FM", FM_mod);
			XML.addValue("AM", AM_mod);
			XML.addValue("filterFreq", filterFreq);
			XML.addValue("autoPan", autoPan);
			XML.saveFile("settings.xml");
			XML.popTag();

		}
	} 
}

//--------------------------------------------------------------
void ofApp::loadButtonPressed() {

	numADSR = XML.getNumTags("settings");	// check number of tags and load values from xml if tags exist
	if (selectPreset <= numADSR-1) {
		XML.pushTag("settings", selectPreset);
		adsrSlider = ofVec4f(XML.getValue("X", 0.0), XML.getValue("Y", 0.0), XML.getValue("Z", 0.0), XML.getValue("W", 0.0));
		
		gain_0 = XML.getValue("gainZero", 0.00);
		gain_1 = XML.getValue("gainOne", 0.00);
		gain_2 = XML.getValue("gainTwo", 0.00);
		pan_0 = XML.getValue("panZero", 0.00);
		pan_1 = XML.getValue("panOne", 0.00);

		FM_mod = XML.getValue("FM", 0.00);
		AM_mod = XML.getValue("AM", 0.00);
		filterFreq = XML.getValue("filterFreq", 0.00);
		autoPan = XML.getValue("autoPan", 0.00);
		XML.popTag();
	}
}

//--------------------------------------------------------------
void ofApp::update(){

	// updates the total number of saved presets to the gui
	int numADSR = XML.getNumTags("settings");
	ofParameter<int>* guiParam;
	guiParam = (ofParameter<int>*)&numPresets.getParameter();
	int i = guiParam->cast<int>();
	guiParam->cast<int>() = numADSR-1;


	//--ENVELOPES--//
	for (int i = 0; i < NUMCHANNELS; i++) {
		env[i].setAttack(adsrSlider->x);
		env[i].setDecay(adsrSlider->y);
		env[i].setSustain(adsrSlider->z);
		env[i].setRelease(adsrSlider->w);
	}

}

//--------------------------------------------------------------
void ofApp::draw(){

	ofBackground(0);
	midiDebug();
	drawWaveform();
	gui.draw();

	/*std::cout << envOut[0] << std::endl;*/

}

//-----------------------AUDIO----------------------------------
void ofApp::audioOut(float * output, int bufferSize, int nChannels) {

	for (int i = 0; i < bufferSize; i++) {

		//---------SYNTHESIS-------//
	
		// convert midi note to frequency
		pitch[0] = mtof.mtof(midiNote);
		pitch[1] = mtof.mtof(midiNote+5);

		//note * I tried putting these in a for loop but it seemed to break the code could you explain, thanks
		envOut[0] = env[0].adsr(1., midiTrigger);
		envOut[1] = env[1].adsr(1., midiTrigger);
		envOut[2] = env[2].adsr(1., midiTrigger);
	
		//Voice 0, frequency modulation
		lfoOut[0] = lfo[0].sinewave(FM_mod) * 4;								// Frequency Modulation
		oscOut[0] = osc[0].sinewave( pitch[0] + lfoOut[0] ) * envOut[0];		// multiply by envelope
		mix_0 = oscOut[0] * gain_0;												// set output level
		channelMix[0].stereo(mix_0, outputs_0, pan_0);							// create stereo mix
		
		//Voice 1, Amplitude modulation
		lfoOut[1] = (lfo[1].sinewave(AM_mod) + 1) / 2;							// Amplitude Modulation
		oscOut[1] = osc[1].sinewave(pitch[0] + 2) * lfoOut[1] * envOut[1];	    // multiply by envelope
		mix_1 = oscOut[1] * gain_1;												// set output level
		channelMix[1].stereo(mix_1, outputs_1, pan_1);							// create stereo mix
		
		//Voice 2, Subtractive Synthesis and auto pan modulation
		lfoOut[2] = (lfo[2].sinewave(autoPan) + 1) / 2;							// LFO for auto panner
		oscOut[2] = osc[2].saw(pitch[1] + 2) * envOut[2];						// multiply by envelope
		mix_2 = oscOut[2] * gain_2;												// set output level
		filterOut[2] = filter[2].lores(mix_2, envOut[2] * filterFreq, envOut[2]*20);	// filter triggered by envelope (Subtractive Synthesis)
		channelMix[2].stereo(filterOut[2], outputs_2, lfoOut[2]);				// create stereo mix


		masterMix[0] = (outputs_0[0] + outputs_1[0] + outputs_2[0]);			//add stereo outputs to master mix
		masterMix[1] = (outputs_0[1] + outputs_1[1] + outputs_2[1]);
		
		//==========MASTER-OUT==============//
		output[ i*nChannels     ] = masterMix[0] * masterGainSlider;
		output[ i*nChannels + 1 ] = masterMix[1] * masterGainSlider;
		//==================================//


		//used for drawing fft
		//-----------PROCESS-FFT------------//
		double fftMix = (masterMix[0] + masterMix[1]) * .2;
		if (fft.process(fftMix)) {
			oct.calculate(fft.magnitudes);
		}
		//----------------------------------//

	}
}

//--------------------------------------------------------------
void ofApp::drawWaveform() {

	ofNoFill();	
	ofSetLineWidth(1);
	ofBeginShape();
	for (int i = 0; i < oct.nAverages; i++) {

		ofColor colour;
		colour.setHsb((int)ofMap(i, 0, oct.nAverages, 1, 255), 255, 255);
		ofSetColor(colour);

		float size = 5 + oct.averages[i];
		int x = (int)((ofGetWidth() / 2) * i / oct.nAverages) + ofGetWidth() / 4;
		float y = ofGetHeight() * .5;
		float fftHeight = fft.magnitudes[i];

		ofVertex(x, y + fftHeight * -10);				//draw a line of fft magnitude
		ofVertex(x, y + fft.magnitudes[i] * 20);
		ofDrawRectangle(x, y, 1, + fftHeight * -10);	//draw rectangles with fft magnitude
		ofDrawRectangle(x, y, 1, + fftHeight * 10);	   //draw rectangles with fft magnitude

	}
	ofEndShape(false);
}

//-------------------------MIDI---------------------------------
void ofApp::newMidiMessage(ofxMidiMessage &message) {
	
	//-------------------------------//
	if (message.status == MIDI_NOTE_ON) {          // setup midi to trigger note 
		midiNote = message.pitch;
		midiTrigger = 1;
	
	}
	else if (message.status == MIDI_NOTE_OFF) {
		//midiNote = message.pitch;
		midiTrigger = 0;
	}
	                    
	//-------------------------------//
												   // (this could be used to create a sequence later)
	messages.push_back(message);                   // create an array of midi messages and set size
	if (messages.size() > 16) {
		messages.erase(messages.begin());
	}


}

//--------------------------------------------------------------
void ofApp::midiDebug() {

	//--------------------------------------------// print midi note on/off and pitch to screen
	for (int i = 0; i < messages.size(); i++) { 
		ofxMidiMessage &message = messages[i];
		string midiTriggerDebug;
		if (message.status == MIDI_NOTE_ON) {
			midiTriggerDebug = "On";
		}
		else if (message.status == MIDI_NOTE_OFF) {
			midiTriggerDebug = "Off";
		}
													// draw midi notes to the screen
		ofSetColor(255);
		ofDrawBitmapString("Note " + midiTriggerDebug + ": Pitch: " + ofToString(message.pitch), ofGetWidth() * .8, ofGetHeight() * 0.05 + i * 16);
	}

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

	
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

//--------------------------------------------------------------
void ofApp::exit() {
	saveButton.removeListener(this, &ofApp::saveButtonPressed);
	midi.closePort();
	midi.removeListener(this);
	ofSoundStreamClose();
}
