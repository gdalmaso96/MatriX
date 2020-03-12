void ordering(TString name){
	TFile* F = TFile::Open(name);
	TTree* T = (TTree*) F->Get("T");

	vector<int>* Cells = 0;
	vector<int>* Channel = 0;
	vector<double>* CellTime = 0;

	vector<int> CellsAll;
	vector<int> ChannelAll;
	vector<double> CellTimeAll;
	int NCells;

	T->SetBranchAddress("Cells", &Cells);
	T->SetBranchAddress("Channel", &Channel);
	T->SetBranchAddress("CellTime", &CellTime);
	T->SetBranchAddress("NCells", &NCells);
	int eventsPerCycle = 100;
	int Ni = (T->GetEntries() / eventsPerCycle);
	if(T->GetEntries()%eventsPerCycle == 0) Ni -= 1;
	int nHalf = 0;
	vector<int> tempCells;
	vector<int> tempChannel;
	vector<double> tempCellTime;

	TFile* N = TFile::Open("output.root", "RECREATE");
	TTree* Tnew = new TTree("T", "output");
	int sCells = 0, sChannel = 0;
	double sCellTime = 0;

	Tnew->Branch("Cells", &sCells);
	Tnew->Branch("Channel", &sChannel);
	Tnew->Branch("CellTime", &sCellTime);

	for(int i = 0; i < Ni; i++){
		for(int k = i * eventsPerCycle; k < eventsPerCycle * (i + 1); k++){
			T->GetEntry(k);
			int n = Cells->size();
			if(n > 0){
				for(int j = 0; j < n; j++){
					CellsAll.push_back(Cells->at(j));
					ChannelAll.push_back(Channel->at(j));
					CellTimeAll.push_back(CellTime->at(j));
				}
			}
		}
		Cells->clear();
		Channel->clear();
		CellTime->clear();

		int n = CellsAll.size();
		double CellTimeL[n];
		int I[n];

		for(int i = 0; i < n; i++){
			CellTimeL[i] = CellTimeAll.at(i);
		}

		CellTimeAll.clear();
		TMath::Sort(n, CellTimeL, I, false);
		
		int tempnHalf = n/2;
		if(i == 0){
			for(int j = 0; j < tempnHalf; j++){
				sCells = CellsAll.at(I[j]);
				sChannel = ChannelAll.at(I[j]);
				sCellTime = CellTimeAll[I[j]];
				Tnew->Fill();
			}

			for(int j = tempnHalf; j < n; j++){
				tempCells.push_back(CellsAll.at(I[j]));
				tempChannel.push_back(ChannelAll.at(I[j]));
				tempCellTime.push_back(CellTimeL[I[j]]);
			}
			nHalf = n - tempnHalf;
		}

		else{
			for(int j = 0; j < tempnHalf; j++){
				tempCells.push_back(CellsAll.at(I[j]));
				tempChannel.push_back(ChannelAll.at(I[j]));
				tempCellTime.push_back(CellTimeL[I[j]]);
			}

			int ntemp = tempCellTime.size();
			double tempCellTimeL[ntemp];
			int tempI[ntemp];
			for(int j = 0; j < ntemp; j++){
				tempCellTimeL[j] = tempCellTime.at(j);
			}
			tempCellTime.clear();

			TMath::Sort(ntemp, tempCellTimeL, tempI, false);

			for(int j = 0; j < ntemp; j++){
				sCells = tempCells.at(tempI[j]);
				sChannel = tempChannel.at(tempI[j]);
				sCellTime = tempCellTimeL[tempI[j]];
				Tnew->Fill();
			}
			tempCells.clear();
			tempChannel.clear();

			nHalf = n - tempnHalf;
			for(int j = tempnHalf; j < n; j++){
				tempCells.push_back(CellsAll.at(j));
				tempChannel.push_back(ChannelAll.at(j));
				tempCellTime.push_back(CellTimeL[j]);
			}
		}
		CellsAll.clear();
		ChannelAll.clear();
	}

	for(int i = Ni*eventsPerCycle; i < T->GetEntries(); ++i){
		T->GetEntry(i);
		int n = Cells->size();
		if(n > 0){
			for(int j = 0; j < n; j++){
				CellsAll.push_back(Cells->at(j));
				ChannelAll.push_back(Channel->at(j));
				CellTimeAll.push_back(CellTime->at(j));
			}
		}
	}
	Cells->clear();
	Channel->clear();
	CellTime->clear();

	int n = CellsAll.size();
	double CellTimeL[n];
	int I[n];

	for(int i = 0; i < n; i++){
		CellTimeL[i] = CellTimeAll.at(i);
	}

	CellTimeAll.clear();
	TMath::Sort(n, CellTimeL, I, false);
	
	for(int j = 0; j < n; j++){
		tempCells.push_back(CellsAll.at(I[j]));
		tempChannel.push_back(ChannelAll.at(I[j]));
		tempCellTime.push_back(CellTimeL[I[j]]);
	}

	CellsAll.clear();
	ChannelAll.clear();

	int ntemp = tempCellTime.size();
	double tempCellTimeL[ntemp];
	int tempI[ntemp];
	for(int j = 0; j < ntemp; j++){
		tempCellTimeL[j] = tempCellTime.at(j);
	}
	tempCellTime.clear();

	TMath::Sort(ntemp, tempCellTimeL, tempI, false);

	for(int j = 0; j < ntemp; j++){
		sCells = tempCells.at(tempI[j]);
		sChannel = tempChannel.at(tempI[j]);
		sCellTime = tempCellTimeL[tempI[j]];
		Tnew->Fill();
	}

	tempCells.clear();
	tempChannel.clear();


	N->cd();
	N->Write();
	N->Close();
	F->Close();
}		

