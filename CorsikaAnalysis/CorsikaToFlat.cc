#include "CorsikaAnalysis.h"

int main(int argc, char *argv[]) {
	// Argument check
	if (argc != 2) {
		std::cout << "[Error] Wrong number of arguments" << std::endl;
		std::cout << "Usage : ./CorsikaAnalysis.exe <Run Number> " << std::endl;
		return 0;
	}
	
	// Load CORSIKA file
	std::string runNum = argv[1];
	TFile* aFile = getInputFile(runNum);

	// Load shower info (crsIO::TShower class, which contains information of particles in same shower)
	TTree* aTree = (TTree*) aFile->Get("sim");
	crsIO::TShower* pShower = new crsIO::TShower();
	aTree->SetBranchAddress("shower.", &pShower);
	std::cout << "Total Event : " << aTree->GetEntries() << std::endl << std::endl;

	// containners
	std::vector<int>* branch_CorsikaID = new std::vector<int>(0);
	std::vector<int>* branch_ParticleID = new std::vector<int>(0);
	std::vector<int>* branch_PDGID = new std::vector<int>(0);
	std::vector<int>* branch_ObservationLevel = new std::vector<int>(0);
	std::vector<int>* branch_HadronicGeneration = new std::vector<int>(0);
	std::vector<double>* branch_Px = new std::vector<double>(0);
	std::vector<double>* branch_Py = new std::vector<double>(0);
	std::vector<double>* branch_Pz = new std::vector<double>(0);
	std::vector<double>*  branch_E = new std::vector<double>(0);
	std::vector<double>*  branch_x = new std::vector<double>(0);
	std::vector<double>*  branch_y = new std::vector<double>(0);
	std::vector<double>* branch_Time = new std::vector<double>(0);
	std::vector<double>* branch_Weight = new std::vector<double>(0);

	std::stringstream stream;
	stream <<  std::setw(6) << std::setfill('0') << runNum;
	TFile* outputFile = TFile::Open( (TString)("./Flat_CORSIKA_" + stream.str() + ".root"), "RECREATE");
	TTree* outTree = new TTree("sim", "CORSIKA_sim");
	outTree->Branch("CorsikaID", &branch_CorsikaID);
	outTree->Branch("ParticleID", &branch_ParticleID);
	outTree->Branch("PDGID", &branch_PDGID);
	outTree->Branch("ObservationLevel", &branch_ObservationLevel);
	outTree->Branch("HadronicGeneration", &branch_HadronicGeneration);
	outTree->Branch("Px", &branch_Px);
	outTree->Branch("Py", &branch_Py);
	outTree->Branch("Pz", &branch_Pz);
	outTree->Branch("E", &branch_E);
	outTree->Branch("x", &branch_x);
	outTree->Branch("y", &branch_y);
	outTree->Branch("Time", &branch_Time);
	outTree->Branch("Weight", &branch_Weight);

	// Shower loop (== Event loop)
	int nPtc = 0;
	for (int iEvt =0; iEvt < aTree->GetEntries(); iEvt++) {
		branch_CorsikaID->clear();
		branch_ParticleID->clear();
		branch_PDGID->clear();
		branch_ObservationLevel->clear();
		branch_HadronicGeneration->clear();
		branch_Px->clear();
		branch_Py->clear();
		branch_Pz->clear();
		branch_E->clear();
		branch_x->clear();
		branch_y->clear();
		branch_Time->clear();
		branch_Weight->clear();
		
		aFile->cd();
		aTree->GetEntry(iEvt);
		// Get particle data (in TClonesArray with class crsIO::TParticle)
		TClonesArray shower_particles = pShower->GetParticles();
		std::cout << "Evt # : " << iEvt << " total # of ptc : " << shower_particles.GetEntries() << std::endl;

		//Loop on particles in same shower
		crsIO::TParticle* ptc;
		for (int idx = 0; idx < shower_particles.GetEntries(); idx++) {
			ptc = (crsIO::TParticle*) shower_particles.At(idx);
			branch_CorsikaID->push_back(ptc->CorsikaID);
			branch_ParticleID->push_back(ptc->ParticleID);
			branch_PDGID->push_back(getPDGID(ptc));
			branch_ObservationLevel->push_back(ptc->ObservationLevel);
			branch_HadronicGeneration->push_back(ptc->HadronicGeneration);
			branch_Px->push_back(ptc->Px);
			branch_Py->push_back(ptc->Py);
			branch_Pz->push_back(ptc->Pz);
			branch_E->push_back(calculateE(ptc));
			branch_x->push_back(ptc->x);
			branch_y->push_back(ptc->y);
			branch_Time->push_back(ptc->Time);
			branch_Weight->push_back(ptc->Weight);
			nPtc++;
		}
		outputFile->cd();
		outTree->Fill();
	}
	std::cout << "Total number of particles per shower   : " << nPtc << std::endl;
	std::cout << "Average number of particles per shower : " << nPtc / aTree->GetEntries() << std::endl;

	// Open output root file
	outputFile->cd();
	outTree->Write();
	outputFile->Close();
	return 0;
}
