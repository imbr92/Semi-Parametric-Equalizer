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
    //fft = ofxFft::create(kNumBands, OF_FFT_WINDOW_BARTLETT);
    //audiofile.setVerbose(true);
    //ofSetLogLevel(OF_LOG_VERBOSE);
    //settings.setOutListener(this);
    //settings.sampleRate = 44100;
    //settings.numOutputChannels = 2;
    //settings.numInputChannels = 0;
    //settings.bufferSize = kNumBands;
    //soundStream.setup(settings);
    //step = audiofile.samplerate() / 44100;
    //audiofile.load("C:/Users/Yash/Documents/109650_Operation_Evolution.mp3");
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
            //audiofile.load(path);
        }
    }
    if (play) {
        settings.setOutListener(this);
        //step = audiofile.samplerate() / 44100;
    }
    gui.draw();
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
