using namespace std;

void ecl_check()
{
    gStyle->SetOptStat(0);
    gSystem->Load("libPhysics");

    TChain mc("KS_3PI0/h1");
    mc.Add("/home/nadusia/Documents/KLOE-2_analysis/mc/*.root");

    TChain data("KS_3PI0/h1");
    data.Add("/home/nadusia/Documents/KLOE-2_analysis/data/*.root");

    Float_t ecl[100];
    Char_t npronti;
    Int_t ncl;
    Int_t flagcl[100];
    Int_t pnum1[100];
    Int_t pnum2[100];
    Int_t pnum3[100];

    Float_t poso[5];
    UInt_t filfowd;
    UShort_t Ecltag;
    UChar_t nsel[4];
    Int_t vetocos;
    Int_t Crflag;

    mc.SetBranchAddress("ecl", ecl);
    mc.SetBranchAddress("npronti", &npronti);
    mc.SetBranchAddress("ncl", &ncl);
    mc.SetBranchAddress("flagcl", flagcl);
    mc.SetBranchAddress("pnum1", pnum1);
    mc.SetBranchAddress("pnum2", pnum2);
    mc.SetBranchAddress("pnum3", pnum3);

    mc.SetBranchAddress("poso", poso);
    mc.SetBranchAddress("Ecltag", &Ecltag);
    mc.SetBranchAddress("nsel", nsel);
    mc.SetBranchAddress("filfowd", &filfowd);
    mc.SetBranchAddress("vetocos", &vetocos);
    mc.SetBranchAddress("Crflag", &Crflag);

    data.SetBranchAddress("ecl", ecl);
    data.SetBranchAddress("npronti", &npronti);
    data.SetBranchAddress("ncl", &ncl);

    data.SetBranchAddress("poso", poso);
    data.SetBranchAddress("Ecltag", &Ecltag);
    data.SetBranchAddress("nsel", nsel);
    data.SetBranchAddress("filfowd", &filfowd);
    data.SetBranchAddress("vetocos", &vetocos);
    data.SetBranchAddress("Crflag", &Crflag);

    TH1F *hECL[9];
    TH1F *hECLd[9];
    TH1F *hECLpo[9];

    for (int n = 1; n <= 8; n++)
    {
        hECL[n] = new TH1F(
            Form("hECL%d", n),
            Form("Cluster energy, npronti = %d;E_{cl} [MeV];Events", n),
            100, 100, 2700
        );
        hECLd[n] = new TH1F(
            Form("hECLd%d", n),
            Form("Cluster energy, npronti = %d;E_{cl} [MeV];Events", n),
            100, 100, 2700
        );
        hECLpo[n] = new TH1F(
            Form("hECLpo%d", n),
            Form("Cluster energy, npronti = %d;E_{cl} [MeV];Events", n),
            100, 100, 2700
        );
    }

    Long64_t Nmc = mc.GetEntries();
    Long64_t Ndata = data.GetEntries();

    for (Long64_t iev = 0; iev < Nmc; iev++)
    {
        mc.GetEntry(iev);

        if (npronti < 1 || npronti > 8) continue;

        // --- ECLTAG ---
        if (Ecltag == 0) continue;

        // --- FILFOWD ---
        if ((filfowd & (1 << 20)) == 0) continue;

        // --- CRFLAG ---
        if (Crflag <= 0) continue;

        // --- VETOCOS ---
        if (vetocos > 0) continue;

        // --- ECL ---
        for (int i = 0; i < ncl; i++)
        {
            // --- FLAGCL ---
            // if (flagcl[i] != 5) continue;

            // --- PNUM1 ---
            if (pnum1[i] == 0 && pnum2[i] == 0 && pnum3[i] == 0){
                hECL[npronti]->Fill(ecl[i]);
            }
            else
            hECLpo[npronti]->Fill(ecl[i]);
        }
    }

    

    for (Long64_t iev = 0; iev < Ndata; iev++)
    {
        data.GetEntry(iev);

        if (npronti < 1 || npronti > 8) continue;

        // --- ECLTAG ---
        if (Ecltag == 0) continue;

        // --- FILFOWD ---
        if ((filfowd & (1 << 20)) == 0) continue;

        // --- CRFLAG ---
        if (Crflag <= 0) continue;

        // --- VETOCOS ---
        if (vetocos > 0) continue;

        // --- ECL ---
        for (int i = 0; i < ncl; i++)
        {
            hECLd[npronti]->Fill(ecl[i]);
        }
    }

    TFile *out = new TFile(
        "KLOE_analysis_results/mc_ecl_check.root",
        "RECREATE"
    );

    for (int n = 1; n <= 8; n++)
    {
        hECL[n]->Write();
        hECLpo[n]->Write();
        hECLd[n]->Write();
    }

    out->Close();

    TString pdf = "KLOE_analysis_results/mc_ecl_check_pnum1_pnum2_pnum3_NEW.pdf";

    TCanvas *c = new TCanvas("c", "c", 900, 700);

     c->Print(pdf + "[");
    for(int n=1; n<=8; n++)
    {
        c->Clear();
        c->SetLogy();

        hECL[n]->SetLineColor(kRed);
        hECL[n]->SetLineWidth(2);
        hECL[n]->SetStats(0);

        hECLpo[n]->SetLineColor(kRed+1);
        hECLpo[n]->SetLineStyle(2);
        hECLpo[n]->SetLineWidth(2);

        hECLd[n]->SetMarkerStyle(20);
        hECLd[n]->SetMarkerSize(0.7);
        hECLd[n]->SetMarkerColor(kBlack);
        hECLd[n]->SetLineColor(kBlack);

        hECLd[n]->Draw("E1");
        hECL[n]->Draw("HIST SAME");
        hECLpo[n]->Draw("HIST SAME");

        TLegend *leg = new TLegend(0.55,0.68,0.88,0.88);

        leg->AddEntry(hECL[n],"MC pnum1, pnum2, pnum3 = 0","l");
        leg->AddEntry(hECLpo[n],"MC pnum1, pnum2, pnum3 /= 0","l");
        leg->AddEntry(hECLd[n],"Data","lep");
        leg->SetTextSize(0.03);
        leg->SetBorderSize(0);
        leg->Draw();

        c->Modified();
        c->Update();

        c->Print(pdf);

        delete leg;

    }
    c->Print(pdf + "]");
    
}
