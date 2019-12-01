#include "ofApp.h"
#include "ofMain.h"

const signed kWidth = 1200;
const signed kHeight = 800;

//========================================================================
int main() {
    ofSetupOpenGL(kWidth, kHeight,
                  OF_WINDOW);  // <-------- setup the GL context

    // this kicks off the running of my app
    // can be OF_WINDOW or OF_FULLSCREEN
    // pass in width and height too:
    ofRunApp(new ofApp());
}
