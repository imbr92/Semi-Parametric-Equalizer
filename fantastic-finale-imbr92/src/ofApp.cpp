#include "ofApp.h"

const int kBufferSize = 16384;
const int kHeaderLength = 5000;

//--------------------------------------------------------------
void ofApp::setup() {
    gui.setup();
    gui.add(load.setup("Load"));
    gui.add(play.setup("Play"));
    ofBackground(0, 0, 0);
    background.load(kBackgroundImagePath);
    for (int i = -2; i <= 2; ++i) {
        pts.push_back(make_pair((long double)kWidth / 2.0 + i * kCircleSpacing,
                                (long double)kHeight / 2.0));
    }
}

//--------------------------------------------------------------
void ofApp::draw() {
    background.draw(0, 0);
    if (load) {
        // from
        // https://openframeworks.cc/documentation/utils/ofSystemUtils/#!show_ofSystemLoadDialog
        ofFileDialogResult result = ofSystemLoadDialog("Load file");
        if (result.bSuccess) {
            string respath = result.getPath();
            path = new char[respath.size()];
            std::strcpy(path, respath.c_str());
        }
    }
    if (play) {
        processSound();
        mySound.load(kOutPathv2);
        mySound.play();
    }
    ofSetColor(0, 0, 0);
    // Draw draggable circles
    for (int i = 0; i < pts.size(); ++i) {
        ofDrawCircle(pts[i].first, pts[i].second, kRadius);
    }

    // Draw resulting curve
    vector<pair<long double, long double>> pp = pts;
    pp.push_back({0 - (kWidth/16), kHeight / 2});
    pp.push_back({kWidth + (kWidth/16), kHeight / 2});
    Polynomial p(pp);
    curvegraph = p.graph(0, kWidth);
    ofSetColor(250, 4, 135);
    for (int i = 0; i < curvegraph.size(); ++i) {
        ofDrawCircle(i, curvegraph[i], 1);
    }
    ofSetColor(255, 255, 255);
    gui.draw();
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {
    if (toMove == kNoCircle) return;
    pts[toMove].first = x;
    pts[toMove].second = y;
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
    // Check if mouse is pressed within any of the movable circles' radii
    for (int i = 0; i < pts.size(); ++i) {
        pair<long double, long double> cur = pts[i];
        long double rsq = pow(pts[i].first - x, 2) + pow(pts[i].second - y, 2);
        if (sqrt(rsq) <= kRadius) {
            toMove = i;
        }
    }
}

//--------------------------------------------------------------
void ofApp::processSound() {
    // Open wav file reader with user-specified path
    WavInFile *inFile = new WavInFile(path);

    // Open wav file writer to output path
    WavOutFile *outFile = new WavOutFile(kOutPath, inFile->getNumSamples(), kBitsPerSample,
                                         inFile->getNumChannels());

    // Loads samples from input wav
    float sampleBuffer[kBufferSize];

    // Setting up FFTW (From
    // http://www.fftw.org/fftw3_doc/Complex-One_002dDimensional-DFTs.html#Complex-One_002dDimensional-DFTs)
    fftw_complex *in, *out, *rev;
    fftw_plan p, q;
    in = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * kBufferSize);
    out = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * kBufferSize);
    rev = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * kBufferSize);
    p = fftw_plan_dft_1d(kBufferSize, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
    q = fftw_plan_dft_1d(kBufferSize, out, rev, FFTW_BACKWARD, FFTW_ESTIMATE);

    // Read input wav samples in order
    while (inFile->eof() == 0) {
        size_t samplesRead = inFile->read(sampleBuffer, kBufferSize);

        // Load current samples into input array
        for (int i = 0; i < kBufferSize; i++) {
            in[i][0] = (double)sampleBuffer[i];
            in[i][1] = 0;
        }

        // Hanning Window Function (From
        // https://stackoverflow.com/questions/3555318/implement-hann-window)
        for (int i = 0; i < kBufferSize; i++) {
            sampleBuffer[i] =
                0.5 * (1 - cos(2 * PI * i / kBufferSize - 1)) * sampleBuffer[i];
        }

        // Compute Complex 1D DFT
        fftw_execute(p);

        for (int i = 1; i < kBufferSize - 1; ++i) {
            // Check current bin is within 20kHz (Human Hearing Threshold)
            if (i * inFile->getSampleRate() / kBufferSize >= kHearingThreshold)
                break;

            out[i][0] = updateBin(out[i][0], i);
        }

        // Perform Complex 1D IDFT
        fftw_execute(q);

        for (int i = 0; i < kBufferSize; ++i) {
            sampleBuffer[i] = rev[i][0] / kBufferSize;
        }

        // Write out data to output wav
        outFile->write(sampleBuffer, kBufferSize);
    }

    delete outFile;
    delete inFile;

    fftw_destroy_plan(p);
    fftw_destroy_plan(q);
    fftw_free(in);
    fftw_free(out);
    fftw_free(rev);

    rewriteHeader();
}

double ofApp::updateBin(double re, int i) {
    // Normalizing FFT Output
    re /= kBufferSize;

	// Magnitude, X-position corresponding to bin
    double pre = re * re, pos = log2(1024.0 * i / kBufferSize);

	//Converting from amplitude to decibels
	double cc = 20 * log10(pre);
    pos *= curvegraph.size() / 10.0;
    
	//Make sure pos stays within bounds
	pos = max(pos, 0.0);
    pos = min(pos, (int)curvegraph.size() - 1.0);
    
	//Calculate value to change sound intensity by
	double tmp =
        (curvegraph[(int)pos] - kHeight / 2) * kMaxGain / (kHeight / 2);

	//Cap values at kMaxGain = 50 dB
    if (tmp != 0) tmp = tmp / abs(tmp) * min(abs(tmp), kMaxGain);
    
	//Revert to amplitude form
	cc += -1 * tmp;
    cc /= 20;
    cc = pow(10, cc);
    cc = sqrt(cc);
    return (re != 0 ? re / abs(re) : 1) * cc * kBufferSize;
}

// Rewriting first 5000 bytes to ensure header is correct
void ofApp::rewriteHeader() {
    fstream ifs(path, ios::in | ios::binary);
    fstream ofs(kOutPath, ios::in | ios::out | ios::binary);
    char x;
    char y[kHeaderLength];
    memset(y, ' ', sizeof(y));
    ofs.seekg(0, ios::beg);
    for (int i = 0; i < kHeaderLength; ++i) {
        ifs.read((&x), 1);
        y[i] = x;
    }
    ofs.write(y, sizeof(y));
    ofs.close();
}
