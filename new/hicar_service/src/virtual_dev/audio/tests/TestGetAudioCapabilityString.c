#include "HiCarAudioHal.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
  uint32_t l;
  printf("GetAudioCapabilityString return json is %s, length is %u ",
         GetAudioCapabilityString(&l), l);
  return 0;
}
