

void myGraphDivide(){
	Int_t n = 6;
	//30cm de aresta
	Double_t x[] = {0.001,0.25,0.5,1.,2.,5.}; //campo magnético
	//Double_t y[] = {0.,0.525773,0.773196,0.838384,0.938144,1.}; //porcentagem
	//Double_t y[] = {0.,0.520619,0.773196,0.838384,0.938144,1.}; //porcentagem propagates fit error
	Double_t y[] = {0.,0.525773,0.773196,0.838384,0.938144,1.};//binominal probability density function for efficiency
	Double_t ex[] = {0.,0.,0.,0.,0.,0.};
	//Double_t ey[] = {0.,0.0909407,0.118888,0.124773,0.136912,0.143592}; //sigmaN = sqrt(N)
	//Double_t ey[] = {0.,0.0531948,0.0785061,0.0842607,0.0952541,0.121654}; //propagates fit error
	Double_t ey[] = {0.,0.00249362,0.00178349,0.00137241,0.000657076,0.}; //binominal probability density function for efficiency
	//error (1mt) = 0.00010627
	
	//20cm de aresta
	Double_t x2[] = {0.001,0.25,0.5,1.,2.,5.}; //campo magnético
	Double_t y2[] = {0.0505051,0.427083,0.55102,0.791667,0.926316,1.};//binominal probability density function for efficiency
	Double_t ex2[] = {0.,0.,0.,0.,0.,0.};
	Double_t ey2[] = {0.000547812,0.00247372,0.0024505,0.00170068,0.000772166,0.}; //binominal probability density function for efficiency
	
	Int_t Bin;
	//filling 30cm edge histograms
	Int_t nbinsAcc1 = 60;//60 bins
	TH1D* passEff1 = new TH1D("pass_n", "Accepted values for efficiency", 
				nbinsAcc1, 0.0, 6.0); //from 0 to 5T
	Bin = passEff1->FindBin(0.001);
	passEff1->SetBinContent(Bin, 0);
	Bin = passEff1->FindBin(0.25);
	passEff1->SetBinContent(Bin, 51);
	Bin = passEff1->FindBin(0.5);
	passEff1->SetBinContent(Bin, 75);
	Bin = passEff1->FindBin(1.);
	passEff1->SetBinContent(Bin, 83);
	Bin = passEff1->FindBin(2.);
	passEff1->SetBinContent(Bin, 91);
	Bin = passEff1->FindBin(5.);
	passEff1->SetBinContent(Bin, 97);
	TH1D* totalEff1 = new TH1D("total_N", "Total values for efficiency", 
				nbinsAcc1, 0.0, 6.0); //from 0 to 5T
	Bin = totalEff1->FindBin(0.001);
	totalEff1->SetBinContent(Bin, 94);
	Bin = totalEff1->FindBin(0.25);
	totalEff1->SetBinContent(Bin, 97);
	Bin = totalEff1->FindBin(0.5);
	totalEff1->SetBinContent(Bin, 97);
	Bin = totalEff1->FindBin(1.);
	totalEff1->SetBinContent(Bin, 99);
	Bin = totalEff1->FindBin(2.);
	totalEff1->SetBinContent(Bin, 97);
	Bin = totalEff1->FindBin(5.);
	totalEff1->SetBinContent(Bin, 97);
	//filling 20cm edge histograms
	Int_t nbinsAcc2 = 60;//60 bins
	TH1D* passEff2 = new TH1D("pass_n", "Accepted values for efficiency", 
				nbinsAcc2, 0.0, 6.0); //from 0 to 5T
	Bin = passEff2->FindBin(0.001);
	passEff2->SetBinContent(Bin, 5);
	Bin = passEff2->FindBin(0.25);
	passEff2->SetBinContent(Bin, 41);
	Bin = passEff2->FindBin(0.5);
	passEff2->SetBinContent(Bin, 54);
	Bin = passEff2->FindBin(1.);
	passEff2->SetBinContent(Bin, 76);
	Bin = passEff2->FindBin(2.);
	passEff2->SetBinContent(Bin, 88);
	Bin = passEff2->FindBin(5.);
	passEff2->SetBinContent(Bin, 97);
	TH1D* totalEff2 = new TH1D("total_N", "Total values for efficiency", 
				nbinsAcc2, 0.0, 6.0); //from 0 to 5T
	Bin = totalEff2->FindBin(0.001);
	totalEff2->SetBinContent(Bin, 99);
	Bin = totalEff2->FindBin(0.25);
	totalEff2->SetBinContent(Bin, 96);
	Bin = totalEff2->FindBin(0.5);
	totalEff2->SetBinContent(Bin, 98);
	Bin = totalEff2->FindBin(1.);
	totalEff2->SetBinContent(Bin, 96);
	Bin = totalEff2->FindBin(2.);
	totalEff2->SetBinContent(Bin, 95);
	Bin = totalEff2->FindBin(5.);
	totalEff2->SetBinContent(Bin, 97);
	//filling 40cm edge histograms
	Int_t nbinsAcc3 = 60;//60 bins
	TH1D* passEff3 = new TH1D("pass_n", "Accepted values for efficiency", 
				nbinsAcc3, 0.0, 6.0); //from 0 to 5T
	Bin = passEff3->FindBin(0.001);
	passEff3->SetBinContent(Bin, 0);
	Bin = passEff3->FindBin(0.25);
	passEff3->SetBinContent(Bin, 56);
	Bin = passEff3->FindBin(0.5);
	passEff3->SetBinContent(Bin, 73);
	Bin = passEff3->FindBin(1.);
	passEff3->SetBinContent(Bin, 83);
	Bin = passEff3->FindBin(2.);
	passEff3->SetBinContent(Bin, 92);
	Bin = passEff3->FindBin(5.);
	passEff3->SetBinContent(Bin, 96);
	TH1D* totalEff3 = new TH1D("total_N", "Total values for efficiency", 
				nbinsAcc3, 0.0, 6.0); //from 0 to 5T
	Bin = totalEff3->FindBin(0.001);
	totalEff3->SetBinContent(Bin, 97);
	Bin = totalEff3->FindBin(0.25);
	totalEff3->SetBinContent(Bin, 98);
	Bin = totalEff3->FindBin(0.5);
	totalEff3->SetBinContent(Bin, 97);
	Bin = totalEff3->FindBin(1.);
	totalEff3->SetBinContent(Bin, 97);
	Bin = totalEff3->FindBin(2.);
	totalEff3->SetBinContent(Bin, 99);
	Bin = totalEff3->FindBin(5.);
	totalEff3->SetBinContent(Bin, 97);
	
	//Canvas para plot de eficiência
	TCanvas* cEff = new TCanvas("myCanvas","Canvas for efficiency");
	cEff->cd();
	//30cm de aresta com erros assimétricos
	TGraphAsymmErrors* gr1 = new TGraphAsymmErrors(passEff1,totalEff1, "v");
	gr1->SetTitle("Porcentagem de dados no intervalo de ajuste 3#sigma \n para ArCO2(70/30), E=2GeV");
	gr1->GetXaxis()->SetTitle("B(T)");
	gr1->GetYaxis()->SetTitle("Efici#hat{e}ncia");
	gr1->SetMarkerColor(kBlue);
	gr1->SetLineColor(kBlue);
	gr1->SetMarkerStyle(kFullCircle);
	gr1->SetMinimum(0.);
	gr1->SetMaximum(1.);
	gr1->Draw("AP");
	//20cm de aresta com erros assimétricos
	TGraphAsymmErrors* gr2 = new TGraphAsymmErrors(passEff2,totalEff2, "v");
	gr2->SetMarkerColor(kRed);
	gr2->SetLineColor(kRed);
	gr2->SetMarkerStyle(kOpenCircle);
	gr2->Draw("P");
	//40cm de aresta com erros assimétricos
	TGraphAsymmErrors* gr3 = new TGraphAsymmErrors(passEff3,totalEff3, "v");
	gr3->SetMarkerColor(kGreen);
	gr3->SetLineColor(kGreen);
	gr3->SetMarkerStyle(kOpenSquare);
	gr3->SetMarkerSize(0.5);
	gr3->Draw("P");
	//Legenda
	TLegend *leg = new TLegend(0.65,0.15,0.85,0.28);//(0.65,0.15,0.85,0.28)
	leg->AddEntry(gr2,"aresta=20cm","lp");
	leg->AddEntry(gr1,"aresta=30cm","lp");
	leg->AddEntry(gr3,"aresta=40cm","lp");
	leg->Draw();
	cEff->SaveAs("magneticEffAsym.png","png");
	
	//fitting azul-30cm
	TF1 *f30cm = new TF1("f30cm","[0]*(x/([1]-x))");
	f30cm->SetLineColor(kBlue);
	f30cm->SetParameters(0.1,-2.);
	f30cm->SetParNames("AssintoticValue","constant");
	gr1->Fit(f30cm);
	Double_t par1[2]; //array to store the fit parameters
	f30cm->GetParameters(&par1[0]);
	//fitting vermelho-20cm
	TF1 *f20cm = new TF1("f20cm","[0]*(x/([1]-x))");//ou [0]-[1]/x
	f20cm->SetLineColor(kRed);
	f20cm->SetParameters(0.1,-2.);
	f20cm->SetParNames("AssintoticValue","constant");
	gr2->Fit(f20cm);
	Double_t par2[2]; //array to store the fit parameters
	f20cm->GetParameters(&par2[0]);
	
	leg->AddEntry(f30cm,"saturac#tilde{a}o para 30cm","l");
	leg->AddEntry(f20cm,"saturac#tilde{a}o para 20cm","l");
	
	//configuring statistics box
	/*gStyle->SetOptStat("e");
	gStyle->SetOptFit(0011);
	gStyle->SetStatY(0.9);// Set y-position (fraction of pad size)
	gStyle->SetStatX(0.9);// Set x-position (fraction of pad size)
	gStyle->SetStatW(0.4);// Set width of stat-box (fraction of pad size)
	gStyle->SetStatH(0.5);// Set height of stat-box (fraction of pad size)
	cIon->Update();*/
	cEff->SaveAs("magneticEffAsymFit.png","png");

}

int main(){
	myGraphDivide();
	return 0;
}