void preprocessing(){
	TFile* F = TFile::Open("./output.root");
	TTree* T = (TTree*) F->Get("T");

	double activationTime[81][2668];
	for(int i = 0; i < 81; i++){
		for(int j = 0; j < 2668; j++){
			activationTime[i][j] = -30; // ns
		}
	}

	vector<TTree*> Tnew;
	int Cell = 0, Channel;
	double Time = 0;

	T->SetBranchAddress("Cells", &Cell);
	T->SetBranchAddress("Channel", &Channel);
	T->SetBranchAddress("CellTime", &Time);

	for(int i = 0; i < 81; i++){
		Tnew.push_back(new TTree());
		Tnew.at(i)->Branch("Cell", &Cell);
		Tnew.at(i)->Branch("Time", &Time);
	}

	for(int i = 0; i < T->GetEntries(); i++){
		T->GetEntry(i);
		if(Time - activationTime[Channel][Cell] > 20){
			activationTime[Channel][Cell] = Time;
			Tnew.at(Channel)->Fill();
		}
	}

	F->Close();
	TFile* N = TFile::Open("./output.root", "RECREATE");
	N->cd();
	for(int i = 0; i < 81; i++){
		Tnew.at(i)->Write(TString::Format("Ch%02d", i), TObject::kOverwrite);
	}
	N->Close();
}

double expo(double t, double offset){
	if(t > offset) return TMath::Exp(-(t - offset) / 15.);
	else return 0;
}

void processing(double threashold){
	TFile* F = TFile::Open("./output.root");
	TTree* T;
	TTree* Twaves = new TTree();
	
	double deltaT = 200; // ns
	double pitch  = 0.1; // ns

	vector<double> signal;
	vector<double> signalT;
	signal.resize(int(deltaT / pitch) + 1);
	for(int i = 0; i < int(deltaT / pitch) + 1; i++) signalT.push_back(-200 + pitch * i);
	vector<double> activationTime;

	int Channel;
	double signalTime;
	
	Twaves->Branch("Wave", &signal);
	Twaves->Branch("WaveT", &signalT);
	Twaves->Branch("Channel", &Channel);
	Twaves->Branch("Time", &signalTime);

	int Nsignals = 0, thCheck = 0;

	double globalTime = 0; // ns, time of the Last Point
	double Time = 0;

	double totalTime = 0;

	ofstream myfile;
	myfile.open("out.txt");

	for(int i = 0; i < 81; i++){
		Nsignals = 0;
		thCheck = 0;
		Channel = i;
		T = (TTree*) F->Get(TString::Format("Ch%02d", i));
		T->SetBranchAddress("Time", &Time);
		for(int j = 0; j < T->GetEntries(); j++){
			T->GetEntry(j);
			while(globalTime < Time){
				signal.erase(signal.begin());
				signalT.erase(signalT.begin());
				double amplitude = 0;
				int n = activationTime.size();
				int actual = 0;
				for(int k = 0; k < n; k++){
					amplitude += expo(globalTime, activationTime.at(actual));
					if(globalTime - activationTime.at(actual) > 30){ 
						activationTime.erase(activationTime.begin() + actual);
						actual --;
					}
					actual ++;
				}
				signal.push_back(amplitude);
				globalTime += pitch;
				signalT.push_back(globalTime);
				if(signal.at(int(deltaT/pitch/2)) > threashold && thCheck == 0){
					Nsignals += 1;
					thCheck = 1;
					signalTime = globalTime - deltaT/2;
					Twaves->Fill();
				}
				else if(signal.at(int(deltaT/pitch/2)) < threashold && thCheck == 1){
					thCheck = 0;
				}
			}
			activationTime.push_back(Time);
		}

		for(int j = 0; j < int(deltaT/pitch/2); j++){
			signal.erase(signal.begin());
			signalT.erase(signalT.begin());
			double amplitude = 0;
			int n = activationTime.size();
			int actual = 0;
			for(int k = 0; k < n; k++){
				amplitude += expo(globalTime, activationTime.at(actual));
				if(globalTime - activationTime.at(actual) > 30){ 
					activationTime.erase(activationTime.begin() + actual);
					actual --;
				}
				actual ++;
			}
			signal.push_back(amplitude);
			globalTime += pitch;
			signalT.push_back(globalTime);
			if(signal.at(int(deltaT/pitch/2)) > threashold && thCheck == 0){
				Nsignals += 1;
				thCheck = 1;
				signalTime = globalTime - deltaT/2;
				Twaves->Fill();
			}
			else if(signal.at(int(deltaT/pitch/2)) < threashold && thCheck == 1){
				thCheck = 0;
			}
		}
		myfile << Nsignals << endl;
		signal.clear();
		signal.resize(int(deltaT / pitch) + 1);
		if(globalTime > 0) totalTime = globalTime;
		globalTime = 0;
	}
	myfile << 0.22 << endl;
	myfile << totalTime/1e9 << endl;
	myfile.close();
	F->Close();

	TFile* N = TFile::Open("./output.root", "UPDATE");
	Twaves->Write("waves", TObject::kOverwrite);
	N->Close();
}


void signals(){
	ordering("data.root");
	preprocessing();
	processing(10);
}

void signals(double threashold){
	ordering("data.root");
	preprocessing();
	processing(threashold);
}

void signals(TString name){
	ordering(name);
	preprocessing();
	processing(10);
}

void signals(TString name, double threashold){
	ordering(name);
	preprocessing();
	processing(threashold);
}


