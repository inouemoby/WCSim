///////////////////////////////////////////////////////////////////////////////
//                 Spectrum of radiative neutron capture by Nickel
//                                    version 1.0.0                                
//                                    (Nov.18.2013)                               


//This file contains the gammas spectrum produced in radiative capture of 
//neutrons by nickel
//This work is adapted from earlier work in geant3 for SK.


//Second version by Takatomi Yano, Feb,2013
//Some improvement for speed is applied.
//Ratio ([Branching ratio]/[Total Branching ratio]) is caliculated in advance.
//Binary tree scan is applied for branching ratio scan.

//First version by Takatomi Yano, Nov, 2013
///////////////////////////////////////////////////////////////////////////////


#include "skGADNickelGenerator.hh"
#include "Randomize.hh"
#include <vector>
#include "G4Gamma.hh"
#include "G4SystemOfUnits.hh"
using namespace std;

skGADNickelGenerator::skGADNickelGenerator () {
	this->Initialize();
}

skGADNickelGenerator::~skGADNickelGenerator () {}

const float skGADNickelGenerator::game[37][4] = {
	{		9.000, 0., 0., 0.},
	{		8.534, 0.466, 0.  ,0.  },
	{		8.122, 0.878 , 0., 0. },
	{		7.698, 1.302 ,0.  ,0.  },
	{		7.266, 0.856, 0.878, 0.},
	{		6.584, 1.950 ,0.466,0.   },
	{		6.106, 2.894 , 0., 0.},
	{		6.106, 2.555 ,0.340,0.   },
	{		5.974, 3.026 , 0., 0.},
	{		5.974, 2.686 ,0.340,0.   },
	{		5.817, 3.182 , 0., 0.},
	{		5.817, 2.842 ,0.340,0.   },
	{		5.621, 3.039 , 0., 0.},
	{		5.437, 3.563 ,0. , 0.   },
	{		5.437, 2.686, 0.878,0.},
	{		5.313, 3.347 ,0.340, 0. },
	{		5.313, 3.221, 0.466,0.},
	{		5.313, 3.687 ,0.878, 0. },
	{		5.270, 3.266, 0.466,0.},
	{		4.977, 3.555 ,0.466 ,0. },
	{		4.859, 4.141, 0.   ,0.},
	{		4.859, 3.675 ,0.466 ,0. },
	{		4.284, 4.716, 0. , 0. },
	{		4.031, 4.969 ,0.    ,0. },
	{		3.930, 4.192, 0.878,0.},
	{		7.820, 0.    ,0.    ,0. },
	{		7.537, 0.283, 0.   ,0.},
	{		6.720, 1.031 ,0.    ,0. },
	{		6.720, 0.816, 0.284,0.},
	{		6.635, 1.185 ,0.    ,0. },
	{		5.696, 2.124, 0.   ,0.},
	{		6.838, 0.    ,0.    ,0. },
	{		6.682, 0.156, 0.   ,0.},
	{		6.320, 0.362 ,0.156 ,0. },
	{		5.837, 0.846, 0.156,0.},
	{		5.837,0.483,0.362, 0.156},
	{		5.515, 1.168 , 0.156 , 0.}};

const float skGADNickelGenerator::game4[1][4] = {
  {		4.400, 0., 0., 0.}
};

const int skGADNickelGenerator::multi4[1] = {
  1
};

const int skGADNickelGenerator::multi[37] = {
	1, 2, 2, 2, 3, 3, 2, 3, 2, 3, 2, 3, 2, 2, 3, 3,
	3, 3, 3, 3, 2, 3, 2, 2, 3, 1, 2, 2, 3, 2, 2, 1,
	2, 3, 3, 4, 3};
/*
const float skGADNickelGenerator::bra[37] = {
	34.26, 16.64, 3.023, 0.832, 0.2015, 1.781, 0.2825,
	1.271, 0.3274, 0.2381, 0.5051, 1.900, 0.104, 0.1957,
	0.2723, 0.3085, 0.6041, 0.2185, 0.1885, 0.1690, 0.2307,
	0.7638, 0.2340, 0.2990, 0.1610, 8.220, 4.800, 0.1485,
	0.1054, 0.1455, 0.9465, 10.31, 0.1764, 0.4285, 0.3984,
	0.3114, 0.1012};
*/
const float skGADNickelGenerator::bra[37] = {
	34.26, 16.64, 3.023, 0.832, 0.2015, 1.781, 0.2825,
	1.271, 0.3274, 0.2381, 0.5051, 1.900, 0.104, 0.1957,
	0.2723, 0.3085, 0.6041, 0.2185, 0.1885, 0.1690, 0.2307,
	0.7638, 0.2340, 0.2990, 0.1610, 8.220, 4.800, 0.1485,
	0.1054, 0.1455, 0.9465, 10.31, 0.1764, 0.4285, 0.3984,
	0.3114, 0.1012};

