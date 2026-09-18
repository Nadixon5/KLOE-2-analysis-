using namespace std;

void pidmc_phidec_check()
{
    gStyle->SetOptStat(0);
    gSystem->Load("libPhysics");

    TChain mc("KS_3PI0/h1");
    mc.Add("/home/nadusia/Documents/KLOE-2_analysis/mc/*.root");

    UChar_t nsel[4];
    UInt_t filfowd;
    UShort_t Ecltag;
    Int_t vetocos;
    Int_t Crflag;
    Int_t pidmc[25];
    Int_t phidec;
    UInt_t phiwordmc;
    Int_t ntmc;
    //&... adres zmiennej; przekazuję do funkcji miejsce w pamięci, gdzie znajduje się zmienna.
    mc.SetBranchAddress("nsel", nsel);          //tablica
    mc.SetBranchAddress("vetocos",&vetocos);
    mc.SetBranchAddress("Crflag",&Crflag);
    mc.SetBranchAddress("filfowd", &filfowd);
    mc.SetBranchAddress("Ecltag", &Ecltag);
    mc.SetBranchAddress("pidmc", pidmc);        //tablica
    mc.SetBranchAddress("phidec", &phidec);
    mc.SetBranchAddress("phiwordmc",&phiwordmc);
    mc.SetBranchAddress("ntmc",&ntmc);

    vector<Long64_t> pidCounts(51, 0);

    Long64_t nMC = mc.GetEntries();

    for(Long64_t ie = 0; ie < nMC; ie++)
    {
        mc.GetEntry(ie);

        if(phiwordmc != 1864129)
            continue;

        for(int i = 0; i < ntmc; i++)
        {
            if(pidmc[i] >= 1 && pidmc[i] <= 50)
                pidCounts[pidmc[i]]++;
        }
    }

    const char* particleName[51] = {};

    particleName[1]  = "gamma";
    particleName[2]  = "e+";
    particleName[3]  = "e-";
    particleName[4]  = "neutrino";
    particleName[5]  = "mu+";
    particleName[6]  = "mu-";
    particleName[7]  = "pi0";
    particleName[8]  = "pi+";
    particleName[9]  = "pi-";
    particleName[10] = "K0long";
    particleName[11] = "K+";
    particleName[12] = "K-";
    particleName[13] = "neutron";
    particleName[14] = "proton";
    particleName[15] = "antiproton";
    particleName[16] = "K0short";
    particleName[17] = "eta";
    particleName[18] = "lambda";
    particleName[19] = "sigma+";
    particleName[20] = "sigma0";
    particleName[21] = "sigma-";
    particleName[25] = "antineutrino";
    particleName[26] = "antilambda";
    particleName[27] = "antisigma-";
    particleName[28] = "antisigma0";
    particleName[29] = "antisigma+";
    particleName[45] = "deuteron";
    particleName[46] = "tritium";
    particleName[47] = "alpha";
    particleName[50] = "phi";

    cout << "Particle             PID       Count" << endl;
    cout << "--------------------------------------------------------" << endl;

    for(int pid = 1; pid <= 50; pid++)
    {
        if(pidCounts[pid] > 0)
        {
            cout << pidCounts[pid]
                << "    "
                << particleName[pid]
                << endl;
        }
    }
}