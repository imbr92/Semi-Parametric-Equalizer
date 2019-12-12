#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "fftw3.h"
#include "WavFile.h"
#include "Polynomial.h"
#include <windows.h>
#include <mmsystem.h>

class ofApp : public ofBaseApp {
   public:
    const int kNumBands = 512;
    const int kRadius = 10;
    const int kWidth = 1200;
    const int kHeight = 800;
    const int kCircleSpacing = 200;
    const int kHearingThreshold = 20000;
    const int kNoCircle = -1;
    const int kBitsPerSample = 16;
    const double kMaxGain = 50.0;
    const char* kOutPath = "C:/Users/Yash/Documents/out.wav";
    const char* kOutPathv2 = "C:\\Users\\Yash\\Documents\\out.wav";
    const char* kBackgroundImagePath = "background.png";
	
	void setup();
    void draw();

    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);

   private:
    void processSound();
    void rewriteHeader();
    double updateBin(double re, int i);
    char* path;
    int to_move = kNoCircle;
    ofxPanel gui;
    ofxButton load, play;
    ofImage background;
    ofSoundPlayer sound_player;
	vector<pair<long double, long double>> pts;
    vector<long double> poly_curve;
};
