#ifndef DETECTOR_H
#define DETECTOR_H

typedef struct {
    const char* type;
    const unsigned char* signature;
    size_t length;
} FileSignature;

const char *detectFileType(const char* filepath);

#endif