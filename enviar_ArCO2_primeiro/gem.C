/* This code simulates an avalanche from particles_energy_Bfield_size.root entries.
It takes an integer as a parameter n, such that entries from 10*n until 10*n+10 are 
simulated.
Generates a result_energy_Bfield_size_n.root file containing initial, final positions, gain and event 
respective to each ionization*/


#include <iostream>
#include <fstream>

#include <TApplication.h>
#include <TTree.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TH1F.h>
#include <TH2.h>

#include "Garfield/ComponentElmer.hh"
#include "Garfield/ComponentAnalyticField.hh"
#include "Garfield/ViewField.hh"
#include "Garfield/ViewFEMesh.hh"
#include "Garfield/ViewCell.hh"
#include "Garfield/ViewSignal.hh"
#include "Garfield/MediumMagboltz.hh"
#include "Garfield/Sensor.hh"
#include "Garfield/AvalancheMicroscopic.hh"
#include "Garfield/AvalancheMC.hh"
#include "Garfield/Random.hh"
#include "Garfield/Plotting.hh"
#include "Garfield/ViewMedium.hh"

using namespace Garfield;
using namespace std;
/*
class Particle{
  public:
    //retorna posição
    float GetX(){ return x; }
    float GetY(){ return y; }
    float GetZ(){ return z; }
    //retorna vetor direção
    float GetDX(){ return dx; }
    float GetDY(){ return dy; }
    float GetDZ(){ return dz; }
    //retorna energia
    double GetEnergy(){ return energy; }
    
    //muda posição
    void SetX(float newX){ x = newX; }
    void SetY(float newY){ y = newY; }
    void SetZ(float newZ){ z = newZ; }
    //muda vetor direção
    void SetDX(float newDX){ dx = newDX; }
    void SetDY(float newDY){ dy = newDY; }
    void SetDZ(float newDZ){ dz = newDZ; }
    //muda energia
    void SetEnergy(double newEnergy){ energy = newEnergy; }
    
    
  private:
    float x, y, z; // posição
    float dx, dy, dz; //vetor direção
    double energy;
}

*/

//cria struct de nome PARTICLE para conter os dados do elétron inicial da ionização
typedef struct {
    Double_t energy;
    Float_t x, y, z;
    Float_t dx, dy, dz;
    Int_t eventNumber;
} PARTICLE;

