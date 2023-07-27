#ifndef skGADNickelGenerator_hh
#define skGADNickelGenerator_hh 1

#include "G4ReactionProductVector.hh"
#include <vector>

using namespace std;

class skGADNickelGenerator
{
	private:
		skGADNickelGenerator();
		~skGADNickelGenerator();
		void Initialize ();

		int mode;
		int ngam;
		double egam[4];

		double ratio[37];
		const static float game[37][4];
		const static int multi[37];
		const static float game4[1][4];
		const static int multi4[1];
		const static float bra[37];


	public:
		static skGADNickelGenerator* getInstance(void){
			static skGADNickelGenerator singleton;
			return &singleton;
		}
		G4ReactionProductVector * GetGammas ();
		G4ThreeVector           * GetPos ();

};
#endif
