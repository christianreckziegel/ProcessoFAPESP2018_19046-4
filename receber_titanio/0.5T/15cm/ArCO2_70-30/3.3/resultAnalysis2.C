/*Esta macro lê o resultFinal.root selecionando as entries que pertencem ao mesmo evento,
realiza o ajuste de curva obtendo o raio da circunferência para cada caso,
preenche um histograma com o raio para cada evento,
preenche um histograma 2D do raio em função do nº de pontos dos ajustes,
preenche um histograma 1D do valor de chi quadrado reduzido dos ajustes,
preenche um histograma 2D do chi quadrado reduzido em função do número de pontos.*/

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

//método para cálculo de quartil
Double_t getInterQuartileRange(TH1D *ht)
{
    Double_t quartis[] = {0.25,0.75};
    Double_t xquartis[2];
    //GetQuantiles(in maxArraySizeQ, out array q with quantiles, in position where to compute the quantiles in [0,1])
    ht->GetQuantiles(2,xquartis,quartis);
    return xquartis[1]-xquartis[0];
}

//cria struct de nome PARTICLE para conter os dados do elétron inicial da ionização
typedef struct {
    Double_t energy;
    Float_t x, y, z;
    Float_t dx, dy, dz;
    Int_t eventNumber;
} PARTICLE;

//função para ajuste de Poisson
Double_t myPoisson(Double_t*x,Double_t*par){
  return par[0]*TMath::Poisson(x[0],par[1]);
}     

