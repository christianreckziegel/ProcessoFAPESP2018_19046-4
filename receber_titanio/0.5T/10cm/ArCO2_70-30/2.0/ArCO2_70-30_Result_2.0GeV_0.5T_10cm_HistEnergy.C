void ArCO2_70-30_Result_2.0GeV_0.5T_10cm_HistEnergy()
{
//=========Macro generated from canvas: cE/Canvas Energy
//=========  (Thu Nov 26 16:36:30 2020) by ROOT version 6.14/06
   TCanvas *cE = new TCanvas("cE", "Canvas Energy",67,57,700,500);
   cE->Range(-1.25,-1.3125,11.25,11.8125);
   cE->SetFillColor(0);
   cE->SetBorderMode(0);
   cE->SetBorderSize(2);
   cE->SetFrameBorderMode(0);
   cE->SetFrameBorderMode(0);
   
   TH1D *Energy__1 = new TH1D("Energy__1","Estimated energy for a 2.0GeV muon under 0.5T ArCO_{2} (70-30)",37,0,10);
   Energy__1->SetBinContent(2,1);
   Energy__1->SetBinContent(4,1);
   Energy__1->SetBinContent(5,9);
   Energy__1->SetBinContent(6,8);
   Energy__1->SetBinContent(7,10);
   Energy__1->SetBinContent(8,7);
   Energy__1->SetBinContent(9,5);
   Energy__1->SetBinContent(10,7);
   Energy__1->SetBinContent(11,4);
   Energy__1->SetBinContent(12,1);
   Energy__1->SetBinContent(14,1);
   Energy__1->SetBinContent(25,1);
   Energy__1->SetBinContent(34,1);
   Energy__1->SetBinContent(35,1);
   Energy__1->SetBinContent(38,41);
   Energy__1->SetEntries(98);
   
   TPaveStats *ptstats = new TPaveStats(0.78,0.775,0.98,0.935,"brNDC");
   ptstats->SetName("stats");
   ptstats->SetBorderSize(1);
   ptstats->SetFillColor(0);
   ptstats->SetTextAlign(12);
   ptstats->SetTextFont(42);
   TText *ptstats_LaTex = ptstats->AddText("Energy");
   ptstats_LaTex->SetTextSize(0.0368);
   ptstats_LaTex = ptstats->AddText("Entries = 98     ");
   ptstats_LaTex = ptstats->AddText("Mean  =  2.264");
   ptstats_LaTex = ptstats->AddText("Std Dev   =  1.566");
   ptstats->SetOptStat(1111);
   ptstats->SetOptFit(0);
   ptstats->Draw();
   Energy__1->GetListOfFunctions()->Add(ptstats);
   ptstats->SetParent(Energy__1);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#000099");
   Energy__1->SetLineColor(ci);
   Energy__1->GetXaxis()->SetTitle("E(GeV)");
   Energy__1->GetXaxis()->SetLabelFont(42);
   Energy__1->GetXaxis()->SetLabelSize(0.035);
   Energy__1->GetXaxis()->SetTitleSize(0.035);
   Energy__1->GetXaxis()->SetTitleFont(42);
   Energy__1->GetYaxis()->SetTitle("Entries");
   Energy__1->GetYaxis()->SetLabelFont(42);
   Energy__1->GetYaxis()->SetLabelSize(0.035);
   Energy__1->GetYaxis()->SetTitleSize(0.035);
   Energy__1->GetYaxis()->SetTitleOffset(0);
   Energy__1->GetYaxis()->SetTitleFont(42);
   Energy__1->GetZaxis()->SetLabelFont(42);
   Energy__1->GetZaxis()->SetLabelSize(0.035);
   Energy__1->GetZaxis()->SetTitleSize(0.035);
   Energy__1->GetZaxis()->SetTitleFont(42);
   Energy__1->Draw("");
   
   TPaveText *pt = new TPaveText(0.15,0.9166842,0.85,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   TText *pt_LaTex = pt->AddText("Estimated energy for a 2.0GeV muon under 0.5T ArCO_{2} (70-30)");
   pt->Draw();
   cE->Modified();
   cE->cd();
   cE->SetSelected(cE);
}
