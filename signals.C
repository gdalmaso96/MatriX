void ordering(TString name, TString file){
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

	int eventsPerCycle = 100;
	int Ni = (T->GetEntries() / eventsPerCycle);
	if(T->GetEntries()%eventsPerCycle == 0) Ni -= 1;
	int nHalf = 0;
	vector<int> tempCells;
	vector<int> tempChannel;
	vector<double> tempCellTime;

	TFile* N = TFile::Open(file + ".root", "RECREATE");
	TTree* Tnew = new TTree("Tnew", "signals");
	N->cd();
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
	Tnew->Write("T", TObject::kOverwrite);
	N->Close();
	F->Close();
}		

void preprocessing(TString file){
	TFile* F = TFile::Open(file + ".root");
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

	TFile* N = TFile::Open(file + ".root", "RECREATE");

	for(int i = 0; i < 81; i++){
		Tnew.push_back(new TTree(TString::Format("T%02d", i), TString::Format("Ch%02d", i)));
		Tnew.at(i)->Branch("Cell", &Cell);
		Tnew.at(i)->Branch("Time", &Time);
	}


	int Ntot = T->GetEntries();
	for(int i = 0; i < Ntot; i++){
		T->GetEntry(i);
		if(Time - activationTime[Channel][Cell] > 20){
			activationTime[Channel][Cell] = Time;
			Tnew.at(Channel)->Fill();
		}
	}

	F->Close();
	N->cd();
	for(int i = 0; i < 81; i++){
		Tnew.at(i)->Write(TString::Format("Ch%02d", i), TObject::kOverwrite);
	}
	N->Close();
}

double Gp[5] = {8.88913032e-01,  6.46867235e+01,  4.16687779e-01, 212.49027149107357, 1.5};

double C(double x,double a,double b){
	return b * sqrt(TMath::Pi()/2) * exp(b*b/2/a/a - x/a)*(TMath::Erf((a*x - b*b)/(a*b*sqrt(2))) + 1);
}

double Signal(double x){
	return Gp[3]*(C(x - Gp[4], Gp[0], Gp[2]) - C(x - Gp[4], Gp[0]*Gp[1]/(Gp[0]+Gp[1]), Gp[2]));
}

