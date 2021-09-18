/*Esta macro junta os results_i.root em um resultFinal.root só,
lê o resultFinal.root selecionando as entries que pertencem ao mesmo evento,
realiza o ajuste de curva obtendo o raio da circunferência para cada caso,
e preenche um histograma com o raio para cada evento.*/

#include <iostream>
#include <fstream>
#include <sstream> 
#include <vector> 

#include <TApplication.h>
#include <TTree.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TH1F.h>
#include <TH1D.h>
#include <TF1.h>
#include "TGraph.h"
#include <TGraphErrors.h>
#include <TString.h>
#include "Fit/Fitter.h"
#include "TMath.h"

using namespace std;

//cria struct de nome PARTICLE para conter os dados do elétron inicial da ionização
typedef struct {
    Double_t energy;
    Float_t x, y, z;
    Float_t dx, dy, dz;
    Int_t eventNumber;
} PARTICLE;

void resultAnalysis1(){
	/* 1 - merging ROOT files*/
	//https://root-forum.cern.ch/t/how-to-use-tchain-to-combine-many-root-files/25164
	TChain* chain = new TChain ("Results"); //"Results" is one of the trees inside the file
	//XeCO2_90-10_Result_5.0GeV_1mT_10cm_358.root
	TString sEnergy = "5.0";
	TString strBase = "XeCO2_90-10_Result_"+sEnergy+"GeV_1mT_10cm_";
	for(int i = 0; i <= 358; i++){
		TString resultPart = strBase + i +".root";
		chain->Add(resultPart);
		cout << "File " << i << "chained.\n";
	}
	chain->Merge(strBase + "final.root");
	
	/* 2 - select same event, fit curve, fill histograms*/
	TFile *inputFile = TFile::Open(strBase + "final.root");
	TTree* theTree = (TTree*) inputFile->Get("Results");
	Double_t xi, yi, xf, yf, incxf, incyf;
	Int_t nEndPoint, eventN;
	theTree->SetBranchAddress("Avalanche_xi",&xi);
	theTree->SetBranchAddress("Avalanche_yi",&yi);
	theTree->SetBranchAddress("Avalanche_x",&xf);
	theTree->SetBranchAddress("Avalanche_y",&yf);
	theTree->SetBranchAddress("Avalanche_incx",&incxf);
	theTree->SetBranchAddress("Avalanche_incy",&incyf);
	theTree->SetBranchAddress("Number_of_Electrons_endpoint",&nEndPoint);
	theTree->SetBranchAddress("Event_number_in_Geant4",&eventN);
	
	int n_entries = theTree->GetEntries();
	int status, np, nEnd;
	cout << "Há " << n_entries << " entries(s).\n";
	//creating canvas and histogram for radius obtained from the fitting
	TCanvas *cRadius = new TCanvas("cR","Canvas Radius");
	int nbinsR = 200;
	TH1D* hRadius = new TH1D("Radius", "Estimated radius for a "+sEnergy+"GeV muon under 0.5T ArCO_{2} (70-30)", 
				nbinsR, 
				0.0, 500.0); //from 0 to 200cm
	hRadius->GetXaxis()->SetTitle("R(cm)");
	hRadius->GetYaxis()->SetTitle("Entries");
	//creating canvas and histogram for energy obtained from calculation after each radius obtained from the fitting
	TCanvas *cEnergy = new TCanvas("cE","Canvas Energy");
	int nbinsE = 100;
	TH1D* hEnergy = new TH1D("Energy", "Estimated energy for a "+sEnergy+"GeV muon under 0.5T ArCO_{2} (70-30)", 
				nbinsE, 
				0.0, 6.0); //from o to 10GeV
	hEnergy->GetXaxis()->SetTitle("E(GeV)");
	hEnergy->GetYaxis()->SetTitle("Entries");
	//creating arrays for saving data
	double x[n_entries];
	double y[n_entries];
	double ex[n_entries];
	double ey[n_entries];
	int endP[n_entries];
	int eventNumb[n_entries];
	// collecting the first entry
	theTree->GetEntry(0);
	x[0] = xf;
	y[0] = yf;
	ex[0] = incxf;
	ey[0] = incyf;
	endP[0] = nEndPoint;
	eventNumb[0] = eventN;
	//initialization of position and error vectors
	vector<double> vX;
	vX.push_back(xf);
	vector<double> vY;
	vY.push_back(yf);
	vector<double> vIncX;
	vIncX.push_back(incxf);
	vector<double> vIncY;
	vIncY.push_back(incyf);
	//looping through the entries
	for(int j = 1; j < n_entries; j++){
		//collecting j-th entry
		theTree->GetEntry(j);
		x[j] = xf;
		y[j] = yf;
		ex[j] = incxf;
		ey[j] = incyf;
		endP[j] = nEndPoint;
		eventNumb[j] = eventN;
		if(eventNumb[j] == eventNumb[j-1]){
			vX.push_back(x[j]);
			vY.push_back(y[j]);
			vIncX.push_back(ex[j]);
			vIncY.push_back(ey[j]);
		}
		else{
			/* 3 - fit curve to the set of points on the vectors from 0 to (vX->size()-1) */
			
			//Bolas verdes são os pontos de avalanche
			TGraphErrors *gr = new TGraphErrors(vX.size(),&(vX[0]),&(vY[0]),&(vIncX[0]),&(vIncY[0]));
			gr->SetTitle("Muon trajectory in the x-y plane");
			gr->GetXaxis()->SetTitle("x(cm)");
			gr->GetYaxis()->SetTitle("y(cm)");
			gr->SetMarkerStyle(kFullDotLarge);
			gr->SetMarkerColor(8); //verde
			
			//Minimisation function computing the sum of squares of residuals looping at the graph points
			//Considera incerteza dos pontos, mas se a incerteza é mto pequena, 
			//a incerteza da energia diminui muito também
			auto chi2Function = [&](const Double_t *par) {
				Int_t np = gr->GetN();
				Double_t f = 0;
				Double_t *vXA = gr->GetX();
				Double_t *vYA = gr->GetY();
				Double_t *vIncXA = gr->GetEX();
				Double_t *vIncYA = gr->GetEY();
				for (Int_t i=0;i<np;i++) {
					Double_t u = vXA[i] - par[0]; //par[0] = x center
					Double_t v = vYA[i] - par[1]; //par[1] = y center
					Double_t fi = std::sqrt(u*u+v*v);

					//incerteza padrão de x e de y
					//Double_t incPadx2 = ex[i]*ex[i] + 0.025*0.025; 
					//erro estatístico(RMS) + erro sistemático
					//Double_t incPady2 = ey[i]*ey[i] + 0.025*0.025; 
					//erro estatístico(RMS) + erro sistemático
					//Double_t incFi2 = (u/fi)*(u/fi)*incPadx2 + (v/fi)*(v/fi)*incPady2
					Double_t incFi2 = (u*vIncXA[i]/fi)*(u*vIncXA[i]/fi) + (v*vIncYA[i]/fi)*(v*vIncYA[i]/fi);
					Double_t dr = par[2] - fi; //par[2] = radious
					f += dr*dr/incFi2;
				}
				
				return f;
			};
			// wrap chi2 function in a function object for the fit
			// 3 is the number of fit parameters (size of array par)
			ROOT::Math::Functor fcn(chi2Function,3);
			ROOT::Fit::Fitter  fitter;
			//starting guess
			double pStart[3] = {15,14,15};
			fitter.SetFCN(fcn, pStart);
			fitter.Config().ParSettings(0).SetName("x0");
			fitter.Config().ParSettings(1).SetName("y0");
			fitter.Config().ParSettings(2).SetName("R");
			// do the fit 
			bool ok = fitter.FitFCN();
			if (!ok) {
			Error("line3Dfit","Line3D Fit failed");
			}   
			const ROOT::Fit::FitResult & result = fitter.Result();
			//result.Print(std::cout);
			
			
			//propagading Radius to get Energy
			double B = 0.5;//em tesla
			double estimRadius = result.Parameter(2); //em cm
			double R = result.Parameter(2)*0.01;//em metros
			double pc = 1.6*0.01*3.*6.242*B*R;
			double mc2 = 0.105;//energia de repouso do muon -> necessário incluir incerteza dessa massa
			double estimEnergy = sqrt(pc*pc + mc2*mc2);
			double incR = result.ParError(2)*0.01;//em metros
			double incE = pc*B*1.6*0.01*3.*6.242*incR/estimEnergy;
			//filling histograms
			hRadius->Fill(estimRadius);
			hEnergy->Fill(estimEnergy);
			//clear content of vectors for the next event
			vX.clear();
			vY.clear();
			vIncX.clear();
			vIncY.clear();
			//add current value to the new vectors
			vX.push_back(x[j]);
			vY.push_back(y[j]);
			vIncX.push_back(ex[j]);
			vIncY.push_back(ey[j]);
		}
		
	}
	
	
	
	cRadius->cd();
	hRadius->Draw();
	cRadius->SaveAs(strBase+"HistRadius.png","png");
	
	cEnergy->cd();
	hEnergy->Draw();
	cEnergy->SaveAs(strBase+"HistEnergy.png","png");
	
	//fitting gaussian distribuition for radius histogram
	cRadius->cd();
	TF1 *fR = new TF1("fitR","gaus");
	hRadius->Fit(fR);
	Double_t parR[3]; //array to store the fit parameters
	//par[0]=constant
	//par[1]=most probable value
	//par[2]=sigma
	fR->GetParameters(&parR[0]);
	cRadius->Update();
	//configuring statistics box
	gStyle->SetOptStat("e");
	gStyle->SetOptFit(0011);
	/*gStyle->SetStatY(0.9);// Set y-position (fraction of pad size)
	gStyle->SetStatX(0.9);// Set x-position (fraction of pad size)
	gStyle->SetStatW(0.4);// Set width of stat-box (fraction of pad size)
	gStyle->SetStatH(0.5);// Set height of stat-box (fraction of pad size)*/
	cRadius->Update();
	hRadius->Draw();
	cRadius->SaveAs(strBase+"HistRadiusFitted.png","png");
	
	
	
	//fitting gaussian distribuition for energy histogram
	cEnergy->cd();
	TF1 *fE = new TF1("fitE","gaus");
	hEnergy->Fit(fR);
	Double_t parE[3]; //array to store the fit parameters
	//par[0]=constant
	//par[1]=most probable value
	//par[2]=sigma
	fE->GetParameters(&parE[0]);
	cEnergy->Update();
	//configuring statistics box
	gStyle->SetOptStat("e");
	gStyle->SetOptFit(0011);
	/*gStyle->SetStatY(0.9);// Set y-position (fraction of pad size)
	gStyle->SetStatX(0.9);// Set x-position (fraction of pad size)
	gStyle->SetStatW(0.4);// Set width of stat-box (fraction of pad size)
	gStyle->SetStatH(0.5);// Set height of stat-box (fraction of pad size)*/
	cEnergy->Update();
	hEnergy->Draw();
	cEnergy->SaveAs(strBase+"HistEnergyFitted.png","png");
	
	//calculating energy using results from R histogram
	double B = 5.;
	double R = parR[1]*0.01;//em metros
	double pc = 1.6*0.01*3.*6.242*B*R;
	double mc2 = 0.105;//energia de repouso do muon -> necessário incluir incerteza dessa massa
	double estimEnergy = sqrt(pc*pc + mc2*mc2);
	double incR = parR[2]*0.01;//em metros
	double incE = pc*B*1.6*0.01*3.*6.242*incR/estimEnergy;
	cout << "Usando o histograma do raio R, obtemos um valor de: " << endl;
	cout << estimEnergy << "+/-" << incE << "GeV" << endl;
	
	inputFile->Close();
	
	
	
	
	
}

int main(){
	resultAnalysis1();
	return 0;
	
}
