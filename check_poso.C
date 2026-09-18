void check_poso()
{
    TChain chain("KS_3PI0/h1");
    chain.Add("Documents/Dane/Dane1/*.root");

    chain.Scan("poso[0]","","",20);
}