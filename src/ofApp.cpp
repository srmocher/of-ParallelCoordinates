#include "ofApp.h"
#include <string>

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetBackgroundColor(ofColor(255, 255, 255));//set white background
	string dataSetName = "cars.tsv";
	string path = ofToDataPath(dataSetName);
	reader.read_data(path);
	int width = 500;
	int height = 300;
	plot.setup(width, height, reader.get_columns(), 60, 40, 200);
    cout<<"Enter the following keys to switch between datasets"<<endl;
    cout<<"C - cars.tsv"<<endl;
    cout<<"N - nutrients.tsv"<<endl;
    cout<<"M - cameras.tsv"<<endl;
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	plot.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == 'C' || key == 'c') {
		string dataSetName = "cars.tsv";
		string path = ofToDataPath(dataSetName);
		reader.read_data(path);
        plot.setup(500, 500, reader.get_columns(), 60, 40, 200);
	}
	if (key == 'N' || key == 'n') {
		string dataSetName = "nutrients.tsv";
		string path = ofToDataPath(dataSetName);
		reader.read_data(path);
        plot.setup(500, 500, reader.get_columns(), 60, 40, 200);
	}
	if (key == 'M' || key == 'm') {
		string dataSetName = "cameras.tsv";
		string path = ofToDataPath(dataSetName);
		reader.read_data(path);
        plot.setup(500, 500, reader.get_columns(), 60, 40, 200);
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
