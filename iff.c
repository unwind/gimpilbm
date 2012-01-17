#include "iff.h"

void
idToString(IffID id, char *str)
{
  int i;
  g_assert(str != NULL);
  str += 4;
  *str-- = '\0';
  for (i = 4; i > 0; --i) {
    *str-- = id & 0xFF;
    id >>= 8;
  }
}

gboolean
readUlong(FILE * fil, guint32 * dest)
{
  guint32 val;
  gboolean success;
  g_assert(fil != NULL);
  success = (1 == fread(&val, sizeof(val), 1, fil));
  if (!success) {
    g_warning("Error reading (4).");
  } else {
    if (NULL != dest) {
      *dest = ntohl(val);
    }
  }
  return (success);
}

gboolean
writeUlong(FILE * fil, guint32 val)
{
  gboolean success;
  g_assert(fil != NULL);
  val = htonl(val);
  success = (1 == fwrite(&val, sizeof(val), 1, fil));
  if (!success) {
    g_warning("Error writing (4).");
  }
  return (success);
}

gboolean
readUword(FILE * fil, guint16 * dest)
{
  guint16 val;
  gboolean success;
  g_assert(fil != NULL);
  success = (1 == fread(&val, sizeof(val), 1, fil));
  if (!success) {
    g_warning("Error reading (2).");
  } else {
    if (NULL != dest) {
      *dest = ntohs(val);
    }
  }
  return (success);
}

gboolean
writeUword(FILE * fil, guint16 val)
{
  gboolean success;
  g_assert(fil != NULL);
  val = htons(val);
  success = (1 == fwrite(&val, sizeof(val), 1, fil));
  if (!success) {
    g_warning("Error writing (2).");
  }
  return (success);
}

gboolean
readUchar(FILE * fil, guint8 * dest)
{
  int erg;
  gboolean success;
  g_assert(fil != NULL);
  erg = fgetc(fil);
  success = EOF != erg;
  if (!success) {
    g_warning("Error reading (1).");
  } else {
    if (NULL != dest) {
      *dest = erg;
    }
  }
  return (success);
}

gboolean
writeUchar(FILE * fil, guint8 val)
{
  gboolean success;
  g_assert(fil != NULL);
  success = EOF != fputc((int) val, fil);
  if (!success) {
    g_warning("Error writing (1).");
  }
  return (success);
}

gboolean
writeLongAt(FILE * fil, gint32 val, gint32 fileOffset)
{
  gboolean success;
  success = -1 != fseek(fil, fileOffset, SEEK_SET);  /* FIXME */
  if (success)
    success = writeUlong(fil, val);
  return (success);
}

static long afterChunk = 0;	/* FIXME */

gboolean
iffReadHeader(FILE * fil, IffChunkHeader * chd)
{
  gboolean success = TRUE;
  g_assert(fil != NULL);
  g_assert(chd != NULL);
  if (afterChunk) {
    success = 0 == fseek (fil, afterChunk, SEEK_SET);
  }
  if (success) {
    success = readUlong(fil, &chd->id);
    if (success) {
      success = readUlong(fil, &chd->len);
      if (success) {
        if (ID_FORM != chd->id) {	/* FIXME!! */
          afterChunk = ftell (fil);
          success = -1 != afterChunk;
          if (!success) {
            afterChunk = 0;	/* To be sure */
          } else {
            afterChunk += chd->len;	/* FIXME */
          }
        }
      }
    }
  } else {
    g_warning ("Error fseek()ing.");
  }
  return (success);
}

gboolean
iffWriteHeader(FILE * fil, const IffChunkHeader * chd)
{
  gboolean success;
  g_assert(fil != NULL);
  g_assert(chd != NULL);
  success = writeUlong(fil, chd->id);
  if (success) {
    success = writeUlong(fil, chd->len);
  }
  return (success);
}

void
iffInitHeader(IffChunkHeader * chd, IffID id, guint32 len)
{
  g_assert(chd != NULL);
  chd->id = id;
  chd->len = len;
}

void
iffDumpHeader(const IffChunkHeader * chd)
{
  char idstr[5];
  g_assert(chd != NULL);
  idToString(chd->id, idstr);
  printf("Header %s of %lu byte.\n",
         idstr, (unsigned long) chd->len);
}

gboolean
iffReadData(FILE * fil, void *data, size_t len)
{
  gboolean success;
  g_assert(fil != NULL);
  g_assert(data != NULL);
  success = 1 == fread(data, len, 1, fil);
  if (!success)
    g_warning("Error reading.");
  return (success);
}

gboolean
iffWriteData(FILE * fil, const void *data, size_t len)
{
  gboolean success;
  g_assert(fil != NULL);
  g_assert(data != NULL);
  success = 1 == fwrite(data, len, 1, fil);
  if (!success)
    g_warning("Error writing.");
  return (success);
}
