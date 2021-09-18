

void myGraph(){
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
	
	TCanvas* cEff = new TCanvas("myCanvas","Canvas for efficiency");
	cEff->cd();
	
	//TGraphAsymmErrors* gr = new TGraphAsymmErrors(n,x,y,exl,exh,eyl,eyh);
	TGraphErrors* gr1 = new TGraphErrors(n1,x1,y1,ex1,ey1);
	gr1->SetTitle("Porcentagem de dados no intervalo de ajuste 3#sigma para ArCO2(70/30), E=2GeV, B=0.5T");
	gr1->GetXaxis()->SetTitle("Aresta(cm)");
	gr1->GetYaxis()->SetTitle("Efici#hat{e}ncia");
	gr1->SetMarkerColor(kBlue);
	gr1->SetMarkerStyle(20);
	gr1->Draw("AP");
	//same, for exponential fitting
	TGraphErrors* gr12 = new TGraphErrors(n1,x1,y1,ex1,ey1);
	gr12->SetMarkerColor(kBlue);
	gr12->SetMarkerStyle(20);
	gr12->Draw("P");
	//same, for saturation fitting
	TGraphErrors* gr13 = new TGraphErrors(n1,x1,y1,ex1,ey1);
	gr13->SetMarkerColor(kBlue);
	gr13->SetMarkerStyle(20);
	gr13->Draw("P");
	//With different seed
	TGraphErrors* gr2 = new TGraphErrors(n2,x2,y2,ex2,ey2);
	gr2->SetMarkerColor(kRed);
	gr2->SetMarkerStyle(20);
	gr2->Draw("P");
	
	
	
	TLegend *leg = new TLegend(0.72,0.75,0.85,0.88);
	leg->AddEntry(gr1,"Seed 1","p");
	leg->AddEntry(gr2,"Seed 2","p");
	leg->Draw();
	cEff->SaveAs("sizeEffNew.png","png");
	
	/*TF1 *fStrLine = new TF1("fStrLine","x*[0] + [1]");
	gr1->Fit(fStrLine);
	Double_t par[2]; //array to store the fit parameters
	//par[0]=angular coeficient
	//par[1]=linear coeficient
	fStrLine->GetParameters(&par[0]);*/
	//assintotic function
	TF1 *fAssint = new TF1("fAssint","[0] - [1]/x");
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
	fexp->GetParameters(&par12[0]);
	//saturation function
	TF1 *fSat = new TF1("fSat","[0]*(x/([1]-x))");
	fSat->SetLineColor(kBlue);
	fSat->SetLineStyle(kDotted);
	fSat->SetParameters(0.1,-2.);
	fSat->SetParNames("AssintoticValue","constant");
	gr13->Fit(fSat);
	Double_t par13[2]; //array to store the fit parameters
	fSat->GetParameters(&par13[0]);
	
	leg->AddEntry(fAssint,"assintotic","l");
	leg->AddEntry(fexp,"exponential","l");
	leg->AddEntry(fSat,"saturation","l");
	
	cEff->SaveAs("sizeEffFitNew.png","png");

}

int main(){
	myGraph();
	return 0;
}
