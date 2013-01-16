#ifndef LIGHTMANAGER_H_INCLUDED
#define LIGHTMANAGER_H_INCLUDED

class LightManager {
	public:
		LightManager();
		~LightManager();
		
		int id;
		bool light1;
		bool light2;
		bool light3;
		bool light4;
		bool material;
		bool metal;
		bool braking;
		bool headlight;

		void InitializeLights();
        void selectSpotlight();
};


#endif
