#include "WavFile.h"
//#include "stdafx.h"

#include "fftw3.h"
#include "ofApp.h"
#include "ofMain.h"

const signed kWidth = 1200;
const signed kHeight = 800;
const signed N = 2048;
#define BUFF_SIZE 2048
#define MAX_FREQ 22  // KHz
//========================================================================
int main() {
    ofSetupOpenGL(kWidth, kHeight,
                  OF_WINDOW);  // <-------- setup the GL context

    // this kicks off the running of my app
    // can be OF_WINDOW or OF_FULLSCREEN
    // pass in width and height too:
    // audiofile.load("C:/Users/Yash/Documents/100Hz.wav");
    WavInFile inFile("C:/Users/Yash/Documents/exwav.wav");
    float sampleBuffer[BUFF_SIZE];
    fftw_complex *in, *out;
    fftw_plan p;
    in = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * N);
    out = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * N);
    p = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
	
	// sample rate is 44100 ==> Follows Nyquist thm
    // cout << inFile.getSampleRate();

	//goes through entire file and executes each dft
	while (inFile.eof() == 0) {
        size_t samplesRead = inFile.read(sampleBuffer, BUFF_SIZE);

        for (int i = 0; i < BUFF_SIZE; i++) {
            in[i][0] = (double)sampleBuffer[i];
        }

        fftw_execute(p); /* repeat as needed */
        double mag = 0, pos = 0;
        for (int i = 0; i < BUFF_SIZE; ++i) {
			//mag = re^2 + im^2
            //cout << out[i][0] * out[i][0] + out[i][1] * out[i][1] << " "; 
            if (mag < out[i][0] * out[i][0] + out[i][1] * out[i][1]) {
                mag = out[i][0] * out[i][0] + out[i][1] * out[i][1];
                pos = i;
			}
        }
		//db levels of each sample time 
        vector<vector<int>> spdbl;

        //break;
        //cout << "\n";
        cout << mag << " pos : " << pos << "\n";
    }
    fftw_destroy_plan(p);
    fftw_free(in);
    fftw_free(out);
    ofRunApp(new ofApp());
}
