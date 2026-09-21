using namespace std;

void mcsig_ks3pi0_check()
{
    gStyle->SetOptStat(0);
    gSystem->Load("libPhysics");

    TChain mcsig("KS_3PI0/h1");
    mcsig.Add("/home/nadusia/Documents/KLOE-2_analysis/mc_sig/*.root");

    Float_t poso[5];
    Char_t npronti;
    UInt_t Kswordmc;
    UChar_t nsel[4];
    UInt_t filfowd;
    UShort_t Ecltag;
    Int_t vetocos;
    Int_t Crflag;

    mcsig.SetBranchAddress("poso", poso);
    mcsig.SetBranchAddress("npronti", &npronti);
    mcsig.SetBranchAddress("nsel", nsel);
    mcsig.SetBranchAddress("vetocos",&vetocos);
    mcsig.SetBranchAddress("Crflag",&Crflag);
    mcsig.SetBranchAddress("filfowd", &filfowd);
    mcsig.SetBranchAddress("Ecltag", &Ecltag);
    mcsig.SetBranchAddress("Kswordmc", &Kswordmc);

    TH1F *hEcrashMCsig_All[9];
    TH1F *hEcrashMCsig_Veto[9];

    for(int n=0; n<=8; n++)
    {
        hEcrashMCsig_All[n] = new TH1F(Form("hEcrashMC_All_npronti%d",n),
                     Form("KL crash energy, npronti = %d;E_{crash} [MeV];Events",n),
                     100,100,2800);

        hEcrashMCsig_Veto[n] = new TH1F(Form("hEcrashMC_Veto_npronti%d",n),
                     Form("KL crash energy after veto, npronti = %d;E_{crash} [MeV];Events",n),
                     100,100,2800);
    }

    Long64_t Nmcsig = mcsig.GetEntries();

    for(Long64_t ie=0; ie<Nmcsig; ie++)
    {
        mcsig.GetEntry(ie);

        double Ecrash = poso[4];

        if(npronti < 0 || npronti > 8) continue;

        if(Kswordmc != 460551) continue;

        //---ECLTAG---//
        if(Ecltag == 0) continue;

        //---FILFOWD---//
        if ((filfowd & (1 << 20)) == 0) continue;

        //---CRFLAG---//
        if(Crflag<=0) continue;

        //---VETOCOS---//
        if(vetocos>0) continue;

        hEcrashMCsig_All[npronti]->Fill(Ecrash);

        //---TRACK VETO---//
        if(nsel[0] > 0) continue;

        hEcrashMCsig_Veto[npronti]->Fill(Ecrash);

    }

    TFile *out = new TFile("KLOE_analysis_results/mcsig_energy_check.root","RECREATE");

    for(int n=0; n<=8; n++)
    {
        hEcrashMCsig_All[n]->Write();
        hEcrashMCsig_Veto[n]->Write();
    }
    out->Close();

    TCanvas *c = new TCanvas("c", "c", 900, 700);

    TString pdf = "KLOE_analysis_results/mcsig_energy_check.pdf";

    c->Print(pdf + "[");
    for(int n=0; n<=8; n++)
    {
        c->Clear();
        // c->SetLogy();

        hEcrashMCsig_All[n]->SetLineColor(kRed);
        hEcrashMCsig_All[n]->SetLineWidth(2);

        hEcrashMCsig_Veto[n]->SetLineColor(kRed+1);
        hEcrashMCsig_Veto[n]->SetLineStyle(2);
        hEcrashMCsig_Veto[n]->SetLineWidth(2);

        hEcrashMCsig_All[n]->SetStats(0);

        hEcrashMCsig_All[n]->Draw("HIST");
        hEcrashMCsig_Veto[n]->Draw("HIST SAME");

        TLegend *leg = new TLegend(0.65,0.68,0.88,0.88);

        leg->AddEntry(hEcrashMCsig_All[n],"MC sig","l");
        leg->AddEntry(hEcrashMCsig_Veto[n],"MC sig after track veto","l");

        leg->SetBorderSize(0);
        leg->Draw();

        c->Modified();
        c->Update();

        c->Print(pdf);

        delete leg;
    }

    c->Print(pdf + "]");

    delete c;


}
