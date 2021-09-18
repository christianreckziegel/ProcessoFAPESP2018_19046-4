

void myGraph(){
	//provavelmente o mais apropriado seria uma curva para cada composição, variando as concentrações
	Int_t n = 3;
	Double_t x[] = {}; //ArCH4, ArCO2, XeCO2
	Double_t y[] = {0.65625,0.773196,0.742268,}; //porcentagem
	Double_t exl[] = {0.,0.,0.,0.,0.,0.};
	Double_t eyl[] = {0.,0.,0.,0.,0.,0.};
	Double_t exh[] = {0.,0.,0.,0.,0.,0.};
	Double_t eyh[] = {0.,0.,0.,0.,0.,0.};
	Double_t ex[] = {0.,0.,0.,0.,0.,0.};
	Double_t ey[] = {0.106405,0.118888,0.115465,};
	
	TCanvas* cEff = new TCanvas("myCanvas","Canvas for efficiency");
	cEff->cd();
	
	//TGraphAsymmErrors* gr = new TGraphAsymmErrors(n,x,y,exl,exh,eyl,eyh);
	TGraphErrors* gr = new TGraphErrors(n,x,y,ex,ey);
	gr->SetTitle("Porcentagem de dados no intervalo de ajuste 3#mu \n para ArCO2(70/30), E=2GeV, Aresta=30cm, B=0.5T");
	gr->GetXaxis()->SetTitle("Gas");
	gr->GetYaxis()->SetTitle("Efficiency");
	gr->SetMarkerColor(9);
	gr->SetMarkerStyle(20);
	gr->Draw("AP");
	cEff->SaveAs("magneticEff.png","png");

}

int main(){
	myGraph();
	return 0;
}
