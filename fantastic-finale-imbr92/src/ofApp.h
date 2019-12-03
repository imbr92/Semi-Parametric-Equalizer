#pragma once

#include "ofMain.h"
//#include "ofxAudioFile.h"
//#include "ofxFft.h"
#include "ofxGui.h"
#include "fftw3.h"

class ofApp : public ofBaseApp {
   public:
    const int kNumBands = 512;
    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    // void ofApp::audioOut(float* input, int bufferSize, int nChannels);
    //void ofApp::audioOut(ofSoundBuffer& buffer);

   private:
    ofxPanel gui;
    ofxButton load;
    ofxButton play;
    //ofSoundPlayer sound_file;
    //ofxAudioFile audiofile;
    //ofxFft* fft;
};
