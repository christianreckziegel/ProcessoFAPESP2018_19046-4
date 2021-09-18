

void myGraphDivide(){
	Int_t n1 = 8;
	//Double_t x[] = {400.,625.,900.,1225.,1600.}; //área
	Double_t x1[] = {20.,25.,30.,35.,40.,60.,80.,100.}; //aresta
	Double_t y1[] = {0.548469,0.660526,0.773196,0.666667,0.752577,0.762887,0.767677,0.658163}; //porcentagem
	Double_t exl[] = {0.,0.,0.,0.,0.,0.};
	Double_t eyl[] = {0.,0.,0.,0.,0.,0.};
	Double_t exh[] = {0.,0.,0.,0.,0.,0.};//OBS: o último ponto (60cm) só tem 20 eventos, os outros tem 100
	Double_t eyh[] = {0.,0.,0.,0.,0.,0.};
	Double_t ex1[] = {0.,0.,0.,0.,0.,0.,0.,0.};
	//Double_t ey1[] = {0.0933855,0.107749,0.118888,0.107583,0.116608,0.117749,0.107239}; //sqrt(N)
	Double_t ey1[] = {0.0556382,0.0679726,0.0785061,0.0680414,0.0764126,0.0774594,0.0967151,0.0687936}; //propaga o erro do ajuste
	
	Int_t n2 = 2;
	//Double_t x[] = {900.,1225.}; //área
	Double_t x2[] = {30.,35.}; //aresta
	Double_t y2[] = {0.623711,0.721649}; //porcentagem
	Double_t ex2[] = {0.,0.};
	//Double_t ey2[] = {0.101594,0.113175}; //sqrt(N)
	Double_t ey2[] = {0.0657975,0.0732724}; //propagates fit error
	
	//30cm de aresta, seed 1
	Int_t Bin;
	Int_t nbinsAcc = 55;
	TH1D* passEff1 = new TH1D("pass_n", "Accepted values for efficiency", 
				nbinsAcc, 0.0, 5.5); //from 0 to 6GeV
	Bin = passEff1->FindBin(2.);
	passEff1->SetBinContent(Bin, 75);
	Bin = passEff1->FindBin(2.3);
	passEff1->SetBinContent(Bin, 63);
	Bin = passEff1->FindBin(2.5);
	passEff1->SetBinContent(Bin, 63);
	Bin = passEff1->FindBin(2.8);
	passEff1->SetBinContent(Bin, 52);
	Bin = passEff1->FindBin(3.);
	passEff1->SetBinContent(Bin, 55);
	Bin = passEff1->FindBin(3.3);
	passEff1->SetBinContent(Bin, 57);
	Bin = passEff1->FindBin(3.5);
	passEff1->SetBinContent(Bin, 54);
	Bin = passEff1->FindBin(3.8);
	passEff1->SetBinContent(Bin, 67);
	Bin = passEff1->FindBin(4.);
	passEff1->SetBinContent(Bin, 61);
	Bin = passEff1->FindBin(4.3);
	passEff1->SetBinContent(Bin, 55);
	Bin = passEff1->FindBin(4.5);
	passEff1->SetBinContent(Bin, 62);
	Bin = passEff1->FindBin(4.8);
	passEff1->SetBinContent(Bin, 58);
	Bin = passEff1->FindBin(5.);
	passEff1->SetBinContent(Bin, 55);
	TH1D* totalEff1 = new TH1D("total_N", "Total values for efficiency", 
				nbinsAcc, 0.0, 5.5); //from 0 to 120cm
	Bin = totalEff1->FindBin(2.);
	totalEff1->SetBinContent(Bin, 97);
	Bin = totalEff1->FindBin(2.3);
	totalEff1->SetBinContent(Bin, 97);
	Bin = totalEff1->FindBin(2.5);
	totalEff1->SetBinContent(Bin, 99);
	Bin = totalEff1->FindBin(2.8);
	totalEff1->SetBinContent(Bin, 99);
	Bin = totalEff1->FindBin(3.);
	totalEff1->SetBinContent(Bin, 98);
	Bin = totalEff1->FindBin(3.3);
	totalEff1->SetBinContent(Bin, 95);
	Bin = totalEff1->FindBin(3.5);
	totalEff1->SetBinContent(Bin, 95);
	Bin = totalEff1->FindBin(3.8);
	totalEff1->SetBinContent(Bin, 98);
	Bin = totalEff1->FindBin(4.);
	totalEff1->SetBinContent(Bin, 98);
	Bin = totalEff1->FindBin(4.3);
	totalEff1->SetBinContent(Bin, 99);
	Bin = totalEff1->FindBin(4.5);
	totalEff1->SetBinContent(Bin, 98);
	Bin = totalEff1->FindBin(4.8);
	totalEff1->SetBinContent(Bin, 99);
	Bin = totalEff1->FindBin(5.);
	totalEff1->SetBinContent(Bin, 97);
	
	TCanvas* cEff = new TCanvas("myCanvas","Canvas for efficiency");
	cEff->cd();
	//30cm de aresta com erros assimétricos
	TGraphAsymmErrors* gr1 = new TGraphAsymmErrors(passEff1,totalEff1, "v");
	gr1->SetTitle("Porcentagem de dados no intervalo de ajuste 3#sigma para ArCO2(70/30), aresta=30cm, B=0.5T");
	gr1->GetXaxis()->SetTitle("Energia(GeV)");
	gr1->GetYaxis()->SetTitle("Efici#hat{e}ncia");
	gr1->SetMarkerColor(kBlue);
	gr1->SetLineColor(kBlue);
	gr1->SetMarkerStyle(kFullCircle);
	gr1->SetMinimum(0.);
	gr1->SetMaximum(1.);
	gr1->Draw("AP");
	
	cEff->SaveAs("energyEffAsym.png","png");
	
	TF1 *fStrLine = new TF1("fStrLine","x*[0] + [1]");
	gr1->Fit(fStrLine);
	Double_t par[2]; //array to store the fit parameters
	//par[0]=angular coeficient
	//par[1]=linear coeficient
	fStrLine->GetParameters(&par[0]);
	
	cEff->SaveAs("energyEffAsymFit.png","png");

}

int main(){
	myGraphDivide();
	return 0;
}
