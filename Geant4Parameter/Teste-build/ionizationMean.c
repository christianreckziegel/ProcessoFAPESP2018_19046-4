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


void ionizationMean(){
	//initial definitions
	TString proporc = "70-30";
	TString gas = "ArCF_{4}";
	TString energyString = "ionization"+gas+"_"+proporc;
	double ionizationNumber;
	//criando histograma com (50-0)/300bins = (35-0)/210bins = (40-0)/240bins = (30-0)/180bins
	TH1F* meuHist = new TH1F("Histograma" , "Frequ#hat{e}ncia do n#acute{u}mero de ionizac#tilde{o}es", 50,0.,50.);
	meuHist->GetXaxis()->SetTitle("n#circ de ionizac#tilde{o}es");
	meuHist->GetYaxis()->SetTitle("n#circ de simulac#tilde{o}es");
	//criando um canvas para o histograma
	TCanvas* mycanvas = new TCanvas();
	mycanvas->SetGrid();
	//arquivo dos vetores e energia vetores
	ifstream theFile;
	theFile.open(energyString+".txt");
	cout << "Arquivo aberto, iniciando leitura..." << endl;
	//leitura do arquivo
	while(!theFile.eof()){
		theFile >> ionizationNumber;
		cout << "Ionization number = " << ionizationNumber << std::endl;
		meuHist->Fill(ionizationNumber);
	}
	cout << "Arquivo lido. Desenhando histograma no canvas...\n";
	meuHist->SetTitle("Frequ#hat{e}ncia de ionizac#tilde{a}o "+gas+" "+proporc);
	meuHist->GetMean();
	meuHist->GetMeanError();
	
	// configuring landau fit
	TF1 *f1 = new TF1("m1","gaus");
	meuHist->Fit(f1);
	Double_t par[3]; //array to store the fit parameters
	//par[0]=constant
	//par[1]=most probable value
	//par[2]=sigma
	f1->GetParameters(&par[0]);
	//cout << "The most probable value is " << par[1] << "+/-" << par[2] << "keV/cm\n";
	mycanvas->Update();
	//configuring statistics box
	gStyle->SetOptStat("e");
	gStyle->SetOptFit(0011);
	gStyle->SetStatY(0.9);// Set y-position (fraction of pad size)
	gStyle->SetStatX(0.9);// Set x-position (fraction of pad size)
	gStyle->SetStatW(0.4);// Set width of stat-box (fraction of pad size)
	gStyle->SetStatH(0.5);// Set height of stat-box (fraction of pad size)
	mycanvas->Update();
	
	meuHist->Draw();
	
	cout << "The mean number of ionization was " << meuHist->GetMean() << "+/-" << meuHist->GetMeanError() << "\n";
	mycanvas->SaveAs(energyString+"_gauss.png","png");
	//meuHist->Print("energyLossArCO2_70-30.png");
	
	//close file
	theFile.close();
	
	
	
	
}
int main(){
  ionizationMean();
  return 0;
}
