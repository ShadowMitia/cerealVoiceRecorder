#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
  int bufferSize = 256;


  ofSetVerticalSync(true);

  soundStream.printDeviceList();

  left.assign(bufferSize, 0.0);
  right.assign(bufferSize, 0.0);


  //ofSoundStreamSettings settings;
  /*
  auto devices = soundStream.getDeviceList();
  settings.setOutDevice(devices[0]);

  // or get the default device for an specific api:
  //
  // settings.api = ofSoundDevice::Api::PULSE;

  // or by name:
  //
  //	auto devices = soundStream.getMatchingDevices("default");
  //	if(!devices.empty()){
  //		settings.setOutDevice(devices[0]);
  //	}


  settings.setOutListener(this);
  settings.sampleRate = 48000;
  settings.numOutputChannels = 2;
  settings.numInputChannels = 1;
  settings.bufferSize = bufferSize;
  soundStream.setup(settings);
  */

  soundStream.setup(this, 2, 1, 48000, 256, 4);

  ofDirectory dir(".");
  dir.allowExt("cereal");
  dir.listDir();
  numFiles = dir.getFiles().size();

  std::cout << numFiles << "\n";
}

//--------------------------------------------------------------
void ofApp::update(){
  if (rec) {
    unique_lock<mutex> lock(audioMutex);
    line.clear();
    float i = 0;
    while (i < TWO_PI) { // make a heart
      float r = (2-2*sin(i) + sin(i)*sqrt(abs(cos(i))) / (sin(i)+1.4)) * -80;
      float x = ofGetWidth()/2 + cos(i) * (r * ofRandom(5));
      float y = ofGetHeight()/2 + sin(i) * r;
      line.addVertex(ofVec2f(x, y));
      i+=0.005*HALF_PI*0.5;
    }
    line.close(); // close the shape
  }
}

void ofApp::audioIn(ofSoundBuffer& buffer) {
  if (rec) {
    soundBuffers.back().append(buffer);
    unique_lock<mutex> lock(audioMutex);
    soundPolylines.push_back(line);
    for (int i = 0; i < buffer.getNumFrames(); i++) {
      left[i] = buffer.getSample(i, 0) * 0.5;
      right[i] = buffer.getSample(i, 1) * 0.5;
    }
  }
}

void ofApp::audioOut(ofSoundBuffer& buffer) {
  /*
  if (soundBuffers.size() > 0) {
    std::cout << "Buffer size: " << soundBuffers.back().size() << " sound stream buff size " << soundStream.getBufferSize() << "\n";
    std::cout << "num frames " << buffer.getNumFrames() << "\n";
  }
  */
  if (play && bufferCounter < soundBuffers.back().size()/soundStream.getBufferSize() - 1) {

    //std::cout << "Buffer counter: " << bufferCounter << "\n";
    for (int i = 0; i < buffer.getNumFrames(); i++) {
      auto sample = soundBuffers.back().getSample(i + (bufferCounter) * buffer.getNumFrames(), 0);
      buffer.getSample(i, 0) = sample;
      buffer.getSample(i, 1) = sample;
    }
    bufferCounter++;
  } else {
    play = false;
    bufferCounter = 0;
  }
}

//--------------------------------------------------------------
void ofApp::draw(){
  ofNoFill();

  // draw the left channel:
  ofPushStyle();
  ofPushMatrix();
  ofTranslate(32, 170, 0);

  ofSetColor(225);
  ofDrawBitmapString("Left Channel", 4, 18);

  ofSetLineWidth(1);
  ofDrawRectangle(0, 0, 512, 200);

  ofSetColor(245, 58, 135);
  ofSetLineWidth(3);

  ofBeginShape();
  for (unsigned int i = 0; i < left.size(); i++){
    ofVertex(i*2, 100 -left[i]*180.0f);
  }
  ofEndShape(false);

  ofPopMatrix();
  ofPopStyle();

  // draw the right channel:
  ofPushStyle();
  ofPushMatrix();
  ofTranslate(32, 370, 0);

  ofSetColor(225);
  ofDrawBitmapString("Right Channel", 4, 18);

  ofSetLineWidth(1);
  ofDrawRectangle(0, 0, 512, 200);

  ofSetColor(245, 58, 135);
  ofSetLineWidth(3);

  ofBeginShape();
  for (unsigned int i = 0; i < right.size(); i++){
    ofVertex(i*2, 100 -right[i]*180.0f);
  }
  ofEndShape(false);

  ofPopMatrix();
  ofPopStyle();

  if (play) {
    /*
    ofPushMatrix();
    ofSetColor(ofColor::blue);
    ofTranslate(100, 100);
    soundPolylines[bufferCounter].draw();
    ofPopMatrix();
    */
  }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
  if (key == 'r') {
    std::cout << "Rec\n";
    if (!rec) {
      soundBuffers.emplace_back();
      soundPolylines.clear();
    }
    rec = !rec;
  }

  if (key == 'p') {
    if (play) {
      std::cout << "Not play\n";
    } else {
      std::cout << "Play\n";
    }

    play = !play;
  }

  if (key == 's') {
    std::ofstream os(ofToDataPath("soundbuffer"+std::to_string(numFiles)+".cereal"), std::ios::binary);
    cereal::PortableBinaryOutputArchive archive( os );
    std::cout << "save\n";
    archive( soundBuffers );

    ofDirectory dir(".");
    dir.allowExt("cereal");
    dir.listDir();
    numFiles = dir.getFiles().size();
  }

  if (key == 'l') {
    bool found = false;
    do {
      auto path = ofToDataPath("soundbuffer"+std::to_string((int)ofRandom(0,numFiles+1))+".cereal");
      std::ifstream os(path, std::ios::binary);
      if (os.is_open()) {
	found = true;
	cereal::PortableBinaryInputArchive archive( os );
	std::cout << "load\n";
	std::vector<ofSoundBuffer2> buff;
	archive( buff );
	soundBuffers.swap(buff);
      }

    } while (!found);
  }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
