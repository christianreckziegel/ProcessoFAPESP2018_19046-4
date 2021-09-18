

void myGraph(){
	//a partir de 4.5 não funcionou mais pelo mesmo problema do campo 1mT: 3 sigma ultrapassa os limites do histograma e a integral vira 100%
	Int_t n1 = 10;
	//energia
	Double_t x1[] = {2.0,2.3,2.5,2.8,3.0,3.3,3.5,3.8,4.0,4.3};
	//porcentagem
	Double_t y1[] = {0.773196,0.649485,0.636364,0.525253,0.561224,0.6,0.568421,0.683673,0.622449,0.555556}; 
	Double_t exl[] = {0.,0.,0.,0.,0.,0.};
	Double_t eyl[] = {0.,0.,0.,0.,0.,0.};
	Double_t exh[] = {0.,0.,0.,0.,0.,0.};//OBS: o último ponto (60cm) só tem 20 eventos, os outros tem 100
	Double_t eyh[] = {0.,0.,0.,0.,0.,0.};
	Double_t ex1[] = {0.,0.,0.,0.,0.,0.,0.,0.,0.,0.};
	Double_t ey1[] = {0.118888,0.105093,0.102559,0.0899575,0.0945558,0.100525,0.0968734,0.108378,0.101514,0.0934306};
	
	
	Int_t n2 = 13;
	//energia real
	Double_t x2[] = {2.0,2.3,2.5,2.8,3.0,3.3,3.5,3.8,4.0,4.3,4.5,4.8,5.0};
	//energia medida
	Double_t y2[] = {2.22886,2.43232,2.41161,2.84722,3.1277,3.37639,3.48972,3.57593,4.15633,3.94152,4.25797,4.68252,4.64935};
	Double_t ex2[] = {0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.};
	Double_t ey2[] = {0.0604213,0.0762864,0.114388,0.10896,0.122898,0.279289,0.243256,0.512137,0.264764,0.358352,0.477889,0.837213,1.02619};
	
	TCanvas* cEff = new TCanvas("myCanvas","Canvas for efficiency");
	cEff->cd();
	//TGraphAsymmErrors* gr = new TGraphAsymmErrors(n,x,y,exl,exh,eyl,eyh);
	TGraphErrors* gr1 = new TGraphErrors(n1,x1,y1,ex1,ey1);
	gr1->SetTitle("Porcentagem de dados no intervalo de ajuste 3#sigma para ArCO2(70/30), Aresta=30cm, B=0.5T"); //"#splitline{Porcentagem de dados no intervalo de ajuste 3#sigma}{para ArCO2(70/30), Aresta=30cm, B=0.5T}"
	gr1->GetXaxis()->SetTitle("Energia(GeV)");
	gr1->GetYaxis()->SetTitle("Efici#hat{e}ncia");
	gr1->SetMarkerColor(kBlue);
	gr1->SetMarkerStyle(20);
	gr1->Draw("AP");
	/*
	//With different seed
	TGraphErrors* gr2 = new TGraphErrors(n2,x2,y2,ex2,ey2);
	gr2->SetMarkerColor(kRed);
	gr2->SetMarkerStyle(20);
	gr2->Draw("P");
	
	TLegend *leg = new TLegend(0.15,0.75,0.35,0.85);
	leg->AddEntry(gr1,"Seed 1","lp");
	leg->AddEntry(gr2,"Seed 2","lp");
	leg->Draw();
	*/
	cEff->SaveAs("energyEff.png","png");
	
	TCanvas* cTEnergy = new TCanvas("myEnergy","Canvas for measured energy");
	cTEnergy->cd();
	gStyle->SetOptFit(1);
	TGraphErrors* gr2 = new TGraphErrors(n2,x2,y2,ex2,ey2);
	gr2->SetTitle("Energia medida em func#tilde{a}o da energia verdadeira para ArCO2(70/30), B=0.5T, Size=30cm"); //"#splitline{Measured energy as a fuction of true energy with fitting}{for ArCO2(70/30), B=0.5T, Size=30cm}"
	gr2->GetXaxis()->SetTitle("True energy (GeV)");
	gr2->GetYaxis()->SetTitle("Measured energy (GeV)");
	gr2->SetMarkerColor(kGreen);
	gr2->SetMarkerStyle(20);
	gr2->Draw("AP");
	
	/*TF1 *fStrLine = new TF1("fStrLine","x*[0] + [1]");
	gr2->Fit(fStrLine);
	Double_t par[2]; //array to store the fit parameters
	//par[0]=angular coeficient
	//par[1]=linear coeficient
	fStrLine->GetParameters(&par[0]);*/
	cTEnergy->SaveAs("trueEnergy.png","png");
}

int main(){
	myGraph();
	return 0;
}
