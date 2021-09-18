/*Esta macro lê dados do arquivo result.root proveniente 
da simulação no Garfield++. 
Imprime as posições e respectivos erros, ganho e nº do 
evento para cada entrada do arquivo particles.root simulada.*/

#include <iostream>
#include <fstream>

#include <TApplication.h>
#include <TTree.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TH1F.h>
#include <TF1.h>
#include <TGraphErrors.h>


using namespace std;

void lerResultEventNum(){
	TFile *inputFile = TFile::Open("ArCO2_70-30_Result_2.0GeV_0.5T_15cm_final.root");
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
	
	int n_events = theTree->GetEntries();
	int status, np, nEnd;
	cout << "Há " << n_events << " entries(s).\n";
	double maior = 0.;
	
	//creating arrays for the plot
	double x[n_events];
	double y[n_events];
	double ex[n_events];
	double ey[n_events];
	
	for (int i = 0; i < n_events; i++) {
		theTree->GetEntry(i);
		
		cout << "Avalanche número " << i << endl;
		cout << "Posição inicial: ";
		cout << "(" << xi << "," << yi << ")cm\n";
		cout << "Média da posição final: ";
		cout << "(" << xf << "+-" << incxf << "," << yf << "+-" << incyf << ")cm\n";
		cout << "Ganho/Número de elétrons finais: " << nEndPoint << "\n";
		cout << "Evento número: " << eventN << "\n";
		cout << endl;
		if(maior < yi){ maior = yi; }
		
		// filling arrays
		x[i] = xf;
		y[i] = yf;
		ex[i] = incxf;
		ey[i] = incyf;
		
	}
	cout << endl << "maior = " << maior << endl;
	/*
	TCanvas* c1 = new TCanvas("canvas1","Muon transversal trajectory");
	c1->SetGrid();
	TGraphErrors* gr1 = new TGraphErrors(n_events,x,y,ex,ey);
	gr1->SetTitle("Muon trajectory in the x-y plane");
	gr1->GetXaxis()->SetTitle("x(cm)");
	gr1->GetYaxis()->SetTitle("y(cm)");
	gr1->SetMarkerColor(4);//blue
	gr1->SetMarkerStyle(20);
	//gr1->SetMarkerStyle(21); //square
	//gr1->GetXaxis()->SetLimits(0.,15.);
	//gr1->GetYaxis()->SetLimits(0.,15.);
	gr1->Draw();
	c1->Update();
	*/
	
	/*
	TCanvas* c1 = new TCanvas("canvas1","Muon transversal trajectory");
	c1->SetGrid();
	TGraph* gr2 = new TGraph(n_events,x,y);
	gr2->SetTitle("Muon trajectory in the x-y plane");
	gr2->GetXaxis()->SetTitle("x(cm)");
	gr2->GetYaxis()->SetTitle("y(cm)");
	gr2->SetMarkerColor(4);//blue
	gr2->SetMarkerStyle(20); //circunference
	//[0] = y centro = b
	//[1] = R
	//[2] = x centro = a
	TF1* fitFunc = new TF1("fit","[0] - sqrt(([1]^2) - ((x - [2])^2))",0,200);
	fitFunc->SetParameters(14.8,160.,160.5); //original
	//fitFunc->SetParameters(14.8,150.,160.5);
	fitFunc->SetParNames ("Ycenter","Radius","Xcenter");
	gr2->Fit("fit","W");
	gr2->Draw();
	//fitFunc->Draw("SAME");
	
	//c1->Update();
	c1->SaveAs("ResultPlot.png");
	*/
}



int main(){
	lerResultEventNum();
	return 0;
}
