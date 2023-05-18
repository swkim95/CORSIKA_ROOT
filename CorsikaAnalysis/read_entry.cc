#include "CorsikaAnalysis.h"

int main(int argc, char *argv[]) {
    // Argument check
	if (argc != 2) {
		std::cout << "[Error] Wrong number of arguments" << std::endl;
		std::cout << "Usage : ./read_entry.exe <Run Number> " << std::endl;
		return 0;
	}

    // Load CORSIKA file
	std::string runNum = argv[1];
	TFile* aFile = getFlatInputFile(runNum);
    // Load event tree
    TTree* aTree = (TTree*) aFile->Get("sim");
	std::vector<int>* branch_CorsikaID = new std::vector<int>(0);
    aTree->SetBranchAddress("CorsikaID", &branch_CorsikaID);

    int nPtc = 0;
    std::cout << "Total # of shower(=event) : " << aTree->GetEntries() << std::endl;
    for(int iEvt = 0; iEvt < aTree->GetEntries(); iEvt++) {
        aTree->GetEntry(iEvt);
		std::cout << "Evt # : " << iEvt << " total # of ptc : " << branch_CorsikaID->size() << std::endl;
        nPtc += branch_CorsikaID->size();
    }
    std::cout << "Total number of particles per shower   : " << nPtc << std::endl;
	std::cout << "Average number of particles per shower : " << nPtc / aTree->GetEntries() << std::endl;

    return 0;
}