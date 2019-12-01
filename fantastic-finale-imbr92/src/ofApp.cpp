#include "ofApp.h"
ofSoundStream soundStream;
ofSoundStreamSettings settings;
int step = INT_MAX;
//--------------------------------------------------------------
void ofApp::setup() {
    gui.setup();
    gui.add(load.setup("Load"));
    gui.add(play.setup("Play"));
    ofDrawLine(0, 0, 100, 100);
    fft = ofxFft::create(kNumBands, OF_FFT_WINDOW_BARTLETT);
    audiofile.setVerbose(true);
    ofSetLogLevel(OF_LOG_VERBOSE);
    settings.setOutListener(this);
    settings.sampleRate = 44100;
    settings.numOutputChannels = 2;
    settings.numInputChannels = 0;
    settings.bufferSize = kNumBands;
    soundStream.setup(settings);
    step = audiofile.samplerate() / 44100;
    audiofile.load("C:/Users/Yash/Documents/109650_Operation_Evolution.mp3");
	}
//--------------------------------------------------------------
void ofApp::update() {}

//--------------------------------------------------------------
void ofApp::draw() {
    if (load) {
        // from
        // https://openframeworks.cc/documentation/utils/ofSystemUtils/#!show_ofSystemLoadDialog
        ofFileDialogResult result = ofSystemLoadDialog("Load file");
        if (result.bSuccess) {
            // abs path
            std::string path = result.getPath();
            audiofile.load(path);
        }
    }
    if (play) {
        settings.setOutListener(this);
        step = audiofile.samplerate() / 44100;
    }
    gui.draw();
}
//--------------------------------------------------------------
// void ofApp::audioOut(ofSoundBuffer &outBuffer) {
/*for (int i = 0; i < outBuffer.size(); i += 2) {
        float sample = sin(phase); // generating a sine wave sample
        outBuffer[i] = sample; // writing to the left channel
        outBuffer[i + 1] = sample; // writing to the right channel
        phase += 0.05;
} */
//}

//--------------------------------------------------------------
void ofApp::audioOut(ofSoundBuffer &buffer) {
    // cout << "1";
	/*for (int i = 0; i < kNumBands; ++i) {
        output[i] *= ofRandom(0, 0);
	}
    output[0] = 100;
        output[1] = 1.0;
	fft->setSignal(output);
	float* modified_audio = fft->getAmplitude();
    for (int i = 0; i < 4; ++i) {
        modified_audio[i] = 0;
	}
    fft->setPolar(modified_audio, fft->getPhase());
    fft->clampSignal();*/
    float pan = 0.5f;
    float leftScale = 1 - pan;
    float rightScale = pan;
    float phase = 0;
    int volume = 1;

    // sin (n) seems to have trouble when n is very large, so we
    // keep phase in the range of 0-TWO_PI like this:
    while (phase > TWO_PI) {
        phase -= TWO_PI;
    }
    int cnt = 0;
    for (size_t i = 0; i < buffer.getNumFrames(); i++) {
        if (buffer[i * buffer.getNumChannels()]) cnt++;
	}
    if (cnt < 10) return;
    for (size_t i = 0; i < buffer.getNumFrames(); i++) {
        buffer[i * buffer.getNumChannels()] =
            ofRandom(0, 1) * volume * leftScale;
        buffer[i * buffer.getNumChannels() + 1] =
            ofRandom(0, 1) * volume * rightScale;
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {}
