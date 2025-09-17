void plot_exem(){


	auto *file = new TFile("test.root","READ");
	auto *tree = (TTree*)file->Get("spec");
	auto *itree= (TTree*)file->Get("info");
	float wavelength, counts;
	float start, stop, step, exem;
	tree->SetBranchAddress("wavelength",&wavelength);
	tree->SetBranchAddress("counts",&counts);
	itree->SetBranchAddress("start",&start);
	itree->SetBranchAddress("stop",&stop);
	itree->SetBranchAddress("step",&step);
	itree->SetBranchAddress("exem",&exem);

	itree->GetEntry(0);
	
	float xmin, xmax, xstep, xbin;
	xstep = step;
	xmin = start - 0.5*xstep; xmax = stop + 0.5*xstep;
	xbin = xmax - xmin;

	auto *can = new TCanvas("can","can",1200,1200);
	auto *his = new TH1D("his","his",xbin,xmin,xmax);

	int nentry = tree->GetEntries();
	cout << "Number of Entry : " << nentry << endl;

	float temp_w;

	for(int i = 0; i<nentry; i++){
		tree->GetEntry(i);
		temp_w = 0;
		temp_w = (wavelength-start) + 1;
		his->SetBinContent(temp_w,counts);
	}

	his->Draw();
	his->GetXaxis()->SetTitle("Excitation Wavelength (nm)");
	// his->GetXaxis()->SetTitle("Emission Wavelength (nm)");
	his->GetXaxis()->CenterTitle(true);
	his->GetXaxis()->SetTitleOffset(1.2);
	his->GetYaxis()->SetTitle("Intensity (arb. unit)");
	his->GetYaxis()->CenterTitle(true);
	his->GetYaxis()->SetTitleOffset(1.2);
	his->SetLineWidth(3);
	his->SetStats(0);
	can->SetGrid();

}
