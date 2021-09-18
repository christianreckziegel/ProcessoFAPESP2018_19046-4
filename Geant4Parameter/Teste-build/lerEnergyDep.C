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

void lerEnergyDep(){ //"/home/christian/Documents/Simulação/Geant4Parte2/Teste-build/particles.root"
	
	
	
	
	//Creating canvas and histogram for ionization energy
	TCanvas *cEnergyStep = new TCanvas("cE","Canvas Energy");
	int nbinsE = 1000;//1000 (mil) bins para 10000 (dez mil eV)
	TH1D* hEnergyStep = new TH1D("Energy", "Ionization energy for 1000 events on pure Ar", 
				nbinsE, 
				0.0, 10000.0);
	hEnergyStep->GetXaxis()->SetTitle("E(eV)");
	hEnergyStep->GetYaxis()->SetTitle("Entries");
	//creating canvas and histogram for energy vs. position
	TCanvas *cEnPos = new TCanvas("cRadPoint","Canvas 2D radiusXpoints");
	int nbinsEnPos = 240;
	TH2D* hEnPos = new TH2D("EnergyPosition", "Ionization energy per y position for 1000 events on pure Ar", 
				nbinsEnPos, -15., 15., //position in cm
				nbinsE, 0., 10000.0); //energy in eV
	hEnPos->GetXaxis()->SetTitle("Y(cm)");
	hEnPos->GetYaxis()->SetTitle("E(eV)");
	hEnPos->GetZaxis()->SetTitle("Entries");
	
	
	//Collecting ionization energy data
	TFile *inputFile1 = TFile::Open("energyDepStep.root");
	TTree* theTree1 = (TTree*) inputFile1->Get("Ionization");
	TBranch* energy_branch = theTree1->GetBranch("stepEnergy_branch");
	double energyStep;
	energy_branch->SetAddress(&energyStep);
	double estimEnergy;
	int n_events = theTree1->GetEntries();
	cout << "Há " << n_events << " entries.\n";
	double ionEnergy[n_events];
	for (int i = 0; i < n_events; i++) {
		theTree1->GetEntry(i);
		estimEnergy = energyStep;
		ionEnergy[i] = estimEnergy;
		cout << "Ionização número " << i << " com " << ionEnergy[i] << "eV\n";
		
	}
	inputFile1->Close();
	
	//Collecting position data
	TFile *inputFile2 = TFile::Open("particles.root");
	TTree* theTree2 = (TTree*) inputFile2->Get("Particles");
	TBranch* electron_branch = theTree2->GetBranch("electron_branch");
	PARTICLE myElectron;
	electron_branch->SetAddress(&myElectron);
	double x[n_events];
	double y[n_events];
	double en[n_events];
	for (int i = 0; i < n_events; i++) {
		theTree2->GetEntry(i);
		x[i] = myElectron.x;
		y[i] = myElectron.y;
		en[i] = myElectron.energy;
		cout << "myElectron.y = " << y[i] << "\n";
	}
	inputFile2->Close();
	
	//Filling histograms
	for(int i = 0; i < n_events; i++){
		hEnergyStep->Fill(ionEnergy[i]);
		hEnPos->Fill(y[i],ionEnergy[i]);
		cout << "E(ionization)=" << ionEnergy[i] << "\t E(kinetic)=" << en[i] << endl;
	}
	
	//Drawing and saving histograms 
	cEnergyStep->cd();
	hEnergyStep->Draw();
	cEnergyStep->SaveAs("HistEnergyIonization.png","png");
	
	cEnPos->cd();
	hEnPos->SetStats(false);
	gStyle->SetPalette(55,0);
	hEnPos->Draw("colz");
	//hEnPos->Draw();
	cEnPos->SaveAs("HistEnPosIonization.png","png");
	
}



int main(){
	lerEnergyDep();
	return 0;
}
