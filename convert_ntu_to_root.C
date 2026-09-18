#include <TSystem.h>
#include <TSystemDirectory.h>
#include <TSystemFile.h>
#include <TList.h>

#include <iostream>

void convert_ntu_to_root(
    const char* inputDir =
        "/run/media/nadusia/ADATA_HV620/FIBM15/signal_newt0_for_preliminaryv38",
    const char* outputDir =
        "/home/nadusia/Documents/Dane/Dane3")
{
    gSystem->mkdir(outputDir, kTRUE);

    TSystemDirectory dir("input", inputDir);
    TList* files = dir.GetListOfFiles();

    if (!files)
    {
        std::cerr << "Blad odczytu katalogu." << std::endl;
        return;
    }

    Int_t total = 0;
    Int_t converted = 0;
    Int_t failed = 0;
    Int_t skipped = 0;

    TIter next(files);
    TSystemFile* file;

    while ((file = (TSystemFile*)next()))
    {
        TString fname = file->GetName();

        if (file->IsDirectory())
            continue;

        if (!fname.EndsWith(".ntu"))
            continue;

        total++;

        TString inputFile =
            TString(inputDir) + "/" + fname;

        TString outputFile =
            TString(outputDir) + "/" +
            fname.ReplaceAll(".ntu", ".root");

        // pomiń jeśli root już istnieje
        if (!gSystem->AccessPathName(outputFile))
        {
            skipped++;
            continue;
        }

        // brak wypisywania logów h2root
        TString cmd =
            "h2root \"" + inputFile +
            "\" \"" + outputFile +
            "\" > /dev/null 2>&1";

        Int_t status = gSystem->Exec(cmd);

        if (status == 0)
            converted++;
        else
            failed++;

        // raport co 500 plików
        if ((converted + failed) % 500 == 0)
        {
            std::cout
                << "Przetworzono: "
                << (converted + failed)
                << " plikow..."
                << std::endl;
        }
    }

    std::cout << "\n====================================\n";
    std::cout << "KONIEC\n";
    std::cout << "Wszystkich .ntu : " << total << "\n";
    std::cout << "Przekonwertowane: " << converted << "\n";
    std::cout << "Pominiete       : " << skipped << "\n";
    std::cout << "Bledy           : " << failed << "\n";
    std::cout << "====================================\n";
}