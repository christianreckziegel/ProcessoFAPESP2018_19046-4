

void myGraph(){
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
	
	TCanvas* cEff = new TCanvas("myCanvas","Canvas for efficiency");
	cEff->cd();
	
	TGraphErrors* gr1 = new TGraphErrors(n,x,y,ex,ey);
	gr1->SetTitle("Porcentagem de dados no intervalo de ajuste 3#sigma \n para ArCO2(70/30), E=2GeV");
	gr1->GetXaxis()->SetTitle("B(T)");
	gr1->GetYaxis()->SetTitle("Efficiency");
	gr1->SetMarkerColor(kBlue);
	gr1->SetMarkerStyle(kFullCircle);
	gr1->Draw("AP");
	TGraphErrors* gr2 = new TGraphErrors(n,x2,y2,ex2,ey2);
	gr2->SetMarkerColor(kRed);
	gr2->SetMarkerStyle(kOpenCircle);
	gr2->Draw("P");
	//Legenda
	TLegend *leg = new TLegend(0.7,0.18,0.85,0.3);
	leg->AddEntry(gr1,"aresta=30cm","lp");
	leg->AddEntry(gr2,"aresta=20cm","lp");
	leg->Draw();
	cEff->SaveAs("magneticEffNew.png","png");
	//fitting azul-30cm
	TF1 *f30cm = new TF1("f30cm","[0]*(x/([1]-x))");
	f30cm->SetLineColor(kBlue);
	f30cm->SetParameters(0.1,-2.); //0.1,-2.
	f30cm->SetParNames("AssintoticValue","constant");
	gr1->Fit(f30cm);
	Double_t par1[2]; //array to store the fit parameters
	f30cm->GetParameters(&par1[0]);
	//fitting vermelho-20cm
	TF1 *f20cm = new TF1("f20cm","[0]*(x/([1]-x))");//ou [0]-[1]/x
	f20cm->SetLineColor(kRed);
	f20cm->SetParameters(-2.,-2.); //0.1,-2.
	f20cm->SetParNames("AssintoticValue","constant");
	gr2->Fit(f20cm);
	Double_t par2[2]; //array to store the fit parameters
	f20cm->GetParameters(&par2[0]);
	//configuring statistics box
	/*gStyle->SetOptStat("e");
	gStyle->SetOptFit(0011);
	gStyle->SetStatY(0.9);// Set y-position (fraction of pad size)
	gStyle->SetStatX(0.9);// Set x-position (fraction of pad size)
	gStyle->SetStatW(0.4);// Set width of stat-box (fraction of pad size)
	gStyle->SetStatH(0.5);// Set height of stat-box (fraction of pad size)
	cIon->Update();*/
	cEff->SaveAs("magneticEffNewFit.png","png");

}

int main(){
	myGraph();
	return 0;
}