int main(int argc, char * argv[]) {
  float t;
  t = clock();
  //argumentos passados pelo terminal ao executável
  //entrada no particles.root a ser lida (de 10 em 10)
  int index = int(atoi(argv[1]));
  std::string strIndex = std::string(argv[1]);
  int iniAvalanche = index*10;
  int finalAvalanche = iniAvalanche+10;
  
  //energia do múon
  //double muonEnergy = atof(argv[1]);
  //std::string strMuonEnergy = std::string(argv[1]);
  double muonEnergy = 2.0;
  std::string strMuonEnergy = "2.0";
  //campo magnético
  //double B = atof(argv[2]);
  //std::string strB = std::string(argv[2]);
  double B = 0.001; //em tesla
  std::string strB = "1m";
  //tamanho: metade da aresta
  //double metArest = atof(argv[3]);
  //std::string strMetArest = std::string(argv[3]);
  double metArest = 20.;
  std::string strMetArest = "20";
  
  std::string strGas = "ArCO2_70-30";
  
  //TApplication app("app", &argc, argv); //sem isso não plota
  //plottingEngine.SetDefaultStyle();
  //Garfield::randomEngine.Seed(2);
  // Dimensions of the GEM [cm]
  const double pitch = 0.1;//0.0003; distancia entre centros de 2 buracos adjacentes
  //const double kapton = 50.e-4;
  //const double metal = 5.e-4;
  //const bool debug = true;
  const double pad_side = 0.95;

  // Setup the gas.
  MediumMagboltz* gas = new MediumMagboltz();
  gas->SetComposition("ar", 70., "co2", 30.);
  gas->SetTemperature(293.15);
  gas->SetPressure(760.); // 1 atm in [Torr]
  gas->EnableDrift();
  gas->SetMaxElectronEnergy(10000.); //10keV
  gas->EnablePenningTransfer(0.6, 0);
  //gas->LoadGasFile("xe_70_co2_30.gas");
  
  
  
  ComponentElmer* elm = new ComponentElmer(
     "thgem/mesh.header", 
     "thgem/mesh.elements", 
     "thgem/mesh.nodes",
     "thgem/dielectrics.dat", 
     "thgem/thgem.result", "cm");
  
  elm->EnablePeriodicityY();
  elm->EnableMirrorPeriodicityX();
  elm->PrintRange();
  elm->SetMedium(0, gas);
  elm->SetMagneticField(0., 0., B); // Magnetic field in Tesla
  
  
  
  // Set up a sensor object.
  Sensor* sensor = new Sensor();
  sensor->AddComponent(elm);
  sensor->SetArea(-metArest -1., -metArest -1., -1.5, metArest + 1., metArest + 1.,  1.5); //(-x,-y,-z,x,y,z)
  
  const double tmin = 0., tmax = 200;
  const int nTimeBins = 5000;
  const double tstep = (tmax-tmin)/nTimeBins;

  sensor->SetTimeWindow(tmin, tstep, nTimeBins);

  // Create an avalanche object
  AvalancheMicroscopic* aval = new AvalancheMicroscopic();
  aval->SetSensor(sensor);
  aval->EnableSignalCalculation();
  aval->SetCollisionSteps(100);
  aval->EnableMagneticField();
  // Set up the object for drift line visualization.
  //ViewDrift* viewDrift = new ViewDrift();
  //aval->EnablePlotting(viewDrift);
  
  
  bool verCampo = false;
  if(verCampo){
    TCanvas* eField = new TCanvas("eField", "Potential Plot");
    //visualizando o mapa do campo elétrico ao longo do eixo z
    ViewField* fieldView = new ViewField();
    fieldView->SetSensor(sensor);
    fieldView->SetCanvas(eField);
    fieldView->SetComponent(elm);
    // Plot the potential along the hole axis.
    //fieldView->PlotProfile(0.025, 0., 0.02, 0.025, 0., -0.02);
    // Set the normal vector (0, -1, 0) of the viewing plane in the middle of the hole of coordinates (0.025,0.,0.)
    fieldView->SetPlane(0., -1., 0., 0.025, 0., 0.);
    // Plot the potential along the plane of the foil
    fieldView->SetArea(-pitch / 2., -0.02, pitch / 2., 0.02);
    //fieldView->SetNumberOfContours(40);
    //fieldView->EnableAutoRange(true); didn't worked
    fieldView->SetVoltageRange(-3700., 0.); // originally -160 to 160
    // Creating a contour, surface or other two-dimensional plot in the chosen viewing plane
    fieldView->PlotContour("v"); //pode ser "e" p/ campo elétrico ou "v" para potencial ou nada
    //fieldView->PlotSurface("v");
    //fieldView->Plot("v", "arr");
    eField->Draw();
    //If you want to see the field lines, you can use root, but you need to use the data in *.dat file and write you own code.
    //eField->SaveAs(campoEletrico.png);
  }

  
  
  
  //Coleta de dados de "particles.root"
  std::string particlesROOT = "Particles/"
  +strB+"T/"
  +strMetArest+"cm/"
  +strGas+
  "/particles_"+strMuonEnergy+"GeV_"+strB+"T_"+strMetArest+"cm.root"; //particle file name
  // Open an existing file for reading (default) = READ
  TFile *inputFile = TFile::Open(particlesROOT.c_str());
  TTree* theTree = (TTree*) inputFile->Get("Particles");
  TBranch* electron_branch = theTree->GetBranch("electron_branch");
  PARTICLE electron;
  electron_branch->SetAddress(&electron);

  int n_events = theTree->GetEntries();
  if(finalAvalanche > n_events){
    finalAvalanche = n_events;
  }
  double x0, y0, z0, xi, yi, zi, xa, ya, incxa, incya, ea, x1, y1, z1, ti, t1, ei, e1;
  int status, np, eventN;

  //TH2* multiplied_hist = new TH2D("h1","Electrons after multiplied", /* X-dimension */ 10, -1.5, 1.5, /* Y-dimension */ 10, -1.5, 1.5);

  double bin_width = 0.05; // In centimeters
  double x_start = -metArest, x_end = metArest;
  int n_bins = (x_end - x_start)/bin_width;

  TH1D *readout_x = new TH1D("readout_x","X (cm) position of electrons",n_bins,x_start,x_end);
  TH1D *readout_y = new TH1D("readout_y","Y (cm) position of electrons",n_bins,x_start,x_end);
  TH2* multiplied_hist = new TH2D(/*name*/"2D_histogram", 
  				  /*title*/"Histograma plano x-y",
  				  /*x-axis*/n_bins,x_start,x_end,
  				  /*y-axis*/n_bins,x_start,x_end);
  				  
  
  // Creating the file and Tree object that will contain the results
  TString fileName = "Result/"
  +strB+"T/"
  +strMetArest+"cm/"
  +strGas+"/"
  +strMuonEnergy+"/"
  +strGas+"_Result_"+strMuonEnergy+"GeV_"+strB+"T_"+strMetArest+"cm_"+strIndex+".root"; //result file name
  TFile * resultFile = new TFile(fileName,"RECREATE","Results from THGEM simulation");
  TTree* resultTree = new TTree("Results","Tree that contains the electrons from the avalanche");
  resultTree->Branch("Avalanche_xi",&x0);
  resultTree->Branch("Avalanche_yi",&y0);
  resultTree->Branch("Avalanche_x",&xa);
  resultTree->Branch("Avalanche_y",&ya);
  resultTree->Branch("Avalanche_incx",&incxa);
  resultTree->Branch("Avalanche_incy",&incya);
  resultTree->Branch("Number_of_Electrons_endpoint",&np);
  resultTree->Branch("Event_number_in_Geant4",&eventN);

  for (int i = iniAvalanche; i < finalAvalanche; i++) {
    theTree->GetEntry(i);
    
    if(i==0){
    xi=0.;
    yi=0.;
    zi=1.;

    x0=0.;
    y0=0.;
    }
    else if(i==1){
    xi=0.;
    yi=0.;
    zi=-1.;

    x0=0.;
    y0=0.;
    }
    else{
    xi=electron.x;
    yi=electron.y;
    zi=electron.z;

    x0=electron.x;
    y0=electron.y;
    }
    //ei = electron.energy;
    ei = 0.;
    eventN = electron.eventNumber;
    
    //calculate an electron avalanche
    aval->AvalancheElectron(xi, yi, zi, 0., ei, electron.dx, electron.dy, electron.dz);
    std::cout << "X coordinate: " << xi << std::endl;
    std::cout << "Y coordinate: " << yi << std::endl;
    std::cout << "Z coordinate: " << zi << std::endl;
    std::cout << "Mono energy: " << ei << std::endl;
    std::cout << "Avalanche number " << i << " is done." << std::endl;
    //get the number of electron drift lines
    np=aval->GetNumberOfElectronEndpoints();
    std::cout << "There were " << np << " electron drift lines." << std::endl;
    std::cout << "Looping over the avalanche electrons..." << std::endl;
    
    double xincMax = -500.;
    double xincMin = 500.;
    double yincMax = -500.;
    double yincMin = 500.;
    
    //loop over the avalanche electrons
    for (int j=0; j<np; j++){
        aval->GetElectronEndpoint(j, xi, yi, zi, ti, ei, x1, y1, z1, t1, e1, status);
        //futuramente: criar arquivo .root para armazenar dados referente a cada avalanche (informações dos elétrons)
        readout_x->Fill(x1);
        readout_y->Fill(y1);
        multiplied_hist->Fill(x1,y1);
        //coleta os valores máximos e mínimos da posição em x e em y
        if(x1 > xincMax){
            xincMax = x1;
        }
        if(x1 < xincMin){
            xincMin = x1;
        }
        if(y1 > yincMax){
            yincMax = y1;
        }
        if(y1 < yincMin){
            yincMin = y1;
        }
    }
    
    //visualização electron velocity
    bool viewMed = false;
    if(viewMed){
    //TCanvas* cMedium = new TCanvas("mediumView", "View medium");
    ViewMedium* mediumView = new ViewMedium();
    //mediumView->SetCanvas(cMedium);
    mediumView->SetMedium(gas);
    mediumView->PlotElectronVelocity('e');

    }
    
    // Cálculo da posição final da avalanche
    int e_total_x = 0, e_total_y = 0, e_temp_x, e_temp_y;
    double x_parcial = 0, y_parcial = 0;
    for (int k = 1; k <= n_bins; k++) {
        //cout << endl;
    	e_temp_x = readout_x->GetBinContent(k);
        e_total_x += e_temp_x;
        //somatório da média ponderada
        x_parcial += (x_start + (k*1.0-0.5)*bin_width)*e_temp_x;

	e_temp_y = readout_y->GetBinContent(k);
        e_total_y += e_temp_y;
        y_parcial += (x_start + (k*1.0-0.5)*bin_width)*e_temp_y;/*
        cout << "Houveram " << np << "electron end points." << endl;
        cout << "Bin número: " << k << endl;
        cout << "e_temp_x=" << e_temp_x << "\t" << "e_total_x=" << e_total_x << "\t" 
             << "e_temp_y=" << e_temp_y << "\t" << "e_total_y=" << e_total_y << "\t" << endl;*/
    }
    //calculo da incerteza da posição (i)
    if (e_total_x != 0 && e_total_y != 0){
    	    //posição obtida a partir da média ponderada
            //xa = x_parcial/e_total_x;
	    //ya = y_parcial/e_total_y;
	    xa = readout_x->GetMean();
	    ya = readout_y->GetMean();
	    //incxa = readout_x->GetMeanError();
	    //incya = readout_y->GetMeanError();
	    //incxa = readout_x->GetRMSError();
	    //incya = readout_y->GetRMSError();
	    //incerteza é a metade da largura que a avalanche se espalhou no pad
	    //incxa = (xincMax-xincMin)/2.;
	    //incya = (yincMax-yincMin)/2.;
	    //desvio padrão de uma população gaussiana é o RMS value
	    incxa = readout_x->GetRMS();
	    incya = readout_y->GetRMS();
	    cout << "xincMax=" << xincMax << "\t" << "xincMin=" << xincMin << endl;
	    cout << "yincMax=" << yincMax << "\t" << "yincMin=" << yincMin << endl;
            //cout << "Da média ponderada manual: xa=" << xa << "\t" << "ya=" << ya << endl;
            cout << "Da média de TH1: xa=" << xa << "+-"  << incxa <<"\t" << "ya=" << ya << "+-" << incya << endl;
            cout << endl;
            resultTree->Fill();
    }
    readout_x->Reset();
    readout_y->Reset();
  }
  
  TCanvas* Histograma2d = new TCanvas("2dHist", "Plot 2D da avalanche");
  multiplied_hist->SetTitle("2D Histogram");
  multiplied_hist->GetXaxis()->SetTitle("x(cm)");
  multiplied_hist->GetYaxis()->SetTitle("y(cm)");
  multiplied_hist->SetStats(false);
  
  multiplied_hist->Draw();
  std::string Hist = "Images/2DHistogram_"+strMuonEnergy+"GeV_"+strB+"T_"+strMetArest+"cm.png";
  Histograma2d->SaveAs(Hist.c_str());
  
  multiplied_hist->Draw("COLZ");
  std::string Histcolz = "Images/2DHistogram_"+strMuonEnergy+"GeV_"+strB+"T_"+strMetArest+"cm_colz.png";
  Histograma2d->SaveAs(Histcolz.c_str());
  
    
  
  
  
  resultTree->Print();
  resultFile->Write();
  resultFile->Close();
  inputFile->Close();

  std::cout << "Simulation done!" << std::endl;
  t=clock()-t;
  std::cout << "It took " << t/CLOCKS_PER_SEC << " seconds to run" << std::endl;

  
  //app.Run(kTRUE);
  return 0;
}
