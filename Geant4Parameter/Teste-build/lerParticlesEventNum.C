#include <iostream>
#include <fstream>

#include <TApplication.h>
#include <TTree.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TH1F.h>

//cria struct de nome PARTICLE para conter os dados do elétron inicial da ionização
typedef struct {
    Double_t energy;
    Float_t x, y, z;
    Float_t dx, dy, dz;
    Int_t eventNumber;
} PARTICLE;

using namespace std;

void lerParticlesEventNum(){ //"/home/christian/Documents/Simulação/Geant4Parte2/Teste-build/particles.root"
	TFile *inputFile = TFile::Open("particles.root");
	TTree* theTree = (TTree*) inputFile->Get("Particles");
	TBranch* electron_branch = theTree->GetBranch("electron_branch");
	PARTICLE electron;
	electron_branch->SetAddress(&electron);

	int entries = theTree->GetEntries();
	double x0, y0, z0, xi, yi, zi, xa, ya, ea, x1, y1, z1, ti, t1, ei, e1;
	int status, np, eventN;
	double maior = 0.;
	double menor = 500.;
	cout << "Há " << entries << " eventos.\n";
	for (int i = 0; i < entries; i++) {
		theTree->GetEntry(i);

		xi=electron.x;
		yi=electron.y;
		zi=electron.z;
		ei = electron.energy;
		eventN = electron.eventNumber;
		cout << "Ionização número " << i << endl;
		cout << "Posição: ";
		cout << "(" << xi << "," << yi << "," << zi << ")cm\t";
		cout << "Energia: " << ei << "eV\t"
		     << "Evento número: " << eventN << "\n"
		     << endl;
		if(maior < yi){ maior = yi; }
		if(menor > yi){ menor = yi; }
	}
	cout << endl << "maiorY = " << maior << endl;
	cout << "menorY = " << menor << endl;
}



int main(){
	lerParticlesEventNum();
}
