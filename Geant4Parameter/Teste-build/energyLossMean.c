#include "TH1F.h"
#include "TVector3.h"
#include "TCanvas.h"
#include <iostream>
#include <string.h>
#include <string>
#include <stdio.h>
#include <sys/stat.h>
#include <ctime>
#include <cmath>
#include <sstream>
#include <cstdio>
#include <Riostream.h>
#include <TString.h>
using namespace std;

/*
std::string to_string(double i)
{
    std::stringstream ss;
    ss << i;
    return ss.str();
}
*/


void energyLossMean(){
	//initial definitions
	//std::string proporc = "70-30";
	//std::string energyString = "energyLossArCO2_"+proporc;
	TString proporc = "90-10";
	TString energyString = "energyLossXeCO2_"+proporc;
	double iEnergy; //MeV
	double fEnergy;
	double individualLoss; //deltaE/deltax for each simulation
	double thgemSize = 20.; //size in cm
	//criando histograma
	TH1F* meuHist = new TH1F("h" , "Frequ#hat{e}ncia da perda de energia", 300,0.,50.);
	meuHist->GetXaxis()->SetTitle("#frac{dE}{dx} (keV/cm)");
	meuHist->GetYaxis()->SetTitle("n^{#circ} de simulac#tilde{o}es");
	//criando um canvas para o histograma
	TCanvas* mycanvas = new TCanvas();
	mycanvas->SetGrid();
	//arquivo dos vetores e energia vetores
	ifstream theFile;
	theFile.open(energyString+".txt");
	cout << "Arquivo aberto, iniciando leitura..." << endl;
	//leitura do arquivo
	while(!theFile.eof()){
		theFile >> iEnergy >> fEnergy;
		individualLoss = (iEnergy-fEnergy)*1000./thgemSize; //em keV/cm
		cout << "iEnergy=" << iEnergy << "\tfEnergy=" << fEnergy << "\tLoss=" << individualLoss << std::endl;
		meuHist->Fill(individualLoss);
	}
	cout << "Arquivo lido. Desenhando histograma no canvas...\n";
	meuHist->SetTitle("Frequ#hat{e}ncia da perda de energia XeCO_{2}-"+proporc);
	meuHist->GetMean();
	meuHist->GetMeanError();
	//gStyle->SetOptStat("eMR"); //statistics box
	meuHist->Fit("landau");
	meuHist->Draw();
	
	cout << "The mean loss energy was dE/dx " << meuHist->GetMean() << "+/-" << meuHist->GetMeanError() << "\n";
	mycanvas->SaveAs(energyString+".png","png");
	//meuHist->Print("energyLossArCO2_70-30.png");
	//close file
	theFile.close();
	
	
	
	
}
int main(){
  energyLossMean();
  return 0;
}
