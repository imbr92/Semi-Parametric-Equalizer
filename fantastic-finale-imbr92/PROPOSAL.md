# *Proposal -- Standalone Semi-Parametric Equalizer*



## *Project Overview*

For this project I would like to make a standalone audio program which can load in WAV files, visualize the decibel gain at various frequencies for the given WAV and edit them by customizing audio intensity levels at varying frequency. An example of a similar program is the Fruity Parametric EQ which comes built into FL Studio. The main difference between this project and the Fruity Parametric EQ is that the latter is a built-in plugin for a Digital Audio Workstation (DAW) while the program I plan to create would be standalone. Most existant standalone EQ programs are dynamic rather than parametric, though this idea is by no means entirely original. If I have extra time I would also like to experiment with functions to edit waveforms based on certain parameters.


  ![Parametric EQ](http://beatproduction.net/wp-content/uploads/2008/01/parametric-eq2.jpg)

## *Background Knowledge and Motivation*

I have no experience making audio software, but I have played around with a few on LMMS and FL Studio before so I am familiar with what they are expected to do. Though I have no interest in making music, I think the process of making an EQ would be fun and educational. Additionally, I find this kind of project more interesting than making some sort of game which was my only other idea. 

## *External Libraries*

I plan on using Soundstretch's wav file reader to load wav files into my program and its wav file writer to write out to wav files. I also plan to use FFTW to perform FFTs and IFFTs on the data. 


  Mentioned ofx Addons:
* [FFTW++](https://sourceforge.net/projects/fftwpp/)
* [Soundstretch](https://www.surina.net/soundtouch/)

## *Timeline*

* Learn how OpenFrameworks and the libraries I plan to use work. Implement Audio File Input and Make a basic layout for the UI
* Add Core Functionality (Actual EQ portion)
* Wave Interpolation (Curve fitting of chosen points) and Improve UI 
* Testing, Finish UI work, and Add Preset Filters

## *Stretch/Reach Goals*

* Waveform Editor
* Frequency Modulation Option of Multiple Tracks
* Exporting of WAV files after editing

## *Minimum Viable Product*

The MVP for this project would be a working equalizer that shows a user-controllable graph for how the sound intensity of different frequencies in a WAV file. It should allow the user to drag a fixed number of points (probably around 5) to any positions in the UI which should change the graph to a polynomial which passes through all of these points. Moving a point upwards at a given frequency should boost the intensity of the sound wave at that point, and moving a point downwards at the same frequency should decrease the intensity of the sound wave at that point. After setting the EQ, the user should be able to play the WAV file with these changes applied using the UI. 