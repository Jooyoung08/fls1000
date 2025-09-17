void plot_QY_powder(){


	auto *file_direct   = new TFile("./20250916_root/20250916_LiBZ_powder_ex312_Direct_QY.root","READ");
	auto *file_blank    = new TFile("./20250916_root/20250916_LiBZ_powder_ex312_blank_QY.root","READ");
	auto *file_indirect = new TFile("./20250916_root/20250916_LiBZ_powder_ex312_Indirect_QY.root","READ");

	auto *tree_direct  = (TTree*)file_direct->Get("spec");
	auto *tree_blank   = (TTree*)file_blank->Get("spec");
	auto *tree_indirect= (TTree*)file_indirect->Get("spec");
	auto *itree_direct = (TTree*)file_direct->Get("info");

	float wavelength, counts;
	float start, stop, step, exem;

	tree_direct->SetBranchAddress("wavelength",&wavelength);
	tree_direct->SetBranchAddress("counts",&counts);
	tree_blank->SetBranchAddress("wavelength",&wavelength);
	tree_blank->SetBranchAddress("counts",&counts);
	tree_indirect->SetBranchAddress("wavelength",&wavelength);
	tree_indirect->SetBranchAddress("counts",&counts);

	itree_direct->SetBranchAddress("start",&start);
	itree_direct->SetBranchAddress("stop",&stop);
	itree_direct->SetBranchAddress("step",&step);
	itree_direct->SetBranchAddress("exem",&exem);

	itree_direct->GetEntry(0);
	
	float xmin, xmax, xstep, xbin;
	xstep = step;
	xmin = start - 0.5*xstep; xmax = stop + 0.5*xstep;
	xbin = xmax - xmin;

	//Range
	float exmin, exmax; float emmin, emmax;
	//values
	float ex_bl, ex_di, ex_in, em_bl, em_di, em_in;
	//Excitation Range
	exmin = (305 - start) + 1; exmax = (320 - start) + 1;
	emmin = (325 - start) + 1; emmax = (600 - start) + 1;

	auto *can = new TCanvas("can","test",1200,1200);
	auto *his_di = new TH1D("his_di","Quantum Yield",xbin,xmin,xmax);
	auto *his_in = new TH1D("his_in","Quantum Yield",xbin,xmin,xmax);
	auto *his_bl = new TH1D("his_bl","Quantum Yield",xbin,xmin,xmax);

	int dientry = tree_direct->GetEntries();
	cout << "Number of Entry : " << dientry << endl;

	float temp_w;

	for(int i = 0; i<dientry; i++){
		tree_blank->GetEntry(i);
		temp_w = 0;
		temp_w = (wavelength-start)/xstep + 1;
		his_bl->SetBinContent(temp_w,counts);
	}

	for(int i = 0; i<dientry; i++){
		tree_indirect->GetEntry(i);
		temp_w = 0;
		temp_w = (wavelength-start) + 1;
		his_in->SetBinContent(temp_w,counts);
	}

	for(int i = 0; i<dientry; i++){
		tree_direct->GetEntry(i);
		temp_w = 0;
		temp_w = (wavelength-start) + 1;
		his_di->SetBinContent(temp_w,counts);
	}

	//QY calculation
	ex_bl = 0; ex_di = 0; ex_in = 0;
	em_bl = 0; em_di = 0; em_in = 0;
	for(int a = exmin; a<exmax; a++){
		ex_bl += his_bl->GetBinContent(a);
		ex_di += his_di->GetBinContent(a);
		ex_in += his_in->GetBinContent(a);
	}
	for(int b = emmin; b<emmax; b++){
		em_bl += his_bl->GetBinContent(b);
		em_di += his_di->GetBinContent(b);
		em_in += his_in->GetBinContent(b);
	}

	cout << "EXs : " << ex_bl << ", " << ex_di << ", " << ex_in << endl;
	cout << "Ems : " << em_bl << ", " << em_di << ", " << em_in << endl;

	float qy, qy_01, qy_02, qy_03;
	qy = 0; qy_01 = 0; qy_02 = 0; qy_03 = 0;
	qy_01 = ex_di*(em_in-em_bl);
	qy_02 = ex_in*(em_di-em_bl);
	qy_03 = ex_bl*(ex_di-ex_in);
	qy = (qy_01-qy_02)/qy_03;

	cout << qy_01 << ", " << qy_02 << ", " << qy_03 << endl;
	cout << "Q.Y. is " << qy*100 << " %" << endl;


	//Draw
	his_bl->Draw();
	his_bl->GetXaxis()->SetTitle("Emission Wavelength (nm)");
	his_bl->GetXaxis()->CenterTitle(true);
	his_bl->GetXaxis()->SetTitleOffset(1.2);
	his_bl->GetYaxis()->SetTitle("Intensity (arb. unit)");
	his_bl->GetYaxis()->CenterTitle(true);
	his_bl->GetYaxis()->SetTitleOffset(1.2);
	his_bl->SetLineWidth(3);
	his_bl->SetLineColor(3);

	his_di->Draw("same");
	his_di->SetLineColor(2);
	his_di->SetLineWidth(3);
	his_in->Draw("same");
	his_in->SetLineWidth(3);
	his_in->SetLineColor(4);
	can->SetGrid();
	can->SetLogy();
	gStyle->SetOptStat(0);

	auto *legend = new TLegend(0.7,0.7,0.9,0.9);
	legend->AddEntry(his_bl, "Blank", "l");
	legend->AddEntry(his_di, "Direct", "l");
	legend->AddEntry(his_in, "Indirect", "l");
	legend->Draw();

	TString result("test");
	result.Form("Quantum Yield is %.2f %%", qy*100);
	auto *latex = new TLatex();
	latex->SetNDC();
	latex->SetTextSize(0.04);
	latex->DrawLatex(0.3,0.8,result);

}
