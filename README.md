<h1 align="center">Cosmic Ray Muon Spectrometer</h1>
<p align="center">This project simulates cosmic ray muons shooted to a one foil of THGEM detector. The muons</p>



* Geant4Parameter:
is an energy parameterized Geant4 simulation.
-THGEM is represented by a simple gas volume;
-cuts were set to 250eV in PhysicsList using Penelope model for low eletromagnetic processes;
-magnetic field turned on in implemented class from G4MagneticField
-meuScript.sh runs a loop of Geant4 simulations for 2-5 GeV muon energies 

* enviar_:
are the Garfield++ step of simulation and are build separately for each gas composition to be more easily sent to the Titanio computing cluster from UFABC.

* Particles and Result:
throughout the work progress more combinations of magnetic field, gas composition and THGEM cross section area were required.
Particles/ and Result/ are directory structures and data files needed for Garfield++ simulation and for more easy update of them, the most recent version were left outside of "enviar_". So that when any change is required all that needs to be done is copy them and paste inside "enviar_" folder.

* receber_titanio:
is the data obtained after Garfield++ simulation occurred in Titanio cluster.
