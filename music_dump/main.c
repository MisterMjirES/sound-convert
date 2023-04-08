#include <stdint.h>
#include <stdio.h>

#include "rr.h"
#include "pjs.h"

#define SECTOR_SIZE 512

struct song {
  const uint8_t *song;
  uint32_t       len;
};

#define SONG_COUNT 2
struct song song_table[SONG_COUNT] = {
  {rr,  2544614},
  {pjs, 1653760}
};

int main(int argc, char *args[])
{
  FILE *file;
  file = fopen("songs.img", "wb");

  for (uint32_t song = 0; song < SONG_COUNT; ++song) {
    printf("Sector start: %d\n", ftell(file) / SECTOR_SIZE);
    uint32_t bytes = song_table[song].len;
    const uint8_t *ptr = &song_table[song].song[0];
    while (bytes > SECTOR_SIZE) {
      fwrite(ptr, 1, SECTOR_SIZE, file);
      ptr += SECTOR_SIZE;
      bytes -= SECTOR_SIZE;
    }
    /* Write the remaining bytes */
    fwrite(ptr, 1, bytes, file);
    for (uint32_t i = 0; i < SECTOR_SIZE - bytes; ++i) {
      fputc(0, file);
    }
  }

  fclose(file);
  return 0;
}