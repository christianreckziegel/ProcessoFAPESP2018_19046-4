void myQuanti(){
   // demo for quantiles
   const Int_t nq = 2;
   TH1F *h = new TH1F("h","demo quantiles",100,-3,3);
   h->FillRandom("gaus",5000);
 
   //Double_t xq[nq];  // position where to compute the quantiles in [0,1]
   Double_t xq[] = {0.25,0.75}; 
   Double_t yq[nq];  // array to contain the quantiles
   /*std::cout << "xq[] = [";
   for (Int_t i=0;i<nq;i++){ 
   	xq[i] = Float_t(i+1)/nq; 
   	std::cout << ", " << xq[i];
   }
   std::cout << "]" << std::endl;*/
   h->GetQuantiles(nq,yq,xq);
   std::cout << "yq[] = ["; 
   for (Int_t i=0;i<nq;i++){ 
   	std::cout << ", " << yq[i];
   }
   std::cout << "]" << std::endl;
   
   //show the original histogram in the top pad
   TCanvas *c1 = new TCanvas("c1","demo quantiles",10,10,700,900);
   c1->Divide(1,2);
   c1->cd(1);
   h->Draw();
 
   // show the quantiles in the bottom pad
   c1->cd(2);
   gPad->SetGrid();
   TGraph *gr = new TGraph(nq,xq,yq);
   gr->SetMarkerStyle(21);
   gr->Draw("alp");
}

int main(){
	void myQuanti();
	return 0;
}
