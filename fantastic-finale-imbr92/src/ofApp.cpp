#include "ofApp.h"
ofSoundStream soundStream;
ofSoundStreamSettings settings;
int step = INT_MAX;
int toMove = -1;
//--------------------------------------------------------------
void ofApp::setup() {
    gui.setup();
    gui.add(load.setup("Load"));
    gui.add(play.setup("Play"));
    for (int i = -2; i <= 2; ++i) {
        pts.push_back(make_pair((long double)kWidth / 2.0 + i * kCircleSpacing,
                                (long double)kHeight / 2.0));
    }
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
            path = result.getPath();
            // audiofile.load(path);
        }
    }
    if (play) {
        processSound(path);
		    
	}

    for (int i = 0; i < pts.size(); ++i) {
        ofDrawCircle(pts[i].first, pts[i].second, kRadius);
    }
    Polynomial p(pts);
    vector<long double> curve = p.graph(0, kWidth);
    // for (auto i : curve) cout << i << "\n";
    ofSetColor(255, 255, 255);
    ofFill();
    for (int i = 0; i < curve.size(); ++i) {
        ofDrawCircle(i, curve[i], 1);
    }
    ofSetColor(0, 0, 0);
    //ofFill();
    gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {
    if (toMove == -1) return;
    pts[toMove].first = x;
    pts[toMove].second = y;
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
    for (int i = 0; i < pts.size(); ++i) {
        pair<long double, long double> cur = pts[i];
        long double rsq = pow(pts[i].first - x, 2) + pow(pts[i].second - y, 2);
        if (sqrt(rsq) <= kRadius) {
            toMove = i;
        }
    }
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) { toMove = -1; }

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

void processSound(std::string curpath) {
    WavInFile inFile(curpath);
    WavOutFile outFile(kOutPath, inFile.getNumSamples(), 16,
                       inFile.getNumChannels());
}