void skGADNickelGenerator::Initialize (){

  // -- CALCULATE INTEGRAL RATIO
  double totbra=0.;
  for (int i=0; i<37; i++){
    totbra += bra[i];
  }
  for (int k=0; k<36; k++){
    double subbra=0.;
    for (int i=0; i<=k; i++){
      subbra += bra[i];
    }
    ratio[k]=subbra/totbra;
#ifdef DEBUG
    G4cout << "Nickel branching ratio table intializing " << ratio[k] << G4endl;
    G4cout << k << " " << ratio[k] << G4endl;
#endif
  }
  ratio[36]=1.;
  
}
/*************************************************************/
G4ReactionProductVector *  skGADNickelGenerator::GetGammas () 
/*************************************************************/
{
  /*
    C     
    C     ++ FUNCTION TO RETURN RANDOM GAMMA ENERGY FROM NI(N,GAMMA)NI
    C     M.NAKAHATA 07-NOV-86
    C     ++ modified by '77 NI(N,GAMMA)NI data
    C     T.Maruyama 12-Oct-'95
    C--INPUT
    C     NOTHING
    C--OUTPUT
    C     MODE    : DECAY MODE
    C     NGAM    : NUMBER OF GAMMA
    C     EGAM(I) : ENERGY OF "I"TH GAMMA  (MEV)
    
*/

  double ran = G4UniformRand();
  //double ran = float(rand())/float(RAND_MAX);
  int imode = 0;
  while(ran > ratio[imode]) imode++;
  if(imode < 0 || imode > 36) {
    printf("mode is out of range %d\n",imode);
    exit(0);
  }
  //imode = 0;
  mode = imode;
  ngam = multi[imode];
  for (int i=0; i<4; i++){
    egam[i] = game[imode][i];
  }
  
  G4ReactionProductVector * theGammas = new G4ReactionProductVector;

  //mori
  //ngam = 1;
  //egam[0] = 4.43;
  
  for(int i=0; i<ngam; i++)
    {
      G4ReactionProduct * theOne = new G4ReactionProduct;
      theOne->SetDefinition( G4Gamma::Gamma() );
      
      G4double costheta = 2.*G4UniformRand()-1;
      G4double theta    = acos(costheta);
      G4double phi      =  2.*3.1415927*G4UniformRand();
      G4double sinth    = sin(theta);
#if 0
      G4cout << "x:y:z:=" 
	     << egam[i]*sinth*cos(phi) << ":"
	     << egam[i]*sinth*sin(phi) << ":" 
	     <<egam[i]*sinth*sin(phi)  << G4endl;
#endif
      theOne->SetTotalEnergy( egam[i] * MeV );
      G4ThreeVector temp(egam[i]*sinth*cos(phi), 
			 egam[i]*sinth*sin(phi),
			 egam[i]*cos(theta) );
      theOne->SetMomentum( temp ) ;
      theGammas->push_back(theOne);
    }
  //G4cout << "NiGenerator : " << ngam << G4endl;
  return theGammas;
}

/************************************************/
G4ThreeVector *  skGADNickelGenerator::GetPos () 
/************************************************/
{
	G4double rndm1 = G4UniformRand();
	G4double rndm3 = G4UniformRand();
	G4double rndm4 = G4UniformRand();

	//while( 9.*sqrt(rndm1)<2.5 && 17.*rndm3>5.) {
	//rndm1 = G4UniformRand();
	//rndm3 = G4UniformRand();
	//}
	/*
	G4double posx = 90.*sqrt(rndm1)*cos(2.*3.1415927*rndm4);
	G4double posy = 90.*sqrt(rndm1)*sin(2.*3.1415927*rndm4);
	G4double posz = 170.*rndm3-85 ;
	*/
	G4double  r   = 100.-200.*rndm4;
	G4double theta=     3.141592*rndm1;
	G4double phi  = 2.* 3.141592*rndm3;	
	G4double posx = r * sin(theta)*cos(phi);
	G4double posy = r * sin(theta)*sin(phi);
	G4double posz = r * cos(theta);
	//G4double posx = 0.;
	//G4double posy = 0.;
	//G4double posz = 0.;
	G4ThreeVector * vec = new G4ThreeVector(posx *mm,posy *mm,posz *mm);
	return vec;
}