void resultAnalysis2(){
	
	/* 1 - select same event, fit curve, fill histograms*/
	TString strEnergy = "3.3";
	TString strSize = "15"; //"+strSize+"
	TString strMagField = "0.5"; //"+strMagField+"
	TString strGas = "ArCO2_70-30"; //strGas+"
	TString strGasHist = "ArCO_{2} (70-30)"; //"+strGasHist
	TString strBase = strGas+"_Result_"+strEnergy+"GeV_"+strMagField+"T_"+strSize+"cm_";
	//abrindo arquivo que salvará o histograma de energia
	TFile f("EnergyHistog_"+strGas+" "+strEnergy+"GeV_"+strMagField+"T_"+strSize+"cm.root","recreate");
	//abrindo arquivo contendo as avalanches
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
	int nbinsR = 25; //25 é o ideal
	TH1D* hRadius = new TH1D("Radius", "Estimated radius for a "+strEnergy+"GeV muon under "+strMagField+"T "+strGasHist, 
				nbinsR, 
				0.0, 4600.0); //from 0 to 200cm
	hRadius->GetXaxis()->SetTitle("R(cm)");
	hRadius->GetYaxis()->SetTitle("Entries");
	//creating canvas and histogram for energy obtained from calculation after each radius obtained from the fitting
	TCanvas *cEnergy = new TCanvas("cE","Canvas Energy");
	int nbinsE = 37; //37 é o ideal
	TH1D* hEnergy = new TH1D("Energy", "Estimated energy for a "+strEnergy+"GeV muon under "+strMagField+"T "+strGasHist, 
				nbinsE, 
				0.0, 10.0); //from 0 to 6GeV
	hEnergy->GetXaxis()->SetTitle("E(GeV)");
	hEnergy->GetYaxis()->SetTitle("Entries");
	//creating canvas and histogram for straight line fit chi^2_red
	TCanvas *cChi2Line = new TCanvas("cChi2Line","Canvas Reduced Chi2Line");
	int nbinsChi2Line = 100;
	TH1D* hChi2Line = new TH1D("Chi2", "#Chi^{2}_{red} of straight line fit "+strEnergy+"GeV muon under "+strMagField+"T "+strGasHist, 
				nbinsChi2Line, 0.0, 10.0); //from 0 to 10
	hChi2Line->GetXaxis()->SetTitle("#Chi^{2}_{red}");
	hChi2Line->GetYaxis()->SetTitle("Entries");
	//creating canvas and histogram for circle fit chi^2_red
	TCanvas *cChi2 = new TCanvas("cChi2","Canvas Reduced Chi2");
	int nbinsChi2 = 100;
	TH1D* hChi2 = new TH1D("Chi2", "#Chi^{2}_{red} of circle fit "+strEnergy+"GeV muon under "+strMagField+"T "+strGasHist, 
				nbinsChi2, 0.0, 10.0); //from 0 to 10
	hChi2->GetXaxis()->SetTitle("#Chi^{2}_{red}");
	hChi2->GetYaxis()->SetTitle("Entries");
	
	//creating canvas and histogram for number of ionization points
	TCanvas *cIon = new TCanvas("cChi2","Canvas ionization points");
	int nbinsIon = 30;
	TH1I* hIon = new TH1I("Ion", "n^{o} of ionization points for a "+strEnergy+"GeV muon under "+strMagField+"T "+strGasHist, 
				nbinsIon, 0.0, 30.0); //from 0 to 10
	hIon->GetXaxis()->SetTitle("n^{o} of ionization points");
	hIon->GetYaxis()->SetTitle("Entries");
	
	//creating canvas and histogram for radius per points in fit
	TCanvas *cRadPoint = new TCanvas("cRadPoint","Canvas 2D radiusXpoints");
	int nbinsPoint = 30;
	TH2D* hRadPoint = new TH2D("RadiusPerPoint", "Radius per #points of fit "+strEnergy+"GeV muon under "+strMagField+"T "+strGasHist, 
				nbinsPoint, 0., 30., //nº of points of the fitting
				nbinsR, 0.0, 4600.0); //radius from 0 to 500
	hRadPoint->GetXaxis()->SetTitle("# points in the fit");
	hRadPoint->GetYaxis()->SetTitle("R(cm)");
	hRadPoint->GetZaxis()->SetTitle("Entries");
	//creating canvas and histogram for chi^2_red per degrees of freedom of circle fit
	TCanvas *cChiPoint = new TCanvas("cChiPoint","Canvas 2D radiusX#nu");
	TH2D* hChiPoint = new TH2D("ChiPerPoint", "#Chi^{2}_{red} per # of degrees of freedom of circle fit "+strEnergy+"GeV muon under "+strMagField+"T "+strGasHist, 
				nbinsPoint, 0., 30., //nº of degrees of freedom of the fitting
				nbinsChi2, 0.0, 10.0); //Chi2Red from 0 to 10
	hChiPoint->GetXaxis()->SetTitle("#nu");
	hChiPoint->GetYaxis()->SetTitle("#Chi^{2}_{red}");
	hChiPoint->GetZaxis()->SetTitle("Entries");
	//creating canvas and histogram for chi^2_red per degrees of freedom of straight line fit
	TCanvas *cChiPointLine = new TCanvas("cChiPointLine","Canvas 2D radiusX#nuLine");
	TH2D* hChiPointLine = new TH2D("ChiPerPoint", "#Chi^{2}_{red} per # of degrees of freedom of linear fit "+strEnergy+"GeV muon under "+strMagField+"T "+strGasHist, 
				nbinsPoint, 0., 30., //nº of degrees of freedom of the fitting
				nbinsChi2, 0.0, 10.0); //Chi2Red from 0 to 10
	hChiPointLine->GetXaxis()->SetTitle("#nu");
	hChiPointLine->GetYaxis()->SetTitle("#Chi^{2}_{red}");
	hChiPointLine->GetZaxis()->SetTitle("Entries");
	
	//creating avalanche histogram
	TCanvas *cAval = new TCanvas("cA","Canvas Avalanche");
	int nbinsA = 50; //50 é o ideal para 1000 elétrons
	TH1I* hAval = new TH1I("Avalanche", "Avalanche Gain for a "+strEnergy+"GeV muon under "+strMagField+"T "+strGasHist, 
				nbinsA, 
				0., 1000.0); //from 0 to 10GeV
	hAval->GetXaxis()->SetTitle("n^{o} of electrons");
	hAval->GetYaxis()->SetTitle("Entries");
	
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
	hAval->Fill(nEndPoint);
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
		hAval->Fill(nEndPoint);
		eventNumb[j] = eventN;
		if(eventNumb[j] == eventNumb[j-1]){
			vX.push_back(x[j]);
			vY.push_back(y[j]);
			vIncX.push_back(ex[j]);
			vIncY.push_back(ey[j]);
		}
		else{
			/* 3 - fit curve to the set of points on the vectors from 0 to (vX->size()-1) */
			
			//TGraph para fitting da reta
			TGraphErrors *SLgr = new TGraphErrors(vX.size(),&(vX[0]),&(vY[0]),&(vIncX[0]),&(vIncY[0]));
			
			//TGraph para fitting da circunferência - bolas verdes são os pontos de avalanche
			TGraphErrors *gr = new TGraphErrors(vX.size(),&(vX[0]),&(vY[0]),&(vIncX[0]),&(vIncY[0]));
			gr->SetTitle("Muon trajectory in the x-y plane");
			gr->GetXaxis()->SetTitle("x(cm)");
			gr->GetYaxis()->SetTitle("y(cm)");
			gr->SetMarkerStyle(kFullDotLarge);
			gr->SetMarkerColor(8); //verde
			int dataSize = gr->GetN(); //get ploted array dimention
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
			const double nParameters = 3.;
			double pStart[3] = {10,10,10};
			//fitter.SetFCN(fcn, pStart);
			fitter.SetFCN(3, //number of parameters
					fcn, //function
					pStart, //start parameter guess
					dataSize, //number of points
					true);//enable Chi2 calculation
			fitter.Config().ParSettings(0).SetName("x0");
			fitter.Config().ParSettings(1).SetName("y0");
			fitter.Config().ParSettings(2).SetName("R");
			// do the fit 
			//bool ok = fitter.FitFCN(fcn, pStart, dataSize);
			bool ok = fitter.FitFCN();
			if (!ok) {
			Error("line3Dfit","Line3D Fit failed");
			}   
			const ROOT::Fit::FitResult & result = fitter.Result();
			//result.Print(std::cout);
			
			double chi2 = result.Chi2();
			int nDegrFreedom = result.Ndf(); //number of degrees of freedom
			int freeParam = result.NFreeParameters();
			int nFitPoints = nDegrFreedom + freeParam;
			/*cout << "Chi2 = " << chi2 << endl;
			cout << "Degrees of freedom = " << nDegrFreedom << "\t"
			     << "Number of free parameters = " << freeParam << "\t"
			     << "Number of fit points = " << nFitPoints << endl;
			cout << "NFreeParameters()=" << result.NFreeParameters() << "\t"
			     << "NPar()=" << result.NPar() << "\t"
			     << "NTotalParameters()=" << result.NTotalParameters() << "\n";*/
			double chi2Red = chi2/nDegrFreedom;
			//cout << "#Chi^{2}_{red} = " << chi2Red << endl;
			
			// straight line fitting:
			TF1 *fStrLine = new TF1("fStrLine","x*[0] + [1]");
			SLgr->Fit(fStrLine);
			Double_t par[2]; //array to store the fit parameters
			//par[0]=angular coeficient
			//par[1]=linear coeficient
			fStrLine->GetParameters(&par[0]);
			double nDegrFreedomLine = fStrLine->GetNDF();
			double chi2RedLine = (fStrLine->GetChisquare())/nDegrFreedomLine;
			//cout << "The straight line fitted is:	y = " << par[0] << "*x + " << par[1] << endl;
			
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
			hChi2->Fill(chi2Red);
			hChi2Line->Fill(chi2RedLine);
			hRadPoint->Fill(nFitPoints,estimRadius);
			hChiPoint->Fill(nDegrFreedom,chi2Red);
			hChiPointLine->Fill(nDegrFreedomLine,chi2RedLine);
			hIon->Fill(nFitPoints);
			
			
			//clear TGraphs' pointers
			delete gr;
			delete SLgr;
			
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
	Double_t INquartis[] = {0.25,0.5,0.75};
	Double_t OUTquartis[3];
	//GetQuantiles(in maxArraySizeQ, 
			//out array q with quantiles, 
			//in position where to compute the quantiles in [0,1])
	hEnergy->GetQuantiles(3,OUTquartis,INquartis);
	cout << "2º quartil (50%) posicionado em E = " << OUTquartis[1] << endl;

	Double_t IQR = getInterQuartileRange(hEnergy);
	printf("Intervalo Interquartil:%1.2f\n\n",IQR);
	
	
	f.cd();
	
	cAval->cd();
	hAval->Draw();
	cAval->SaveAs(strBase+"HistAval.png","png");
	
	cRadius->cd();
	hRadius->Draw();
	cRadius->SaveAs(strBase+"HistRadius.png","png");
	
	cEnergy->cd();
	hEnergy->Draw();
	cEnergy->SaveAs(strBase+"HistEnergy.png","png");
	hEnergy->Write();
	
	cChi2->cd();
	hChi2->Draw();
	cChi2->SaveAs(strBase+"HistChi2.png","png");
	
	cChi2Line->cd();
	hChi2Line->Draw();
	cChi2Line->SaveAs(strBase+"HistChi2Line.png","png");
	
	cIon->cd();
	hIon->Draw();
	cIon->SaveAs(strBase+"IonizPoints.png","png");
	
	cRadPoint->cd();
	hRadPoint->SetStats(false);
	hRadPoint->Draw();
	cRadPoint->SaveAs(strBase+"HistRadPoint.png","png");
	gStyle->SetPalette(55,0);
	hRadPoint->Draw("colz");
	cRadPoint->SaveAs(strBase+"HistRadPointColor.png","png");
	
	cChiPoint->cd();
	hChiPoint->SetStats(false);
	//hChiPoint->Draw();
	//cChiPoint->SaveAs(strBase+"HistChiPoint.png","png");
	gStyle->SetPalette(55,0);
	hChiPoint->Draw("colz");
	cChiPoint->SaveAs(strBase+"HistChiPointColor.png","png");
	
	cChiPointLine->cd();
	hChiPointLine->SetStats(false);
	//hChiPointLine->Draw();
	//cChiPointLine->SaveAs(strBase+"HistChiPoint.png","png");
	gStyle->SetPalette(55,0);
	hChiPointLine->Draw("colz");
	cChiPointLine->SaveAs(strBase+"HistChiPointLineColor.png","png");
	
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
	hEnergy->Fit(fE);
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
	
	if(parE[2] > 1.5){
		cout << "if parE[2] > 1.5, then...\n";
		//percentage of data in fit
		double binL = hEnergy->FindBin(1.2); //left far end of interval
		double binR = hEnergy->FindBin(3.2); //right far end of interval
		double eParcial = hEnergy->Integral(binL, binR); //integrate interval
		double eTotal = hEnergy->GetEntries(); //total entries of histogram
		double eff = eParcial/eTotal;
		double k = eParcial;
		double n = eTotal;
		double incEff = (k+1.)*(k+2.)/((n+2.)*(n+3.)) - (pow(k+1.,2))/(pow(n+2.,2));
		cout << endl << "eParcial = " << eParcial << endl <<
				"eTotal = " << eTotal << endl <<
				"Porcentagem de dados num intervalo de 3 sigma = " << eff << "+-" << incEff << endl;
	}
	else{
		//percentage of data in fit
		double binL = hEnergy->FindBin(parE[1]-3.*parE[2]); //left far end of interval
		double binR = hEnergy->FindBin(parE[1]+3.*parE[2]); //right far end of interval
		double eParcial = hEnergy->Integral(binL, binR); //integrate interval
		double eTotal = hEnergy->GetEntries(); //total entries of histogram
		double eff = eParcial/eTotal;
		
		/*Propagando o erro do ajuste para a eficiência*/
		//calculando os N diferentes de N/M
		double N[4];
		//N1: μ' = μ + Error(μ)
		/*binL = hEnergy->FindBin((parE[1]+parError[1])-3.*parE[2]); //left far end of interval
		binR = hEnergy->FindBin((parE[1]+parError[1])+3.*parE[2]); //right far end of interval
		N[0] = hEnergy->Integral(binL, binR); //integrate interval
		//N2: μ'' = μ - Error(μ)
		binL = hEnergy->FindBin((parE[1]-parError[1])-3.*parE[2]); //left far end of interval
		binR = hEnergy->FindBin((parE[1]-parError[1])+3.*parE[2]); //right far end of interval
		N[1] = hEnergy->Integral(binL, binR); //integrate interval
		//N3: σ' = σ + Error(σ)
		binL = hEnergy->FindBin(parE[1]-3.*(parE[2]+parError[2])); //left far end of interval
		binR = hEnergy->FindBin(parE[1]+3.*(parE[2]+parError[2])); //right far end of interval
		N[2] = hEnergy->Integral(binL, binR); //integrate interval
		//N4: σ'' = σ - Error(σ)
		binL = hEnergy->FindBin(parE[1]-3.*(parE[2]-parError[2])); //left far end of interval
		binR = hEnergy->FindBin(parE[1]+3.*(parE[2]-parError[2])); //right far end of interval
		N[3] = hEnergy->Integral(binL, binR); //integrate interval
		cout << "N[] = [" << N[0] << ","<< N[1] << ","<< N[2] << ","<< N[3] << "]\n";
		double sigN = TMath::RMS(4,N); //size,array,weight
		double sigM = sqrt(eTotal);
		eParcial = TMath::Mean(4,N);
		eff = eParcial/eTotal;
		double incEff = eff*sqrt(TMath::Power(sigN/eParcial,2) + TMath::Power(sigM/eTotal,2));*/
		/*Assumindo distribuição de probabilidade binomial*/
		double k = eParcial;
		double n = eTotal;
		double incEff = (k+1.)*(k+2.)/((n+2.)*(n+3.)) - (pow(k+1.,2))/(pow(n+2.,2));
		
		cout << endl << "eParcial = " << eParcial << endl <<
				"eTotal = " << eTotal << endl <<
				"Porcentagem de dados num intervalo de 3 sigma = " << eff << "+-" << incEff << endl;	
	}
	
	//fitting poisson distribuition for ionization points histogram
	cIon->cd();
	TF1 *fIon = new TF1("fitIon",myPoisson,0.,2.,2); // x in [0;50] with 2 parameters
	fIon->SetParameters(10.,10.); //necessary guess for poisson                                              
   	fIon->SetParNames("Constant","#mu");
   	hIon->Fit("fitIon");
	Double_t parIon[2];
	fIon->GetParameters(&parIon[0]);
	cIon->Update();
	cout << "par[0] = " << parIon[0] << "\t" //constant
	     << "par[1] = " << parIon[1] << endl; //valor esperado
	//configuring statistics box
	gStyle->SetOptStat("e");
	gStyle->SetOptFit(0011);
	gStyle->SetStatY(0.9);// Set y-position (fraction of pad size)
	gStyle->SetStatX(0.9);// Set x-position (fraction of pad size)
	gStyle->SetStatW(0.4);// Set width of stat-box (fraction of pad size)
	gStyle->SetStatH(0.5);// Set height of stat-box (fraction of pad size)
	cIon->Update();
	hIon->Draw();
	cIon->SaveAs(strBase+"HistIonFitted.png","png");
	//cout << "The mean number of ionization was " << parIon[1] << "+/-" << sqrt(parIon[1]) << "\n";
	
	//calculating energy using results from R histogram
	double B = 0.5;
	double R = parR[1]*0.01;//em metros
	double pc = 1.6*0.01*3.*6.242*B*R;
	double mc2 = 0.105;//energia de repouso do muon -> necessário incluir incerteza dessa massa
	double estimEnergy = sqrt(pc*pc + mc2*mc2);
	double incR = parR[2]*0.01;//em metros
	double incE = pc*B*1.6*0.01*3.*6.242*incR/estimEnergy;
	cout << "Usando o histograma do raio R, obtemos um valor de: " << endl;
	cout << estimEnergy << "+/-" << incE << "GeV" << endl;
	
	inputFile->Close();
	f.Close();
	
	
	
	
}

int main(){
	resultAnalysis2();
	return 0;
	
}
