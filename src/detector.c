#include <stdio.h>
#include <string.h>
#include "detectFile.h"

/// Definitions of known signatures
const unsigned char pdfSig[] = { '%', 'P', 'D', 'F', '-' };
const unsigned char jpgSig[] = { 0xFF, 0xD8, 0xFF };
const unsigned char pngSig[] = { 0x89, 'P', 'N', 'G', 0x0D, 0x0A, 0x1A, 0x0A };
const unsigned char gifSig[] = { 'G', 'I', 'F', '8' };
const unsigned char zipSig[] = { 'P', 'K', 0x03, 0x04 };
const unsigned char mp3Sig[] = { 0xFF, 0xFB };
const unsigned char exeSig[] = { 'M', 'Z' }; 
const unsigned char tiffLE[] = { 0x49, 0x49, 0x2A, 0x00 }; // Little-endian
const unsigned char tiffBE[] = { 0x4D, 0x4D, 0x00, 0x2A }; // Big-endian
const unsigned char mp4Sig[] = { 0x66, 0x74, 0x79, 0x70 }; // "ftyp"
const unsigned char movSig[] = { 0x6D, 0x6F, 0x6F, 0x76 }; // "moov"
const unsigned char htmlSig1[] = { 0x3C, 0x21, 0x44, 0x4F, 0x43 };       // <!DOC
const unsigned char htmlSig2[] = { 0x3C, 0x68, 0x74, 0x6D, 0x6C };       // <html
const unsigned char cssSig[] = { 0x2F, 0x2A };                        // /*
const unsigned char javaSig[] = { 'p', 'a', 'c', 'k', 'a', 'g', 'e' }; // package
const unsigned char phpSig[] = { 0x3C, 0x3F, 0x70, 0x68, 0x70 };      // <?php
const unsigned char jsonSig1[] = { 0x7B };                              
const unsigned char jsonSig2[] = { 0x5B };                              
const unsigned char xmlSig[] = { 0x3C, 0x3F, 0x78, 0x6D, 0x6C };      // <?xml
const unsigned char docSig[] = { 0xD0, 0xCF, 0x11, 0xE0, 0xA1, 0xB1, 0x1A, 0xE1 }; // DOC
const unsigned char xlsSig[] = { 0xD0, 0xCF, 0x11, 0xE0, 0xA1, 0xB1, 0x1A, 0xE1 }; // XLS
const unsigned char pptSig[] = { 0xD0, 0xCF, 0x11, 0xE0, 0xA1, 0xB1, 0x1A, 0xE1 }; // PPT
const unsigned char rarSig[] = { 0x52, 0x61, 0x72, 0x21, 0x1A, 0x07, 0x00 };       // RAR
const unsigned char z7Sig[] = { 0x37, 0x7A, 0xBC, 0xAF, 0x27, 0x1C };             // 7Z
const unsigned char isoSig[] = { 0x43, 0x44, 0x30, 0x30, 0x31 };                   // ISO
const unsigned char mp3Sig1[] = { 0xFF, 0xFB };                                     // MP3 (MPEG)
const unsigned char mp3Sig2[] = { 0x49, 0x44, 0x33 };                               // MP3 (ID3)
const unsigned char wavSig[] = { 0x52, 0x49, 0x46, 0x46 };                         // WAV (RIFF)
const unsigned char flacSig[] = { 0x66, 0x4C, 0x61, 0x43 };                         // FLAC
const unsigned char opusSig[] = { 0x4F, 0x70, 0x75, 0x73, 0x48, 0x65, 0x61, 0x64 }; // Opus
const unsigned char bmpSig[] = { 0x42, 0x4D };                                     // BMP
const unsigned char psdSig[] = { 0x38, 0x42, 0x50, 0x53 };                         // PSD
const unsigned char ttfSig[] = { 0x00, 0x01, 0x00, 0x00 };                         // TTF
const unsigned char otfSig[] = { 0x4F, 0x54, 0x54, 0x4F };                         // OTF


FileSignature signatures[] = 
{
    { "pdf", pdfSig, sizeof(pdfSig) },
    { "jpg", jpgSig, sizeof(jpgSig) },
    { "png", pngSig, sizeof(pngSig) },
    { "gif", gifSig, sizeof(gifSig) },
    { "zip", zipSig, sizeof(zipSig) },
    { "mp3", mp3Sig, sizeof(mp3Sig) },
    { "exe", exeSig, sizeof(exeSig) },
    { "mp4", mp4Sig, sizeof(mp4Sig) },
    { "mov", movSig, sizeof(movSig) },
    { "tiff", tiffLE, sizeof(tiffLE) },
    { "tiff", tiffBE, sizeof(tiffBE) },
    { "html", htmlSig1, sizeof(htmlSig1) },
    { "html", htmlSig2, sizeof(htmlSig2) },
    { "css", cssSig, sizeof(cssSig) },
    { "java", javaSig, sizeof(javaSig) },
    { "php", phpSig, sizeof(phpSig) },
    { "json", jsonSig1, sizeof(jsonSig1) },
    { "json", jsonSig2, sizeof(jsonSig2) },
    { "xml", xmlSig, sizeof(xmlSig) },
    { "doc",  docSig,  sizeof(docSig) },
    { "xls",  xlsSig,  sizeof(xlsSig) },
    { "ppt",  pptSig,  sizeof(pptSig) },
    { "docx", zipSig,  sizeof(zipSig) },
    { "xlsx", zipSig,  sizeof(zipSig) },
    { "pptx", zipSig,  sizeof(zipSig) },
    { "rar",  rarSig,  sizeof(rarSig) },
    { "7z",   z7Sig,   sizeof(z7Sig) },
    { "iso",  isoSig,  sizeof(isoSig) },
    { "mp3",  mp3Sig1, sizeof(mp3Sig1) },
    { "mp3",  mp3Sig2, sizeof(mp3Sig2) },
    { "wav",  wavSig,  sizeof(wavSig) },
    { "flac", flacSig, sizeof(flacSig) },
    { "opus", opusSig, sizeof(opusSig) },
    { "bmp",  bmpSig,  sizeof(bmpSig) },
    { "psd",  psdSig,  sizeof(psdSig) },
    { "ttf",  ttfSig,  sizeof(ttfSig) },
    { "otf",  otfSig,  sizeof(otfSig) }
};

/// Detection of a file type
const char* detectFileType(const char* filepath) {
    unsigned char buffer[16] = { 0 };
    FILE* file = fopen(filepath, "rb");
    if (!file) return NULL;

    fread(buffer, 1, sizeof(buffer), file);
    fclose(file);

    for (int i = 0; i < sizeof(signatures) / sizeof(signatures[0]); i++) {
        if (memcmp(buffer, signatures[i].signature, signatures[i].length) == 0) {
            return signatures[i].type;
        }
    }

    return "unknown";
}