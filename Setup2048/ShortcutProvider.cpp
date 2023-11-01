#include "ShortcutProvider.h"
#include <shlobj.h>
#include <winnls.h>
#include <shobjidl.h>
#include <objbase.h>
#include <objidl.h>
#include <shlguid.h>

// lpszPathObj = pszTargetfile
// lpszPathLink = pszLinkfile

HRESULT ShortcutProvider::Create(
   LPCWSTR pszTargetfile, LPCWSTR pszTargetargs,
   LPCWSTR pszLinkfile, LPCWSTR pszDescription,
   int iShowmode, LPCWSTR pszCurdir,
   LPCWSTR pszIconfile, int iIconindex) {

   HRESULT       hRes;                  /* Returned COM result code */
   IShellLink*   pShellLink;            /* IShellLink object pointer */
   IPersistFile* pPersistFile;          /* IPersistFile object pointer */
   
   hRes = CoInitialize(NULL);
   if (SUCCEEDED(hRes)) {
      hRes = E_INVALIDARG;

      if (
         (pszTargetfile != NULL) && (wcslen(pszTargetfile) > 0) &&
         (pszTargetargs != NULL) &&
         (pszLinkfile != NULL) && (wcslen(pszLinkfile) > 0) &&
         (pszDescription != NULL) &&
         (iShowmode >= 0) &&
         (pszCurdir != NULL) &&
         (pszIconfile != NULL) &&
         (iIconindex >= 0)
         )
      {
         hRes = CoCreateInstance(
            CLSID_ShellLink,      /* pre-defined CLSID of the IShellLink object */
            NULL,                 /* pointer to parent interface if part of aggregate */
            CLSCTX_INPROC_SERVER, /* caller and called code are in same process */
            IID_IShellLink,       /* pre-defined interface of the IShellLink object */
            (LPVOID*)&pShellLink);/* Returns a pointer to the IShellLink object */

         if (SUCCEEDED(hRes)) {
            /* Set the fields in the IShellLink object */
            hRes = pShellLink->SetPath(pszTargetfile);
            hRes = pShellLink->SetArguments(pszTargetargs);

            if (wcslen(pszDescription) > 0) {
               hRes = pShellLink->SetDescription(pszDescription);
            }

            if (iShowmode > 0) {
               hRes = pShellLink->SetShowCmd(iShowmode);
            }

            if (wcslen(pszCurdir) > 0) {
               hRes = pShellLink->SetWorkingDirectory(pszCurdir);
            }

            if (wcslen(pszIconfile) > 0 && iIconindex >= 0) {
               hRes = pShellLink->SetIconLocation(pszIconfile, iIconindex);
            }

            /* Use the IPersistFile object to save the shell link */
            hRes = pShellLink->QueryInterface(
               IID_IPersistFile,         /* pre-defined interface of the IPersistFile object */
               (LPVOID*)&pPersistFile);  /* returns a pointer to the IPersistFile object */

            if (SUCCEEDED(hRes)) {
               hRes = pPersistFile->Save(pszLinkfile, TRUE);
               pPersistFile->Release();
            }

            pShellLink->Release();
         }

      }

      CoUninitialize();
   }


   return (hRes);
}