void processing(double threashold, TString file){
	TFile* F = TFile::Open(file + ".root", "UPDATE");
	TTree* T;
	TTree* Twaves = new TTree("waves", "signals");
	
	double deltaT = 200; // ns
	double pitch  = 0.1; // ns

	vector<double> signal;
	vector<double> signalT;
	signal.resize(int(deltaT / pitch) + 1);
	for(int i = 0; i < int(deltaT / pitch) + 1; i++) signalT.push_back(-200 + pitch * i);
	vector<double> activationTime;

	int Channel;
	double signalTime, Amplitude = 0;

	TGraph G;
	
	Twaves->Branch("Signal", &G);
	Twaves->Branch("Channel", &Channel);
	Twaves->Branch("Amplitude", &Amplitude);
	Twaves->Branch("Time", &signalTime);

	int Nsignals = 0, thCheck = 0, timeCheck = - 30;

	double globalTime = 0; // ns, time of the Last Point
	double Time = 0;

	double totalTime = 0;
	
	for(int i = 0; i < 81; i++){
		Nsignals = 0;
		thCheck = 0;
		Channel = i;
		T = (TTree*) F->Get(TString::Format("Ch%02d", i));
		T->SetBranchAddress("Time", &Time);
		int last = -1;
		int n_entries = T->GetEntries();
		for(int j = 0; j < T->GetEntries(); j++){
			if(int(100. * j / n_entries)%10 == 0 && int(100. * j / n_entries)/10 > last){
				TString output = TString::Format("Ch. %02d, advancement: [", i);
				last = int(100. * j / n_entries)/10;
				for(int i = 0; i < 10; i++){
					if (i < last) output = output + "=";
					else if (i == last) output = output + ">";
					else output = output + " ";
				}
				output = output + "]";
				cout << output << endl;
			}

			T->GetEntry(j);
			while(globalTime < Time){
				signal.erase(signal.begin());
				signalT.erase(signalT.begin());
				double amplitude = 0;
				int n = activationTime.size();
				int actual = 0;
				for(int k = 0; k < n; k++){
					amplitude += Signal(globalTime - activationTime.at(actual));
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
					timeCheck = signalT.at(int(deltaT/pitch/2));
					signalTime = globalTime - deltaT/2;
					G = TGraph(signal.size());
					for(int l = 0; l < signal.size(); l++){
						G.SetPoint(l, signalT.at(l), signal.at(l));
					}
					int l = 0;
					double DeltaT = 0;
					while(true){
						if(signal.at(int(deltaT/pitch/2) + l) < threashold){
							DeltaT = signalT.at(int(deltaT/pitch/2) + l) - signalT.at(int(deltaT/pitch/2));
							break;
						}
						l++;
					}
					Amplitude = TMath::MaxElement(int(DeltaT/pitch), &G.GetY()[int(deltaT/pitch/2)]);
					Twaves->Fill();
				}
				else if(signal.at(int(deltaT/pitch/2)) < threashold && thCheck == 1){
// && signalT.at(int(deltaT/pitch/2)) - timeCheck > 20
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
				amplitude += Signal(globalTime - activationTime.at(actual));
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
				timeCheck = signalT.at(int(deltaT/pitch/2));
				signalTime = globalTime - deltaT/2;
				G = TGraph(signal.size());
				for(int l = 0; l < signal.size(); l++){
					G.SetPoint(l, signalT.at(l), signal.at(l));
				}
				int l = 0;
				double DeltaT = 0;
				while(true){
					if(signal.at(int(deltaT/pitch/2) + l) < threashold){
						DeltaT = signalT.at(int(deltaT/pitch/2) + l) - signalT.at(int(deltaT/pitch/2));
						break;
					}
					l++;
				}
				Amplitude = TMath::MaxElement(int(DeltaT/pitch), &G.GetY()[int(deltaT/pitch/2)]);
				Twaves->Fill();
			}
			else if(signal.at(int(deltaT/pitch/2)) < threashold && thCheck == 1){
// && signalT.at(int(deltaT/pitch/2)) - timeCheck > 20
				thCheck = 0;
			}
		}
		signal.clear();
		signal.resize(int(deltaT / pitch) + 1);
		if(globalTime > 0) totalTime = globalTime;
		globalTime = 0;
		timeCheck = -30;
	}
	Twaves->Write("waves", TObject::kOverwrite);
	F->Close();
}

void reprocess(double threashold, TString name){
	TFile* F = TFile::Open(name + ".root");
	TTree* T = (TTree*) F->Get("waves");

	double Amplitude = 0;
	int Channel = 0;
	int Counts[81] = {0};
	double Time = 0, deltaT = 0;

	T->SetBranchAddress("Amplitude", &Amplitude);
	T->SetBranchAddress("Channel", &Channel);
	T->SetBranchAddress("Time", &Time);

	int n_entries = T->GetEntries();
	for(int i = 0; i < n_entries; i++){
		if(i%(n_entries/100) == 0) cout << 100*i/n_entries << " %" << endl;
		T->GetEntry(i);
		if(i == 0) deltaT = Time;
		else if (Time > deltaT) deltaT = Time;
		if(Amplitude >= threashold) Counts[Channel]++;
	}

	ofstream myfile;
	myfile.open(name + ".txt");
	for(int i = 0; i < 81; i++){
		myfile << Counts[i] << endl;
	}
	myfile << 0.22 << endl;
	myfile << deltaT/1e9 << endl;

	F->Close();
}

void signals(){
	ordering("data.root", "out");
	preprocessing("out");
	processing(0.5, "out");
	reprocess(10, "out");
}

void signals(double threashold){
	ordering("data.root", "out");
	preprocessing("out");
	processing(threashold, "out");
	reprocess(threashold, "out");
}

void signals(TString name){
	ordering(name, "out");
	preprocessing("out");
	processing(0.5, "out");
	reprocess(0.5, "out");
}


void signals(TString name, TString out){
	ordering(name, out);
	preprocessing(out);
	processing(0.5, out);
	reprocess(10, out);
}

void signals(TString name, double threashold, TString out){
	ordering(name, out);
	preprocessing(out);
	processing(threashold, out);
	reprocess(threashold, out);
}


