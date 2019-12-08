#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "fftw3.h"
#include "WavFile.h"
#include "../Polynomial.h"
#include <windows.h>
#include <mmsystem.h>

class ofApp : public ofBaseApp {
   public:
    const int kNumBands = 512;
    const int kRadius = 10;
    const int kWidth = 1200;
    const int kHeight = 800;
    const int kCircleSpacing = 200;
    const char* kOutPath = "C:/Users/Yash/Documents/out.wav";
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

   private:
    void processSound();
    ofxPanel gui;
    ofxButton load;
	// equalize before each play
    ofxButton play;
    vector<pair<long double, long double>> pts;
    vector<long double> curvegraph;
    char* path;
    ofSoundPlayer mySound;
};
