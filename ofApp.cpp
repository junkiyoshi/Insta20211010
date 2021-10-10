#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openFrameworks");

	ofBackground(239);
	ofSetRectMode(ofRectMode::OF_RECTMODE_CENTER);
	ofEnableDepthTest();
	ofDisableArbTex();

	this->cap_size = cv::Size(180, 180);
	this->image.allocate(this->cap_size.width, this->cap_size.height, OF_IMAGE_COLOR);
	this->frame = cv::Mat(cv::Size(this->image.getWidth(), this->image.getHeight()), CV_MAKETYPE(CV_8UC3, this->image.getPixels().getNumChannels()), this->image.getPixels().getData(), 0);

	this->number_of_frames = 0;
	vector<string> file_path_list = {
		"D:\\tmp\\1.mp4",
		"D:\\tmp\\2.mp4",
		"D:\\tmp\\3.mp4",
		"D:\\tmp\\4.mp4",
		"D:\\tmp\\5.mp4",
		"D:\\tmp\\6.mp4",
		"D:\\tmp\\7.mp4",
		"D:\\tmp\\8.mp4",
	};

	for (auto& file_path : file_path_list) {

		this->cap.open(file_path);
		int frame_count = this->cap.get(cv::CAP_PROP_FRAME_COUNT);
		for (int i = 0; i < frame_count; i++) {

			cv::Mat src, tmp;
			this->cap >> src;
			if (src.empty()) {

				continue;
			}

			cv::resize(src, tmp, this->cap_size);
			cv::cvtColor(tmp, tmp, cv::COLOR_BGR2RGB);

			this->frame_list.push_back(tmp);
		}

		this->number_of_frames += frame_count;
	}
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();

	for (int base_x = -300; base_x <= 300; base_x += 600) {

		for (int base_y = -300; base_y <= 300; base_y += 600) {

			ofPushMatrix();
			ofTranslate(base_x, base_y, 0);
			ofRotateZ(ofRandom(360) + ofGetFrameNum() * 0.3);
			ofRotateY(ofRandom(360) + ofGetFrameNum() * 0.3);
			ofRotateX(ofRandom(360) + ofGetFrameNum() * 0.3);

			for (int k = 0; k < 6; k++) {

				if (k > 0 && k < 4) { ofRotateY(90); }
				if (k == 4) { ofRotateX(90); }
				if (k == 5) { ofRotateX(180); }

				for (int x = -90; x <= 90; x += 180) {

					for (int y = -90; y <= 90; y += 180) {

						int frame_index = (int(ofRandom(this->number_of_frames)) + ofGetFrameNum()) % this->number_of_frames;
						this->frame_list[frame_index].copyTo(this->frame);
						this->image.update();

						ofSetColor(255);
						ofFill();
						this->image.getTexture().bind();
						ofDrawPlane(glm::vec3(x, y, 180), 180, 180);
						this->image.unbind();

						ofSetColor(139);
						ofNoFill();
						ofDrawRectangle(glm::vec3(x, y, 180), 180, 180);
					}
				}
			}

			ofPopMatrix();
		}
	}

	this->cam.end();
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}