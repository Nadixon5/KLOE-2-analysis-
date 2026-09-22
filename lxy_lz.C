using namespace std;

void lxy_lz()
{
    gStyle->SetOptStat(0);
    gSystem->Load("libPhysics");

    TChain mc("KS_3PI0/h1");
    mc.Add("/home/nadusia/Documents/KLOE-2_analysis/mc/*.root");

    Float_t lxy;
    Float_t lz;
    Char_t npronti;
    Float_t poso[5];
    UInt_t filfowd;
    UShort_t Ecltag;
    UChar_t nsel[4];
    Int_t vetocos;
    Int_t Crflag;

    mc.SetBranchAddress("lxy", &lxy);
    mc.SetBranchAddress("lz", &lz);
    mc.SetBranchAddress("npronti",&npronti);
    mc.SetBranchAddress("poso",poso);
    mc.SetBranchAddress("Ecltag", &Ecltag);
    mc.SetBranchAddress("nsel", nsel);
    mc.SetBranchAddress("filfowd", &filfowd);
    mc.SetBranchAddress("vetocos",&vetocos);
    mc.SetBranchAddress("Crflag",&Crflag);

    //TH2D *hlxylz = new TH2D("hlxylz","lz vs lxy;lz;lxy",100, -220, 220,100, 0, 300);

    TH2D *hlxylz[10];
    TH2D *hlxylzEn[10];

    for (int n = 1; n <= 9; n++)
    {
        hlxylz[n] = new TH2D(
            Form("hlxylz%d", n),
            Form("lz vs lxy, npronti = %d;lz;lxy", n),
            100, -220, 220,
            100, 0, 300
        );

        hlxylzEn[n] = new TH2D(
            Form("hlxylzEn%d", n),
            Form("lz vs lxy, npronti = %d, Ecrash > 650 MeV;lz;lxy", n),
            100, -220, 220,
            100, 0, 300
        );
    }

    Long64_t Nmc = mc.GetEntries();
    for(Long64_t iev=0; iev<Nmc; iev++)
    {
        mc.GetEntry(iev);

        //---ECLTAG---//
        if(Ecltag == 0) continue;

        //---FILFOWD---//
        if ((filfowd & (1 << 20)) == 0) continue;

        //---CRFLAG---//
        if(Crflag<=0) continue;

        //---VETOCOS---//
        if(vetocos>0) continue;

        //--- TRACK VETO ---//
        //if(nsel[0] > 0) continue;

        int n = (int)npronti;

        if (n < 1 || n > 9) continue;

        hlxylz[n]->Fill(lz, lxy);

        // With Ecrash > 650 MeV
        double Ecrash = poso[4];
        if (Ecrash > 650)
        {
            hlxylzEn[n]->Fill(lz, lxy);
        }
    }

    TFile fout("KLOE_analysis_results/lxy_lz.root","RECREATE");
    // hlxylz->Write();
    // fout.Close();

    for (int n = 1; n <= 9; n++)
    {
        hlxylz[n]->Write();
        hlxylzEn[n]->Write();
    }

    fout.Close();

    TCanvas *c = new TCanvas("c","lz vs lxy",800, 700);

    c->Print("KLOE_analysis_results/lxy_lz_bez_trackveto.pdf[");

    // Without Ecrash cut
    for (int n = 1; n <= 9; n++)
    {
        hlxylz[n]->SetStats(0);
        hlxylz[n]->Draw("COLZ");

        c->Modified();
        c->Update();
        c->Print("KLOE_analysis_results/lxy_lz_bez_trackveto.pdf");
    }

    // With Ecrash > 650 MeV
    for (int n = 1; n <= 9; n++)
    {
        hlxylzEn[n]->SetStats(0);
        hlxylzEn[n]->Draw("COLZ");

        c->Modified();
        c->Update();
        c->Print("KLOE_analysis_results/lxy_lz_bez_trackveto.pdf");
    }

    c->Print("KLOE_analysis_results/lxy_lz_bez_trackveto.pdf]");

    delete c;

}