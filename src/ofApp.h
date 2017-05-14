#pragma once

#include "ofMain.h"
#include <cereal/types/vector.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/archives/portable_binary.hpp>
#include <fstream>
#include "ofSoundStream.h"

class ofSoundBuffer2 : public ofSoundBuffer {

  friend class cereal::access;

  template<typename Archive>
    void serialize(Archive& archive) {
    archive( buffer, channels, samplerate, tickCount, soundStreamDeviceID);
  }
};


class ofApp : public ofBaseApp {

 public:
  void setup();
  void update();
  void draw();

  void keyPressed(int key);
  void keyReleased(int key);
  void mouseMoved(int x, int y );
  void mouseDragged(int x, int y, int button);
  void mousePressed(int x, int y, int button);
  void mouseReleased(int x, int y, int button);
  void mouseEntered(int x, int y);
  void mouseExited(int x, int y);
  void windowResized(int w, int h);
  void dragEvent(ofDragInfo dragInfo);
  void gotMessage(ofMessage msg);

  void audioIn(ofSoundBuffer& buffer);
  void audioOut(ofSoundBuffer& buffer);

  ofSoundStream soundStream;
  std::vector<ofSoundBuffer2> soundBuffers;

  std::vector<ofPolyline> soundPolylines;

  ofPolyline line;

  int bufferCounter = 0;

  bool play = false;
  bool rec = false;

  std::vector<float> left;
  std::vector<float> right;

  mutex audioMutex;

  int numFiles = 0;
};
