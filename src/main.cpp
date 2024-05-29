#include "Emitter.h"


int main(int argc, char *argv[], char *envp[])
{
	setlocale(LC_ALL, "ru");

	al::AudioSystem aSys;
	aSys.setDevice();
	aSys.setContext();
	aSys.setListener();

	
	audio::Emitter sound1;
	//sound1.generateSamples();
	sound1.setBuffer();
	sound1.play();
	

	aSys.exit();

	return 0;
}