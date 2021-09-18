

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
	Int_t nbinsAcc = 60;
	TH1D* passEff1 = new TH1D("pass_n", "Accepted values for efficiency", 
				nbinsAcc, 0.0, 120.0); //from 0 to 120cm
	Bin = passEff1->FindBin(20.);
	passEff1->SetBinContent(Bin, 54);
	Bin = passEff1->FindBin(25.);
	passEff1->SetBinContent(Bin, 63);
	Bin = passEff1->FindBin(30.);
	passEff1->SetBinContent(Bin, 75);
	Bin = passEff1->FindBin(35.);
	passEff1->SetBinContent(Bin, 64);
	Bin = passEff1->FindBin(40.);
	passEff1->SetBinContent(Bin, 73);
	Bin = passEff1->FindBin(60.);
	passEff1->SetBinContent(Bin, 74);
	Bin = passEff1->FindBin(80.);
	passEff1->SetBinContent(Bin, 81);
	Bin = passEff1->FindBin(100.);
	passEff1->SetBinContent(Bin, 66);
	TH1D* totalEff1 = new TH1D("total_N", "Total values for efficiency", 
				nbinsAcc, 0.0, 120.0); //from 0 to 120cm
	Bin = totalEff1->FindBin(20.);
	totalEff1->SetBinContent(Bin, 98);
	Bin = totalEff1->FindBin(25.);
	totalEff1->SetBinContent(Bin, 95);
	Bin = totalEff1->FindBin(30.);
	totalEff1->SetBinContent(Bin, 97);
	Bin = totalEff1->FindBin(35.);
	totalEff1->SetBinContent(Bin, 96);
	Bin = totalEff1->FindBin(40.);
	totalEff1->SetBinContent(Bin, 97);
	Bin = totalEff1->FindBin(60.);
	totalEff1->SetBinContent(Bin, 97);
	Bin = totalEff1->FindBin(80.);
	totalEff1->SetBinContent(Bin, 99);
	Bin = totalEff1->FindBin(100.);
	totalEff1->SetBinContent(Bin, 98);
	//30cm, 35cm, 50cm de aresta, seed 2
	TH1D* passEff2 = new TH1D("pass_n", "Accepted values for efficiency", 
				nbinsAcc, 0.0, 120.0); //from 0 to 120cm
	Bin = passEff2->FindBin(30.);
	passEff2->SetBinContent(Bin, 60);
	Bin = passEff2->FindBin(35.);
	passEff2->SetBinContent(Bin, 70);
	Bin = passEff2->FindBin(100.);
	passEff2->SetBinContent(Bin, 75);
	
	TH1D* totalEff2 = new TH1D("total_N", "Total values for efficiency", 
				nbinsAcc, 0.0, 120.0); //from 0 to 120cm
	Bin = totalEff2->FindBin(30.);
	totalEff2->SetBinContent(Bin, 97);
	Bin = totalEff2->FindBin(35.);
	totalEff2->SetBinContent(Bin, 97);
	Bin = totalEff2->FindBin(100.);
	totalEff2->SetBinContent(Bin, 98);
	
	
	TCanvas* cEff = new TCanvas("myCanvas","Canvas for efficiency");
	cEff->cd();
	//30cm de aresta com erros assimétricos
	TGraphAsymmErrors* gr1 = new TGraphAsymmErrors(passEff1,totalEff1, "v");
	gr1->SetTitle("Porcentagem de dados no intervalo de ajuste 3#sigma para ArCO2(70/30), E=2GeV, B=0.5T");
	gr1->GetXaxis()->SetTitle("Aresta(cm)");
	gr1->GetYaxis()->SetTitle("Efici#hat{e}ncia");
	gr1->SetMarkerColor(kBlue);
	gr1->SetLineColor(kBlue);
	gr1->SetMarkerStyle(kFullCircle);
	gr1->SetMinimum(0.);
	gr1->SetMaximum(1.);
	gr1->Draw("AP");
	//20 cm de aresta com erros assimétricos
	TGraphAsymmErrors* gr2 = new TGraphAsymmErrors(passEff2,totalEff2, "v");
	gr2->SetMarkerColor(kRed);
	gr2->SetLineColor(kRed);
	gr2->SetMarkerStyle(kOpenCircle);
	gr2->Draw("P");
	//Legenda
	TLegend *leg = new TLegend(0.72,0.2,0.85,0.3);//(0.65,0.18,0.85,0.3)
	leg->AddEntry(gr1,"Seed 1","lp");
	leg->AddEntry(gr2,"Seed 2","lp");
	leg->Draw();
	cEff->SaveAs("sizeEffAsym.png","png");
	
	/*TF1 *fStrLine = new TF1("fStrLine","x*[0] + [1]");
	gr1->Fit(fStrLine);
	Double_t par[2]; //array to store the fit parameters
	//par[0]=angular coeficient
	//par[1]=linear coeficient
	fStrLine->GetParameters(&par[0]);*/
	//assintotic function
	/*TF1 *fAssint = new TF1("fAssint","[0] - [1]/x");
	fAssint->SetLineColor(kBlue);
	fAssint->SetParameters(0.1,0.1);
	fAssint->SetParNames("AssintoticValue","constant");
	gr1->Fit(fAssint);
	Double_t par1[2]; //array to store the fit parameters
	fAssint->GetParameters(&par1[0]);
	//exponential function
	TF1 *fexp = new TF1("fexp","[0] - [1]*TMath::Exp(-[2]*x)");
	fexp->SetLineColor(kBlue);
	fexp->SetLineStyle(kDashed);
	fexp->SetParameters(0.1,0.1,0.1);
	fexp->SetParNames("AssintoticValue","constant");
	gr12->Fit(fexp);
	Double_t par12[2]; //array to store the fit parameters
	fexp->GetParameters(&par12[0]);*/
	//saturation function
	TF1 *fSat = new TF1("fSat","[0]*(x/([1]-x))");
	fSat->SetLineColor(kBlue);
	fSat->SetParameters(0.1,-2.);
	fSat->SetParNames("AssintoticValue","constant");
	gr1->Fit(fSat);
	Double_t par13[2]; //array to store the fit parameters
	fSat->GetParameters(&par13[0]);
	
	//leg->AddEntry(fAssint,"assintotic","l");
	//leg->AddEntry(fexp,"exponential","l");
	leg->AddEntry(fSat,"curva de saturac#tilde{a}o","l");
	
	cEff->SaveAs("sizeEffAsymFit.png","png");

}

int main(){
	myGraphDivide();
	return 0;
}
