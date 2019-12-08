#include "ofApp.h"
ofSoundStream soundStream;
ofSoundStreamSettings settings;
int step = INT_MAX;
int toMove = -1;
const int BUFF_SIZE = 16384;

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
            string respath = result.getPath();
            path = new char[respath.size()];
            std::strcpy(path, respath.c_str());
            cout << path;
            // audiofile.load(path);
        }
    }
    if (play) {
        processSound();
        mySound.load("C:\\Users\\Yash\\Documents\\out.wav");
        mySound.play();
    }

    for (int i = 0; i < pts.size(); ++i) {
        ofDrawCircle(pts[i].first, pts[i].second, kRadius);
    }
    vector<pair<long double, long double>> pp = pts;
    pp.push_back({0, kHeight / 2});
    pp.push_back({1200, kHeight / 2});
    Polynomial p(pp);
    curvegraph = p.graph(0, kWidth);
    // for (auto i : curve) cout << i << "\n";
    ofSetColor(255, 255, 255);
    ofFill();
    for (int i = 0; i < curvegraph.size(); ++i) {
        ofDrawCircle(i, curvegraph[i], 1);
    }
    ofSetColor(0, 0, 0);
    // ofFill();
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

void ofApp::processSound() {
    std::ofstream del;
    del.open(kOutPath, std::ofstream::out | std::ofstream::trunc);
    del.close();
    WavInFile *inFile = new WavInFile(path);
    WavOutFile *outFile = new WavOutFile(kOutPath, inFile->getNumSamples(), 16,
                                         inFile->getNumChannels());
    float sampleBuffer[BUFF_SIZE];
    fftw_complex *in, *out, *rev;
    fftw_plan p, q;
    in = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * BUFF_SIZE);
    out = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * BUFF_SIZE);
    rev = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * BUFF_SIZE);
    p = fftw_plan_dft_1d(BUFF_SIZE, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
    q = fftw_plan_dft_1d(BUFF_SIZE, out, rev, FFTW_BACKWARD, FFTW_ESTIMATE);
    rev[0][0] = -60;
    while (inFile->eof() == 0) {
        size_t samplesRead = inFile->read(sampleBuffer, BUFF_SIZE);

        for (int i = 0; i < BUFF_SIZE; i++) {
            in[i][0] = (double)sampleBuffer[i];
            in[i][1] = 0;
        }

        // Hanning Window Function
        // where does this go?
        for (int i = 0; i < BUFF_SIZE; i++) {
            sampleBuffer[i] =
                0.5 * (1 - cos(2 * PI * i / BUFF_SIZE - 1)) * sampleBuffer[i];
        }
        fftw_execute(p); /* repeat as needed */
        for (int i = 1; i < BUFF_SIZE - 1; ++i) {
            // check it is within normal range of hearing (Within 20kHz)
            if (i * inFile->getSampleRate() / BUFF_SIZE >= 20000) break;

            //// Normalizing FFT Output
            out[i][0] /= BUFF_SIZE;
            out[i][1] /= BUFF_SIZE;
            double cc = out[i][0] * out[i][0];
            cc = 20 * log10(cc);
            // 0 to 10
            double pos = log2(1024.0 * i / BUFF_SIZE);
            pos *= curvegraph.size() / 10.0;
            pos = max(pos, 0.0);
            pos = min(pos, (int)curvegraph.size() - 1.0);
            double tmp =
                (curvegraph[(int)pos] - kHeight / 2) * 50.0 / (kHeight / 2);
            if (tmp != 0) tmp = tmp / abs(tmp) * min(abs(tmp), 50.0);
            cc += -1 * tmp;
            //cout << tmp << " " << i << "\n";
                 //if(rev[0][0] == -60) cout << i * inFile->getSampleRate() / BUFF_SIZE << " " << -1 * tmp
                 //    << "\n";
                cc /= 20;
            cc = pow(10, cc);
            // cc -= out[i][1] * out[i][1];
            cc = sqrt(cc);
            // cout << tmp;
            // if(out[i][0] != cc) cout << cc << " " << out[i][0] << "\n";
            out[i][0] = (out[i][0] != 0 ? out[i][0] / abs(out[i][0]) : 1) * cc;
            //// Revert FFT Output back to pre-normalized form
            out[i][0] *= BUFF_SIZE;
            out[i][1] *= BUFF_SIZE;
            // for (auto i : curvegraph) cout << i << " ";
        }
        // break;
        // confirmed that IFFT of in + out gives samples back in rev[i][0]
        fftw_execute(q);
        for (int i = 0; i < BUFF_SIZE; ++i) {
            // put back in rev
            sampleBuffer[i] = rev[i][0] / BUFF_SIZE;
            // cout << out[i][0] << " " << rev[i][0]/BUFF_SIZE << " " <<
            // in[i][0] << "\n";
        }
        // for (auto i : sampleBuffer) {
        //    cout << i << " ";
        //}
        // cout << "\n";
        outFile->write(sampleBuffer, BUFF_SIZE);
    }

    // confirmed through comparison of binary files that the header is not being
    // recreated properly but the rest of the file is
    // cout << outFile.valid();
    // outFile->close();
    // outFile->finishHeader();
    // for some reason deleting these is causing an issue (Clicking Play once is
    // causing this method to be called multiple times???)
    delete outFile;
    delete inFile;

    fftw_destroy_plan(p);
    fftw_destroy_plan(q);
    fftw_free(in);
    fftw_free(out);
    fftw_free(rev);

    // overwrite header (not sure about exact header length, check)
    fstream ifs(path, ios::in | ios::binary);
    fstream ofs(kOutPath, ios::in | ios::out | ios::binary);
    char x;
    char y[5000];
    memset(y, 'a', sizeof(y));
    ofs.seekg(0, ios::beg);
    for (int i = 0; i < 5000; ++i) {
        ifs.read((&x), 1);
        y[i] = x;
    }
    ofs.write(y, sizeof(y));
    ofs.close();
}
