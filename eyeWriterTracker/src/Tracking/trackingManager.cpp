#include "trackingManager.h"
//--------------------------------------------------------------
trackingManager::trackingManager(){
	
}

//--------------------------------------------------------------
trackingManager::~trackingManager(){
	
}

//--------------------------------------------------------------
void trackingManager::setup(){
	
	IM.setup();
	
	setupGui();
	
	//--- set up tracking
	printf("VideoWidth, VideoHeight = %d, %d \n", IM.width, IM.height);
	tracker.setup(IM.width, IM.height);
	
	minBlob		= 10*10;
	maxBlob		= 100*100;
	threshold		= 21;
	
	bFoundEye		= false;
	eyePoint.set(0,0,0);

	scanX = 0;
	scanY = 0;
	
	
}

//--------------------------------------------------------------
void trackingManager::update(){
	//--- update video/camera input
	IM.update();
	
	//--- eye tracking (on new frames)	
	if (IM.bIsFrameNew){									// check new frame.
		trackEyes();
	}
	
	glintPupilVector = tracker.getVectorGlintToPupil(true);

	
	// to make trail
	currentdrawPoint.x = currentdrawPoint.x * 0.80 + glintPupilVector.x * 0.20;
	currentdrawPoint.y = currentdrawPoint.y * 0.80 + glintPupilVector.y * 0.20;
	
	ofPoint	tempPoint(currentdrawPoint.x * 20, currentdrawPoint.y * 20);
	trail.push_back(tempPoint);
	if (trail.size() > 100) trail.erase(trail.begin());
	
	//--- gui
	panel.update();
	updateGui();
	
}

//--------------------------------------------------------------
ofPoint	trackingManager::getEyePoint(){
	return eyePoint;
}

//--------------------------------------------------------------
bool trackingManager::bGotAnEyeThisFrame(){
	return bFoundEye;
}

//--------------------------------------------------------------
void trackingManager::trackEyes(){
	tracker.update(IM.grayImage);
	
	bFoundEye	= tracker.bFoundOne;						
	eyePoint	= tracker.getVectorGlintToPupil(false);					// TODO: CHECK here.

}

//--------------------------------------------------------------
void trackingManager::videoSettings(){
	
	// TODO: fix this!! [zach]
	//if( !bUseVideoFiles ) ((ofVideoGrabber *)videoSource)->videoSettings();
	
}

//--------------------------------------------------------------
void trackingManager::draw(){
	
	// Draw Input
//	IM.grayOddImage.draw(0, 0, IM.width/4, IM.height/4);
//	IM.grayEvenImage.draw(IM.width/4,0, IM.width/4, IM.height/4);
	drawInput(0, 0, IM.width/4, IM.height/4, IM.width/4, 0, IM.width/4, IM.height/4);	
	
	//test draw input
//	IM.grayOddImage.draw(IM.width/2 + tracker.pFinder.imgBeforeThreshold.width + 40, 0, IM.width/4, IM.height/4);
//	IM.grayEvenImage.draw(IM.width/2 + IM.width/4 + tracker.pFinder.imgBeforeThreshold.width + 40,0, IM.width/4, IM.height/4);
	
	
	// Draw EyeFinder
	drawEyeFinder(0, IM.height/4+30, IM.width/2, IM.height/2);
//	tracker.diffImg.draw(0, IM.height/4+30, IM.width/2, IM.height/2);
	
	// Draw Pupil Finder	
	drawEyeImageBeforePupilThreshold(IM.width/2 + 20, 0, tracker.pFinder.imgBeforeThreshold.width, tracker.pFinder.imgBeforeThreshold.height);
	drawPupilFinder(IM.width/2 + 20, tracker.pFinder.imgBeforeThreshold.height + 30);

	// Draw Glint Finder
	drawGlintFinder(IM.width/2 + 20, tracker.pFinder.imgBeforeThreshold.height*2 + 60);
	
	// Draw BrightEye, DarkEye
	drawBrightDarkPupil(0, IM.height/4 + IM.height/2 + 60, tracker.targetWidth, IM.height/4 + IM.height/2 + 60);
	
	// Draw auto threshold Line for bright/dark eye.
	drawAutoThresholdBrightnessGraph(0, IM.height/4 + IM.height/2 + 60);
	
	// Draw brightness graph
	unsigned char * pupilpixels = tracker.pFinder.imgBeforeThreshold.getPixels();
	drawBrighnessScanGraph(pupilpixels, IM.width/2 + tracker.pFinder.imgBeforeThreshold.width + 40, 0, scanY, false, "BrightnessScan/Horizontal");
	drawBrighnessScanGraph(pupilpixels, IM.width/2 + tracker.pFinder.imgBeforeThreshold.width + 40, 255 + 30, scanX, true, "BrightnessScan/Vertical");
	
	ofSetColor(0, 0, 0);
	
	
	// Draw trail circles.
	drawCircles();
	
	panel.draw();
	
}
//--------------------------------------------------------------
void trackingManager::mouseDragged(int x, int y, int button){
	
	panel.mouseDragged(x, y, button);
	
}
//--------------------------------------------------------------
void trackingManager::mousePressed(int x, int y, int button){
	
	panel.mousePressed(x, y, button);
	trail.clear();
	
}
//--------------------------------------------------------------
void trackingManager::mouseReleased(){
	
	panel.mouseReleased();
	
}