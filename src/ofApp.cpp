#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
  int bufferSize = 256;


  ofSetVerticalSync(true);



  soundStream.printDeviceList();


  left.assign(bufferSize, 0.0);
  right.assign(bufferSize, 0.0);

  soundBuffer.setNumChannels(2);


  soundStream.setup(this, 2, 2, 44100, bufferSize, 4);

}

//--------------------------------------------------------------
void ofApp::update(){

}

void ofApp::audioIn(ofSoundBuffer& buffer) {
  soundBuffer.append(buffer);
  for (int i = 0; i < buffer.getNumFrames(); i++) {
    left[i] = buffer.getSample(i, 0) * 0.5;
    right[i] = buffer.getSample(i, 1) * 0.5;
  }
}

void ofApp::audioOut(ofSoundBuffer& buffer) {
  if (play) {
    for (int i = 0; i < buffer.getNumFrames(); i++) {
      buffer.getSample(i, 0) = buffer.getSample(i, 1) = soundBuffer.getSample(i + bufferCounter * buffer.getNumFrames(), 0);
    }
    bufferCounter++;
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
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

  if (key == ' ') {
    if (play) {
      std::cout << "Not play\n";
    } else {
      std::cout << "Play\n";
    }
    play = !play;
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
