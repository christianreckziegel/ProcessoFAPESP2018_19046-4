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
} PARTICLE;

using namespace std;

void lerParticles(){ //"/home/christian/Documents/Simulação/Geant4Parte2/Teste-build/particles.root"
	TFile *inputFile = TFile::Open("particles.root");
	TTree* theTree = (TTree*) inputFile->Get("Particles");
	TBranch* electron_branch = theTree->GetBranch("electron_branch");
	PARTICLE electron;
	electron_branch->SetAddress(&electron);

	int n_events = theTree->GetEntries();
	double x0, y0, z0, xi, yi, zi, xa, ya, ea, x1, y1, z1, ti, t1, ei, e1;
	int status, np;
	cout << "Há " << n_events << " enventos.\n";
	for (int i = 0; i < n_events; i++) {
		theTree->GetEntry(i);

		xi=electron.x;
		yi=electron.y;
		zi=electron.z;
		ei = electron.energy;
		cout << "Ionização número " << i << endl;
		cout << "Posição: ";
		cout << "(" << xi << "," << yi << "," << zi << ")cm\t";
		cout << "Energia: " << ei << "eV\n";
		cout << endl;
		
	}
	
	
}



int main(){
	lerParticles();
}
