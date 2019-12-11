- **11/16/19** Made basic GUI with ofxGui and hooked up an MP3 file to confirm functionality Play and Load buttons.
- **11/17/19 - 11/19/19** Failed at using ofxAudifile, ofxFft and ofxSoundObjects due to lack of documentation and lack of knowledge about threads and DSP.

- **11/20/19** Made Polynomial.h and Polynomial.cpp to simplify the math involved in the project with abstraction. 

- **12/1/19** Read articles about the DSP theory behind Equalizers and FFT. Then read documentation of FFTW and libsndfile libraries.
	- Links: 
		*https://www.meldaproduction.com/tutorials/text/equalizers
		*https://www.zytrax.com/tech/audio/equalization.html#fft
		*http://fftw.org/fftw3_doc/Complex-One_002dDimensional-DFTs.html#Complex-One_002dDimensional-DFTs
		*http://www.fftw.org/~fftw/fftw2.pdf
		*http://digitalsoundandmusic.com/5-3-3-reading-and-writing-formatted-audio-files-in-c/

- **12/2/19**
	- Went to Office Hours to get help linking DLL libraries to my project. 
	- Implemented wav file reader and performed FFT after each read 
	- Stored relative sound intensities of each frequency at each time point in a 2D vector
	- Performed IFFT and output the result back to a wav file
	- Having issues with RIFF header being different in output and input wav files

- **12/3/19** 
	- Rewrote first 5000 bytes of binary data in out.wav with info from in.wav (to make RIFF header consistent)
	- Went to Office Hours to fix an issue with binary data not being overwritten (Forgot to flush) and an issue with the library not being compatible with a certain wav file type. 
	- Got IFFT after changing the sound intensity at various frequencies working.

- **12/4/19** 
	- Added Hann window function to reduce spectral leakage (but there are still issues with sound quality).
	- Finished making basic GUI with points and a curve
	- Began connecting GUI and DSP components

- **12/5/19** Fixed all issues in connecting GUI and DSP (Achieved MVP goal)

- **12/10/19** Cleaned up code and added comments to explain more complex portions of the code.

- **12/11/19** 
	- Wrote readme.md
	- Formatted development log
	- Added background scale to project