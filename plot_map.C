void plot_map(){

	auto *file = new TFile("test_map.root","READ");
	auto *tree = (TTree*)file->Get("spec");
	auto *itree= (TTree*)file->Get("info");
	float wavelength, counts, ex;
	float start, stop, step, estart, estop, estep;
	tree->SetBranchAddress("wavelength",&wavelength);
	tree->SetBranchAddress("counts",&counts);
	tree->SetBranchAddress("ex",&ex);
	itree->SetBranchAddress("start",&start);
	itree->SetBranchAddress("stop",&stop);
	itree->SetBranchAddress("step",&step);
	itree->SetBranchAddress("estart",&estart);
	itree->SetBranchAddress("estop",&estop);
	itree->SetBranchAddress("estep",&estep);

	float xmin, xmax, xbin, ymin, ymax, ybin;
	float xstep, ystep;
	itree->GetEntry(0);
	//Wavelength(start, stop, step)
	xstep = step; xmin = start - xstep*0.5; xmax = stop + xstep*0.5;
	//Excitation(estart, estop, estep)
	ystep = estep; ymin = estart - ystep*0.5; ymax = estop + ystep*0.5;
	//Bin
	xbin = (xmax-xmin-xstep)/xstep + 1;
	ybin = (ymax-ymin-ystep)/ystep + 1;
	cout << xbin << ", " << ybin << endl;

	auto *can = new TCanvas("can","can",1200,1200);
	auto *his = new TH2D("his","Ex & Em Map",xbin,xmin,xmax,ybin,ymin,ymax);

	int nentry = tree->GetEntries();
	cout << "Number of Entry : " << nentry << endl;

	float temp_w, temp_e;

	for(int i = 0; i<nentry; i++){
		tree->GetEntry(i);
		temp_w = 0; temp_e = 0;
		temp_w = (wavelength-xmin)/xstep + 1;
		temp_e = (ex-ymin)/ystep + 1;
		his->SetBinContent(temp_w,temp_e,counts);
	}

	his->Draw();
	his->GetXaxis()->SetTitle("Emission Wavelength (nm)");
	his->GetXaxis()->CenterTitle(true);
	his->GetXaxis()->SetTitleOffset(1.2);
	his->GetYaxis()->SetTitle("Excitation Wavelength (nm)");
	his->GetYaxis()->CenterTitle(true);
	his->GetYaxis()->SetTitleOffset(1.2);
	his->SetStats(0);
	can->SetGrid();

}
