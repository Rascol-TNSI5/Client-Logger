#include <windows.h>
#include <shlobj.h>
#include <objbase.h>
#include <shobjidl.h>

void add_winstart(const char *repertoireLogiciel, const char *nomExecutable) {
    // Chemin du dossier de démarrage de Windows
    char cheminDossierDemarrage[MAX_PATH];
    HRESULT resultat = SHGetFolderPath(NULL, CSIDL_STARTUP, NULL, 0, cheminDossierDemarrage);
    if (resultat != S_OK) {
        printf("Erreur lors de la récupération du chemin du dossier de démarrage.\n");
        return;
    }

    // Chemin complet du raccourci
    char cheminRaccourci[MAX_PATH];
    snprintf(cheminRaccourci, MAX_PATH, "%s\\%s.lnk", cheminDossierDemarrage, nomExecutable);

    // Chemin complet de l'exécutable
    char cheminExecutable[MAX_PATH];
    snprintf(cheminExecutable, MAX_PATH, "%s\\%s", repertoireLogiciel, nomExecutable);

    // Initialiser COM
    CoInitialize(NULL);

    // Créer l'objet IShellLink
    IShellLink *pShellLink = NULL;
    resultat = CoCreateInstance(&CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, &IID_IShellLink, (void**)&pShellLink);
    if (SUCCEEDED(resultat)) {
        // Définir le chemin de l'exécutable
        pShellLink->lpVtbl->SetPath(pShellLink, cheminExecutable);

        // Obtenir l'interface IPersistFile
        IPersistFile *pPersistFile = NULL;
        resultat = pShellLink->lpVtbl->QueryInterface(pShellLink, &IID_IPersistFile, (void**)&pPersistFile);
        if (SUCCEEDED(resultat)) {
            // Convertir le chemin du raccourci en wchar_t
            wchar_t wsz[MAX_PATH];
            MultiByteToWideChar(CP_ACP, 0, cheminRaccourci, -1, wsz, MAX_PATH);

            // Enregistrer le raccourci
            resultat = pPersistFile->lpVtbl->Save(pPersistFile, wsz, TRUE);
            pPersistFile->lpVtbl->Release(pPersistFile);
        }
        pShellLink->lpVtbl->Release(pShellLink);
    }

    // Libérer COM
    CoUninitialize();
}