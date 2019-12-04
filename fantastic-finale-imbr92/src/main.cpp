#include "WavFile.h"
#include "fftw3.h"
#include "ofApp.h"
#include "ofMain.h"

const signed kWidth = 1200;
const signed kHeight = 800;
const signed N = 32768;
#define BUFF_SIZE 32768
#define MAX_FREQ 22  // KHz
#define IN_PATH "C:/Users/Yash/Documents/in3.wav"
#define OUT_PATH "C:/Users/Yash/Documents/out.wav"

//========================================================================
int main() {
    ofSetupOpenGL(kWidth, kHeight,
                  OF_WINDOW);  // <-------- setup the GL context

    // this kicks off the running of my app
    // can be OF_WINDOW or OF_FULLSCREEN
    // pass in width and height too:
    // audiofile.load("C:/Users/Yash/Documents/100Hz.wav");
    WavInFile inFile(IN_PATH);
    WavOutFile outFile(OUT_PATH, inFile.getNumSamples(),
                       16, inFile.getNumChannels());
    // outFile.fillInHeader(inFile.getSampleRate(), inFile.getBytesPerSample() *
    // 8, inFile.getNumChannels());
    float sampleBuffer[BUFF_SIZE];
    fftw_complex *in, *out, *rev;
    fftw_plan p, q;
    in = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * N);
    out = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * N);
    rev = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * N);
    p = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
    q = fftw_plan_dft_1d(N, out, rev, FFTW_BACKWARD, FFTW_ESTIMATE);

    // sample rate is 44100 ==> Follows Nyquist thm
    // cout << inFile.getSampleRate();

    // goes through entire file and executes each dft
    // db levels of each sample time
    vector<vector<double>> spdbl;
    double add_factor = 0;
    while (inFile.eof() == 0) {
        size_t samplesRead = inFile.read(sampleBuffer, BUFF_SIZE);

        for (int i = 0; i < BUFF_SIZE; i++) {
            in[i][0] = (double)sampleBuffer[i];
        }

		// Hanning Window Function
        // where does this go?
        for (int i = 0; i < BUFF_SIZE; i++) {
            sampleBuffer[i] =
                0.5 * (1 - cos(2 * PI * i / BUFF_SIZE - 1)) * sampleBuffer[i];
        }

        fftw_execute(p); /* repeat as needed */

        double mag = 0, pos = 0;
        double cdb = 0;
        vector<double> cur;
         for (int i = 1; i < BUFF_SIZE - 1; ++i) {
            // cout << sampleBuffer[i] << " " <<  in[i][0] << "\n";
            // check it is within normal range of hearing (Within 20kHz)
            if (i * inFile.getSampleRate() / BUFF_SIZE >= 20000) break;

            // Normalizing FFT Output
            out[i][0] /= N;
            out[i][1] /= N;

            // mag = re^2 + im^2
            // cout << out[i][0] * out[i][0] + out[i][1] * out[i][1] << " ";
            if (mag < out[i][0] * out[i][0] + out[i][1] * out[i][1]) {
                mag = out[i][0] * out[i][0] + out[i][1] * out[i][1];
                pos = i;
            }
            cdb +=
                20 * log10(sqrt(out[i][0] * out[i][0] + out[i][1] *
                out[i][1]));
            if (!((i) % (1 << 6))) {
                cur.push_back(cdb / (1 << 6));
                add_factor = min(add_factor, cdb / (1 << 6));
                cdb = 0;
            }
            out[i][0] *= N;
            out[i][1] *= N;
                     if (i < 200) {
                         double cc =
                             out[i][0] * out[i][0] + out[i][1] * out[i][1];
                         cc = 20 * log10(cc);
                         cc += 30;
                         cc /= 20;
                         cc = pow(10, cc);
                         cc -= out[i][1] * out[i][1];
                         cc = sqrt(cc);
                         out[i][0] = cc;
            }
        }
        spdbl.push_back(cur);
        // confirmed that IFFT of in + out gives samples back in in[i][0]
        fftw_execute(q);
        for (int i = 0; i < BUFF_SIZE; ++i) {
            sampleBuffer[i] = rev[i][0] / N;
            // cout << rev[i][0]/N << " " << in[i][0] << "\n";
        }
        
		outFile.write(sampleBuffer, BUFF_SIZE);
        // break;
        // cout << "\n";
        // cout << mag << " pos : " << pos << "\n";
    }

    // for (auto i : spdbl) {
    //    for (auto j : i) {
    //        cout << j - add_factor << " ";
    //    }
    //     cout << "\n";
    //}

    // confirmed through comparison of binary files that the header is not being
    // recreated properly but the rest of the file is
    outFile.close();
    fftw_destroy_plan(p);
    fftw_destroy_plan(q);
    fftw_free(in);
    fftw_free(out);
    fftw_free(rev);

        // overwrite header (not sure about exact header length, check)
    fstream ifs(IN_PATH, ios::in | ios::binary);
    fstream ofs(OUT_PATH,
                ios::in | ios::out | ios::binary);
    char x;
    char y[5000];
    memset(y, 'a', sizeof(y));
    ofs.seekg(0, ios::beg);
    for (int i = 0; i < 5000; ++i) {
		ifs.read((&x), 1);
		y[i] = x;
	}
    cout << y << endl;
    cout << ofs.is_open();

    ofs.write(y, sizeof(y));
    ofs.close();
    ofRunApp(new ofApp());
    return 0;
}
