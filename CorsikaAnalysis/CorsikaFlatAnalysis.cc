#include "CorsikaAnalysis.h"

/*

Corsika Flat ROOT

TTree sim :
   vector<int>     *CorsikaID;
   vector<int>     *ParticleID;
   vector<int>     *PDGID;
   vector<int>     *ObservationLevel;
   vector<int>     *HadronicGeneration;
   vector<double>  *Px;
   vector<double>  *Py;
   vector<double>  *Pz;
   vector<double>  *E;
   vector<double>  *x;
   vector<double>  *y;
   vector<double>  *Time;
   vector<double>  *Weight;

*/

int main(int argc, char *argv[]) {
    	// Argument check
	if (argc != 2) {
		std::cout << "[Error] Wrong number of arguments" << std::endl;
		std::cout << "Usage : ./CorsikaFlatAnalysis.exe <Run Number> " << std::endl;
		return 0;
	}

    // Load CORSIKA file
	std::string runNum = argv[1];
	TFile* aFile = getFlatInputFile(runNum);

	// Load shower info
	TTree* aTree = (TTree*) aFile->Get("sim");
    std::cout << "Total Event : " << aTree->GetEntries() << std::endl << std::endl;
	// containners
	std::vector<int>* branch_PDGID = new std::vector<int>(0);
	std::vector<double>* branch_Px = new std::vector<double>(0);
	std::vector<double>* branch_Py = new std::vector<double>(0);
	std::vector<double>* branch_Pz = new std::vector<double>(0);
	std::vector<double>*  branch_E = new std::vector<double>(0);
	std::vector<double>*  branch_x = new std::vector<double>(0);
	std::vector<double>*  branch_y = new std::vector<double>(0);
	std::vector<double>* branch_Time = new std::vector<double>(0);
	aTree->SetBranchAddress("PDGID", &branch_PDGID);
	aTree->SetBranchAddress("Px", &branch_Px);
	aTree->SetBranchAddress("Py", &branch_Py);
	aTree->SetBranchAddress("Pz", &branch_Pz);
	aTree->SetBranchAddress("E", &branch_E);
	aTree->SetBranchAddress("x", &branch_x);
	aTree->SetBranchAddress("y", &branch_y);
	aTree->SetBranchAddress("Time", &branch_Time);

	// Histogram declaration
	TH1D* ptc_E_total  = new TH1D("Ptc_energy", "Ptc_energy;GeV;Ptc", 2000, 0, 2000);
	TH1D* ptc_ID_total = new TH1D("Ptc_PDGID", "Ptc_PDGID;ID;Ptc", 195, 1, 196); // CORSIKA ID : 1 ~ 196, different from PDGID
	TH1D* ptc_T_total  = new TH1D("Ptc_time", "Ptc_time;ns;Ptc/10ns", 100000, 0, 1000000);
	TH2D* ptc_XY_total = new TH2D("Ptc_XY", "Ptc_XY;m;m", 4000, -20000, 20000, 4000, -20000, 20000);
	// Container for histogram per shower
	std::vector<TH1D*> ptc_E;
	std::vector<TH1D*> ptc_ID;
	std::vector<TH1D*> ptc_T;
	std::vector<TGraph*> ptc_XY;
	for (int iEvt = 0; iEvt < aTree->GetEntries(); iEvt++) {
		ptc_E. push_back( new TH1D(Form("Ptc_energy_%d", iEvt), Form("Ptc_energy_%d;GeV;Ptc", iEvt), 2000, 0, 2000) );
		ptc_ID.push_back( new TH1D(Form("Ptc_PDGID_%d", iEvt), Form("Ptc_PDGID_%d;ID;Ptc", iEvt), 195, 1, 196) );
		ptc_T. push_back( new TH1D(Form("Ptc_time_%d", iEvt), Form("Ptc_time_%d;ns;Ptc/10ns", iEvt), 100000, 0, 1000000) );
		ptc_XY.push_back( new TGraph() ); ptc_XY.at(iEvt)->SetNameTitle( Form("Ptc_XY_%d" ,iEvt), Form("Ptc_XY_%d;cm;cm" ,iEvt) );
	}

	// Shower loop (== Event loop)
	int nPtc = 0;
	for (int iEvt =0; iEvt < aTree->GetEntries(); iEvt++) {
		aTree->GetEntry(iEvt);
        std::cout << "Evt # : " << iEvt << " total # of ptc : " << branch_PDGID->size() << std::endl;
        int nPtcPerEvent = branch_PDGID->size();

        for (int idx = 0; idx < nPtcPerEvent; idx++) {
			ptc_XY.at(iEvt)->SetPoint(idx, branch_x->at(idx), branch_y->at(idx)); ptc_XY_total->Fill(branch_x->at(idx), branch_y->at(idx));
			ptc_E.at(iEvt)->Fill( branch_E->at(idx) );  ptc_E_total->Fill( branch_E->at(idx) );
			ptc_ID.at(iEvt)->Fill( branch_PDGID->at(idx) ); ptc_ID_total->Fill( branch_PDGID->at(idx) );
			ptc_T.at(iEvt)->Fill( branch_Time->at(idx) );		 ptc_T_total->Fill( branch_Time->at(idx) );
			nPtc++;
        }
    }
    std::cout << "Total number of particles per shower   : " << nPtc << std::endl;
	std::cout << "Average number of particles per shower : " << nPtc / aTree->GetEntries() << std::endl;

	// Open output root file
	std::stringstream stream;
	stream <<  std::setw(6) << std::setfill('0') << runNum;
	TFile* outputFile = TFile::Open( (TString)("./Hist_Flat_CORSIKA_" + stream.str() + ".root"), "RECREATE");
	outputFile->cd();
	ptc_XY_total->Write();
	ptc_E_total ->Write();
	ptc_ID_total->Write();
	ptc_T_total ->Write();
	for (int iEvt = 0; iEvt < aTree->GetEntries(); iEvt++) {
		ptc_XY.at(iEvt)->Write();
		ptc_E.at(iEvt) ->Write(); 
		ptc_ID.at(iEvt)->Write();
		ptc_T.at(iEvt) ->Write();
	}
	outputFile->Close();

	return 0;
}