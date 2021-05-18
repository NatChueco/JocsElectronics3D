#pragma once
class Stage {
public:
	//Stage();
	void render() {};
	void update(float dt) {};
};

class titleStage : public Stage {
public:
	//titleStage();
	void render() ;
	void update(float dt) ;
};

class tutorialStage : public Stage {
public:
	//tutorialStage();
	void render() ;
	void update(float dt) ;
};

class playStage : public Stage {
public:
	//playStage();
	virtual void render() ;
	void update(float dt) ;
};

class endStage : public Stage {
public:
	//endStage();
	void render();
	void update(float dt);
};

